// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventurePawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"

// Sets default values
AAdventurePawn::AAdventurePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAdventurePawn::BeginPlay()
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

// Called every frame
void AAdventurePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAdventurePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//---替换为EnhancedInput组件--------------
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//---绑定输入动作--------------
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAdventurePawn::Move);
	}
}

void AAdventurePawn::Move(const FInputActionValue& Value)
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

