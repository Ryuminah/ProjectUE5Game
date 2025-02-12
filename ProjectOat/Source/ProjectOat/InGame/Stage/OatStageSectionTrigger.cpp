// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Stage/OatStageSectionTrigger.h"
#include "Components/BoxComponent.h"
#include "InGame/Physics/OatCollision.h"
#include "InGame/Character/OatCharacterNPC.h"
#include "Core/Interface/OatGameInterface.h"
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

	CurrentState = EStageSectionState::NONE;

	SectionStateChangedCallback.Add(EStageSectionState::READYBATTLE,
									FStageSectionChangedDelegateWrapper(FOnStageSectionStateChangedDelegate::CreateUObject(this, &AOatStageSectionTrigger::SetReadyBattle)));

	SectionStateChangedCallback.Add(EStageSectionState::INBATTLE,
									FStageSectionChangedDelegateWrapper(FOnStageSectionStateChangedDelegate::CreateUObject(this, &AOatStageSectionTrigger::SetInBattle)));

	SectionStateChangedCallback.Add(EStageSectionState::ENDBATTLE,
									FStageSectionChangedDelegateWrapper(FOnStageSectionStateChangedDelegate::CreateUObject(this, &AOatStageSectionTrigger::SetEndBattle)));


	OpponentClass = AOatCharacterNPC::StaticClass();
	OatSpawnPointClass = AOatSpawnPoint::StaticClass();
}

void AOatStageSectionTrigger::BeginPlay()
{
	Super::BeginPlay();

	CreateSpawnPointData();
}

void AOatStageSectionTrigger::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (CurrentState == EStageSectionState::ENDBATTLE)
	{
		return;
	}

	SetSectionState(EStageSectionState::READYBATTLE);
}

void AOatStageSectionTrigger::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{

}

void AOatStageSectionTrigger::SetSectionState(EStageSectionState InNewState)
{
	if (CurrentState == InNewState)
	{
		return;
	}

	// 굳이 Switch를 사용하지 않고 Callback을 통해 연동
	CurrentState = InNewState;

	// 이코드가 문제가 생긴다...^^
	//if (SectionStateChangedCallback.Contains(CurrentState))
	//{
	//	SectionStateChangedCallback[CurrentState].SectionDelegate.ExecuteIfBound();
	//}
	switch (InNewState)
	{
	case EStageSectionState::READYBATTLE:
		SetReadyBattle();
		break;
	case EStageSectionState::INBATTLE:
		SetInBattle();
		break;
	case EStageSectionState::ENDBATTLE:
		SetEndBattle();
		break;
	case EStageSectionState::NONE:
		break;
	default:
		break;
	}
}

void AOatStageSectionTrigger::SetReadyBattle()
{
	//// Collision 활성화 , 몬스터 스폰

	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	
	// 전투의 시작에 알림 보내기
	// 벽 활성화
	// 몬스터 스폰

	float SpawnInterval = 0.3f;
	float SpawnDelay = 0.f;

	if (SpawnPointArray.IsEmpty())
	{
		CreateSpawnPointData();
	}

	for (auto SpawnActor : SpawnPointArray)
	{
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

void AOatStageSectionTrigger::SetInBattle()
{

	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	// 몬스터 스폰 및 처치 카운팅
}

void AOatStageSectionTrigger::SetEndBattle()
{
	// 해당 트리거는 비활성화 해도 됨
	SetActorEnableCollision(false);

	// Section전투가 끝났다고 노티파이 알림 보내기
	// 벽 해제
	// SpawnPoint destroy 혹은 비활성화 하기
}


void AOatStageSectionTrigger::SpawnSectionEnemy(FVector SpawnPos)
{
	//const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * -3500.f;
	
	// ?? 왜 되지 (GetWorld 안됨)
	UWorld* World = GWorld->GetGameInstance()->GetWorld();
	AActor* OpponentActor = World->SpawnActor(OpponentClass, &SpawnPos, &FRotator::ZeroRotator);
	AOatCharacterNPC* AOatEnemy = Cast<AOatCharacterNPC>(OpponentActor);
	if (AOatEnemy)
	{
		AOatEnemy->OnDestroyed.AddDynamic(this, &AOatStageSectionTrigger::OnOpponentDestroyed);
	}
}

void AOatStageSectionTrigger::CreateSpawnPointData()
{
	TArray<AActor*> AllSpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), OatSpawnPointClass, AllSpawnPoints);
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
