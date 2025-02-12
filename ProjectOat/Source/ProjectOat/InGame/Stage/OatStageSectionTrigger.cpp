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

	// Ʈ���ſ� �±� ����
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

	// ���� Switch�� ������� �ʰ� Callback�� ���� ����
	CurrentState = InNewState;

	// ���ڵ尡 ������ �����...^^
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
	//// Collision Ȱ��ȭ , ���� ����

	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	
	// ������ ���ۿ� �˸� ������
	// �� Ȱ��ȭ
	// ���� ����

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

	// ���� ���� �� óġ ī����
}

void AOatStageSectionTrigger::SetEndBattle()
{
	// �ش� Ʈ���Ŵ� ��Ȱ��ȭ �ص� ��
	SetActorEnableCollision(false);

	// Section������ �����ٰ� ��Ƽ���� �˸� ������
	// �� ����
	// SpawnPoint destroy Ȥ�� ��Ȱ��ȭ �ϱ�
}


void AOatStageSectionTrigger::SpawnSectionEnemy(FVector SpawnPos)
{
	//const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * -3500.f;
	
	// ?? �� ���� (GetWorld �ȵ�)
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
