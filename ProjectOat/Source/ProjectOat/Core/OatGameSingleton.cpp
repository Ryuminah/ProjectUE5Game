// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/OatGameSingleton.h"

DEFINE_LOG_CATEGORY(LogOatGameSingleton);

UOatGameSingleton::UOatGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/ProjectOat/DataTables/DT_TestData.DT_TestData'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, TestTable, [](uint8* Value) { return *reinterpret_cast<FTestData*>(Value); });
 	}

	MaxLv = TestTable.Num();
	ensure(MaxLv > 0);
}

UOatGameSingleton& UOatGameSingleton::Get()
{
	UOatGameSingleton* Singleton = CastChecked<UOatGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogOatGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UOatGameSingleton>();
}
