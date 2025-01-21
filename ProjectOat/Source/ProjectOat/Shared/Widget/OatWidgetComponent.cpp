// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/Widget/OatWidgetComponent.h"
#include "OatUserWidget.h"

// 위젯에 대한 인스턴스가 생성된 직후에 호출되는 함수
void UOatWidgetComponent::InitWidget()
{
	// Super이후 위젯에 대한 인스턴스가 생성됨.
	Super::InitWidget();

	UOatUserWidget* OatUserWidget = Cast<UOatUserWidget>(GetWidget());
	if (OatUserWidget)
	{
		// 컴포넌트가 소유하고 있는 액터 정보를 설정
		OatUserWidget->SetOwnerActor(GetOwner());
	}
}
