// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Stage/OatStageSectionTrigger.h"
#include "Components/BoxComponent.h"
#include "Core/OatGameInstance.h"
#include "GameCommon/Managers/OatEventHandler.h"
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

	// Ʈ���ſ� �±� ����
	//StageTrigger->ComponentTags.Add();



	OpponentClass = AOatCharacterNPC::StaticClass();
	OatSpawnPointClass = AOatSpawnPoint::StaticClass();
	
	CurrentState = EStageSectionState::NONE;

}

void AOatStageSectionTrigger::BeginPlay()
{
	Super::BeginPlay();




	SectionStateChangedCallback.FindOrAdd(EStageSectionState::READYBATTLE,
					FStageSectionChangedDelegateWrapper(FOnStageSectionStateChangedDelegate::CreateUObject(this, &AOatStageSectionTrigger::SetReadyBattle)));
	
	SectionStateChangedCallback.FindOrAdd(EStageSectionState::INBATTLE,
									FStageSectionChangedDelegateWrapper(FOnStageSectionStateChangedDelegate::CreateUObject(this, &AOatStageSectionTrigger::SetInBattle)));
	
	SectionStateChangedCallback.FindOrAdd(EStageSectionState::ENDBATTLE,
									FStageSectionChangedDelegateWrapper(FOnStageSectionStateChangedDelegate::CreateUObject(this, &AOatStageSectionTrigger::SetEndBattle)));
	
	CreateSpawnPointData();
}

void AOatStageSectionTrigger::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AOatStageSectionTrigger::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
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

	// ���� Switch�� ������� �ʰ� Callback�� ���� ����
	CurrentState = NewState;

	// �̷��� �ϸ� �� SpawnPointArray �迭�� ���󰥱�?
	// ���ε�
	if (SectionStateChangedCallback.Contains(NewState))
	{
		SectionStateChangedCallback[NewState].SectionDelegate.ExecuteIfBound();
	}
	//UE_LOG(LogTemp, Warning, TEXT("AOatStageSectionTrigger Address: %p"), this);

	
	// switch (NewState)
	// {
	// case EStageSectionState::READYBATTLE:
	// 	SetReadyBattle();
	// 	break;
	// case EStageSectionState::INBATTLE:
	// 	SetInBattle();
	// 	break;
	// case EStageSectionState::ENDBATTLE:
	// 	SetEndBattle();
	// 	break;
	// case EStageSectionState::NONE:
	// 	break;
	// default:
	// 	break;
	// }
}

void AOatStageSectionTrigger::SetReadyBattle()
{
	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GetGameInstance());
	if (!OatGameInstance)
	{
		return;
	}
	
	OatGameInstance->GetStageHandler()->ReadyBattle(SectionId);
	
	//// Collision Ȱ��ȭ
	UE_LOG(LogTemp, Warning, TEXT("AOatStageSectionTrigger Address: %p"), this);

	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	
	// �� Ȱ��ȭ
	
	// ���� ����
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

// ��� InBattle == ReadyBattle�ϼ���.. ����������������
void AOatStageSectionTrigger::SetInBattle()
{
	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GetGameInstance());
	if (!OatGameInstance)
	{
		return;
	}
	
	OatGameInstance->GetStageHandler()->OnStageSectionChanged.Broadcast(SectionId, EStageSectionState::INBATTLE);
	
	// ���� ���� �� óġ ī����
}

void AOatStageSectionTrigger::SetEndBattle()
{
	// �ش� Ʈ���Ŵ� ��Ȱ��ȭ �ص� ��
	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GetGameInstance());
	if (!OatGameInstance)
	{
		return;
	}
	OatGameInstance->GetEventHandler()->OnStageSectionChanged.Broadcast(SectionId, EStageSectionState::ENDBATTLE);
	SetActorEnableCollision(false);
	
	// �� ����
	// SpawnPoint destroy Ȥ�� ��Ȱ��ȭ �ϱ�
}


void AOatStageSectionTrigger::SpawnSectionEnemy(FVector SpawnPos)
{
	// (GetWorld �ȵ�)
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
