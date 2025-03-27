// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OatAnimFightInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOatAnimFightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	전투에 관련된 애니메이션 관련 인터페이스 
 */
class PROJECTOAT_API IOatAnimFightInterface
{
	GENERATED_BODY()

public:
	// 공격 시작 시 호출
	virtual void OnAttackStart() = 0;
	                            
	virtual void OnAttackHitCheck() = 0;
};
