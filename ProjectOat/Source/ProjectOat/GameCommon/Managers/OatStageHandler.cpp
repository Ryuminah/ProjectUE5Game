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
	UOatGameInstance* GetOatGameInstance = Cast<UOatGameInstance>(GameInstance);
	if (!GetOatGameInstance)
	{
		ensure(TEXT("OatGameInstance is Null"));
		return;
	}

	this->OatGameInstance = GetOatGameInstance;

	OatGameInstance->GetEventHandler()->OnStageClearGoal.AddUObject(this, &AOatStageHandler::ReachedStageGoal);
	OatGameInstance->GetEventHandler()->OnStageClearDungeon.AddUObject(this, &AOatStageHandler::ClearDungeon);
	OatGameInstance->GetEventHandler()->OnGameOver.AddUObject(this, &AOatStageHandler::GameOver);
}

void AOatStageHandler::ReadyBattle(int SectionId) 
{
	StageData.SectionId = SectionId;
	StageData.StageSectionState = EStageSectionState::READYBATTLE;
	
	OatGameInstance->GetEventHandler()->OnStageSectionChanged.Broadcast(SectionId, EStageSectionState::READYBATTLE);
}

// ���⼱ ��� SectionId �ʿ���µ�
void AOatStageHandler::InBattle(int SectionId)
{
	StageData.StageSectionState = EStageSectionState::INBATTLE;
	
	 // ī���� ����
	OatGameInstance->GetEventHandler()->OnStageSectionChanged.Broadcast(SectionId, EStageSectionState::INBATTLE);

}

void AOatStageHandler::EndBattle(int SectionId)
{
	StageData.StageSectionState = EStageSectionState::ENDBATTLE;
	OatGameInstance->GetEventHandler()->OnStageSectionChanged.Broadcast(SectionId, EStageSectionState::ENDBATTLE);

	// ��Ÿ ��� �ʱ�ȭ
	// ��Ʋ �ڵ鷯�� ���� �� �����ѵ� ����,.,
	StageData.KillCount = 0;
	StageData.SectionWave = 0;
}

// ���� �� Ÿ�� ���� �ʿ��ұ�
void AOatStageHandler::KillEnemy()
{
	++StageData.KillCount;
}

// ���⼭���ʹ� �ΰ��� ���� �� ȣ��Ǵ� �����̶�, GetWorld()�� �����ϴ�
void AOatStageHandler::ReachedStageGoal()
{
	// �� �������� ��
	// ���� �������� ���� ����
	// ���� ���������� �Ѱ��ش�

	// ������ ���� ����
	OatGameInstance->QuitGame();
}

void AOatStageHandler::ClearDungeon()
{
	// ���� ����
	// 1.��� 
	// 2.�ϼ�
	// 3.������

	// ���ݷ� ��ȭ?
}

void AOatStageHandler::GameOver()
{
	// �κ� ������ ����������
	// ���̵� �ƿ�..
	
	// ���� ���� ���â ?
}

void AOatStageHandler::ExitLevel(ELevelType LevelType)
{
	switch (LevelType)
	{
	case ELevelType::LOBBY:
		break;
	case ELevelType::SELECT:
		break;
	case ELevelType::MAINSTAGE:
		break;
	case ELevelType::DUNGEON:
		break;
	}

}

void AOatStageHandler::EnterLevel(ELevelType LevelType)
{
	switch (LevelType)
	{
	case ELevelType::LOBBY:
		break;
	case ELevelType::SELECT:
		break;
	case ELevelType::MAINSTAGE:
		break;
	case ELevelType::DUNGEON:
		// ��� ���� ����
		break;
	}
}
