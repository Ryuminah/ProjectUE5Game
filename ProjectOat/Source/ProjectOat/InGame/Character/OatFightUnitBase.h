// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InGame/Interface/OatAnimFightInterface.h"
#include "Shared/Interface/OatActorWidgetInterface.h"
#include "GameCommon/Items/OatItemInterface.h"
#include "GameData/TestData.h"


#include "OatFightUnitBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};


DECLARE_MULTICAST_DELEGATE(FOnAttackMonStart); // 공격 몽타주 시작
DECLARE_MULTICAST_DELEGATE(FOnAttackMonEnd);   // 

DECLARE_MULTICAST_DELEGATE(FOnHitMonStart); // 피격		
DECLARE_MULTICAST_DELEGATE(FOnHitMonEnd);

DECLARE_MULTICAST_DELEGATE(FOnDeadMonStart);
DECLARE_MULTICAST_DELEGATE(FOnDeadMonStart);


// 전투 기능이 필요한 캐릭터들 Base
UCLASS()
class PROJECTOAT_API AOatFightUnitBase : public ACharacter, public IOatAnimFightInterface,public IOatActorWidgetInterface
{
	GENERATED_BODY()

public:
	AOatFightUnitBase();
	virtual void PostInitializeComponents() override;

// /* Attack  -----------------------------------------------------*/
protected:
	FOnAttackMonStart OnAttackMonStart;
	FOnAttackMonEnd OnAttackMonEnd;

	FOnHitMonStart OnHitMonStart;
	FOnHitMonEnd OnHitMonEnd;

	FOnDeadMonStart OnDeadMonStart;        
	FOnDeadMonStart OnDeadMonEnd;

// /* Attack  -----------------------------------------------------*/
protected:
	UPROPERTY(Category=Animation, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> AttackMontage;

	// 모든 전투 유닛의 Attack의 시작 시점에 호출
	virtual void OnAttackStart() override;   
	
	virtual void AttackActionMontageBegin(); // 공격 시작 시 호출되는 콜백
	virtual void AttackActionMontageEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// 공격 종료 후 호출되는 노티파이
	virtual void NotifyAttackActionEnd(){};

	// 몽타주 콜백 함수는 부모로 뺀다
private:
//	void AttackActionMontageBegin(); // 공격 시작 시 호출되는 콜백
//	void AttackActionMontageEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
			

/* Hit Check -----------------------------------------------------*/
protected:
	virtual void OnAttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;

/* Dead ----------------------------------------------------------*/
protected:
	UPROPERTY(Category=Animation, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnim();

	float DeadEventDelayTime = 3.f;

/* Stat ---------------------------------------------------------*/
protected:
	UPROPERTY(Category=Stat, VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UOatCharacterStatComponent> Stat;

public:
	void ApplyStat(const FTestData& BaseStat, const FTestData& ModifierStat);

/* Widget --------------------------------------------------------*/
	UPROPERTY(Category=Widget, VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UOatWidgetComponent> WidgetHpBar;

	virtual void SetUpActorWidget(class UOatUserWidget* InUserWidget) override;



};
