// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/AI/BTTask_Attack.h"
#include "Core/OatAIController.h"
#include "InGame/AI/Interface/OatAIInterface.h"


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


	IOatAIInterface* AIPawn = Cast<IOatAIInterface>(ControllingPawn);
	if (!AIPawn) { return EBTNodeResult::Failed;}

	FAIAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	AIPawn->SetAIAttackFinishedDelegate(OnAttackFinished);
	
	AIPawn->AttackByAI();
	return EBTNodeResult::InProgress;

	// ��Ÿ�ְ� ����� ����Ǿ���� ������ �Ϸ��̴�.

	return Result;
}
