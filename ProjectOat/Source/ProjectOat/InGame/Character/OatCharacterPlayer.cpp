// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/OatCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"


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

	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/ProjectOat/Core/Systems/Input/IMC_Default.IMC_Default'"));
	if (InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectOat/Core/Systems/Input/Actions/IA_Move.IA_Move'"));
	if (InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectOat/Core/Systems/Input/Actions/IA_Jump.IA_Jump'"));
	if (InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ProjectOat/Core/Systems/Input/Actions/IA_Look.IA_Look'"));
	if (InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}
}

void AOatCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		// 런타임에서 자유롭게 뺄 수 있다.
		//Subsystem->RemoveMappingContext(DefaultMappingContext);
	}
}

void AOatCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// EnhancedInputComponent가 사용되지 않은 경우는 에러가 발생할 수 있도록 체크
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOatCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOatCharacterPlayer::Look);



}

void AOatCharacterPlayer::Move(const FInputActionValue& InValue)
{
	FVector2D MovementVector = InValue.Get<FVector2D>();

	const FRotator	Rotation = Controller->GetControlRotation();
	const FRotator	YawRotation(0, Rotation.Yaw, 0);

	const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// IA_Move -> Modifier의 Swizzle (입력에서의 XY를 반대로 설정)
	AddMovementInput(FowardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AOatCharacterPlayer::Look(const FInputActionValue& InValue)
{
	FVector2D LookAxisVector = InValue.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

}

