// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Shared/Enums.h"
#include "OatGameInstance.generated.h"

/**
 * 프로그램 실행부터 종료 시 까지 유지되는 전역 객체
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

	ELevelType GetCurrentLevel() const { return CurrentLevelType; } 

/* Handler --------------------------------------------------------*/
private:
	UPROPERTY()
	TObjectPtr<class AOatStageHandler> StageHandler;
		
	// 오브젝트 풀
	
	// 델리게이트 핸들러
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
