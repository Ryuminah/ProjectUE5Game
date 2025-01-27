// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Stage/OatStageSectionTrigger.h"
#include "Components/BoxComponent.h"
#include "InGame/Physics/OatCollision.h"


// Sets default values
AOatStageSectionTrigger::AOatStageSectionTrigger()
{
	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Stage"));
	RootComponent = StageTrigger;

	StageTrigger->SetBoxExtent(FVector(500.f, 500.f, 300.f));
	StageTrigger->SetCollisionProfileName(CPROFILE_OATTRIGGER);
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AOatStageSectionTrigger::OnStageTriggerBeginOverlap);

	// Ʈ���ſ� �±� ����
	//StageTrigger->ComponentTags.Add();

	CurrentState = EStageSectionState::READYBATTLE;
	SectionStateChangedCallback.Add(EStageSectionState::READYBATTLE, 
									FStageSectionChangedDelegateWrapper(FOnStageSectionStateChangedDelegate::CreateUObject(this, AOatStageSectionTrigger::SetReadyBattle)));

	SectionStateChangedCallback.Add(EStageSectionState::INBATTLE,
									FStageSectionChangedDelegateWrapper(FOnStageSectionStateChangedDelegate::CreateUObject(this, AOatStageSectionTrigger::SetInBattle)));

	SectionStateChangedCallback.Add(EStageSectionState::ENDBATTLE,
									FStageSectionChangedDelegateWrapper(FOnStageSectionStateChangedDelegate::CreateUObject(this, AOatStageSectionTrigger::SetEndBattle)));
	//static ConstructorHelpers::FObjectFinder<UStaticMeshComponent> StageMeshRef(TEXT("/Script/En"))


}

void AOatStageSectionTrigger::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetSectionState(CurrentState);
}

void AOatStageSectionTrigger::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	// ���͸� ���� 
}

void AOatStageSectionTrigger::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{

}

void AOatStageSectionTrigger::SetSectionState(EStageSectionState InNewState)
{
	// ���� Switch�� ������� �ʰ� Callback�� ���� ����
	CurrentState = InNewState;
	if (SectionStateChangedCallback.Contains(InNewState))
	{
		SectionStateChangedCallback[CurrentState].SectionDelegate.ExecuteIfBound();
	}
}

void AOatStageSectionTrigger::SetReadyBattle()
{
	//// Collision Ȱ��ȭ , ���� ����
	//StageTrigger->SetCollisionProfileName((CPROFILE_OATTRIGGER));
	
	// ������ ���ۿ� �˸� ������
}

void AOatStageSectionTrigger::SetInBattle()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	
	// SectionOrder -> ���̺��� ���� ������ ��ġ�� ������ ������.
	// �����Ѵ�.
	SpawnSectionEnemy();
	// ���� ī����

}

void AOatStageSectionTrigger::SetEndBattle()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	// Section������ �����ٰ� ��Ƽ���� �˸� ������
}


void AOatStageSectionTrigger::SpawnSectionEnemy()
{}


