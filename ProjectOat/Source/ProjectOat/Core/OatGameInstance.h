// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Shared/Enums.h"
#include "OatGameInstance.generated.h"

/**
 * ���α׷� ������� ���� �� ���� �����Ǵ� ���� ��ü
 */
UCLASS()
class PROJECTOAT_API UOatGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UOatGameInstance();

	virtual void Init() override;
	virtual void PostInitProperties() override;

private:
	void SetupGameEventBindings();

public:
/* Getter ----------------------------------------------------------------------------------------------------*/
	class AOatStageHandler* GetStageHandler() { return StageHandler; }
	class AOatEventHandler* GetEventHandler() { return EventHandler; }

	ELevelType GetCurrentLevel() { return CurrentLevelType; }

/* Handler --------------------------------------------------------*/
private:
	UPROPERTY()
	TObjectPtr<class AOatStageHandler> StageHandler;
		
	// ������Ʈ Ǯ
	
	// ��������Ʈ �ڵ鷯
	UPROPERTY()
	TObjectPtr<class AOatEventHandler> EventHandler;

/* GameCore ----------------------------------------------------------------------------------------------------*/
	// Level ----------------------------------------------------
private:
	ELevelType CurrentLevelType;

public:
	void TravelLevel(ELevelType LevelType);
	void QuitGame();

};
