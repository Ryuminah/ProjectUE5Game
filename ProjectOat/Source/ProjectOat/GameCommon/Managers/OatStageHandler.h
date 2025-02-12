// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameCommon/Managers/OatHandlerInterface.h"
#include "OatStageHandler.generated.h"

/**
 * �������� ���࿡ ���õ� ��� ���� ����
 */
UCLASS()
class PROJECTOAT_API UOatStageHandler : public UObject, public IOatHandlerInterface
{
	GENERATED_BODY()

	friend class UOatGameInstance;

public:
	UOatStageHandler();

	virtual void PostInitProperties() override;
	virtual void Initialize(UGameInstance* GameInstance) override;

/* Stage -------------------------------------------------------------- */
private:
	void ReachedStageGoal();
	void ClearDungeon();
	void GameOver();

/* Stage -------------------------------------------------------------- */
private:
	void ExitLevel();
	void EnterLevel();
};
