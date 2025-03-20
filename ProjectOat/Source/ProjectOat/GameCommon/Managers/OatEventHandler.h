// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCommon/Managers/OatHandlerInterface.h"
#include "Shared/Enums.h"
#include "OatEventHandler.generated.h"

/**
 * 
 */

// Stage
DECLARE_MULTICAST_DELEGATE(FOnStageClearGoalDelegate);
DECLARE_MULTICAST_DELEGATE(FOnStageClearDungeonDelegate);
DECLARE_MULTICAST_DELEGATE(FOnGameOver);
DECLARE_MULTICAST_DELEGATE(FOnQuitGame);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectSceneDelegate, /*const ELevelType*/);

// InGame
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStageSectionChangedDelegate, const int/*SectionID*/,
                                     const EStageSectionState/*bool InBattle*/);

// 게임 내 모든 이벤트는 여기서 관리
UCLASS()
class PROJECTOAT_API AOatEventHandler : public AActor, public IOatHandlerInterface
{
	GENERATED_BODY()

	friend class UOatGameInstance;
	friend class AOatStageHandler;

public:
	virtual void Initialize(UGameInstance* GameInstance) override;

public:
	// 스테이지 클리어의 경우
	FOnStageClearGoalDelegate OnStageClearGoal;
	FOnStageClearDungeonDelegate OnStageClearDungeon;

	FOnGameOver OnGameOver; //게임 패배			


	FOnQuitGame OnQuitGame; //게임 종료시


private: FOnStageSectionChangedDelegate OnStageSectionChanged;
public: void AddStageSectionChangedDelegate(UObject* Object, void (UObject::*Function)(int, EStageSectionState));
	
};
