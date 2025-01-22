// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCommon/Items/OatItemData.h"
#include "OatItemWeaponData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOAT_API UOatItemWeaponData : public UOatItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(Category=Weapon,EditAnywhere)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;
	
};
