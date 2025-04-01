// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOAT_API UEnemyBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEnemyBaseAnimInstance();

protected:
	// 처음 생성될 때 1회 호츨
	virtual void NativeInitializeAnimation() override;

	// 프레임 마다 호출
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(Category=Character, EditAnywhere, BlueprintReadOnly)
	float GroundSpeed;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
	uint8 bIsIdle : 1;

	// 움직이고 있는지?
	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
	float MovingThreshould;
	
	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
	uint8 bIsFalling : 1;

	// 떨어지고 있는지?
	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
	float JumpingThreshould;


public:
	// SpawnEnd 호출 이후 true 
	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
	uint8 bIsSpawn : 1;


};
