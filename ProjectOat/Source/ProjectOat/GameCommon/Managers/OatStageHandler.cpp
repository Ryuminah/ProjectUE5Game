// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCommon/Managers/OatStageHandler.h"
#include "GameCommon/Managers/OatEventHandler.h"
#include "Shared/Enums.h"
#include "Core/OatGameInstance.h"

AOatStageHandler::AOatStageHandler()
{}

void AOatStageHandler::PostInitProperties()
{
	Super::PostInitProperties();
}

void AOatStageHandler::Initialize(UGameInstance* GameInstance)
{
	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GameInstance);
	if (!OatGameInstance)
	{
		ensure(TEXT("OatGameInstance is Null"));
		return;
	}

	OatGameInstance->GetEventHandler()->OnStageClearGoal.AddUObject(this, &AOatStageHandler::ReachedStageGoal);
	OatGameInstance->GetEventHandler()->OnStageClearDungeon.AddUObject(this, &AOatStageHandler::ClearDungeon);
	OatGameInstance->GetEventHandler()->OnGameOver.AddUObject(this, &AOatStageHandler::GameOver);
}

// ���⼭���ʹ� �ΰ��� ���� �� ȣ��Ǵ� �����̶�, GetWorld()�� �����ϴ�
void AOatStageHandler::ReachedStageGoal()
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

void AOatStageHandler::ClearDungeon()
{
	// ���� ����
	// 1.��� 
	// 2.�ϼ�
	// 3.������
}

void AOatStageHandler::GameOver()
{
	// �κ� ������ ����������
	// ���̵� �ƿ�.. 
	// ���� ���� ���â ?
}

void AOatStageHandler::ExitLevel()
{}

void AOatStageHandler::EnterLevel()
{}
