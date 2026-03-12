// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScheduleMainWidget.generated.h"

/**
 * 
 */

class UStatBarWidget;

UCLASS()
class AIGAME_API UScheduleMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// UI的BeginPlay()
	virtual void NativeConstruct() override;
	
	// Subsystem广播后的触发函数
	UFUNCTION()
	void HandleAttributeChanged(EPlayerAttribute AttributeType, float NewValue);
	
	// 绑定三个子属性条
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UStatBarWidget* StatBar_Decision;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UStatBarWidget* StatBar_Social;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UStatBarWidget* StatBar_Empathy;
};
