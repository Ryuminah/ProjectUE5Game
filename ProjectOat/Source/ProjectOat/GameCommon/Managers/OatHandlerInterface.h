// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OatHandlerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOatHandlerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTOAT_API IOatHandlerInterface
{
	GENERATED_BODY()

// 월드 생성 이후
public:
	virtual void Initialize(UGameInstance* GameInstance) = 0;
};
