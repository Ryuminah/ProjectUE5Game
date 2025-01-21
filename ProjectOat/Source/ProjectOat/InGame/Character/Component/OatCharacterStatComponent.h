// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OatCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate,float /*CurrentHp*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTOAT_API UOatCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOatCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	float ApplyDamage(float InDamage);

	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

protected:
	// ��ġ�� ĳ���͸��� �ٸ� ���� ������ �ֱ� ������ �ν��Ͻ����� ������ ����
	UPROPERTY(category=Stat,VisibleInstanceOnly)
	float MaxHp;

	// Transient -> ��ũ�� �������� �ʵ��� �Ͽ� ���ʿ��� ���� ���� ����
	UPROPERTY(category=Stat, Transient, VisibleInstanceOnly)
	float CurrentHp;	

	void SetHp(float NewHp);
};
