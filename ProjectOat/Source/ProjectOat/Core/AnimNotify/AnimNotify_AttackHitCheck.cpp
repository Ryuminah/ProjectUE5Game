// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AnimNotify/AnimNotify_AttackHitCheck.h"
#include "InGame/Interface/OatFightUnitInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IOatFightUnitInterface* AttackPawn = Cast<IOatFightUnitInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AnimNotifyAttackHitCheck();
		}
	}
}
