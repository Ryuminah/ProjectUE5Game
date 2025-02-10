// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Stage/OatStageSectionTrigger.h"
#include "Components/BoxComponent.h"
#include "InGame/Physics/OatCollision.h"
#include "InGame/Character/OatCharacterNPC.h"
#include "Core/Interface/OatGameInterface.h"


// Sets default values
AOatStageSectionTrigger::AOatStageSectionTrigger()
{
	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Stage"));
	SetRootComponent(StageTrigger);


	StageTrigger->SetBoxExtent(FVector(500.f, 500.f, 300.f));
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
	//static ConstructorHelpers::FObjectFinder<UStaticMeshComponent> StageMeshRef(TEXT("/Script/En"))

	OpponentClass = AOatCharacterNPC::StaticClass();

}

void AOatStageSectionTrigger::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AOatStageSectionTrigger::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AOatStageSectionTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void AOatStageSectionTrigger::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
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
	if (SectionStateChangedCallback.Contains(CurrentState))
	{
		SectionStateChangedCallback[CurrentState].SectionDelegate.ExecuteIfBound();
	}
}

void AOatStageSectionTrigger::SetReadyBattle()
{
	//// Collision Ȱ��ȭ , ���� ����

	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	
	// ������ ���ۿ� �˸� ������
	// �� Ȱ��ȭ
	// ���� ����

	SpawnSectionEnemy();
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
}


void AOatStageSectionTrigger::SpawnSectionEnemy()
{
	const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * -3500.f;
	
	// ?? �� ���� (GetWorld �ȵ�)
	UWorld* World = GWorld->GetGameInstance()->GetWorld();
	AActor* OpponentActor = World->SpawnActor(OpponentClass, &SpawnLocation, &FRotator::ZeroRotator);
	AOatCharacterNPC* AOatEnemy = Cast<AOatCharacterNPC>(OpponentActor);
	if (AOatEnemy)
	{
		AOatEnemy->OnDestroyed.AddDynamic(this, &AOatStageSectionTrigger::OnOpponentDestroyed);
	}

}

void AOatStageSectionTrigger::OnOpponentDestroyed(AActor* DestroyedActor)
{
	// GameMode�� Interface�� ���α�
	IOatGameInterface* OatGameMode = Cast<IOatGameInterface>(GetWorld()->GetAuthGameMode());
	if (OatGameMode)
	{
		// Stage
	}
	SetSectionState(EStageSectionState::ENDBATTLE);
}


