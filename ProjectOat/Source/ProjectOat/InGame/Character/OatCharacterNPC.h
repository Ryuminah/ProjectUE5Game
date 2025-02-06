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

	// 몽타주가 끝난 시점을 BTTask_Attack의 공격 성공으로 반환하기 위해
	virtual void SetAIAttackDelegate(const FAIAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;
	virtual void NotifyAttackActionEnd() override;

	FAIAttackFinished OnAttackFinished;

};
