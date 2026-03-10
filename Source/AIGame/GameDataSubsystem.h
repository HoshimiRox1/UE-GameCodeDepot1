// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameTypes.h"
#include "GameDataSubsystem.generated.h"

/**
 游戏单例
 */

// 动态多播：属性变动->UI刷新
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, EPlayerAttribute, Attribute, float, Value);

UCLASS()
class AIGAME_API UGameDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// 初始化数据
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	// 修改属性的通用接口
	UFUNCTION(BlueprintCallable, Category = "游戏属性")
	void ModifyAttribute(EPlayerAttribute Type, float Amount);
	
	// 每日刷新逻辑
	UFUNCTION(BlueprintCallable, Category = "游戏属性")
	void ResetDailyStats();
	
	// 属性Getter函数
	UFUNCTION(BlueprintPure, Category = "游戏属性")
	float GetAttributeValue(EPlayerAttribute Attribute) const;
	
	// UI监听变量
	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnAttributeChanged OnAttributeChanged;
	
private:
	// 数据池
	UPROPERTY()
	TMap<EPlayerAttribute, float> PlayerStats;
};
