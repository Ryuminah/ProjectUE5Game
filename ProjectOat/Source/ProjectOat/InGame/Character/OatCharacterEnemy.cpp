// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/OatCharacterEnemy.h"

#include "Components/CapsuleComponent.h"
#include "Core/OatAIController.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InGame/AI/EnemyBaseAnimInstance.h"
#include "InGame/Character/Component/OatCharacterStatComponent.h"
#include "InGame/Physics/OatCollision.h"

AOatCharacterEnemy::AOatCharacterEnemy()
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

	// Spawn
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SpawnMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ProjectOat/Arts/Characters/Cactus/Animations/AM_Enemy_Cactus_Spawn.AM_Enemy_Cactus_Spawn'"));
	if (SpawnMontageRef.Object)
	{
		SpawnMontage = SpawnMontageRef.Object;
	}

	// Hit
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ProjectOat/Arts/Characters/Cactus/Animations/AM_Enemy_Cactus_Hit.AM_Enemy_Cactus_Hit'"));
	if (HitMontageRef.Object)
	{
		HitMontage = HitMontageRef.Object;
	}
}

void AOatCharacterEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetupCallback();
}

void AOatCharacterEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 첫 생성시
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	OnSpawnStart();
}

void AOatCharacterEnemy::SetupCallback()
{
	Super::SetupCallback();
	
	AddDelegateOnAttackMonStart(FOnAttackMonStart::FDelegate::CreateUObject(this,&AOatCharacterEnemy::AttackMontageBegin));
	AddDelegateOnAttackMonEnd(FOnAttackMonEnd::FDelegate::CreateUObject(this,&AOatCharacterEnemy::AttackMontageEnd));
}

void AOatCharacterEnemy::Reset()
{
	Super::Reset();
}

void AOatCharacterEnemy::SetDead()
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

void AOatCharacterEnemy::InputAttack()
{
	if (TryStartAttack())
	{
		Super::OnAttackStart();
		return;
	}
}

float AOatCharacterEnemy::GetAIPatrolRadius()
{
	return 800.0f;
}

float AOatCharacterEnemy::GetAIDetectRange()
{
	return 400.0f;
}

float AOatCharacterEnemy::GetAIAttackRange()
{
	return Stat->GetTotalStat().AtkRange+ Stat->GetAttackRadius()*2;
}

float AOatCharacterEnemy::GetAITurnSpeed()
{
	return 6.0f;
}

void AOatCharacterEnemy::SetOnBTTaskAttackFinishedDelegate(const FOnBTTaskAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}


bool AOatCharacterEnemy::TryStartAttack()
{
	// 공격 가능한 상황인지 판단
	
	return true;
}

void AOatCharacterEnemy::AttackMontageBegin()
{
	// 일단 만들어는 둠..
}

void AOatCharacterEnemy::AttackMontageEnd()
{
	OnAttackFinished.ExecuteIfBound();
}

void AOatCharacterEnemy::AnimNotifyAttackHitCheck()
{
	Super::AnimNotifyAttackHitCheck();
	
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = Stat->GetTotalStat().AtkRange;
	const float AttackRadius = Stat->GetAttackRadius();
	const float AttackDamage = Stat->GetTotalStat().Atk * 0.5f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();

	const FVector End = Start + GetActorForwardVector() * AttackRange;

	// EnemyAction 에 대해서 반응하는 콜리전이 있는지
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult,Start, End, FQuat::Identity, CCHANNEL_ENEMYACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
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

void AOatCharacterEnemy::AnimNotifySpawnEnd()
{
	UEnemyBaseAnimInstance* AnimInstance = Cast<UEnemyBaseAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance)
	{
		// Debug
		return;
	}

	AnimInstance->bIsSpawn = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

}

void AOatCharacterEnemy::OnSpawnStart()
{
	// 움직임을 막는다

	if (UAnimInstance* AnimInstatnce = GetMesh()->GetAnimInstance())
	{
		float SpeedRate = 1.f;
		AnimInstatnce->Montage_Play(SpawnMontage, SpeedRate);
	}
	
//	FOnMontageEnded EndDelegate;
//	EndDelegate.BindUObject(this, &AOatFightUnitBase::OnAttackEnd);
//	AnimInstatnce->Montage_SetEndDelegate(EndDelegate, AttackMontage);
}

