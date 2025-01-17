// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OatAttackActionData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOAT_API UOatAttackActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(Category=Name, EditAnywhere)
	FString MontageSectionNamePrefix;

	UPROPERTY(Category=Name, EditAnywhere)
	uint8 MaxComboCount;
	
	UPROPERTY(Category=Name, EditAnywhere)
	float FrameRate;
	
	UPROPERTY(Category=ComboData, EditAnywhere)
	TArray<float> EffectiveFrameCount;
};
