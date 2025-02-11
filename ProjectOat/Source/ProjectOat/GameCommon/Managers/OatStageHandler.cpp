// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCommon/Managers/OatStageHandler.h"
#include "GameCommon/Managers/OatEventHandler.h"

#include "Core/OatGameInstance.h"

UOatStageHandler::UOatStageHandler()
{}

void UOatStageHandler::PostInitProperties()
{
	Super::PostInitProperties();

	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GetWorld()->GetGameInstance());
	if (!OatGameInstance)
	{
		ensure(TEXT("OatGameInstance is Null"));
		return;
	}

	OatGameInstance->GetEventHandler()->OnStageClearGoal.AddUObject(this, &UOatStageHandler::ReachedStageGoal);
}

void UOatStageHandler::ReachedStageGoal()
{
	// �� �������� ��
	// ���� �������� ���� ����
	// ���� ���������� �Ѱ��ش�

	// ������ ���� ����
}
