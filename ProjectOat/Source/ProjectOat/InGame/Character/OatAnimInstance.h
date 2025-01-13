// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "OatAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class PROJECTOAT_API UOatAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:
	UOatAnimInstance();

protected:
	// ó�� ������ �� 1ȸ ȣ��
	virtual void NativeInitializeAnimation() override;

	// ������ ���� ȣ��
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

	// �����̰� �ִ���?
	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
	float MovingThreshould;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
	uint8 bIsFalling : 1;

	// �������� �ִ���?
	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
	float JumpingThreshould;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
	uint8 bIsJumping : 1;

};
