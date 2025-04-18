// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Load Spread Sheet and Convert To CSV File 
 */

DECLARE_LOG_CATEGORY_EXTERN(LogDTGenerator, Log, All);
class FDTLoader
{
	friend class FDataTableAutoGeneratorModule;

private:
	FDTLoader();

public:
	~FDTLoader();

public:
	FString SheetURL;
	void IsValidURL(TFunction<void(bool)> OnChecked);
	void GenerateDTProcess();
	
private:
	void LoadSpreadSheetData(TFunction<void(const FString&)> OnSuccess = nullptr);
	void ParseCSV(const FString& CSVData);

private:
	static TSharedPtr<FDTLoader> DTLoaderInst;

public:
	static TSharedRef<FDTLoader> Get()
	{
		if (!DTLoaderInst.IsValid()) { DTLoaderInst = MakeShareable(new FDTLoader); }
		return DTLoaderInst.ToSharedRef();;
	}
};

