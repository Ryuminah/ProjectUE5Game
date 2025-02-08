// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/TestData.h"
#include "OatHUDWidget.generated.h"

/**
 *
 */
 // ����Ƽ�� ġ�� UI�� �ٴ� ���� ��ũ��Ʈ ���� ���̴�~~!
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