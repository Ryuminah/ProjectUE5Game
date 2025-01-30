// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameData/TestData.h"
#include "OatGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogOatGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class PROJECTOAT_API UOatGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UOatGameSingleton();
	static UOatGameSingleton& Get();

public:
	FORCEINLINE FTestData GetTestData(int32 Level) const { return TestTable.IsValidIndex(Level - 1) ? TestTable[Level] : FTestData(); }

	UPROPERTY()
	int32 MaxLv;

private:
	TArray<FTestData> TestTable;
};
