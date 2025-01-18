// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/AnimNotify_AttackHitCheck.h"
#include "InGame/Interface/OatAnimationAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 인자의 정보로부터 액터에 명령을 내려야 함
	if (MeshComp)
	{
		IOatAnimationAttackInterface* AttackPawn = Cast<IOatAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}
}
