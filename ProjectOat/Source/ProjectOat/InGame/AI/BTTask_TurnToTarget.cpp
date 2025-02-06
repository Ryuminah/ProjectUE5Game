// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/AI/BTTask_TurnToTarget.h"
#include "InGame/AI/OatAI.h"
#include "Core/OatAIController.h"
#include "InGame/AI/Interface/OatAIInterface.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_TurnToTarget.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn) { return EBTNodeResult::Failed; }

	IOatAIInterface* AIPawn = Cast<IOatAIInterface>(ControllingPawn);
	if (!AIPawn) { return EBTNodeResult::Failed; }

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (!Target) { return EBTNodeResult::Failed; }

	float TurnSpeed = AIPawn->GetAITurnSpeed();

	// AI가 바라봐야하는 회전값 계산
	FVector LookVector = Target->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Z = 0.f;
	FRotator TargetRot = FRotationMatrix::MakeFromZ(LookVector).Rotator();

	// 회전
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));

	return EBTNodeResult::Succeeded;
}
