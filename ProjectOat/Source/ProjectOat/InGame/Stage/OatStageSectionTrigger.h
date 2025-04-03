// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shared/Enums.h"
#include "OatStageSectionTrigger.generated.h"


DECLARE_DELEGATE(FOnStageSectionStateChangedDelegate)

USTRUCT(BlueprintType)
struct FStageSectionChangedDelegateWrapper
{
	GENERATED_BODY()
	FStageSectionChangedDelegateWrapper() {}
	FStageSectionChangedDelegateWrapper(const FOnStageSectionStateChangedDelegate& InDelegate) : SectionDelegate(InDelegate){}

	FOnStageSectionStateChangedDelegate SectionDelegate;
};


UCLASS()
class PROJECTOAT_API AOatStageSectionTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOatStageSectionTrigger();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


/* Section ------------------------------------------------------*/
protected:
	UPROPERTY(Category=Stage, VisibleAnywhere, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;

	// 현재 스테이지의 섹션 순서
	UPROPERTY(Category=Stage, EditAnywhere, meta=(AllowPrivateAccess = "true"))
	int SectionId = 1;
	
	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									bool bFromSweep, const FHitResult& SweepHitResult);


/* Spawn ------------------------------------------------------*/
	UPROPERTY(Category = Spawn, BlueprintReadOnly)
	TSubclassOf<class AOatSpawnPoint> OatSpawnPointClass;

	UPROPERTY()
	TArray<TSoftObjectPtr<class AOatSpawnPoint>> SpawnPointArray;
	
	void SpawnSectionEnemy(const FVector& SpawnPos/*,스폰 관련 테이블*/);
	void CreateSpawnPointData();

	// 현재 웨이브
	int CurrentWave;
	// 웨이브 전체 적 수
	int WaveTotalEnemyCount;
	// 현재 웨이브에서 처치한 적 수
	int WaveKillCount;

	void CountKilledEnemy(UObject* Object);
	void TrySpawnNextWave(); 
	
/* Section State ------------------------------------------------------*/
private:
	//UPROPERTY(Category=Stage, EditAnywhere, meta=(AllowPrivateAccess = "true"))
	EStageSectionState CurrentState;
	void SetSectionState(EStageSectionState NewState);

	UPROPERTY()
	TMap<EStageSectionState, FStageSectionChangedDelegateWrapper> SectionStateChangedCallback;

private:
	void SetReadyBattle();
	void SetInBattle();
	void SetEndBattle();

/* Spawn Enemy ------------------------------------------------------*/
	// TSubClassOf -> 언리얼엔진이 제공하는 템플릿 : 지정한 클래스로부터 상속받은 클래스 목록만 표시하도록
	// 임시로 록시 넣어놓기
	UPROPERTY(Category=Battle, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AOatCharacterEnemy> OpponentClass;

	UFUNCTION()
	void OnOpponentDestroyed(AActor* DestroyedActor);
};

// // Gate는 따로 만들기
// /* Gate ------------------------------------------------------*/
// 	UPROPERTY(Category=Stage, VisibleAnywhere, meta=(AllowPrivateAccess = "true"))
// 	TMap<FName, TObjectPtr<class UStaticMeshComponent>> GateMap;
//
// 	UPROPERTY(Category=Stage, VisibleAnywhere, meta=(AllowPrivateAccess = "true"))
// 	TArray<TObjectPtr<class UBoxComponent>> GateTriggerArray;
//
// 	UFUNCTION()
// 	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
// 								   bool bFromSweep, const FHitResult& SweepHitResult);
