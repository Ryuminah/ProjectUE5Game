// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/OatCharacterNPC.h"
#include "Core/OatAIController.h"
#include "InGame/Character/Component/OatCharacterStatComponent.h"

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
	return 800.0f;
}

float AOatCharacterNPC::GetAIDetectRange()
{
	return 400.0f;
}

float AOatCharacterNPC::GetAIAttackRange()
{
	return Stat->GetTotalStat().AtkRange+ Stat->GetAttackRadius()*2;
}

float AOatCharacterNPC::GetAITurnSpeed()
{
	return 2.0f;
}

void AOatCharacterNPC::SetAIAttackDelegate(const FAIAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AOatCharacterNPC::AttackByAI()
{
	ProcessAttack();
}

void AOatCharacterNPC::NotifyAttackActionEnd()
{
	// 부모 클래스에서 Attack Montage가 끝난 시점에 호출
	Super::NotifyAttackActionEnd();
	OnAttackFinished.ExecuteIfBound();
}
