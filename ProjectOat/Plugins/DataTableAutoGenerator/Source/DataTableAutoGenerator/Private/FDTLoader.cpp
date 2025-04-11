// Fill out your copyright notice in the Description page of Project Settings.

#include "FDTLoader.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

DEFINE_LOG_CATEGORY(LogDTGenerator);
TSharedPtr<FDTLoader> FDTLoader::DTLoaderInst = nullptr;

FDTLoader::FDTLoader()
{
}

FDTLoader::~FDTLoader()
{

}

void FDTLoader::LoadSpreadSheetData(TFunction<void(const FString&)> OnSuccess)
{
	FHttpModule* Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = Http->CreateRequest();
	HttpRequest->SetURL(SheetURL);
	HttpRequest->SetVerb("GET");

	HttpRequest->OnProcessRequestComplete().BindLambda([OnSuccess](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
	{
		if (bSuccess && Response.IsValid())
		{
			FString CSVData = Response->GetContentAsString();
			if (OnSuccess){OnSuccess(CSVData);}
		}
	});

	HttpRequest->ProcessRequest();
}

void FDTLoader::ParseCSV(const FString& CSVData)
{
	TArray<FString> Lines;
	CSVData.ParseIntoArrayLines(Lines);

	if (Lines.Num() < 2)
	{
		UE_LOG(LogDTGenerator, Error, TEXT("CSV must contain at least 2 header lines (FieldName / FieldType)"));
		return;
	}

	TArray<FString> FieldNames;
	TArray<FString> FieldTypes;
	TArray<TArray<FString>> DataRows;

	// FieldName (두번째 인자로는 구분자가 들어감)
	TArray<FString> RawFieldNames;
	Lines[0].ParseIntoArray(FieldNames, TEXT(","));
	FieldNames.RemoveAt(0);

	// FieldType
	Lines[1].ParseIntoArray(FieldTypes, TEXT(","));
	FieldTypes.RemoveAt(0);
	
	// DataRows
	for(int i = 1; i < Lines.Num(); ++i)
	{
		TArray<FString> Row;
		Lines[i].ParseIntoArray(Row, TEXT(","));
		Row.RemoveAt(0);

		
		DataRows.Add(Row);
		UE_LOG(LogDTGenerator, Log , TEXT("%s"),*FString::Join(Row,TEXT(",")));
	}


	// 비어 있는 열은 제거
	TArray<int32> RemoveIndices;
	const int32 ColumnCount = FieldNames.Num();

	for (int32 Col = 0; Col < ColumnCount; ++Col)
	{
		bool bAllEmpty = true;
		for (const auto& Row : DataRows)
		{
			if (Row.IsValidIndex(Col) && !Row[Col].IsEmpty())
			{
				bAllEmpty = false;
				break;
			}
		}
		if (bAllEmpty){RemoveIndices.Add(Col);}
	}

	// 4. 역순으로 열 제거
	for (int32 i = RemoveIndices.Num() - 1; i >= 0; --i)
	{
		int32 ColIdx = RemoveIndices[i];
		FieldNames.RemoveAt(ColIdx);
		FieldTypes.RemoveAt(ColIdx);
		for (auto& Row : DataRows)
		{
			if (Row.IsValidIndex(ColIdx))
			{
				Row.RemoveAt(ColIdx);
			}
		}
	}
}

void FDTLoader::IsValidURL(TFunction<void(bool)> OnChecked)
{
	FHttpModule* Http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = Http->CreateRequest();
	HttpRequest->SetURL(SheetURL);
	HttpRequest->SetVerb("GET");


	HttpRequest->OnProcessRequestComplete().BindLambda(
		[OnChecked](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
		{
			bool bIsValid = bSuccess && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode());
			OnChecked(bIsValid);
		});

	HttpRequest->ProcessRequest();
}

void FDTLoader::GenerateDTProcess()
{
	// 로딩 후 CSV를 파싱
	LoadSpreadSheetData([this](const FString& CSV){ParseCSV(CSV);});

	// Generate USturct

	// Generate DT or DA Asset
}

