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

	// Montage Asset�� �������Ʈ���� ����

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
	// ���� ��Ÿ�ְ� �������� �ƴ�
	if (CurrentCombo == 0)
	{
		AttackActionMontageBegin();
		return;
	}

	// �̹� Ÿ�̸� �ߵ� Ȥ�� üũ�� �ʿ� X
	if (!ComboTimerHandle.IsValid())
	{
		bHasNextComboCommand = false;
	}
	else
	{
		// Ŀ�ǵ� �̹� �ߵ�
		bHasNextComboCommand = true;
	}
}

void AOatCharacterBase::AttackActionMontageBegin()
{
	// ComboStatus
	CurrentCombo = 1;

	// �̵� ��� �����
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// �̵� ��� �����
	const float AttackSpeedRate = 1.f;

	// ��Ÿ�� ������ ���� AnimInstance ����
	UAnimInstance* AnimInstatnce = GetMesh()->GetAnimInstance();
	AnimInstatnce->Montage_Play(AttackMontage, AttackSpeedRate);

	// ��Ÿ�� ���� ��
	// ����üó�� ���ε��� ��������Ʈ�� ������ �� ���ڷ� �Ѱ��ָ� ��
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AOatCharacterBase::AttackActionMontageEnd);
	// Delegate����, Montage ����
	AnimInstatnce->Montage_SetEndDelegate(EndDelegate, AttackMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void AOatCharacterBase::AttackActionMontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	// ��Ÿ�� ���� �� �ʱ�ȭ
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
		// �ð��� üũ�ϵ�, �ݺ����� �ʵ��� �ѹ��� �߻��ϵ��� false
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle,this,&AOatCharacterBase::ComboCheck,ComboEffectiveTime,false);
	}
}

void AOatCharacterBase::ComboCheck()
{
	// Ÿ�̸� �ڵ� �ʱ�ȭ
	ComboTimerHandle.Invalidate();
	if (bHasNextComboCommand)
	{
		// ���� �������� üũ
		UAnimInstance* AnimInstatnce = GetMesh()->GetAnimInstance();
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, AttackActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *AttackActionData->MontageSectionNamePrefix, CurrentCombo);

		// ������ ������ ���������� ������ ���Ѿ� ��.
		AnimInstatnce->Montage_JumpToSection(NextSection, AttackMontage);


		bHasNextComboCommand = false;
		SetComboCheckTimer();
	}
}

