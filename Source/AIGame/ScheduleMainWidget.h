// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScheduleMainWidget.generated.h"

/**
 * 
 */

class UButton;
class UStatBarWidget;

UCLASS()
class AIGAME_API UScheduleMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// UI的BeginPlay()
	virtual void NativeConstruct() override;
	
	// Subsystem广播后的回调函数
	UFUNCTION()
	void HandleAttributeChanged(EPlayerAttribute AttributeType, float NewValue);
	
	// 绑定三个子属性条
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UStatBarWidget* StatBar_Decision;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UStatBarWidget* StatBar_Social;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UStatBarWidget* StatBar_Empathy;
	
	// 以下是『确认按钮』的逻辑
	// 绑定确认按钮
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* ConfirmButton;
	
	// 确认按钮的点击事件回调函数——锁定选中任务，并使其他任务变灰禁用
	UFUNCTION()
	void OnConfirmButtonClicked();
	
	// 『出发按钮』（默认应当处于隐藏或禁用状态）
	// 绑定
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* DepartButton;
	
	// 点击事件回调函数——执行当日结算函数
	UFUNCTION()
	void OnDepartButtonClicked();
};
