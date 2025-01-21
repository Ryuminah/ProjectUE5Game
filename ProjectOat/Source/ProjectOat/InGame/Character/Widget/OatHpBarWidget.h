// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Widget/OatUserWidget.h"

#include "OatHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOAT_API UOatHpBarWidget : public UOatUserWidget
{
	GENERATED_BODY()

public:
	// UserWidget�� ��� ���ڰ� �ִ� �����ڸ� ����
	UOatHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	// ���� ������ ����
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;
	
};
