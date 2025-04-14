// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTGeneratorWidget.h"

#include "FDTLoader.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDTGeneratorWidget::Construct(const FArguments& InArgs)
{
	FString SavedFilePath;
	GConfig->GetString(TEXT("/Script/DataTableAutoGenerator.Settings"),TEXT("FilePath"),SavedFilePath,GEditorPerProjectIni);
	
	ChildSlot
	[
		// Populate the widget
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.Padding(10)
		.AutoHeight()
		[
			// URLTextBox에 저장해두기 위해서
			//SAssignNew(URLTextBox, SEditableTextBox)
			SNew(SEditableTextBox)
			                      .Text(FText::FromString(SavedFilePath))        // 값 불러오기
			                      .HintText(FText::FromString((TEXT("Set SpreadSheet URL")))) // 비어있을 경우 안내 텍스트                  
			                      .OnTextChanged_Lambda([&](const FText& Text) { FDTLoader::Get()->SheetURL = Text.ToString(); })
		]

		+ SVerticalBox::Slot()
		.Padding(10)
		.AutoHeight()
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Generate")))
			.OnClicked(this, &SDTGeneratorWidget::OnGenerateButtonClicked)
		]
	];

	// 즉시 저장

}

FReply SDTGeneratorWidget::OnGenerateButtonClicked()
{
	// Read Spread Sheet
//	FDTLoader::Get()->IsValidURL([](bool isValid)
//	{
//		// 유효하지 않을 경우
//		if (!isValid)
//		{
//			UE_LOG(LogDTGenerator, Error, TEXT("Unable to access the URL."));
//		}
//		else
//		{
//			UE_LOG(LogDTGenerator, Log, TEXT("Generate Table"));
//			FDTLoader::Get()->GenerateDTProcess();
//			// Genrate
//		}
//	});

	// Read .xlsx File
	UE_LOG(LogDTGenerator, Log, TEXT("Generate Table"));

	if (bool result = FDTLoader::Get()->GenerateDTProcess())
	{
		// DT가 성공적으로 완료되었을 때만 경로저장
		FString FilePath = FDTLoader::Get()->SheetURL;
		GConfig->SetString(TEXT("/Script/DataTableAutoGenerator.Settings"),TEXT("FilePath"),*FilePath,GEditorPerProjectIni);
		GConfig->Flush(false, GEditorPerProjectIni);
	}
	else
	{
		// 가장 마지막에 저장되어 있던 상태로 되돌려줌 ?
		//GConfig->GetString(TEXT("/Script/DataTableAutoGenerator.Settings"),TEXT("FilePath"),FDTLoader::Get()->SheetURL,GEditorPerProjectIni)      
	}


	
	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
