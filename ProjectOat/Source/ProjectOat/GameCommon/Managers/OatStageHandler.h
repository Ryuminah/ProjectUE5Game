// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCommon/Managers/OatHandlerInterface.h"
#include "GameData/OatStageData.h"
#include "Shared/Enums.h"
#include "OatStageHandler.generated.h"

/**
 * �������� ���࿡ ���õ� ��� ���� ����
 */
UCLASS()
class PROJECTOAT_API AOatStageHandler : public AActor, public IOatHandlerInterface
{
	GENERATED_BODY()

	friend class UOatGameInstance;

public:
	AOatStageHandler();

	virtual void PostInitProperties() override;
	virtual void Initialize(UGameInstance* GameInstance) override;

protected:
	TObjectPtr<class UOatGameInstance> OatGameInstance;	

/* Stage -------------------------------------------------------------- */
public:
	// 유저의 현재 스테이지 정보
	UPROPERTY(Category=Stage, VisibleInstanceOnly)
	FOatStageData StageData;

	// Section은 1부터로 바꿀까..
public:
	void ReadyBattle(int SectionId);
	void InBattle(int SectionId);
	void EndBattle(int SectionId);
	
private:
	void KillEnemy();
	
	
	// 게임의 승패
private:
	void ReachedStageGoal();
	void ClearDungeon();
	void GameOver();

	// 다음 섹션 관련 관리해야함

/* Level -------------------------------------------------------------- */
private:
	void ExitLevel(ELevelType LevelType);
	void EnterLevel(ELevelType LevelType);
	
};
