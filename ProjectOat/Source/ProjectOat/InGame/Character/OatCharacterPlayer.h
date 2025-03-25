// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "InGame/Character/OatFightUnitBase.h"
#include "InGame/Interface/OatHUDInterface.h"
#include "GameCommon/Items/OatItemInterface.h"
#include "OatCharacterPlayer.generated.h"


DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UOatItemData* /*InItemData*/);
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper(){}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate){}

	FOnTakeItemDelegate ItemDelegate;
};

/**
 * 
 */
UCLASS()
class PROJECTOAT_API AOatCharacterPlayer : public AOatFightUnitBase, public IOatHUDInterface, public IOatItemInterface
{
	GENERATED_BODY()

/* Initialize -----------------------------------------------------*/
public:
	AOatCharacterPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SetCharacterControlData(const class UOatCharacterControlData* CharcterControlData);

	UPROPERTY(Category = CharacterControl, EditAnywhere, meta=(AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UOatCharacterControlData*> CharacterControlManager;

protected:
	void SetupCallback();

	
/* Combo Action -----------------------------------------------------*/
//protected:
	// UPROPERTY(Category=Animation, EditAnywhere, BlueprintReadWrite)
	// TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(Category=Attack, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UOatAttackActionData> AttackActionData;

	virtual void ProcessAttack() override;
	virtual void AttackActionMontageBegin() override;
	virtual void AttackActionMontageEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded) override;

	virtual void NotifyAttackActionEnd() override {};

	// 현재까지 진행된 콤보 수
	int32 CurrentCombo = 0;
	// 원하는 시간에 특정 함수 호출
	FTimerHandle ComboTimerHandle;
	// 입력 커맨드가 들어왔는지
	bool bHasNextComboCommand = false;
	void SetComboCheckTimer();
	void ComboCheck();
	
/* Character Control -----------------------------------------------------*/
protected :
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetDead() override;

/* Camera ---------------------------------------------------------------------*/
protected:
	// meta : private???? ????? ??? ????????? ????? ?????????? ?????? ?? ????? ???????? ????? ??????
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

/* Input ---------------------------------------------------------------------*/
protected:
	// InGame ???? ============================================================
	UPROPERTY(Category=Input, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(Category=Input, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(Category=Input, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(Category=Input, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(Category=Input, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(Category=Input, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	ECharacterControlType CurrentCharacterControlType;

	void ShoulderMove(const FInputActionValue& InValue);
	void ShoulderLook(const FInputActionValue& InValue);
	void QuaterMove(const FInputActionValue& Value);
	void Attack();


	// ??? ???? ===============================================================
	UPROPERTY(Category=Input, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuitAction;
	void QuitGame();


/* Item --------------------------------------------------------*/
protected:
	// 펫 같은걸로 바꿔보자~~
	UPROPERTY(Category=Equipment, VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> TestSocket;


	virtual void TakeItem(class UOatItemData* InItemData) override;

	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemCallbacks;

	// Widget 
protected:
	virtual void SetupHUDWidget(class UOatHUDWidget* InHUDWidget) override;
};
