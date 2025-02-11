// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OatEventHandler.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnStageClearGoalDelegate);

UCLASS()
class PROJECTOAT_API UOatEventHandler : public UObject
{
	GENERATED_BODY()

public:
	// 스테이지에서 골 도달 시
	FOnStageClearGoalDelegate OnStageClearGoal;

};
