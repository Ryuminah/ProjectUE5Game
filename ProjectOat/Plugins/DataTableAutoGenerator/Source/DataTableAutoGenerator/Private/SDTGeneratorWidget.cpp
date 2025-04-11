// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTGeneratorWidget.h"

#include "FDTLoader.h"
#include "SlateOptMacros.h"



BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDTGeneratorWidget::Construct(const FArguments& InArgs)
{
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
			                      .Text(FText::FromString(FDTLoader::Get()->SheetURL))         // 값 불러오기
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


}

FReply SDTGeneratorWidget::OnGenerateButtonClicked()
{
	FDTLoader::Get()->IsValidURL([](bool isValid)
	{
		// 유효하지 않을 경우
		if (!isValid)
		{
			UE_LOG(LogDTGenerator, Error, TEXT("Unable to access the URL."));
		}
		else
		{
			UE_LOG(LogDTGenerator, Log, TEXT("Generate Table"));
			FDTLoader::Get()->GenerateDTProcess();
			// Genrate
		}
	});

	
	return FReply::Handled();
	
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
