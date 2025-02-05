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

float AOatCharacterNPC::GetAIPatrolRadius()
{
	return 500.0f;
}

float AOatCharacterNPC::GetAIDetectRange()
{
	return 0.0f;
}

float AOatCharacterNPC::GetAIAttackRange()
{
	return 0.0f;
}

float AOatCharacterNPC::GetAITurnSpeed()
{
	return 0.0f;
}
