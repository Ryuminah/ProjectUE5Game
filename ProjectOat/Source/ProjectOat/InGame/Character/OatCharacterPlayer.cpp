// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/OatCharacterPlayer.h"
#include "InGame/Character/OatCharacterControlData.h"
#include "InGame/Widget/OatHUDWidget.h"
#include "InGame/Character/Component/OatCharacterStatComponent.h"
#include "GameCommon/Items/OatItemWeaponData.h"

#include "Core/OatGameInstance.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "OatAttackActionData.h"
#include "Components/CapsuleComponent.h"
#include "Core/Interface/OatGameInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InGame/Physics/OatCollision.h"
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

	// Character Setup
	TestSocket = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TestSocket"));
	TestSocket->SetupAttachment(GetMesh(), TEXT("L_socket1"));

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_OATCAPSULE);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ProjectOat/Arts/Characters/Roxy/SK_Roxy.SK_Roxy'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/ProjectOat/InGame/Character/Oat/ABP_Oat.ABP_Oat_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	} 

	/* Animation ----------------------------------------------------------------------------------*/
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ProjectOat/Arts/Characters/Roxy/Animations/AM_Roxy_ComboAttack.AM_Roxy_ComboAttack'"));
	if (ActionMontageRef.Object)
	{
		AttackMontage = ActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UOatAttackActionData> ActionDataRef(TEXT("/Script/ProjectOat.OatAttackActionData'/Game/ProjectOat/InGame/Character/DA_AttackAction.DA_AttackAction'"));
	if (ActionDataRef.Object)
	{
		AttackActionData = ActionDataRef.Object;
	}

	// Dead
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ProjectOat/Arts/Characters/Roxy/Animations/AM_Roxy_Dead.AM_Roxy_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}
	
	/* Input ----------------------------------------------------------------------------------*/
	static ConstructorHelpers::FObjectFinder<UOatCharacterControlData> ShoulderDataRef(TEXT("/Script/ProjectOat.OatCharacterControlData'/Game/ProjectOat/Core/Systems/Input/DA_Control_Shoulder.DA_Control_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder,ShoulderDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UOatCharacterControlData> QuaterDataRef(TEXT("/Script/ProjectOat.OatCharacterControlData'/Game/ProjectOat/Core/Systems/Input/DA_Control_Quater.DA_Control_Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}
	
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

void AOatCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetupCallback();
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

void AOatCharacterPlayer::SetupCallback()
{
	Super::SetupCallback();
	// Item Callbacks
	//TakeItemCallbacks.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AOatCharacterPlayer::TestEquipSocket)));
	//TakeItemCallbacks.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AOatCharacterPlayer::DrinkPotion)));
	
	/* 기본 공격 관련 Callback*/
	AddDelegateOnAttackMonStart(FOnAttackMonStart::FDelegate::CreateUObject(this, &AOatCharacterPlayer::AttackActionMontageBegin));
	AddDelegateOnAttackMonEnd(FOnAttackMonEnd::FDelegate::CreateUObject(this, &AOatCharacterPlayer::AttackActionMontageEnd));
}

bool AOatCharacterPlayer::TryStartComboAttack()
{
	//Super::OnAttackStart();

	// 공격 가능한 상황인지 판단해야함 (스턴이나 스킬 사용중 같은거 확인)
	
	return true;
}

void AOatCharacterPlayer::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(AttackActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	constexpr float AttackSpeedRate = 1.f;
	float ComboEffectiveTime = (AttackActionData->EffectiveFrameCount[ComboIndex] / AttackActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.f)
	{
		// �ð��� üũ�ϵ�, �ݺ����� �ʵ��� �ѹ��� �߻��ϵ��� false
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle,this,&AOatCharacterPlayer::ComboCheck,ComboEffectiveTime,false);
	}
}

void AOatCharacterPlayer::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (bHasNextComboCommand)
	{
		// ���� �������� üũ
		UAnimInstance* AnimInstatnce = GetMesh()->GetAnimInstance();
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, AttackActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *AttackActionData->MontageSectionNamePrefix, CurrentCombo);

		// ������ ������ ���������� ������ ���Ѿ� ��.
		AnimInstatnce->Montage_JumpToSection(NextSection, AttackMontage);
		
		bHasNextComboCommand = false;
		SetComboCheckTimer();
	}
}

