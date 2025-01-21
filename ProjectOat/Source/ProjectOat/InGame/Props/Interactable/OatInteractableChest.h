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

	// 블프에서도 사용할 수 있는 함수이기 때문에 UFunction
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnEffectFinished(UParticleSystemComponent* ParticleSystem);
	
};
