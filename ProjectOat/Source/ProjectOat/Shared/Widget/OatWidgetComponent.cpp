// Fill out your copyright notice in the Description page of Project Settings.


#include "Shared/Widget/OatWidgetComponent.h"
#include "OatUserWidget.h"

// ������ ���� �ν��Ͻ��� ������ ���Ŀ� ȣ��Ǵ� �Լ�
void UOatWidgetComponent::InitWidget()
{
	// Super���� ������ ���� �ν��Ͻ��� ������.
	Super::InitWidget();

	UOatUserWidget* OatUserWidget = Cast<UOatUserWidget>(GetWidget());
	if (OatUserWidget)
	{
		// ������Ʈ�� �����ϰ� �ִ� ���� ������ ����
		OatUserWidget->SetOwnerActor(GetOwner());
	}
}
