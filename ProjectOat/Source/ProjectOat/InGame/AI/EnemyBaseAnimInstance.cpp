// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/AI/EnemyBaseAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UEnemyBaseAnimInstance::UEnemyBaseAnimInstance()
{
	MovingThreshould = 3.f;
	JumpingThreshould = 1.f;
}

void UEnemyBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UEnemyBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		// z값 제외
		GroundSpeed = Velocity.Size2D();

		bIsIdle = (GroundSpeed < MovingThreshould) & !bIsFalling;
		bIsFalling = Movement->IsFalling();
	}
}
