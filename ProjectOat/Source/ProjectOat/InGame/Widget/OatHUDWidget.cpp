// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Widget/OatHUDWidget.h"
#include "InGame/Interface/OatHUDInterface.h"
#include "InGame/Character//Widget/OatCharacterStatWidget.h"
#include "InGame/Character//Widget/OatHpBarWidget.h"

UOatHUDWidget::UOatHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{}

void UOatHUDWidget::UpdateStat(const FTestData & BaseStat, const FTestData & ModifierStat)
{
	FTestData TotalStat = BaseStat + ModifierStat;
	HpBar->SetMaxHp(TotalStat.MaxHP);

	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UOatHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UOatHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UOatHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	CharacterStat = Cast<UOatCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(HpBar);
	// GetOwningPlayer() -> 컨트롤러를 가져올 수 있음
	// GetOwningPlayerPawn() -> 컨트롤러가 빙의한 폰을 가져올 수도 있음.

	IOatHUDInterface* HUDPawn = Cast<IOatHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
