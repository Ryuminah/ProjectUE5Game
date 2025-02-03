// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "OatAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOAT_API AOatAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AOatAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BB;

		UPROPERTY()
	TObjectPtr<class UBehaviorTree> BT;

};
