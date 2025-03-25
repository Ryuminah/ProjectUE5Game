// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/OatCharacterNPC.h"

#include "Components/CapsuleComponent.h"
#include "Core/OatAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InGame/Character/Component/OatCharacterStatComponent.h"
#include "InGame/Physics/OatCollision.h"

AOatCharacterNPC::AOatCharacterNPC()
{
	AIControllerClass = AOatAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_OATCAPSULE);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ProjectOat/Arts/Characters/Cactus/SK_Enemy_Cactus_Melee.SK_Enemy_Cactus_Melee'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ProjectOat/InGame/AI/Cactus/ABP_Cactus.ABP_Cactus_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	} 

	/* Animation ----------------------------------------------------------------------------------*/
	// Montage Asset�� ��������Ʈ���� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ProjectOat/Arts/Characters/Cactus/Animations/AM_Cactus_Attack.AM_Cactus_Attack'"));
	if (ActionMontageRef.Object)
	{
		AttackMontage = ActionMontageRef.Object;
	}

	// Dead
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ProjectOat/Arts/Characters/Cactus/Animations/AM_Cactus_Dead.AM_Cactus_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}
}

void AOatCharacterNPC::SetDead()
{
	Super::SetDead();

	if (AOatAIController* OatAIController = Cast<AOatAIController>(GetController()))
	{
		OatAIController->StopAI();
	}

	// 이벤트 시간 이후 종료
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([&](){ Destroy(); }), 
										   DeadEventDelayTime, false);

}

float AOatCharacterNPC::GetAIPatrolRadius()
{
	return 800.0f;
}

float AOatCharacterNPC::GetAIDetectRange()
{
	return 400.0f;
}

float AOatCharacterNPC::GetAIAttackRange()
{
	return Stat->GetTotalStat().AtkRange+ Stat->GetAttackRadius()*2;
}

float AOatCharacterNPC::GetAITurnSpeed()
{
	return 2.0f;
}

void AOatCharacterNPC::SetAIAttackFinishedDelegate(const FAIAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AOatCharacterNPC::AttackByAI()
{
	ProcessAttack();
}

void AOatCharacterNPC::NotifyAttackActionEnd()
{
	// 부모 클래스에서 Attack Montage가 끝난 시점에 호출
	Super::NotifyAttackActionEnd();
	OnAttackFinished.ExecuteIfBound();
}

void AOatCharacterNPC::AttackActionMontageEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	Super::AttackActionMontageEnd(TargetMontage, IsProperlyEnded);
}

void AOatCharacterNPC::ProcessAttack()
{
	Super::ProcessAttack();
	
	UAnimInstance* AnimInstatnce = GetMesh()->GetAnimInstance();
	
	float AttackSpeedRate = 1.f;
	AnimInstatnce->Montage_Play(AttackMontage, AttackSpeedRate);

	// Attack 몽타주 재생
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AOatCharacterNPC::AttackActionMontageEnd);
	AnimInstatnce->Montage_SetEndDelegate(EndDelegate, AttackMontage);
	

	

}
