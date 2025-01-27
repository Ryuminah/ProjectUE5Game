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

	// 트리거에 태그 부착
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
	// 몬스터를 스폰 
}

void AOatStageSectionTrigger::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{

}

void AOatStageSectionTrigger::SetSectionState(EStageSectionState InNewState)
{
	// 굳이 Switch를 사용하지 않고 Callback을 통해 연동
	CurrentState = InNewState;
	if (SectionStateChangedCallback.Contains(InNewState))
	{
		SectionStateChangedCallback[CurrentState].SectionDelegate.ExecuteIfBound();
	}
}

void AOatStageSectionTrigger::SetReadyBattle()
{
	//// Collision 활성화 , 몬스터 스폰
	//StageTrigger->SetCollisionProfileName((CPROFILE_OATTRIGGER));
	
	// 전투의 시작에 알림 보내기
}

void AOatStageSectionTrigger::SetInBattle()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	
	// SectionOrder -> 테이블에서 몬스터 스폰의 위치와 종류를 가져옴.
	// 스폰한다.
	SpawnSectionEnemy();
	// 몬스터 카운팅

}

void AOatStageSectionTrigger::SetEndBattle()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	// Section전투가 끝났다고 노티파이 알림 보내기
}


void AOatStageSectionTrigger::SpawnSectionEnemy()
{}


