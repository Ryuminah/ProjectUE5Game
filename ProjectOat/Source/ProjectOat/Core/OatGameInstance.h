// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OatGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOAT_API UOatGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UOatGameInstance();

	virtual void Init() override;
	virtual void PostInitProperties() override;

public:
/* Getter ---------------------------------------------------------*/
	class UOatStageHandler* GetStageHandler() { return StageHandler; }
	class UOatEventHandler* GetEventHandler() { return EventHandler; }


/* Handler --------------------------------------------------------*/
private:
	UPROPERTY()
	TObjectPtr<class UOatStageHandler> StageHandler;
		
	// ������Ʈ Ǯ
	
	// ��������Ʈ �ڵ鷯
	UPROPERTY()
	TObjectPtr<class UOatEventHandler> EventHandler;

	
};
