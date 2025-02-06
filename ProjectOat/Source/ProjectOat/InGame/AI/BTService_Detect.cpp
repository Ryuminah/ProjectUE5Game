// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/AI/BTService_Detect.h"
#include "InGame/AI/Interface/OatAIInterface.h"
#include "InGame/Physics/OatCollision.h"
#include "InGame/AI/OatAI.h"
#include "Core/OatAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Engine/OverlapResult.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn) { return; }

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (!World) { return; }

	IOatAIInterface* AIPawn = Cast<IOatAIInterface>(ControllingPawn);
	if (!AIPawn) { return; }

	// �������̽��κ��� ������ ������ ���� ������
	float DetectRadius = AIPawn->GetAIDetectRange();

	// �÷��̾ �ټ��� ���� ����
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(OverlapResults,Center,FQuat::Identity,CCHANNEL_OATACTION,
												FCollisionShape::MakeSphere(DetectRadius),CollisionQueryParam);

	if (bResult)
	{
		for (auto const& Result : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(Result.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				// ������ Ÿ�ٰ� ����
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
			}

		}
	}

	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	}


}
