// Fill out your copyright notice in the Description page of Project Settings.

#include "FDTLoader.h"

#include "HttpModule.h"
#include "XLDocument.hpp"
#include "XLSheet.hpp"
#include "Interfaces/IHttpResponse.h"

DEFINE_LOG_CATEGORY(LogDTGenerator);
TSharedPtr<FDTLoader> FDTLoader::DTLoaderInst = nullptr;

FDTLoader::FDTLoader()
{
	GConfig->GetString(TEXT("/Script/DataTableAutoGenerator.Settings"),TEXT("FilePath"), SheetURL, GEditorPerProjectIni);
}

FDTLoader::~FDTLoader()
{

}

#pragma region Spread Sheets URL 
void FDTLoader::LoadSpreadSheetData(TFunction<void(const FString&)> OnSuccess)
{
	FHttpModule* Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = Http->CreateRequest();
	//HttpRequest->SetURL(SheetURL);
	HttpRequest->SetVerb("GET");

	HttpRequest->OnProcessRequestComplete().BindLambda([OnSuccess](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
	{
		if (bSuccess && Response.IsValid())
		{
			FString CSVData = Response->GetContentAsString();
			if (OnSuccess) { OnSuccess(CSVData); }
		}
	});

	HttpRequest->ProcessRequest();
}

void FDTLoader::IsValidURL(TFunction<void(bool)> OnChecked)
{
	FHttpModule* Http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = Http->CreateRequest();
	//HttpRequest->SetURL(SheetURL);
	HttpRequest->SetVerb("GET");


	HttpRequest->OnProcessRequestComplete().BindLambda(
													   [OnChecked](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
													   {
														   bool bIsValid = bSuccess && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode());
														   OnChecked(bIsValid);
													   });

	HttpRequest->ProcessRequest();
}

#pragma endregion

// Change ExcelSheet To CSV
TMap<FString, FString> FDTLoader::ReadExcelSheetData()
{
	// Read All Sheets
	TMap<FString, TArray<TArray<FString>>> AllSheetMap;
	
	
	// if not Exist
	if (!FPaths::FileExists(SheetURL))
	{
		UE_LOG(LogDTGenerator, Error, TEXT("Not Exist File: %s"), *SheetURL);
		return TMap<FString, FString>();
	}

	OpenXLSX::XLDocument Doc;
	Doc.open(TCHAR_TO_UTF8(*SheetURL));

	for (const std::string& SheetNameStd : Doc.workbook().worksheetNames())
	{
		FString SheetName = UTF8_TO_TCHAR(SheetNameStd.c_str());

		// if sheet Name starts with '#', it is ignored 
		if (SheetName.StartsWith(TEXT("#"))) { continue; }

		OpenXLSX::XLWorksheet TargetSheet = Doc.workbook().worksheet(SheetNameStd);
		TArray<TArray<FString>> Grid;

		uint32 RowCount = TargetSheet.rowCount();
		uint32 ColCount = TargetSheet.columnCount();

		// 먼저 비어있는 열 판단
		TSet<uint32> EmptyColumns;
		for (uint32 Col = 1; Col <= ColCount; ++Col)
		{
			bool bAllEmpty = true;
			for (uint32 Row = 1; Row <= RowCount; ++Row)
			{
				OpenXLSX::XLCellAssignable Cell = TargetSheet.cell(OpenXLSX::XLCellReference(Row, Col));
				if (!Cell.empty())
				{
					bAllEmpty = false;
					break;
				}
			}
			if (bAllEmpty)
			{
				EmptyColumns.Add(Col);
			}
		}

		// 행 순회
		for (uint32 Row = 1; Row <= RowCount; ++Row)
		{
			TArray<FString> Line;
			bool bRowAllEmpty = true;

			for (uint32 Col = 1; Col <= ColCount; ++Col)
			{
				if (EmptyColumns.Contains(Col))
					continue; // 전체 열이 비어있으면 skip

				OpenXLSX::XLCellAssignable Cell = TargetSheet.cell(OpenXLSX::XLCellReference(Row, Col));
				FString Value;

				if (!Cell.empty())
				{
					std::string CellValue = Cell.value().getString();
					Value = UTF8_TO_TCHAR(CellValue.c_str());
				}

				if (!Value.TrimStartAndEnd().IsEmpty())
				{
					bRowAllEmpty = false;
				}

				Line.Add(Value);
			}

			if (!bRowAllEmpty)
			{
				Grid.Add(Line);
			}
		}

		AllSheetMap.Add(SheetName, Grid);
	}

	Doc.close();

	// Convert All Sheet To CSV
	TMap<FString, FString> SheetToCSVMap;
	for (const auto& Sheet : AllSheetMap)
	{
		auto Grid = Sheet.Value;
		TArray<FString> Lines;

		for (int32 i = 0; i < Grid.Num(); ++i)
		{
			const TArray<FString>& Row = Grid[i];
			Lines.Add(FString::Join(Row,TEXT(",")));
		}

		SheetToCSVMap.Add(Sheet.Key, FString::Join(Lines,TEXT("\n")));
	}

	return SheetToCSVMap;
}



TArray<TArray<FString>> FDTLoader::ParseCSV(const FString& CSVData)
{
	TArray<FString> Lines;
	CSVData.ParseIntoArrayLines(Lines);

	if (Lines.Num() < 2)
	{
		UE_LOG(LogDTGenerator, Error, TEXT("CSV must contain at least 2 header lines (FieldName / FieldType)"));
		return TArray<TArray<FString>>();
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
	for (int i = 0; i < Lines.Num(); ++i)
	{
		TArray<FString> Row;
		Lines[i].ParseIntoArray(Row, TEXT(","));
		Row.RemoveAt(0);
		DataRows.Add(Row);
		//UE_LOG(LogDTGenerator, Log , TEXT("%s"),*FString::Join(Row,TEXT(",")));
	}

	
	return DataRows;
}

bool FDTLoader::GenerateDTProcess()
{
	// 로딩 후 CSV를 파싱
	auto AllSheetMap = ReadExcelSheetData();
	if (AllSheetMap.IsEmpty())
	{
		return false;	
	}

	for (const auto& Sheet : AllSheetMap)
	{
		const FString& SheetName = Sheet.Key;
		const FString& SheetValue = Sheet.Value;

		TArray<TArray<FString>> CSV = ParseCSV(SheetValue);

		UE_LOG(LogDTGenerator, Log, TEXT("=========%s========="), *SheetName);

		for (int32 i = 0; i < CSV.Num(); ++i)
		{
			const FString& LineStr = FString::Join(CSV[i], TEXT(","));
			UE_LOG(LogDTGenerator, Log, TEXT("[%s]"),*LineStr);
		}

		// Generate USturct

		// Generate DT or DA Asset
	}

	return true;
}
