// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameCommon/Managers/OatHandlerInterface.h"
#include "Shared/Enums.h"
#include "OatEventHandler.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnStageClearGoalDelegate);
DECLARE_MULTICAST_DELEGATE(FOnStageClearDungeonDelegate);
DECLARE_MULTICAST_DELEGATE(FOnGameOver);
DECLARE_MULTICAST_DELEGATE(FOnQuitGame);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectSceneDelegate, /*const ELevelType*/ );


UCLASS()
class PROJECTOAT_API AOatEventHandler : public AActor, public IOatHandlerInterface
{
	GENERATED_BODY()

	friend class UOatGameInstance;

public:
	virtual void Initialize(UGameInstance* GameInstance) override;

public:
	// 스테이지에서 골 도달 시 (게임 클리어와 같다)
	FOnStageClearGoalDelegate OnStageClearGoal;
	FOnStageClearDungeonDelegate OnStageClearDungeon;

	// 캐릭터 사망 or 타임 오버 시
	FOnGameOver OnGameOver;

	// 게임 종료
	FOnQuitGame OnQuitGame;

};

