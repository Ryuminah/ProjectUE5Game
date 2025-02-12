// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Stage/OatGoalTrigger.h"
#include "Core/Interface/OatGameInterface.h"
#include "Core/OatGameInstance.h"
#include "Core/OatGameMode.h"
#include "GameCommon/Managers/OatEventHandler.h"
#include "InGame/Physics/OatCollision.h"
#include "InGame/Character/OatCharacterNPC.h"

#include "Components/BoxComponent.h"

// Sets default values
AOatGoalTrigger::AOatGoalTrigger()
{
	Goal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Goal"));
	GoalTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("GoalTrigger"));

	RootComponent = Goal;
	GoalTrigger->SetupAttachment(Goal);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> GoalMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ProjectOat/Arts/Backgrounds/Props/VendingMachine/SM_Goal_VendingMachine.SM_Goal_VendingMachine'"));
	if (GoalMeshRef.Object)
	{
		Goal->SetStaticMesh(GoalMeshRef.Object);
	}

	GoalTrigger->SetBoxExtent(FVector(150.f, 150.f, 230.f));
	GoalTrigger->SetCollisionProfileName(CPROFILE_OATTRIGGER);
	GoalTrigger->OnComponentBeginOverlap.AddDynamic(this, &AOatGoalTrigger::OnGoalTriggerBeginOverlap);
}

void AOatGoalTrigger::OnGoalTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	AOatGameMode* OatGameMode = Cast<AOatGameMode>(GetWorld()->GetAuthGameMode());
	if (OatGameMode)
	{
		OatGameMode->GameClear();
	}
}
