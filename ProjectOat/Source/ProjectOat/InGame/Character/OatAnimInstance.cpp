// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/OatAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UOatAnimInstance::UOatAnimInstance()
{
	MovingThreshould = 3.f;
	JumpingThreshould = 1.f;
}

void UOatAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UOatAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		// z값 제외
		GroundSpeed = Velocity.Size2D();

		bIsIdle = (GroundSpeed < MovingThreshould) & !bIsFalling & !bIsJumping;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}
}
