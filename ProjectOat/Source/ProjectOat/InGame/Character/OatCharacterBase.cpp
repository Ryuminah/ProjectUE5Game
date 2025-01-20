// Fill out your copyright notice in the Description page of Project Settings.


#include "OatCharacterBase.h"

#include "OatCharacterControlData.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "InGame/Physics/OatCollision.h"
#include "OatAttackActionData.h"
#include "Engine/DamageEvents.h"

// Sets default values
AOatCharacterBase::AOatCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_OATCAPSULE);

	// Movement
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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ProjectOat/Arts/Characters/Roxy/SK_Roxy.SK_Roxy'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ProjectOat/InGame/Roxy/ABP_Roxy.ABP_Roxy_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	} 

	// Control
	static ConstructorHelpers::FObjectFinder<UOatCharacterControlData> ShoulderDataRef(TEXT("/Script/ProjectOat.OatCharacterControlData'/Game/ProjectOat/Core/Systems/Input/DA_Control_Shoulder.DA_Control_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder,ShoulderDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UOatCharacterControlData> QuaterDataRef(TEXT("/Script/ProjectOat.OatCharacterControlData'/Game/ProjectOat/Core/Systems/Input/DA_Control_Quater.DA_Control_Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	/* Animation ----------------------------------------------------------------------------------*/
	// Montage Asset은 블루프린트에서 지정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ProjectOat/Arts/Characters/Roxy/Animations/AM_Roxy_ComboAttack.AM_Roxy_ComboAttack'"));
	if (ActionMontageRef.Object)
	{
		AttackMontage = ActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UOatAttackActionData> ActionDataRef(TEXT("/Script/ProjectOat.OatAttackActionData'/Game/ProjectOat/InGame/Character/DA_AttackAction.DA_AttackAction'"));
	if (ActionDataRef.Object)
	{
		AttackActionData = ActionDataRef.Object;
	}

	// Dead
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ProjectOat/Arts/Characters/Roxy/Animations/AM_Roxy_Dead.AM_Roxy_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}
}

void AOatCharacterBase::SetCharacterControlData(const UOatCharacterControlData* CharcterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharcterControlData->bUseControllerRotaitionYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharcterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharcterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharcterControlData->RotationRate;
}

void AOatCharacterBase::ProcessAttack()
{
	// 아직 몽타주가 시작중이 아님
	if (CurrentCombo == 0)
	{
		AttackActionMontageBegin();
		return;
	}

	// 이미 타이머 발동 혹은 체크할 필요 X
	if (!ComboTimerHandle.IsValid())
	{
		bHasNextComboCommand = false;
	}
	else
	{
		// 커맨드 이미 발동
		bHasNextComboCommand = true;
	}
}

void AOatCharacterBase::AttackActionMontageBegin()
{
	// ComboStatus
	CurrentCombo = 1;

	// 이동 기능 사라짐
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	const float AttackSpeedRate = 1.f;

	// 몽타주 접근을 위해 AnimInstance 접근
	UAnimInstance* AnimInstatnce = GetMesh()->GetAnimInstance();
	AnimInstatnce->Montage_Play(AttackMontage, AttackSpeedRate);

	// 몽타주 종료 시
	// 구조체처럼 바인딩할 델리게이트를 선언한 뒤 인자로 넘겨주면 됨
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AOatCharacterBase::AttackActionMontageEnd);
	// Delegate지정, Montage 정보
	AnimInstatnce->Montage_SetEndDelegate(EndDelegate, AttackMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void AOatCharacterBase::AttackActionMontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// 몽타주 종료 후 초기화
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
}

void AOatCharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(AttackActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.f;
	float ComboEffectiveTime = (AttackActionData->EffectiveFrameCount[ComboIndex] / AttackActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.f)
	{
		// 시간을 체크하되, 반복하지 않도록 한번만 발생하도록 false
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle,this,&AOatCharacterBase::ComboCheck,ComboEffectiveTime,false);
	}
}

void AOatCharacterBase::ComboCheck()
{
	// 타이머 핸들 초기화
	ComboTimerHandle.Invalidate();
	if (bHasNextComboCommand)
	{
		// 다음 섹션으로 체크
		UAnimInstance* AnimInstatnce = GetMesh()->GetAnimInstance();
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, AttackActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *AttackActionData->MontageSectionNamePrefix, CurrentCombo);

		// 지정된 프레임 이전까지는 딜레이 시켜야 함.
		AnimInstatnce->Montage_JumpToSection(NextSection, AttackMontage);


		bHasNextComboCommand = false;
		SetComboCheckTimer();
	}
}

void AOatCharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	// 콜리전 분석 시 태그 정보 , 복잡한 형태의 충돌체도 감지 할 지(캡슐.구 -> Convex ) , 무시할 액터 (자기자신)
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 40.f;
	const float AttackRadius = 50.f;
	const float AttackDamage = 30.f;
	// 현재 액터 위치 + 액터 시선방향 + 캡슐 컴포넌트의 반지름값을 추가해서 정면의 캡슐 위치에서부터 시작함
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();

	// 시작 + Attack범위
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	// World에서 제공하는 서비스
	// HitResult에 결과값을 받아올 수 있음 , 시작 - 끝 , FCollisionShpae을 통하여 구체 영역 지정 / 
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_OATACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}
#if ENABLE_DRAW_DEBUG
	// 캡슐 원점 : 시작 + (끝 - 시작) / 2
	FVector CapsuleOrign = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	// 캡슐을 눕혀서 그려줘야 함
	DrawDebugCapsule(GetWorld(), CapsuleOrign, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.f);
#endif
}

float AOatCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// EventInstigator -> 가해자
	// DamageCasuer -> 가해자가 사용한 무기, 가해자가 빙의한 폰(액터 정보)
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	static float TotalDamage = 0;
	TotalDamage += DamageAmount;

	if (TotalDamage >= 150.f)
	{
		SetDead();
	}

	// ex. 스탯 정보가 있을 경우 해당 함수에서 연산해서 최종 값으로 리턴
	return DamageAmount;
}

void AOatCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnim();
	SetActorEnableCollision(false);
}

void AOatCharacterBase::PlayDeadAnim()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.f);
	AnimInstance->Montage_Play(DeadMontage, 1.f);
}

