// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shared/Enums.h"

#include "OatWallBattleField.generated.h"

/// <summary>
/// 
/// </summary>
UCLASS()
class PROJECTOAT_API AOatWallBattleField : public AActor
{
	GENERATED_BODY()

public:
	AOatWallBattleField();

protected:
	virtual void PostInitializeComponents() override;

	/* Component ------------------------------------------------------*/
protected:
	UPROPERTY(Category=Mesh, VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> WallBattlefield;

	UPROPERTY(Category=Mesh, VisibleAnywhere, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> WallCollision;

	UPROPERTY(Category=StageSection,EditAnywhere, meta=(AllowPrivateAccess = "true"))
	int SectionId;


private:
	// 전투 시작의 경우
	void BattleStateChanged(int CurrentSectionId, EStageSectionState CurrentState); 
	
};
