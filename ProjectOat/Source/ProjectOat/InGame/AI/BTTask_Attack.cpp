// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/AI/BTTask_Attack.h"
#include "Core/OatAIController.h"
#include "InGame/AI/Interface/OatAIInterface.h"
#include "InGame/Character/OatCharacterNPC.h"
#include "InGame/Character/OatFightUnitBase.h"
#include "InGame/Interface/OatFightUnitInterface.h"


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

	AOatCharacterNPC* AIPawn = Cast<AOatCharacterNPC>(ControllingPawn);
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
