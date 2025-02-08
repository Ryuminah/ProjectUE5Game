// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/Component/OatCharacterStatComponent.h"
#include "Core/OatGameSingleton.h"

// Sets default values for this component's properties
UOatCharacterStatComponent::UOatCharacterStatComponent()
{
	CurrentLv = 1;
	AttackRadius = 50;

	// �� ������ true������ InitializeComponent()�� ȣ���
	// ���� ������!
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
	
	// �������� ��ȿ�� ���� ����
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

	// Hp�� ����Ǿ����� ��������Ʈ ����
	OnHpChanged.Broadcast(CurrentHp);

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
		// ����
	}
}




