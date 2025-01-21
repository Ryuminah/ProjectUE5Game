// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "OatUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOAT_API UOatUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetOwnerActor(AActor* NewOwner) { OwnerActor = NewOwner; }

protected:
	UPROPERTY(Category=Actor,VisibleAnyWhere,BlueprintReadOnly)
	TObjectPtr<AActor> OwnerActor;
};
