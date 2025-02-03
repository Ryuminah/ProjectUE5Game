// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGame/Character/OatCharacterBase.h"
#include "OatCharacterNPC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOAT_API AOatCharacterNPC : public AOatCharacterBase
{
	GENERATED_BODY()
	
public:
	AOatCharacterNPC();

public:
	void SetDead() override;

};
