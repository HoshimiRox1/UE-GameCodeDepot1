// 主角模块：实现玩家移动输入与交互输入入口


#include "MainCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractableInterface.h"
#include "InputAction.h"

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// ---注入输入映射上下文--------------
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called Every Frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//---替换为EnhancedInput组件--------------
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//---绑定输入动作--------------
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		}

		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMainCharacter::Interact);
		}
	}
}

// 处理玩家移动输入
void AMainCharacter::Move(const FInputActionValue& Value)
{
	FVector2D Direction = Value.Get<FVector2D>();
	
	//---获取向前，向右向量--------------
	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();
	
	//---X轴自由移动--------------
	AddMovementInput(Right, Direction.X);
	
	//---Y轴限制移动--------------
	// TODO 目前做法是暴力钳制，后面再优化做法
	AddMovementInput(Forward, Direction.Y);
	
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Y = FMath::Clamp(CurrentLocation.Y, MinY, MaxY);
	
	// TODO 这行要删的，临时用
	CurrentLocation.X = FMath::Clamp(CurrentLocation.X,	-560, 1960);
	
	SetActorLocation(CurrentLocation);
}

// 处理玩家交互输入
void AMainCharacter::Interact(const FInputActionValue& Value)
{
	if (!IsValid(CurrentInteractTarget))
	{
		return;
	}

	if (!CurrentInteractTarget->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		return;
	}

	if (!IInteractableInterface::Execute_CanInteract(CurrentInteractTarget, this))
	{
		return;
	}

	IInteractableInterface::Execute_Interact(CurrentInteractTarget, this);
}

// 设置当前可交互目标
void AMainCharacter::SetCurrentInteractTarget(AActor* TargetActor)
{
	if (!IsValid(TargetActor))
	{
		return;
	}

	if (!TargetActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		return;
	}

	CurrentInteractTarget = TargetActor;
}

// 清除当前可交互目标
void AMainCharacter::ClearCurrentInteractTarget(AActor* TargetActor)
{
	if (CurrentInteractTarget == TargetActor)
	{
		CurrentInteractTarget = nullptr;
	}
}

// 获取当前交互目标的提示文本
FText AMainCharacter::GetCurrentInteractPrompt()
{
	if (!IsValid(CurrentInteractTarget))
	{
		return FText::GetEmpty();
	}

	if (!CurrentInteractTarget->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		return FText::GetEmpty();
	}

	return IInteractableInterface::Execute_GetInteractPrompt(CurrentInteractTarget, this);
}
