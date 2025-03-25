// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OatAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOatAIInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAIAttackFinished);

/**
 * 
 */
class PROJECTOAT_API IOatAIInterface
{
	GENERATED_BODY()

public:
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;

	virtual void SetAIAttackFinishedDelegate(const FAIAttackFinished& InOnAttackFinished) = 0;
	virtual void AttackByAI() = 0;

};
