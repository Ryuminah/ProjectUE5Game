// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OatSpawnPoint.generated.h"

UCLASS()
class PROJECTOAT_API AOatSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginDestroy() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:	
	AOatSpawnPoint();

	UPROPERTY(Category=Spawn, VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UBoxComponent> SpawnBox;

	UPROPERTY(Category = Spawn, EditAnywhere, BlueprintReadWrite)
	int32 SpawnId;

	UPROPERTY(Category = Spawn, EditAnywhere, BlueprintReadWrite)
	int32 SectionId;

public:
	int32 GetSpawnId() const { return SpawnId; } 
	int32 GetSectionId() const { return SectionId; } 

};
