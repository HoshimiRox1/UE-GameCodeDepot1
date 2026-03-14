// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameTypes.h"
#include "GameDataSubsystem.generated.h"

class UScheduleTaskDataAsset;
/**
 游戏单例
 */

// 动态多播：属性变动->UI刷新
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, EPlayerAttribute, Attribute, float, Value);
// 动态多播：今日结束->界面刷新
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewDayStarted);

UCLASS()
class AIGAME_API UGameDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// 监听变量——属性值变动
	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnAttributeChanged OnAttributeChanged;
	
	// 监听变量——今日结束
	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnNewDayStarted OnNewDayStarted;
	
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

	// 当前选中事件的缓存
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UScheduleTaskDataAsset> SelectedTask;
	
	// 点击——事件状态转换函数
	UFUNCTION(BlueprintCallable)
	void SelectTask(UScheduleTaskDataAsset* NewTask);
	
	// 确认按钮——事件锁定函数
	UFUNCTION(BlueprintCallable)
	void ComfirmSelectedTask();
	
	// 出发——当日的结算函数
	UFUNCTION(BlueprintCallable)
	void ExecuteToday();
	
private:
	// 数据池
	// 角色属性
	UPROPERTY()
	TMap<EPlayerAttribute, float> PlayerStats;
	
	// 当前日期——简化版
	UPROPERTY()
	int CurrentDate;
};
