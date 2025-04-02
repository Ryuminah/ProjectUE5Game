// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Managers/OatSoundHandler.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/AudioComponent.h"
#include "Core/OatGameInstance.h"
#include "Sound/SoundCue.h"

// Sets default values
AOatSoundHandler::AOatSoundHandler()
{
	BGMAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMAudioComponent"));
	SetRootComponent(BGMAudioComponent);
	BGMAudioComponent->bAutoActivate = false;

	LoadSoundCueAssets();
}

void AOatSoundHandler::Initialize(UGameInstance* GameInstance)
{
	UOatGameInstance* GetOatGameInstance = Cast<UOatGameInstance>(GameInstance);
	if (!GetOatGameInstance)
	{
		ensure(TEXT("OatGameInstance is Null"));
		return;
	}

}

void AOatSoundHandler::BeginPlay()
{
	Super::BeginPlay();
	PlayBGM(FName(TEXT("SC_CowboyBebop_Tank")));
}

void AOatSoundHandler::PlaySound(FName BGMName)
{
	USoundCue* TargetSoundCue = BGMSoundMap.FindRef(BGMName);
	if (!TargetSoundCue) { return; }

	// Need Cache
	UAudioComponent* TempAudioComponent = NewObject<UAudioComponent>(this);
	TempAudioComponent->RegisterComponent();
	TempAudioComponent->SetSound(TargetSoundCue);
	TempAudioComponent->Play();
}

void AOatSoundHandler::PlayBGM(FName ChangeBGMName)
{

	if (CurrentBGM != nullptr && CurrentBGM.GetFName() == ChangeBGMName) { return; }

	USoundCue* TargetSoundCue = BGMSoundMap.FindRef(ChangeBGMName);
	if (!TargetSoundCue) { return; }

	// Change BGM
	CurrentBGM = TargetSoundCue;
	
	if (BGMAudioComponent->IsPlaying())
	{
		BGMAudioComponent->FadeOut(1.5f, 0.0f); // 1.5초 동안 꺼지기

		// 캡쳐란 바깥에 있는 지역 변수를 람다 안에서 사용하도록 '복사' 또는 '참조'로 받아오는 것!
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			BGMAudioComponent->SetSound(CurrentBGM);
			BGMAudioComponent->FadeIn(1.5f, 1.0f, 0.0f); // 1.5초 동안 점점 커짐
		}, 0.5f, false);
	}

	else
	{
		BGMAudioComponent->SetSound(CurrentBGM);
		BGMAudioComponent->Play();
	}


}

void AOatSoundHandler::LoadSoundCueAssets()
{
	// Sound 폴더 기준 경로 ("/Game/" 는 반드시 포함!)
	const FString BGMFolderPath = TEXT("/Game/ProjectOat/Sounds/BGM");

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	FARFilter Filter;
	Filter.PackagePaths.Add(*BGMFolderPath);
	//해당 코드는 클래스 이름만 필터 조건에 추가
	//Filter.ClassNames.Add(USoundCue::StaticClass()->GetFName());
	// 5.1 부터 권장 (전체 경로로 필터링함)
	Filter.ClassPaths.Add(USoundCue::StaticClass()->GetClassPathName());
	Filter.bRecursivePaths = true; // 하위 폴더도 탐색

	TArray<FAssetData> AssetList;
	// 경로에 있는 에셋들 가져오기
	AssetRegistryModule.Get().GetAssets(Filter, AssetList);

	for (const FAssetData& Asset : AssetList)
	{
		if (USoundCue* LoadedSound = Cast<USoundCue>(Asset.GetAsset()))
		{
			// 원하는 방식으로 캐싱
			BGMSoundMap.FindOrAdd(Asset.AssetName, LoadedSound);
		}
	}
}
