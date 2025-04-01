// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AnimNotify/AnimNotify_SpawnEnd.h"

#include "InGame/Character/OatCharacterEnemy.h"

void UAnimNotify_SpawnEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		if (AOatCharacterEnemy* EnemyPawn = Cast<AOatCharacterEnemy>(MeshComp->GetOwner()))
		{
			EnemyPawn->AnimNotifySpawnEnd();
		}
	}
	
}
