// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OatGoalTrigger.generated.h"



UCLASS()
class PROJECTOAT_API AOatGoalTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOatGoalTrigger();

/* Stage ------------------------------------------------------*/

/* Component ------------------------------------------------------*/
protected:
	UPROPERTY(Category=Stage, VisibleAnywhere, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;

	UFUNCTION()
	void OnGoalTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									bool bFromSweep, const FHitResult& SweepHitResult);

};
