// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Stage/OatStageSectionTrigger.h"

#include "Components/BoxComponent.h"
#include "Core/OatGameInstance.h"
#include "GameCommon/Managers/OatStageHandler.h"
#include "InGame/Physics/OatCollision.h"
#include "InGame/Character/OatCharacterNPC.h"
#include "InGame/Stage/OatSpawnPoint.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AOatStageSectionTrigger::AOatStageSectionTrigger()
{
	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Stage"));
	SetRootComponent(StageTrigger);

	StageTrigger->SetBoxExtent(FVector(7.f, 150.f, 70.f));
	StageTrigger->SetCollisionProfileName(CPROFILE_OATTRIGGER);
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AOatStageSectionTrigger::OnStageTriggerBeginOverlap);

	// 트리거에 태그 부착
	//StageTrigger->ComponentTags.Add();


	OpponentClass = AOatCharacterNPC::StaticClass();
	OatSpawnPointClass = AOatSpawnPoint::StaticClass();

	CurrentState = EStageSectionState::NONE;
}

void AOatStageSectionTrigger::BeginPlay()
{
	Super::BeginPlay();

	SectionStateChangedCallback.Add(EStageSectionState::READYBATTLE,
	                                      FStageSectionChangedDelegateWrapper(
		                                      FOnStageSectionStateChangedDelegate::CreateUObject(
			                                      this, &AOatStageSectionTrigger::SetReadyBattle)));

	SectionStateChangedCallback.Add(EStageSectionState::INBATTLE,
	                                      FStageSectionChangedDelegateWrapper(
		                                      FOnStageSectionStateChangedDelegate::CreateUObject(
			                                      this, &AOatStageSectionTrigger::SetInBattle)));

	SectionStateChangedCallback.Add(EStageSectionState::ENDBATTLE,
	                                      FStageSectionChangedDelegateWrapper(
		                                      FOnStageSectionStateChangedDelegate::CreateUObject(
			                                      this, &AOatStageSectionTrigger::SetEndBattle)));

	CreateSpawnPointData();
}

void AOatStageSectionTrigger::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AOatStageSectionTrigger::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                         bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (CurrentState == EStageSectionState::ENDBATTLE)
	{
		return;
	}

	SetSectionState(EStageSectionState::READYBATTLE);
}


void AOatStageSectionTrigger::SetSectionState(EStageSectionState NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	// 굳이 Switch를 사용하지 않고 Callback을 통해 연동
	// 이것도 나중에 유저 데이터로 통합한다
	CurrentState = NewState;


	if (SectionStateChangedCallback.Contains(NewState))
	{
		SectionStateChangedCallback[NewState].SectionDelegate.ExecuteIfBound();
	}

}

void AOatStageSectionTrigger::SetReadyBattle()
{
	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GetGameInstance());
	if (!OatGameInstance)
	{
		return;
	}

	OatGameInstance->GetStageHandler()->ChangeToReadyBattle(SectionId);

	//// Collision 활성화
	//UE_LOG(LogTemp, Warning, TEXT("AOatStageSectionTrigger Address: %p"), this);

	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	float SpawnDelay = 0.f;
	for (auto SpawnActor : SpawnPointArray)
	{
		float SpawnInterval = 0.3f;
		FTimerHandle SpawnTimerHandle;
		FVector SpawnPoint = FVector(SpawnActor->GetActorLocation());
		SpawnDelay += SpawnInterval;

		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle,
		                                       [this, SpawnPoint]()
		                                       {
			                                       SpawnSectionEnemy(SpawnPoint);
		                                       }, SpawnDelay, false);
	}


	SetSectionState(EStageSectionState::INBATTLE);
}

// 사실 InBattle == ReadyBattle일수도.. ㅋㅋㅋㅋㅋㅋㅋㅋ
void AOatStageSectionTrigger::SetInBattle()
{
	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GetGameInstance());
	if (!OatGameInstance)
	{
		return;
	}

	OatGameInstance->GetStageHandler()->ChangeToInBattle(SectionId);

	// 몬스터 스폰 및 처치 카운팅
}

void AOatStageSectionTrigger::SetEndBattle()
{
	// 해당 트리거는 비활성화 해도 됨
	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GetGameInstance());
	if (!OatGameInstance)
	{
		return;
	}

	OatGameInstance->GetStageHandler()->ChangeToEndBattle(SectionId);

	SetActorEnableCollision(false);

	// 벽 해제
	// SpawnPoint destroy 혹은 비활성화 하기
}


void AOatStageSectionTrigger::SpawnSectionEnemy(FVector SpawnPos)
{
	// (GetWorld 안됨)


	UWorld* World = GWorld->GetGameInstance()->GetWorld();
	AActor* OpponentActor = World->SpawnActor(OpponentClass, &SpawnPos, &FRotator::ZeroRotator);

	if (AOatCharacterNPC* AOatEnemy = Cast<AOatCharacterNPC>(OpponentActor))
	{
		AOatEnemy->OnDestroyed.AddDynamic(this, &AOatStageSectionTrigger::OnOpponentDestroyed);
	}
}

void AOatStageSectionTrigger::CreateSpawnPointData()
{
	TArray<AActor*> AllSpawnPoints;

	UWorld* World = GWorld->GetGameInstance()->GetWorld();
	UGameplayStatics::GetAllActorsOfClass(World, OatSpawnPointClass, AllSpawnPoints);
	for (AActor* SpawnPointActor : AllSpawnPoints)
	{
		AOatSpawnPoint* SpawnPoint = Cast<AOatSpawnPoint>(SpawnPointActor);
		if (SpawnPoint->GetSectionId() == SectionId)
		{
			SpawnPointArray.Add(SpawnPoint);
		}
	}
}

void AOatStageSectionTrigger::OnOpponentDestroyed(AActor* DestroyedActor)
{
	//SetSectionState(EStageSectionState::ENDBATTLE);
}
