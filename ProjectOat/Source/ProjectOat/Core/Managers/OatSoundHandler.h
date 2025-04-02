// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OatHandlerInterface.h"
#include "GameFramework/Actor.h"
#include "OatSoundHandler.generated.h"

UCLASS()
class PROJECTOAT_API AOatSoundHandler : public AActor ,public IOatHandlerInterface
{
	GENERATED_BODY()
	
public:	
	AOatSoundHandler();

protected:
	virtual void BeginPlay() override;
	virtual void Initialize(UGameInstance* GameInstance) override;

protected:

	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* BGMAudioComponent;

	UPROPERTY()
	TObjectPtr<class USoundCue> CurrentBGM;
	
	UPROPERTY()
	TMap<FName, TObjectPtr<class USoundCue>> BGMSoundMap;
	
public:
	void PlaySound(FName BGMName);
	void PlayBGM(FName ChangeBGMName);

private:
	void LoadSoundCueAssets();


};
