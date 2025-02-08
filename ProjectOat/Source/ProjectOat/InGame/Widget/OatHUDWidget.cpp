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
	// GetOwningPlayer() -> ��Ʈ�ѷ��� ������ �� ����
	// GetOwningPlayerPawn() -> ��Ʈ�ѷ��� ������ ���� ������ ���� ����.

	IOatHUDInterface* HUDPawn = Cast<IOatHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
