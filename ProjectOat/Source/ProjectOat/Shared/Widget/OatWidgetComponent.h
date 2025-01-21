// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "OatWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOAT_API UOatWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	virtual void InitWidget() override;
	
};
