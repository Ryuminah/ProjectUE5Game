// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/TestData.h"
#include "OatCharacterStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOAT_API UOatCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void UpdateStat(const FTestData& BaseStat, const FTestData& ModifierStat);


private:
	UPROPERTY()
	TMap<FName, class UTextBlock*> BaseLookup;

	UPROPERTY()
	TMap<FName, class UTextBlock*> ModifierLookup;

	
};
