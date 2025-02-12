// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCommon/Managers/OatStageHandler.h"
#include "GameCommon/Managers/OatEventHandler.h"
#include "Shared/Enums.h"
#include "Core/OatGameInstance.h"

UOatStageHandler::UOatStageHandler()
{}

void UOatStageHandler::PostInitProperties()
{
	Super::PostInitProperties();
}

void UOatStageHandler::Initialize(UGameInstance* GameInstance)
{
	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GameInstance);
	if (!OatGameInstance)
	{
		ensure(TEXT("OatGameInstance is Null"));
		return;
	}

	OatGameInstance->GetEventHandler()->OnStageClearGoal.AddUObject(this, &UOatStageHandler::ReachedStageGoal);
	OatGameInstance->GetEventHandler()->OnStageClearDungeon.AddUObject(this, &UOatStageHandler::ClearDungeon);
	OatGameInstance->GetEventHandler()->OnGameOver.AddUObject(this, &UOatStageHandler::GameOver);
}

// ���⼭���ʹ� �ΰ��� ���� �� ȣ��Ǵ� �����̶�, GetWorld()�� �����ϴ�
void UOatStageHandler::ReachedStageGoal()
{
	// �� �������� ��
	// ���� �������� ���� ����
	// ���� ���������� �Ѱ��ش�

	// ������ ���� ����

	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GetWorld()->GetGameInstance());
	if (OatGameInstance)
	{
		OatGameInstance->QuitGame();
	}
}

void UOatStageHandler::ClearDungeon()
{
	// ���� ����
	// 1.��� 
	// 2.�ϼ�
	// 3.������
}

void UOatStageHandler::GameOver()
{
	// �κ� ������ ����������
	// ���̵� �ƿ�.. 
	// ���� ���� ���â ?
}

void UOatStageHandler::ExitLevel()
{}

void UOatStageHandler::EnterLevel()
{}
