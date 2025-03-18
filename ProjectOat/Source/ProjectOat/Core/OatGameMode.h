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
	// ���� ����ǰ� �ִ� ������ ������ ���ǵ�.
	// ���� ������ �����ϴ� �����͵��� ���� ��忡�� �����ϴ� ���� ����.
	UPROPERTY(Category=Game, EditDefaultsOnly, BlueprintReadWrite)
	int32 ClearScore;

	// ������ ������ Ŀ���ٸ� PlayerState�� �����ϴ� ���� ���� ���� ����
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
