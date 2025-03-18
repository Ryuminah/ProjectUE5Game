// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/Interface/OatGameInterface.h"
#include "GameData/OatStageData.h"
#include "Shared/Enums.h"
#include "OatGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOAT_API AOatGameMode : public AGameModeBase , public IOatGameInterface
{
	GENERATED_BODY()

public:
	AOatGameMode();

public:
	// 현재 진행되고 있는 게임의 점수와 조건들.
	// 게임 진행을 결정하는 데이터들은 게임 모드에서 보관하는 것이 좋음.
	UPROPERTY(Category=Game, EditDefaultsOnly, BlueprintReadWrite)
	int32 ClearScore;

	// 게임의 볼륨이 커진다면 PlayerState에 보관하는 것이 좋을 수도 있음
	UPROPERTY(Category=Game, VisibleInstanceOnly, BlueprintReadOnly)
	int32 CurrentScore;

	UPROPERTY(Category=Game, VisibleInstanceOnly, BlueprintReadOnly)
	uint8 bIsCleared : 1;
	
/* Game Core -------------------------------------------------*/
public:
	virtual void GameOver() override;
	virtual void GameClear() override;

	virtual bool IsGameCleared() override;
	virtual void  OnPlayerDead() override;

/* System Core ------------------------------------------------*/


};
