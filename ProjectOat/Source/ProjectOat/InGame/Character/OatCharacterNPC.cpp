// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/OatCharacterNPC.h"

#include "Components/CapsuleComponent.h"
#include "Core/OatAIController.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InGame/Character/Component/OatCharacterStatComponent.h"
#include "InGame/Physics/OatCollision.h"

AOatCharacterNPC::AOatCharacterNPC()
{
	AIControllerClass = AOatAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ENEMYCAPSULE);

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

void AOatCharacterNPC::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetupCallback();
}

void AOatCharacterNPC::SetupCallback()
{
	Super::SetupCallback();
	
	AddDelegateOnAttackMonStart(FOnAttackMonStart::FDelegate::CreateUObject(this,&AOatCharacterNPC::AttackMontageBegin));
	AddDelegateOnAttackMonEnd(FOnAttackMonEnd::FDelegate::CreateUObject(this,&AOatCharacterNPC::AttackMontageEnd));
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

void AOatCharacterNPC::InputAttack()
{
	if (TryStartAttack())
	{
		Super::OnAttackStart();
		return;
	}
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
	return 6.0f;
}

void AOatCharacterNPC::SetOnBTTaskAttackFinishedDelegate(const FOnBTTaskAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}


bool AOatCharacterNPC::TryStartAttack()
{
	// 공격 가능한 상황인지 판단
	
	return true;
}

void AOatCharacterNPC::AttackMontageBegin()
{
	// 일단 만들어는 둠..
}

void AOatCharacterNPC::AttackMontageEnd()
{
	OnAttackFinished.ExecuteIfBound();
}

void AOatCharacterNPC::AnimNotifyAttackHitCheck()
{
	Super::AnimNotifyAttackHitCheck();
	
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = Stat->GetTotalStat().AtkRange;
	const float AttackRadius = Stat->GetAttackRadius();
	const float AttackDamage = Stat->GetTotalStat().Atk;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();

	const FVector End = Start + GetActorForwardVector() * AttackRange;

	// CCHANNEL_OATACTION 에 대해서 반응하는 콜리전이 있는지
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult,Start, End, FQuat::Identity, CCHANNEL_OATACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}
#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrign = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrign, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.f);
#endif
}

