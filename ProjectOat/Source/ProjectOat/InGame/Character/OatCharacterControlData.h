// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OatCharacterControlData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOAT_API UOatCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UOatCharacterControlData();

	/* ControlRotation ------------------------------------------------------------------*/
	// 캐릭터의 경우 Yaw값을 주로 체크
	UPROPERTY(Category = Pawn, EditAnywhere)
	uint32 bUseControllerRotaitionYaw : 1;

	UPROPERTY(Category = CharacterMovement, EditAnywhere)
	uint32 bOrientRotationToMovement: 1;

	UPROPERTY(Category = CharacterMovement, EditAnywhere)
	uint32 bUseControllerDesiredRotation: 1;

	UPROPERTY(Category = CharacterMovement, EditAnywhere)
	FRotator RotationRate;
	
	/* InputMapping ------------------------------------------------------------------*/
	UPROPERTY(Category = Input, BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	/* Camera ------------------------------------------------------------------*/
	UPROPERTY(Category = SpringArm, EditAnywhere)
	float TargetArmLength;

	UPROPERTY(Category = SpringArm, EditAnywhere)
	FRotator RelativeRotation;

	UPROPERTY(Category = SpringArm, EditAnywhere)
	uint32 bUsePawnControlRotation : 1;

	UPROPERTY(Category = SpringArm, EditAnywhere)
	uint32 bInheritPitch : 1;

	UPROPERTY(Category = SpringArm, EditAnywhere)
	uint32 bInheritYaw : 1;

	UPROPERTY(Category = SpringArm, EditAnywhere)
	uint32 bInheritRoll : 1;

	UPROPERTY(Category = SpringArm, EditAnywhere)
	uint32 bDoCollisionTest : 1;
};
