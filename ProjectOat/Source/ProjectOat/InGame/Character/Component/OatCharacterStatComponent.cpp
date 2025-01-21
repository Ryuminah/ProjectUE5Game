// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/Component/OatCharacterStatComponent.h"

// Sets default values for this component's properties
UOatCharacterStatComponent::UOatCharacterStatComponent()
{
	MaxHp = 200.f;
}


// Called when the game starts
void UOatCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHp(MaxHp);
}

float UOatCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	
	// 데미지를 유효한 범위 조정
	const float ActualDamage = FMath::Clamp(InDamage, 0, InDamage);
	SetHp(PrevHp - ActualDamage);

	return ActualDamage;
}

void UOatCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.f, MaxHp);

	// Hp가 변경되었으니 델리게이트 실행
	OnHpChanged.Broadcast(CurrentHp);

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
		// 죽음
	}
}




