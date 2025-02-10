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

	// 트리거에 태그 부착
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

	// 굳이 Switch를 사용하지 않고 Callback을 통해 연동
	CurrentState = InNewState;
	if (SectionStateChangedCallback.Contains(CurrentState))
	{
		SectionStateChangedCallback[CurrentState].SectionDelegate.ExecuteIfBound();
	}
}

void AOatStageSectionTrigger::SetReadyBattle()
{
	//// Collision 활성화 , 몬스터 스폰

	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	
	// 전투의 시작에 알림 보내기
	// 벽 활성화
	// 몬스터 스폰

	SpawnSectionEnemy();
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
}


void AOatStageSectionTrigger::SpawnSectionEnemy()
{
	const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * -3500.f;
	
	// ?? 왜 되지 (GetWorld 안됨)
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
	// GameMode를 Interface로 감싸기
	IOatGameInterface* OatGameMode = Cast<IOatGameInterface>(GetWorld()->GetAuthGameMode());
	if (OatGameMode)
	{
		// Stage
	}
	SetSectionState(EStageSectionState::ENDBATTLE);
}


