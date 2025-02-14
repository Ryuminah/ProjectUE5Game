// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OatCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOatCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTOAT_API IOatCombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Dead() = 0;
	virtual void Attack() = 0;
	virtual void Damaged() = 0;
};
