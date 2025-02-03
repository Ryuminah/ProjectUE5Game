// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/OatCharacterNPC.h"
#include "Core/OatAIController.h"

AOatCharacterNPC::AOatCharacterNPC()
{
	AIControllerClass = AOatAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AOatCharacterNPC::SetDead()
{
	Super::SetDead();

	// 이벤트 시간 이후 종료
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([&](){ Destroy(); }), 
										   DeadEventDelayTime, false);

}
