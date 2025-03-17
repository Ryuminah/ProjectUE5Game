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
	// ������������ �� ���� �� (���� Ŭ����� ����)
	FOnStageClearGoalDelegate OnStageClearGoal;
	FOnStageClearDungeonDelegate OnStageClearDungeon;

	// ĳ���� ��� or Ÿ�� ���� ��
	FOnGameOver OnGameOver;

	// ���� ����
	FOnQuitGame OnQuitGame;

};

