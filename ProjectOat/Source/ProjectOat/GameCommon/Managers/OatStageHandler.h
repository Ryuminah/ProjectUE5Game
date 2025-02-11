// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OatStageHandler.generated.h"

/**
 * 스테이지 진행에 관련된 모든 것을 관리
 */
UCLASS()
class PROJECTOAT_API UOatStageHandler : public UObject
{
	GENERATED_BODY()

public:
	UOatStageHandler();

	virtual void PostInitProperties() override;

/* Stage -------------------------------------------------------------- */
private:
	void ReachedStageGoal();
};
