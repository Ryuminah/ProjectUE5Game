// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGame/Character/OatFightUnitBase.h"
#include "InGame/AI/Interface/OatAIInterface.h"
#include "OatCharacterNPC.generated.h"

/**
 *
 */
UCLASS()
class PROJECTOAT_API AOatCharacterNPC : public AOatFightUnitBase, public IOatAIInterface
{
	GENERATED_BODY()

public:
	AOatCharacterNPC();

protected:
	virtual void PostInitializeComponents() override;
	virtual void SetupCallback() override;

public:
	virtual void SetDead() override;

	// Attack 상태
	void InputAttack();
	void SetOnBTTaskAttackFinishedDelegate(const FOnBTTaskAttackFinished& InOnAttackFinished);

/* Get Set ---------------------------------------------------------------*/
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	// 몽타주가 끝난 시점을 BTTask_Attack의 공격 성공으로 반환하기 위해

/* Attack ---------------------------------------------------------------*/
private:
	FOnBTTaskAttackFinished OnAttackFinished;

private:
	bool TryStartAttack();

	// CallbackFunc
	virtual void AttackMontageBegin() override;
	virtual void AttackMontageEnd() override;
	virtual void AnimNotifyAttackHitCheck() override;

};
