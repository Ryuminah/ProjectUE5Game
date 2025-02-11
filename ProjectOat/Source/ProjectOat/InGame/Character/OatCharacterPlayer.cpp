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

	// 입력 막기
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);

		//// GameMode는 멀티를 고려한 모든 플레이어를 통틀어 단 하나만 존재함 (방장이 소유한 게임모드이다)
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

	// EnhancedInputComponent가 사용되지 않은 경우는 에러가 발생할 수 있도록 체크
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
		// 기존의 매핑 삭제
		Subsystem->ClearAllMappings();

		// 데이터 에셋에 존재하는 매핑으로 변경
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

	// Update된 ControlRotation 값을 가져와서
	const FRotator	Rotation = Controller->GetControlRotation();
	const FRotator	YawRotation(0, Rotation.Yaw, 0);

	// 전진 / 오른쪽 방향을 받아온 뒤
	const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// IA_Move -> Modifier의 Swizzle (입력에서의 XY를 반대로 설정)
	AddMovementInput(FowardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AOatCharacterPlayer::ShoulderLook(const FInputActionValue& InValue)
{
	FVector2D LookAxisVector = InValue.Get<FVector2D>();

	// 입력값을 받아서 컨트롤러의 ControlRotation 속성을 업데이트 하는데에 사용
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AOatCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	// 현재 이동 벡터
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();

	// 크기 조정
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
	// Fowrad 방향으로 Control Rotation 지정 -> 
	// Movement Component에서 설정한 옵션을 통해 현재 캐릭터가 이동하는 방향으로 회전
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AOatCharacterPlayer::Attack()
{
	ProcessAttack();
}

void AOatCharacterPlayer::QuitGame()
{
	// 팝업이 없을 경우 게임 종료
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


