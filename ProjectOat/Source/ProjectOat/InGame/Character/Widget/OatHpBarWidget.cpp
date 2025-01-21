// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/Widget/OatHpBarWidget.h"
#include "Shared/Interface/OatActorWidgetInterface.h"
#include "Components/ProgressBar.h"


UOatHpBarWidget::UOatHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.f;
}

void UOatHpBarWidget::NativeConstruct()
{
	// UI���� ��ɵ��� ���� �ʱ�ȭ �� ����
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_Hp")));
	ensure(HpProgressBar);

	// Owning Actor
	// ������ ���Ϳ� UpdateHpBar() ��� �ʿ�
	// ���� ������Ʈ�� ���������� ���谡 �ʿ��ϴ�. �׷��� �������̽��� ���� ����

	IOatActorWidgetInterface* CharacterWidget = Cast<IOatActorWidgetInterface>(OwnerActor);
	if (OwnerActor)
	{
		CharacterWidget->SetUpActorWidget(this);
	}
}

void UOatHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
