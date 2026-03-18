// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameTypes.h"
#include "ScheduleTaskDataAsset.h"
#include "ScheduleSubsystem.generated.h"

/**
 * 刷新今日日程候选列表
 * 点击候选日程，添加进任务槽
 * 点击触发，依序执行日程，播动画，有随机事件就触发
 */

class UGameDataSubsystem;

// ── 委托 ──────────────────────────────────
// 候选日程列表刷新
// 任务槽监听，增删都广播
// 任务槽执行中高亮广播
// 随机任务触发
// 单个任务完成
// 所有任务完成
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCandidateListRefreshed, const TArray<UScheduleTaskDataAsset*>&, CandidateList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotsChanged, const TArray<UScheduleTaskDataAsset*>&, CurrentSlots);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExecutionSlotChanged, int32, CurrentSlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRandomEventTriggered, const FRandomEvent&, RandomEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTaskSettled, UScheduleTaskDataAsset*, Task, FTaskReward&, FinalReward);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllTasksDone);


// ── 任务执行状态枚举 (状态机吗)──────────────────────────────────
UENUM(BlueprintType)
enum class ETaskExecutionState : uint8
{
	IDLE				UMETA(DisplayName = "等待玩家编排槽位"),
	PLAYINGCUTSCENE		UMETA(DisplayName = "正在播放当前任务过场"),
	WAITINGFOREVENT		UMETA(DisplayName = "等待玩家处理随机事件弹窗"),
	SETTLING			UMETA(DisplayName = "当前任务数值结算中"),
	DAYEND				UMETA(DisplayName = "所有槽位任务执行完毕"),
};

//  ── 日程子系统 ──────────────────────────────────
UCLASS()
class AIGAME_API UScheduleSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// ── 委托 ──────────────────────────────────
	UPROPERTY(BlueprintAssignable)
	FOnCandidateListRefreshed OnCandidateListRefreshed;
	
	UPROPERTY(BlueprintAssignable)
	FOnSlotsChanged OnSlotsChanged;	
	
	UPROPERTY(BlueprintAssignable)
	FOnExecutionSlotChanged OnExecutionSlotChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnRandomEventTriggered OnRandomEventTriggered;
	
	UPROPERTY(BlueprintAssignable)
	FOnTaskSettled OnTaskSettled;
	
	UPROPERTY(BlueprintAssignable)
	FOnAllTasksDone OnAllTasksDone;
	
	// ── 初始化各类任务池 ────────────────────────────────────
	// 传分类好的任务池（GameMode或关卡蓝图中调用）
	UFUNCTION(BlueprintCallable, Category = "日程")
	void InitializeTaskPools(
		const TArray<UScheduleTaskDataAsset*>& AlphaTasks,
		const TArray<UScheduleTaskDataAsset*>& BetaTasks,
		const TArray<UScheduleTaskDataAsset*>& GammaTasks
		);
	
	// ── 每日刷新（GameMode 在新一天开始时调用） ───────
	UFUNCTION(BlueprintCallable, Category = "日程")
	void RefreshDailySchedule();
	
	// ── 槽位编排接口（出发前玩家操作） ───────────────
	// 添加日程到槽位，返回其在槽位的索引，-1表示失败
	UFUNCTION(BlueprintCallable, Category = "日程 | 槽位")
	int32 AddTaskToSlot(UScheduleTaskDataAsset* Task);
	
	// 移除某槽位的任务
	UFUNCTION(BlueprintCallable, Category = "日程 | 槽位")
	void RemoveTaskFromSlot(int32 SlotIndex);
	
	// 获取当前槽位列表的内容，空槽位为nullptr
	UFUNCTION(BlueprintCallable, Category = "日程 | 槽位")
	TArray<UScheduleTaskDataAsset*> GetCurrentSlots() const {return Slots;}

	// 获取候选日程列表
	UFUNCTION(BlueprintCallable, Category = "日程")
	TArray<UScheduleTaskDataAsset*> GetCandidateTasks() const {return CandidateTasks;}
	
	// 检查某任务是否在槽中
	UFUNCTION(BlueprintCallable, Category = "日程 | 槽位")
	bool IsTaskInSlot(UScheduleTaskDataAsset* Task) const;
	
	// ── 执行控制 ──────────────────────────────────────
	// 点击出发按钮，按槽位依序进行日程
	// 返回false表示槽位为空，或状态不对
	UFUNCTION(BlueprintCallable, Category = "日程 | 执行")
	bool StartExecution();
	
	// 过场动画播放完毕，UI蓝图回调
	UFUNCTION(BlueprintCallable, Category = "日程 | 执行")
	void NotifyCutSceneFinished();
	
	// 玩家处理随机事件，A或B
	UFUNCTION(BlueprintCallable, Category = "日程 | 执行")
	void ResolveRandomEvent(bool bOptionA);
	
	// 结算界面动画播放完毕，UI蓝图回调
	UFUNCTION(BlueprintCallable, Category = "日程 | 执行")
	void NotifySettlementFinished();
	
	// 确认结束今天，推进日期
	UFUNCTION(BlueprintCallable, Category = "日程 | 执行")
	void ConfirmDayEnd();
	
	// ── 状态查询 ──────────────────────────────────────
	// 获取任务执行状态
	UFUNCTION(BlueprintCallable, Category = "日程")
	ETaskExecutionState GetExecutionState() const {return ExecutionState;}
	
	// 获取当前执行任务
	UFUNCTION(BlueprintCallable, Category = "日程")
	UScheduleTaskDataAsset* GetCurrentExecutingTask() const {return CurrentTask;}
	
	// 获取当前执行任务的槽位索引
	UFUNCTION(BlueprintCallable, Category = "日程")
	int32 GetCurrentSlotIndex() const {return CurrentSlotIndex;}
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
private:
	// 最大槽位数
	static constexpr int32 MaxSlots = 5;
	
	// 任务池 (按分类)
	UPROPERTY()
	TArray<UScheduleTaskDataAsset*> AlphaPool;
	
	UPROPERTY()
	TArray<UScheduleTaskDataAsset*> BetaPool;
	
	UPROPERTY()
	TArray<UScheduleTaskDataAsset*> GammaPool;
	
	// 今日候选列表
	UPROPERTY()
	TArray<UScheduleTaskDataAsset*> CandidateTasks;
	
	// 玩家编排的槽位
	UPROPERTY()
	TArray<UScheduleTaskDataAsset*> Slots;
	
	// 执行状态
	UPROPERTY()
	UScheduleTaskDataAsset* CurrentTask = nullptr;					// 当前执行任务
	ETaskExecutionState ExecutionState = ETaskExecutionState::IDLE;	// 当前执行状态
	int32 CurrentSlotIndex = -1;									// 当前执行任务的槽位索引
	FTaskReward PendingReward;										// 任务结算奖励（含随机事件）
	
	// 内部：执行下一个槽位任务
	void ExecuteNextSlot();
	
	// 内部：检查并触发随机事件
	bool CheckAndTriggerRandomEvent(const UScheduleTaskDataAsset* Task) const;
	
	// 内部：从任务池中抽取今日候选任务
	TArray<UScheduleTaskDataAsset*> DrawCandidateTasks();
	
	// 获取GameDataSubsystem
	UGameDataSubsystem* GetGameDataSubsystem() const;
};
