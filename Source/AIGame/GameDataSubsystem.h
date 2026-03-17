// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameTypes.h"
#include "GameDataSubsystem.generated.h"

class UScheduleTaskDataAsset;
/**
 游戏数据处理子系统
 */

// ── 属性变化委托--UI绑定刷新 ─────────────────────────
/**
 *属性类型及其新值
 *资金变动
 *思考点变动
 *默契值变动
 *日期改动
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, EAttributeType, AttributeType, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCashChanged, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FThoughtPointsChanged, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnnieRapportChanged, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDateChanged, FGameDate, NewDate);


UCLASS()
class AIGAME_API UGameDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// ── 委托（UI 订阅） ───────────────────────────
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged;
	
	UPROPERTY(BlueprintAssignable)
	FCashChanged OnCashChanged;
	
	UPROPERTY(BlueprintAssignable)
	FThoughtPointsChanged OnThoughtPointsChanged;
	
	UPROPERTY(BlueprintAssignable)
	FAnnieRapportChanged OnAnnieRapportChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnDateChanged OnDateChanged;
	
	// ── 属性Getter接口 ───────────────────────────
	UFUNCTION(BlueprintCallable, Category = "游戏属性")
	int32 GetAttributeValue(EAttributeType Type) const;
	
	UFUNCTION(BlueprintCallable, Category = "游戏属性")
	int32 GetCash() const { return GameState.Cash; }
	
	UFUNCTION(BlueprintCallable, Category = "游戏属性")
	int32 GetThoughtPoints() const { return GameState.ThoughtPoints; }
	
	UFUNCTION(BlueprintCallable, Category = "游戏属性")
	int32 GetAnnieRapport() const { return GameState.AnnieRapport; }
	
	UFUNCTION(BlueprintCallable, Category = "游戏属性")
	FGameWholeState GetGameState() const { return GameState; }
	
	// ── 属性修改接口（由 ScheduleSubsystem 调用） ─────
	// 根据任务奖励应用修改
	UFUNCTION(BlueprintCallable, Category = "游戏属性")
	void ApplyTaskReward(const FTaskReward& Reward);
	
	// TODO 单属性修改（万一要用呢）
	
	// ── 日期接口（由 ScheduleSubsystem 调用） ─────────
	// 获取游戏日期
	UFUNCTION(BlueprintCallable, Category = "游戏日期")
	FGameDate GetGameDate() const { return GameState.CurrentDate;}
	
	// 推进日期并广播
	UFUNCTION(BlueprintCallable, Category = "游戏日期")
	void AdvanceDate();
	
	// TODO 存读档
	// void SaveGame();
	// void LoadGame();

protected:
	// 子系统初始化
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
private:
	// 数据池
	// 游戏整体属性
	UPROPERTY()
	FGameWholeState GameState;
};
