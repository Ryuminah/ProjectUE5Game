// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameData/TestData.h"
#include "OatItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType :uint8
{
	Weapon = 0,
	Potion,
	Scroll
};

/**
 * 
 */
UCLASS()
class PROJECTOAT_API UOatItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(Category=ItemType, EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	UPROPERTY(Category=Stat, EditAnywhere)
	FTestData ModifierStat;
	
};
