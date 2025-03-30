// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/Component/OatCharacterStatComponent.h"
#include "Core/OatGameSingleton.h"

// Sets default values for this component's properties
UOatCharacterStatComponent::UOatCharacterStatComponent()
{
	CurrentLv = 1;
	AttackRadius = 50;

	// 이 변수가 true여야지 InitializeComponent()가 호출됨
	// 성능 때문에!
	bWantsInitializeComponent = true;
}


void UOatCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetLv(CurrentLv);
	SetHp(GetBaseStat().MaxHP);
}

float UOatCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	
	// 데미지를 유효한 범위 조정
	const float ActualDamage = FMath::Clamp(InDamage, 0, InDamage);
	SetHp(PrevHp - ActualDamage);

	return ActualDamage;
}

void UOatCharacterStatComponent::SetLv(int32 InLv)
{
	CurrentLv = FMath::Clamp(InLv, 1, UOatGameSingleton::Get().MaxLv);
	SetBaseStat(UOatGameSingleton::Get().GetTestData(CurrentLv));
	check(GetBaseStat().MaxHP > 0.f);
}

void UOatCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.f, UOatGameSingleton::Get().GetTestData(CurrentLv).MaxHP);

	// Hp가 변경되었으니 델리게이트 실행
	OnHpChanged.Broadcast(CurrentHp);

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
		// 죽음
	}
}




