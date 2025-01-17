// Fill out your copyright notice in the Description page of Project Settings.


#include "OatCharacterBase.h"
#include "OatCharacterControlData.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "OatAttackActionData.h"

// Sets default values
AOatCharacterBase::AOatCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

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
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

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

	// Montage Asset은 블루프린트에서 지정

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

	// 이동 기능 사라짐
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

