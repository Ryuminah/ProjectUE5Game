// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/TestData.h"
#include "OatHUDWidget.generated.h"

/**
 *
 */
 // 유니티로 치면 UI에 붙는 전용 스크립트 같은 것이다~~!
UCLASS()
class PROJECTOAT_API UOatHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOatHUDWidget(const FObjectInitializer& ObjectInitializer);
	
public:
	void UpdateStat(const FTestData& BaseStat, const FTestData& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);
		

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UOatHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UOatCharacterStatWidget> CharacterStat;
};