// Fill out your copyright notice in the Description page of Project Settings.


#include "OatCharacterBase.h"

#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

#include "OatCharacterControlData.h"
#include "OatAttackActionData.h"
#include "InGame/Physics/OatCollision.h"
#include "InGame/Character/Widget/OatHpBarWidget.h"
#include "InGame/Character/Component/OatCharacterStatComponent.h"
#include "Shared/Widget/OatWidgetComponent.h"
#include "GameCommon/Items/OatItemWeaponData.h"
#include "GameData/TestData.h"



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

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ProjectOat/InGame/Character/Roxy/ABP_Roxy.ABP_Roxy_C"));
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
	// Montage Asset�� ��������Ʈ���� ����
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

	// Stat Component
	Stat = CreateDefaultSubobject<UOatCharacterStatComponent>(TEXT("Stat"));

	// Widget Component
	// BeginPlay ���� Ŭ���� ������ ���� Instance�� �����Ǵ� ���� (AnimBP�� ���)
	WidgetHpBar = CreateDefaultSubobject<UOatWidgetComponent>(TEXT("Widget"));
	WidgetHpBar->SetupAttachment(GetMesh());

	float CapsuleSize = GetCapsuleComponent()->GetScaledCapsuleHalfHeight()*2;
	WidgetHpBar->SetRelativeLocation(FVector(0.f,0.f, CapsuleSize + 30.f));

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetHpBarRef(TEXT("/Game/ProjectOat/InGame/Widget/WBP_Character_HpBar.WBP_Character_HpBar_C"));
	if (WidgetHpBarRef.Class)
	{
		WidgetHpBar->SetWidgetClass(WidgetHpBarRef.Class);
		WidgetHpBar->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetHpBar->SetDrawSize(FVector2D(150.f, 15.f));
		WidgetHpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Item Callbacks
	// �������� �����ؼ� ������� (���߿� �Ⱦ� �ڵ�)
	TakeItemCallbacks.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AOatCharacterBase::TestEquipSocket)));
	TakeItemCallbacks.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AOatCharacterBase::DrinkPotion)));

	TestSocket = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TestSocket"));
	TestSocket->SetupAttachment(GetMesh(), TEXT("L_soket1"));


}

void AOatCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// ��� ���ε��� �ص� ũ�� ��� X (�����ڳ� BeginPlay�� �����ϴ�.)
	Stat->OnHpZero.AddUObject(this, &AOatCharacterBase::SetDead);
	Stat->OnStatChanged.AddUObject(this, &AOatCharacterBase::ApplyStat);
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

	NotifyAttackActionEnd();
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

void AOatCharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	// �ݸ��� �м� �� �±� ���� , ������ ������ �浹ü�� ���� �� ��(ĸ��.�� -> Convex ) , ������ ���� (�ڱ��ڽ�)
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = Stat->GetTotalStat().AtkRange;
	const float AttackRadius = Stat->GetAttackRadius();
	const float AttackDamage = Stat->GetTotalStat().Atk;
	// ���� ���� ��ġ + ���� �ü����� + ĸ�� ������Ʈ�� ���������� �߰��ؼ� ������ ĸ�� ��ġ�������� ������
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();

	// ���� + Attack����
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	// World���� �����ϴ� ����
	// HitResult�� ������� �޾ƿ� �� ���� , ���� - �� , FCollisionShpae�� ���Ͽ� ��ü ���� ���� / 
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_OATACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}
#if ENABLE_DRAW_DEBUG
	// ĸ�� ���� : ���� + (�� - ����) / 2
	FVector CapsuleOrign = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	// ĸ���� ������ �׷���� ��
	DrawDebugCapsule(GetWorld(), CapsuleOrign, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.f);
#endif
}

float AOatCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// EventInstigator -> ������
	// DamageCasuer -> �����ڰ� ����� ����, �����ڰ� ������ ��(���� ����)
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Stat->ApplyDamage(DamageAmount);

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

void AOatCharacterBase::ApplyStat(const FTestData & BaseStat, const FTestData & ModifierStat)
{
	float MovementSpeed = (BaseStat + ModifierStat).MoveSpeed;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void AOatCharacterBase::SetUpActorWidget(UOatUserWidget* InUserWidget)
{
	UOatHpBarWidget* HpBarWidget = Cast<UOatHpBarWidget>(InUserWidget);
	if (InUserWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetTotalStat().MaxHP);
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UOatHpBarWidget::UpdateHpBar);
	}
}

void AOatCharacterBase::TakeItem(UOatItemData* InItemData)
{
	if (InItemData)
	{
		TakeItemCallbacks[(uint8)InItemData->ItemType].ItemDelegate.ExecuteIfBound(InItemData);
	}
}

void AOatCharacterBase::DrinkPotion(UOatItemData* InItemData)
{}

void AOatCharacterBase::TestEquipSocket(UOatItemData* InItemData)
{
	UOatItemWeaponData* WeaponItemData = Cast<UOatItemWeaponData>(InItemData);
	if (WeaponItemData)
	{
		if (WeaponItemData->WeaponMesh.IsPending()) { WeaponItemData->WeaponMesh.LoadSynchronous(); }
		TestSocket->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());
		Stat->SetModifierStat(WeaponItemData->ModifierStat);
	}
}

