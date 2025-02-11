// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Stage/OatGoalTrigger.h"
#include "Core/Interface/OatGameInterface.h"
#include "Core/OatGameInstance.h"
#include "GameCommon/Managers/OatEventHandler.h"
#include "InGame/Physics/OatCollision.h"
#include "InGame/Character/OatCharacterNPC.h"

#include "Components/BoxComponent.h"

// Sets default values
AOatGoalTrigger::AOatGoalTrigger()
{
	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Goal"));
	SetRootComponent(StageTrigger);

	StageTrigger->SetBoxExtent(FVector(500.f, 500.f, 300.f));
	StageTrigger->SetCollisionProfileName(CPROFILE_OATTRIGGER);
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AOatGoalTrigger::OnGoalTriggerBeginOverlap);
}

void AOatGoalTrigger::OnGoalTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GetGameInstance());
	if (OatGameInstance)
	{
		OatGameInstance->GetEventHandler()->OnStageClearGoal.Broadcast();
	}
}
