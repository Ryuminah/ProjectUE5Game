// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Shared/Interface/OatActorWidgetInterface.h"
#include "GameCommon/Items/OatItemInterface.h"
#include "GameData/TestData.h"
#include "InGame/Interface/OatFightUnitInterface.h"

#include "OatFightUnitBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

DECLARE_MULTICAST_DELEGATE(FOnAttackMonStart); // 공격 몽타주 시작
DECLARE_MULTICAST_DELEGATE(FOnAttackMonEnd);   // 공격 몽타주 종료 (UAnimMontage* TargetMontage, bool IsProperlyEnded 인자로 받아야할까?)

DECLARE_MULTICAST_DELEGATE(FOnHitMonStart); // 피격		
DECLARE_MULTICAST_DELEGATE(FOnHitMonEnd);

DECLARE_MULTICAST_DELEGATE(FOnDeadMonStart);     // 사망
DECLARE_MULTICAST_DELEGATE(FOnDeadMonEnd);           

// 전투 기능이 필요한 캐릭터들 Base
UCLASS()
class PROJECTOAT_API AOatFightUnitBase : public ACharacter, public IOatFightUnitInterface, public IOatActorWidgetInterface
{
	GENERATED_BODY()

public:
	AOatFightUnitBase();
	virtual void PostInitializeComponents() override;

	virtual void SetupCallback()
	{
	};

/* Montage Callback  -----------------------------------------------------*/
private:
	FOnAttackMonStart OnAttackMonStart;
	FOnAttackMonEnd OnAttackMonEnd;

	FOnHitMonStart OnHitMonStart;
	FOnHitMonEnd OnHitMonEnd;

	FOnDeadMonStart OnDeadMonStart;
	FOnDeadMonStart OnDeadMonEnd;

protected:
	virtual void AttackMontageBegin(){};
	virtual void AttackMontageEnd(){};
	void AddDelegateOnAttackMonStart(const FOnAttackMonStart::FDelegate& InDelegate) { OnAttackMonStart.Add(InDelegate); }
	void AddDelegateOnAttackMonEnd(const FOnAttackMonEnd::FDelegate& InDelegate) { OnAttackMonEnd.Add(InDelegate); }

	virtual void DeadMontageBegin(){};
	virtual void DeadMontageEnd(){};
	void AddDelegateOnDeadMonStart(const FOnDeadMonStart::FDelegate& InDelegate) { OnDeadMonStart.Add(InDelegate); }
	void AddDelegateOnDeadMonEnd(const FOnDeadMonEnd::FDelegate& InDelegate) { OnDeadMonStart.Add(InDelegate); }

/* Attack  -----------------------------------------------------*/
protected:
	UPROPERTY(Category=Animation, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UAnimMontage> AttackMontage;

	// 모든 전투 유닛의 Attack의 시작 시점에 호출해주기        
	virtual void OnAttackStart() final override;
	virtual void OnAttackEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded) final override;

/* Hit  -----------------------------------------------------*/
protected:
	// TMap으로 바꿀까!
	UPROPERTY(Category=Animation, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UAnimMontage> HitMontage;
	// 모든 전투 유닛의 Attack의 시작 시점에 호출해주기        
//	virtual void OnHitStart() final override;      
//	virtual void OnHitEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded) final override;
	
	/* Hit Check -----------------------------------------------------*/
	virtual void AnimNotifyAttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// 모든 전투 유닛의 Hit 시작 시점에 호출하기
	//virtual void OnHitStart() final override;

	// 나중에 구현하기 (현재 공격 가능한지)
	//virtual void TryStartAttack();

	
/* Dead ----------------------------------------------------------*/
protected:
	UPROPERTY(Category=Animation, EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UAnimMontage> DeadMontage;

	// 모든 전투 유닛의 Dead의 시작 시점에 호출해주기        
	virtual void OnDeadStart() final override;
	virtual void OnDeadEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded) final override;
	
	//virtual void SetDead();
	void PlayDeadAnim();

	float DeadEventDelayTime = 3.f;

	
/* Montage ----------------------------------------------------------*/
protected:
	//전체 몽타주 중지하는 기능
	void StopAllMontage() const;
	
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
