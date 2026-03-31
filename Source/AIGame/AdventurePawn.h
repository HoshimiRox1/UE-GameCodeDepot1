// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/Pawn.h"
#include "AdventurePawn.generated.h"

UCLASS()
class AIGAME_API AAdventurePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAdventurePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// ---输入资产--------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "输入")
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "输入")
	UInputAction* MoveAction;
	
	// ---回调函数--------------
	void Move(const FInputActionValue& Value);
	
	// ---2.5D移动x轴限制移动变量--------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "移动")
	float MinX = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "移动")
	float MaxX = 2000.f;
};
