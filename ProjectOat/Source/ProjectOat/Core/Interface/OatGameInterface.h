// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OatGameInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOatGameInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTOAT_API IOatGameInterface
{
	GENERATED_BODY()

public:
	virtual bool IsGameCleared() = 0;
	virtual void  OnPlayerDead() = 0;

};
