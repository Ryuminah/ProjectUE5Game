// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OatPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOAT_API AOatPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AOatPlayerController();

protected:
	virtual void BeginPlay() override;
	
/* HUD --------------------------------------------------*/
protected:
	UPROPERTY(Category = HUD, EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UOatHUDWidget> OatHUDWidgetClass;

	UPROPERTY(Category = HUD, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UOatHUDWidget> OatHUDWidget;
};
