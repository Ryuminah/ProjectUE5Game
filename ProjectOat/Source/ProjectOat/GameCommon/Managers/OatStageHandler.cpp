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

// 여기선 사실 SectionId 필요없는디
void AOatStageHandler::InBattle(int SectionId)
{
	StageData.StageSectionState = EStageSectionState::INBATTLE;
	
	 // 카운팅 시작
	OatGameInstance->GetEventHandler()->OnStageSectionChanged.Broadcast(SectionId, EStageSectionState::INBATTLE);

}

void AOatStageHandler::EndBattle(int SectionId)
{
	StageData.StageSectionState = EStageSectionState::ENDBATTLE;
	OatGameInstance->GetEventHandler()->OnStageSectionChanged.Broadcast(SectionId, EStageSectionState::ENDBATTLE);

	// 기타 등등 초기화
	// 배틀 핸들러가 좋을 것 같긴한데 굳이,.,
	StageData.KillCount = 0;
	StageData.SectionWave = 0;
}

// 죽인 적 타입 정보 필요할까
void AOatStageHandler::KillEnemy()
{
	++StageData.KillCount;
}

// 여기서부터는 인게임 실행 뒤 호출되는 로직이라서, GetWorld()도 안전하다
void AOatStageHandler::ReachedStageGoal()
{
	// 골에 도달했을 때
	// 현재 스테이지 정보 갱신
	// 다음 스테이지로 넘겨준다

	// 지금은 게임 종료
	OatGameInstance->QuitGame();
}

void AOatStageHandler::ClearDungeon()
{
	// 정산 보상
	// 1.대시 
	// 2.일섬
	// 3.발차기

	// 공격력 강화?
}

void AOatStageHandler::GameOver()
{
	// 로비 씬으로 돌려보내기
	// 페이드 아웃..
	
	// 씬에 따라 결과창 ?
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
		// 몇레벨 무슨 던전
		break;
	}
}
