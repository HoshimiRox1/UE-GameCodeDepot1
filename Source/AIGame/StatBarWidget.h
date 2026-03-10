// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameTypes.h"
#include "StatBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class AIGAME_API UStatBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 属性条的代表属性
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "属性设置")
	EPlayerAttribute PlayerAttribute;
	
	// 暴露给蓝图的更新函数
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnValueUpdate(float NewValue, float MaxValue = 100.f);
	
	// 绑定UMG中的进度条组件
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar;
};
