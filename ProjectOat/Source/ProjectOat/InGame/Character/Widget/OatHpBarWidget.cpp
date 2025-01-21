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
	// UI관련 기능들이 전부 초기화 된 이후
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_Hp")));
	ensure(HpProgressBar);

	// Owning Actor
	// 소유한 액터에 UpdateHpBar() 등록 필요
	// 위젯 컴포넌트는 범용적으로 설계가 필요하다. 그래서 인터페이스를 통해 설계

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
