// Fill out your copyright notice in the Description page of Project Settings.


#include "OatFightUnitBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

#include "InGame/Character/Widget/OatHpBarWidget.h"
#include "InGame/Character/Component/OatCharacterStatComponent.h"
#include "Shared/Widget/OatWidgetComponent.h"
#include "GameData/TestData.h"



// Sets default values
AOatFightUnitBase::AOatFightUnitBase()
{
	// Stat Component
	Stat = CreateDefaultSubobject<UOatCharacterStatComponent>(TEXT("Stat"));

	// Widget Component
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

}

void AOatFightUnitBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AOatFightUnitBase::SetDead);
	Stat->OnStatChanged.AddUObject(this, &AOatFightUnitBase::ApplyStat);
}

void AOatFightUnitBase::OnAttackStart()
{
	// 어택 몽타주 시작 콜백 호출
	float AttackSpeedRate = 1.f;
	UAnimInstance* AnimInstatnce = GetMesh()->GetAnimInstance();
	AnimInstatnce->Montage_Play(AttackMontage, AttackSpeedRate);
	OnAttackMonStart.Broadcast();
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AOatFightUnitBase::OnAttackEnd);
	AnimInstatnce->Montage_SetEndDelegate(EndDelegate, AttackMontage);
}

// 우선 BindLambda로
void AOatFightUnitBase::OnAttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	OnAttackMonEnd.Broadcast();	
	//AttackMonEnd();
}

void AOatFightUnitBase::StopCurrentMontage() const
{
	UAnimInstance* AnimInstatnce = GetMesh()->GetAnimInstance();
	AnimInstatnce->Montage_Pause();
}

void AOatFightUnitBase::AnimNotifyAttackHitCheck()
{

}

float AOatFightUnitBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Stat->ApplyDamage(DamageAmount);
	
	UAnimInstance* AnimInstatnce = GetMesh()->GetAnimInstance();

	// 바로 죽음 처리
	if (AnimInstatnce->Montage_IsPlaying(DeadMontage))
	{
		return DamageAmount;
	}
	
	// CC기 걸린 상황 체크하기
	// Hit                       
	StopCurrentMontage();

	AnimInstatnce->Montage_Play(HitMontage, 1.f);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindWeakLambda(this,[&](UAnimMontage* Montage, bool bInterrupted) {GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);});
	AnimInstatnce->Montage_SetEndDelegate(EndDelegate, HitMontage);
	
	return DamageAmount;
}

void AOatFightUnitBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnim();
	SetActorEnableCollision(false);
}

void AOatFightUnitBase::PlayDeadAnim()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.f);
	AnimInstance->Montage_Play(DeadMontage, 1.f);
}

void AOatFightUnitBase::ApplyStat(const FTestData & BaseStat, const FTestData & ModifierStat)
{
	float MovementSpeed = (BaseStat + ModifierStat).MoveSpeed;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void AOatFightUnitBase::SetUpActorWidget(UOatUserWidget* InUserWidget)
{
	UOatHpBarWidget* HpBarWidget = Cast<UOatHpBarWidget>(InUserWidget);
	if (InUserWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetTotalStat().MaxHP);
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UOatHpBarWidget::UpdateHpBar);
	}
}

// void AOatFightUnitBase::TakeItem(UOatItemData* InItemData)
// {
// 	if (InItemData)
// 	{
// 		TakeItemCallbacks[(uint8)InItemData->ItemType].ItemDelegate.ExecuteIfBound(InItemData);
// 	}
// }

// void AOatFightUnitBase::DrinkPotion(UOatItemData* InItemData)
// {}

// void AOatFightUnitBase::TestEquipSocket(UOatItemData* InItemData)
// {
// 	UOatItemWeaponData* WeaponItemData = Cast<UOatItemWeaponData>(InItemData);
// 	if (WeaponItemData)
// 	{
// 		if (WeaponItemData->WeaponMesh.IsPending()) { WeaponItemData->WeaponMesh.LoadSynchronous(); }
// 		TestSocket->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());
// 		Stat->SetModifierStat(WeaponItemData->ModifierStat);
// 	}
// }

