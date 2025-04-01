// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OatFightUnitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOatFightUnitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	전투에 관련된 애니메이션 관련 인터페이스 
 */
class PROJECTOAT_API IOatFightUnitInterface
{
	GENERATED_BODY()

	/*Montage 관련*/
protected:
	// 공격 시작 시 호출
	virtual void OnAttackStart() = 0;
	virtual void OnAttackEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded) = 0;
	
	//virtual void OnHitStart() = 0;
	//virtual void OnDeadStart() = 0;

	/*Notify 관련*/
public:
	virtual void AnimNotifyAttackHitCheck() = 0;
	// HitCheck시작 시 호출
};
