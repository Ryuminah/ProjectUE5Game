// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/AI/BT/BTTask_Attack.h"
#include "Core/OatAIController.h"
#include "InGame/AI/Interface/OatAIInterface.h"
#include "InGame/Character/OatCharacterEnemy.h"


UBTTask_Attack::UBTTask_Attack()
{}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	AOatCharacterEnemy* AIPawn = Cast<AOatCharacterEnemy>(ControllingPawn);
	if (!AIPawn) { return EBTNodeResult::Failed;}

	FOnBTTaskAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	AIPawn->SetOnBTTaskAttackFinishedDelegate(OnAttackFinished);
	AIPawn->InputAttack();
	return EBTNodeResult::InProgress;

}
