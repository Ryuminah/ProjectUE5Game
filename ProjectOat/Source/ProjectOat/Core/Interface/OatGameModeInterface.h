// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OatGameModeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOatGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * GameInterface
 */
class PROJECTOAT_API IOatGameModeInterface
{
	GENERATED_BODY()

public:
	virtual void GameOver() = 0;
	virtual void GameClear() = 0;

	virtual bool IsGameCleared() = 0;
	virtual void  OnPlayerDead() = 0;
};