// 공격 첫 시작 시 몽타주 호출
void AOatCharacterPlayer::AttackActionMontageBegin()
{
	// ComboStatus
	CurrentCombo = 1;

//	const float AttackSpeedRate = 1.f;
//
//	UAnimInstance* AnimInstatnce = GetMesh()->GetAnimInstance();
//	AnimInstatnce->Montage_Play(AttackMontage, AttackSpeedRate);
//
//	FOnMontageEnded EndDelegate;
//	EndDelegate.BindUObject(this, &AOatCharacterPlayer::AttackActionMontageEnd);
//	// Delegate����, Montage ����
//	AnimInstatnce->Montage_SetEndDelegate(EndDelegate, AttackMontage);
	//SetComboCheckTimer();

//	ComboTimerHandle.Invalidate();
//
//	int32 ComboIndex = CurrentCombo - 1;
//	ensure(AttackActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));
//
//	constexpr float AttackSpeedRate = 1.f;
//	float ComboEffectiveTime = (AttackActionData->EffectiveFrameCount[ComboIndex] / AttackActionData->FrameRate) / AttackSpeedRate;
//	if (ComboEffectiveTime > 0.f)
//	{
//		// �ð��� üũ�ϵ�, �ݺ����� �ʵ��� �ѹ��� �߻��ϵ��� false
//		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle,this,&AOatCharacterPlayer::ComboCheck,ComboEffectiveTime,false);
//	}
	
	//SetComboCheckTimer();
}

void AOatCharacterPlayer::AttackActionMontageEnd()
{
	// 콤보 초기화
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
}

void AOatCharacterPlayer::SetDead()
{
	Super::SetDead();

	// 입력 막기
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		DisableInput(PlayerController);

//		// GameMode는 멀티를 고려한 모든 플레이어를 통틀어 단 하나만 존재함 (방장이 소유한 게임모드이다)
//		IOatGameInterface* OatGameMode = Cast<IOatGameInterface>(GetWorld()->GetAuthGameMode());
//		if (OatGameMode)
//		{
//			OatGameMode->OnPlayerDead();
//		}
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
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AOatCharacterPlayer::InputQuaterMove);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AOatCharacterPlayer::InputShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AOatCharacterPlayer::InputShoulderLook);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AOatCharacterPlayer::InputAttack);

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
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotaitionYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
	
	// Set Camera
	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void AOatCharacterPlayer::InputShoulderMove(const FInputActionValue& InValue)
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

void AOatCharacterPlayer::InputShoulderLook(const FInputActionValue& InValue)
{
	FVector2D LookAxisVector = InValue.Get<FVector2D>();

	// 입력값을 받아서 컨트롤러의 ControlRotation 속성을 업데이트 하는데에 사용
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AOatCharacterPlayer::InputQuaterMove(const FInputActionValue& Value)
{
	// 현재 이동 벡터
	FVector2D MovementVector = Value.Get<FVector2D>();

	//float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();

	// 크기 조정
	if (MovementVectorSizeSquared > 1.f)
	{
		MovementVector.Normalize();
		//MovementVectorSizeSquared = 1.f;
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

// 입력이 들어왔을 때
void AOatCharacterPlayer::InputAttack()
{
	if (TryStartComboAttack())
	{
		if (CurrentCombo == 0)
		{
			Super::OnAttackStart();
			return;
		}

		bHasNextComboCommand = !(ComboTimerHandle.IsValid());

//		else
//		{
//		}

//		if (!ComboTimerHandle.IsValid())
//		{
//			bHasNextComboCommand = false;
//		}
//		else
//		{
//			bHasNextComboCommand = true;
//		}

	}
	
	
	//OnAttackStart();
}

void AOatCharacterPlayer::QuitGame()
{
	// 게임 종료
	// 팝업이 없을 경우 게임 종료
	
	if (UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GetGameInstance()))
	{
		OatGameInstance->QuitGame();
	}
}

void AOatCharacterPlayer::TakeItem(class UOatItemData* InItemData)
{
	if (InItemData)
	{
		TakeItemCallbacks[static_cast<uint8>(InItemData->ItemType)].ItemDelegate.ExecuteIfBound(InItemData);
	}
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


