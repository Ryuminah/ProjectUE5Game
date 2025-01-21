// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OatInteractableChest.generated.h"

UCLASS()
class PROJECTOAT_API AOatInteractableChest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOatInteractableChest();

protected: 
	UPROPERTY(Category=Box,VisibleAnywhere)
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(Category=Box,VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(Category=Effect,VisibleAnywhere)
	TObjectPtr<class UParticleSystemComponent> Effect;
	
};
