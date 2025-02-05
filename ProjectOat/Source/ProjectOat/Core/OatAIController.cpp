// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/OatAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "InGame/AI/OatAI.h"

AOatAIController::AOatAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/ProjectOat/InGame/AI/BB_OatAI.BB_OatAI'"));
	if (BBAssetRef.Object)
	{
		BB = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/ProjectOat/InGame/AI/BT_OatAI.BT_OatAI'"));
	if (BTAssetRef.Object)
	{
		BT = BTAssetRef.Object;
	}
}

void AOatAIController::RunAI()
{
	// 블랙보드 구동
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BB, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		bool RunResult = RunBehaviorTree(BT);
		ensure(RunResult);
	}
		
}

void AOatAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void AOatAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunAI();
}
