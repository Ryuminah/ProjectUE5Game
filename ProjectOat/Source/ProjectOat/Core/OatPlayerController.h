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


/* Game Core --------------------------------------------------*/
public:
	void GameOver();
	void GameClear();
	void GameScoreChanged(int32 NewScore);

	// K2 -> Kismet 접두사
	// Blueprint에 보여질 함수 명을 meta에서 지정
	UFUNCTION(Category=Game, BlueprintImplementableEvent , meta =(DisplayName = "OnScoreChangedCpp"))
	void K2_OnScoreChanged(int32 NewScore);

	UFUNCTION(Category=Game, BlueprintImplementableEvent , meta =(DisplayName = "OnGameClearCpp"))
	void K2_OnGameClear();

	UFUNCTION(Category=Game, BlueprintImplementableEvent , meta =(DisplayName = "OnGameOverCpp"))
	void K2_GameOver();

	
/* HUD --------------------------------------------------*/
protected:
	UPROPERTY(Category = HUD, EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UOatHUDWidget> OatHUDWidgetClass;

	UPROPERTY(Category = HUD, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UOatHUDWidget> OatHUDWidget;

/* Save Game --------------------------------------------------*/
protected:
	UPROPERTY(category=SaveGame, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UOatSaveGame> SaveGameInstance;

};
