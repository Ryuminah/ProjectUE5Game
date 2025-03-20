// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "InGame/Character/OatFightUnitBase.h"
#include "InGame/Interface/OatHUDInterface.h"
#include "OatCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOAT_API AOatCharacterPlayer : public AOatFightUnitBase , public IOatHUDInterface
{
	GENERATED_BODY()
	
public:
	AOatCharacterPlayer();

protected:
	// MappingCon
	virtual void BeginPlay() override;
	virtual void SetDead() override;


public:
	// Action�� �Լ��� ���� ������
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Character Control Section
protected :
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class UOatCharacterControlData* CharacterControlData) override;

/* Camera ---------------------------------------------------------------------*/
protected:
	// meta : private���� ����� �𸮾� ������Ʈ�� ��ü�� ���������� ������ �� �ֵ��� ������ִ� Ư���� ������
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly , meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

/* Input ---------------------------------------------------------------------*/
protected:	
	// InGame ���� ============================================================
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

	
	// �޴� ���� ===============================================================
	UPROPERTY(Category=Input, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuitAction;
	void QuitGame();
	

	// Widget 
protected:
	virtual void SetupHUDWidget(class UOatHUDWidget* InHUDWidget) override;



};
