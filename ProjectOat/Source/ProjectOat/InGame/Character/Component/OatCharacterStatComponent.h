// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/TestData.h"
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
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	float ApplyDamage(float InDamage);


	FORCEINLINE float GetCurrentLv() const { return CurrentHp; }
	void SetLvStat(int32 InLv);

	FORCEINLINE FTestData GetTotalStat() const { return BaseStat + ModifierStat; }
	void SetModifierStat(const FTestData& InModifierStat) { ModifierStat = InModifierStat; };


protected:
	// Transient -> 디스크에 저장하지 않도록 하여 불필요한 공간 낭비를 줄임
	UPROPERTY(category=Stat, Transient, VisibleInstanceOnly)
	float CurrentHp;	

	void SetHp(float NewHp);

		// Transient -> 디스크에 저장하지 않도록 하여 불필요한 공간 낭비를 줄임
	UPROPERTY(category=Stat, Transient, VisibleInstanceOnly)
	float CurrentLv;	

	UPROPERTY(category=Stat, Transient, VisibleInstanceOnly, meta=(AllowPrivateAccess="true"))
	FTestData BaseStat;

	UPROPERTY(category=Stat, Transient, VisibleInstanceOnly, meta=(AllowPrivateAccess="true"))
	FTestData ModifierStat;

};
