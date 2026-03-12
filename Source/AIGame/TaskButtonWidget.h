// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TaskButtonWidget.generated.h"

class UScheduleTaskDataAsset;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class AIGAME_API UTaskButtonWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 绑定任务数据
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "任务数据")
	TObjectPtr<UScheduleTaskDataAsset> TaskDataAsset;
	
protected:
	// 绑定按钮控件
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* TaskButton;
	
	// 绑定任务名称
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* TaskName;
	
	// 重写构造函数
	virtual void NativeConstruct() override;
	
	// 绑定按钮的回调函数
	UFUNCTION()
	void OnTaskButtonClicked();
};
