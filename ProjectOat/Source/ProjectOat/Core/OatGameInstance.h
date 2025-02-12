// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Shared/Enums.h"
#include "OatGameInstance.generated.h"

/**
 * 
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
	class UOatStageHandler* GetStageHandler() { return StageHandler; }
	class UOatEventHandler* GetEventHandler() { return EventHandler; }

	ELevelType GetCurrentLevel() { return CurrentLevelType; }

/* Handler --------------------------------------------------------*/
private:
	UPROPERTY()
	TObjectPtr<class UOatStageHandler> StageHandler;
		
	// 오브젝트 풀
	
	// 델리게이트 핸들러
	UPROPERTY()
	TObjectPtr<class UOatEventHandler> EventHandler;

/* GameCore ----------------------------------------------------------------------------------------------------*/
	// Level ----------------------------------------------------
private:
	ELevelType CurrentLevelType;

public:
	void TravelLevel(ELevelType LevelType);
	void QuitGame();

};
