// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */


class DATATABLEAUTOGENERATOR_API SDTGeneratorWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDTGeneratorWidget){}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
private:
	//TSharedPtr<SEditableTextBox> URLTextBox;
	FReply OnGenerateButtonClicked();
};
