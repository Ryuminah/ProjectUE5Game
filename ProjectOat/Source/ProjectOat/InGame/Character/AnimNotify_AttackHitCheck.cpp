// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/AnimNotify_AttackHitCheck.h"
#include "InGame/Interface/OatAnimFightInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IOatAnimFightInterface* AttackPawn = Cast<IOatAnimFightInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->OnAttackHitCheck();
		}
	}
}
