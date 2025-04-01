// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGame/Character/OatFightUnitBase.h"
#include "InGame/AI/Interface/OatAIInterface.h"
#include "OatCharacterEnemy.generated.h"

/**
 *
 */
UCLASS()
class PROJECTOAT_API AOatCharacterEnemy : public AOatFightUnitBase, public IOatAIInterface
{
	GENERATED_BODY()

public:
	AOatCharacterEnemy();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
	virtual void SetupCallback() override;
	virtual void Reset();

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


/* Hit ---------------------------------------------------------------*/
//	virtual void AttackMontageBegin() override;
//	virtual void AttackMontageEnd() override;

	
/* Spawn ---------------------------------------------------------------*/
public:
	// Spawn이 끝났을 시점에 호출
	void AnimNotifySpawnEnd();
	
	// 모든 전투 유닛의 Attack의 시작 시점에 호출해주기        
	void OnSpawnStart();      
	//virtual void OnSpawnEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded) final override;
	
protected:
	UPROPERTY(Category=Animation, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> SpawnMontage;


};
