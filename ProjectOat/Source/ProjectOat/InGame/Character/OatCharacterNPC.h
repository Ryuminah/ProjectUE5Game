// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGame/Character/OatCharacterBase.h"
#include "InGame/AI/Interface/OatAIInterface.h"
#include "OatCharacterNPC.generated.h"

/**
 *
 */
UCLASS()
class PROJECTOAT_API AOatCharacterNPC : public AOatCharacterBase, public IOatAIInterface
{
	GENERATED_BODY()

public:
	AOatCharacterNPC();

public:
	void SetDead() override;

	/* AI ---------------------------------------------------------------*/
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

};
