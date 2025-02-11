// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/OatCharacterPlayer.h"
#include "InGame/Character/OatCharacterControlData.h"
#include "InGame/Widget/OatHUDWidget.h"
#include "InGame/Character/Component/OatCharacterStatComponent.h"
#include "Core/Interface/OatGameInterface.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "Kismet/KismetSystemLibrary.h"


AOatCharacterPlayer::AOatCharacterPlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectOat/Core/Systems/Input/Actions/IA_Jump.IA_Jump'"));
	if (InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectOat/Core/Systems/Input/Actions/IA_Change_Control.IA_Change_Control'"));
	if (InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectOat/Core/Systems/Input/Actions/IA_Shoulder_Move.IA_Shoulder_Move'"));
	if (InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectOat/Core/Systems/Input/Actions/IA_Shoulder_Look.IA_Shoulder_Look'"));
	if (InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectOat/Core/Systems/Input/Actions/IA_Quater_Move.IA_Quater_Move'"));
	if (InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectOat/Core/Systems/Input/Actions/IA_Attack.IA_Attack'"));
	if (InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuitRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectOat/Core/Systems/Input/Actions/IA_Quit.IA_Quit'"));
	if (InputActionQuitRef.Object)
	{
		QuitAction = InputActionQuitRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Shoulder;
}

void AOatCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	SetCharacterControl(CurrentCharacterControlType);
}

void AOatCharacterPlayer::SetDead()
{
	Super::SetDead();

	// �Է� ����
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);

		//// GameMode�� ��Ƽ�� ����� ��� �÷��̾ ��Ʋ�� �� �ϳ��� ������ (������ ������ ���Ӹ���̴�)
		//IOatGameInterface* OatGameMode = Cast<IOatGameInterface>(GetWorld()->GetAuthGameMode());
		//if (OatGameMode)
		//{
		//	OatGameMode->OnPlayerDead();
		//}
	}
}

void AOatCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// EnhancedInputComponent�� ������ ���� ���� ������ �߻��� �� �ֵ��� üũ
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AOatCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AOatCharacterPlayer::QuaterMove);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AOatCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AOatCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AOatCharacterPlayer::Attack);

	EnhancedInputComponent->BindAction(QuitAction, ETriggerEvent::Triggered, this, &AOatCharacterPlayer::QuitGame);
}

void AOatCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void AOatCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UOatCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		// ������ ���� ����
		Subsystem->ClearAllMappings();

		// ������ ���¿� �����ϴ� �������� ����
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext,0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void AOatCharacterPlayer::SetCharacterControlData(const UOatCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;

}

void AOatCharacterPlayer::ShoulderMove(const FInputActionValue& InValue)
{
	FVector2D MovementVector = InValue.Get<FVector2D>();

	// Update�� ControlRotation ���� �����ͼ�
	const FRotator	Rotation = Controller->GetControlRotation();
	const FRotator	YawRotation(0, Rotation.Yaw, 0);

	// ���� / ������ ������ �޾ƿ� ��
	const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// IA_Move -> Modifier�� Swizzle (�Է¿����� XY�� �ݴ�� ����)
	AddMovementInput(FowardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AOatCharacterPlayer::ShoulderLook(const FInputActionValue& InValue)
{
	FVector2D LookAxisVector = InValue.Get<FVector2D>();

	// �Է°��� �޾Ƽ� ��Ʈ�ѷ��� ControlRotation �Ӽ��� ������Ʈ �ϴµ��� ���
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AOatCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	// ���� �̵� ����
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();

	// ũ�� ����
	if (MovementVectorSizeSquared > 1.f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	// Fowrad �������� Control Rotation ���� -> 
	// Movement Component���� ������ �ɼ��� ���� ���� ĳ���Ͱ� �̵��ϴ� �������� ȸ��
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AOatCharacterPlayer::Attack()
{
	ProcessAttack();
}

void AOatCharacterPlayer::QuitGame()
{
	// �˾��� ���� ��� ���� ����
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void AOatCharacterPlayer::SetupHUDWidget(UOatHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp());

		// Callback Binding
		Stat->OnStatChanged.AddUObject(InHUDWidget, &UOatHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UOatHUDWidget::UpdateHpBar);
	}
}


