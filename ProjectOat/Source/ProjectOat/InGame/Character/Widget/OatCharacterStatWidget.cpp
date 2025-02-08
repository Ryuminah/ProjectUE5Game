// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/Widget/OatCharacterStatWidget.h"
#include "Components/TextBlock.h"

void UOatCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 구조체가 가진 필드들을 가져오게끔
	// StaticStruct 함수를 활용하여 구조체의 정보를 가져옴
	for (TFieldIterator<FNumericProperty> PropIt(FTestData::StaticStruct()); PropIt; ++PropIt)
	{
		// 속성에 대한 키값
		const FName PropKey(PropIt->GetName());

		// TextBlock Control
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName()); 
		const FName TextModifierControlName= *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName());

		// Widget의 자식들을 미리 불러들여서 저장해둠
		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));
		if (BaseTextBlock)
		{
			BaseLookup.Add(PropKey, BaseTextBlock);
		}

		UTextBlock* ModifierTextBlock= Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName));
		if (ModifierTextBlock)
		{
			ModifierLookup.Add(PropKey, ModifierTextBlock);
		}
	}
}

void UOatCharacterStatWidget::UpdateStat(const FTestData& BaseStat, const FTestData& ModifierStat)
{
	// 구조체가 가진 필드들을 가져오게끔
	// StaticStruct 함수를 활용하여 구조체의 정보를 가져옴
	for (TFieldIterator<FNumericProperty> PropIt(FTestData::StaticStruct()); PropIt; ++PropIt)
	{
		// 속성에 대한 키값
		const FName PropKey(PropIt->GetName());

		float BaseData = 0.f;
		PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData);
		float ModifierData = 0.f;
		PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierData);

		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData)));
		}
		UTextBlock** ModiferTextBlockPtr = ModifierLookup.Find(PropKey);
		if (ModiferTextBlockPtr)
		{
			(*ModiferTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}
	}
}
