// Fill out your copyright notice in the Description page of Project Settings.


#include "ScheduleSubsystem.h"
#include "GameDataSubsystem.h"

void UScheduleSubsystem::InitializeTaskPools(
	const TArray<UScheduleTaskDataAsset*>& AlphaTasks,
	const TArray<UScheduleTaskDataAsset*>& BetaTasks, 
	const TArray<UScheduleTaskDataAsset*>& GammaTasks)
{
	AlphaPool = AlphaTasks;
	BetaPool  = BetaTasks;
	GammaPool = GammaTasks;
}

void UScheduleSubsystem::RefreshDailySchedule()
{
	// 从任务池中抽取今日候选任务
	CandidateTasks = DrawCandidateTasks();
	
	// 清空槽位，状态机复原
	Slots.Init(nullptr, MaxSlots);
	ExecutionState = ETaskExecutionState::IDLE;
	CurrentSlotIndex = -1;
	
	// 委托广播：候选任务列表刷新
	OnCandidateListRefreshed.Broadcast(CandidateTasks);
	// 委托广播：槽位列表刷新
	OnSlotsChanged.Broadcast(Slots);
}

int32 UScheduleSubsystem::AddTaskToSlot(UScheduleTaskDataAsset* Task)
{
	// 边缘检测：任务是否存在以及状态是否正确
	if (not Task or ExecutionState != ETaskExecutionState::IDLE)	return -1;
	
	// 检查任务是否已添加
	if (IsTaskInSlot(Task))	return -1;
	
	// 添加任务到槽位，并广播
	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		if (Slots[i] == nullptr)
		{
			Slots[i] = Task;
			OnSlotsChanged.Broadcast(Slots);
			return i;
		}
	}
	// 槽位已满
	return -1;
}

void UScheduleSubsystem::RemoveTaskFromSlot(int32 SlotIndex)
{ 
	// 边缘检测：索引是否合法
	if (not Slots.IsValidIndex(SlotIndex))	return;
	// 边缘检测：未执行才可删除
	if (ExecutionState != ETaskExecutionState::IDLE)	return;
	// 边缘检测，槽位是否为空
	if (Slots[SlotIndex] == nullptr)	return;
	
	// 移除任务
	Slots[SlotIndex] = nullptr;
	OnSlotsChanged.Broadcast(Slots);
}

bool UScheduleSubsystem::IsTaskInSlot(UScheduleTaskDataAsset* Task) const
{
	for (const auto& Slot : Slots)
	{
		if (Slot == Task)
		{
			return true;
		}
	}
	return false;
}

bool UScheduleSubsystem::StartExecution()
{
	// 边缘检测：状态是否正确
	if (ExecutionState != ETaskExecutionState::IDLE) return	false;
	
	// 至少一个槽位有任务，才执行
	bool bHasAnyTask = Slots.ContainsByPredicate([](const UScheduleTaskDataAsset* T){return T != nullptr;});
	if (not bHasAnyTask)	
		return false;
		
	CurrentSlotIndex = -1;
	ExecuteNextSlot();
	return true;
}

void UScheduleSubsystem::NotifyCutSceneFinished()
{
	// 边缘检测：是否处于播放过场动画状态
	if (ExecutionState != ETaskExecutionState::PLAYINGCUTSCENE)	return;
	
	// 随机事件触发->状态切换
	if (CurrentTask->bHasRandomEvent and CheckAndTriggerRandomEvent(CurrentTask))
	{
		ExecutionState = ETaskExecutionState::WAITINGFOREVENT;
		return;
	}
	
	// 槽位执行完毕->状态切换
	ExecutionState = ETaskExecutionState::SETTLING;
	OnTaskSettled.Broadcast(CurrentTask, PendingReward);
}

void UScheduleSubsystem::ResolveRandomEvent(bool bOptionA)
{
	// 边缘检测：是否处于等待随机事件状态
	if (ExecutionState != ETaskExecutionState::WAITINGFOREVENT) return;
	
	// 读取用户选择
	const FRandomEventOption& Option = bOptionA ? CurrentTask->RandomEvent.OptionA : CurrentTask->RandomEvent.OptionB;
	
	// 更新任务奖励
	for (const auto& AR : Option.Reward.AttributeRewards)
	{
		PendingReward.AttributeRewards.Add(AR);
	}
	
	PendingReward.ThoughtPointReward += Option.Reward.ThoughtPointReward;
	PendingReward.CashCost += Option.Reward.CashCost;
	PendingReward.AnnieReward += Option.Reward.AnnieReward;
	
	// 槽位执行完毕->状态切换
	ExecutionState = ETaskExecutionState::SETTLING;
	OnTaskSettled.Broadcast(CurrentTask, PendingReward);
}

void UScheduleSubsystem::NotifySettlementFinished()
{
	// 边缘检测：处于结算状态
	if (ExecutionState != ETaskExecutionState::SETTLING) return;
	
	// 写入数据
	if (UGameDataSubsystem* DataSubsystem = GetGameDataSubsystem())
	{
		DataSubsystem->ApplyTaskReward(PendingReward);
	}
	
	// 继续下一个槽位
	ExecutionState = ETaskExecutionState::IDLE;
	ExecuteNextSlot();
}

void UScheduleSubsystem::ConfirmDayEnd()
{
	// 边缘检测：是否处于今日结束
	if (ExecutionState != ETaskExecutionState::DAYEND) return;
	
	if (UGameDataSubsystem* DataSubsystem = GetGameDataSubsystem())
	{
		// 日期加1
		DataSubsystem->AdvanceDate();
		
		// 刷新日程
		RefreshDailySchedule();
	}
}

void UScheduleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	Slots.Init(nullptr, MaxSlots);
}

void UScheduleSubsystem::ExecuteNextSlot()
{
	// 找下一个非空槽位执行
	for (int32 i = CurrentSlotIndex + 1; i < MaxSlots; ++i)
	{
		if (Slots[i] != nullptr)
		{
			CurrentSlotIndex = i;
			CurrentTask = Slots[i];
			PendingReward = CurrentTask->Reward;
			ExecutionState = ETaskExecutionState::PLAYINGCUTSCENE;
			// 委托广播：获取正在执行的任务下标
			OnExecutionSlotChanged.Broadcast(CurrentSlotIndex);
			return;
		}
	}
	
	// 没有更多非空槽位，今日结束
	ExecutionState = ETaskExecutionState::DAYEND;
	OnAllTasksDone.Broadcast();
}

bool UScheduleSubsystem::CheckAndTriggerRandomEvent(const UScheduleTaskDataAsset* Task) const
{
	// 边缘检测：任务是否为空以及是否包含随机事件
	if (not Task or not Task->bHasRandomEvent)	return false;
	
	// TODO 这里没写好，还要检测属性类型等
	// 获取角色属性
	const FRandomEvent& RandomEvent = Task->RandomEvent;
	UGameDataSubsystem* DataSubsystem = GetGameDataSubsystem();
	
	if (not DataSubsystem)
	{
		return false;
	}
	
	int32 CurrentAttributeValue = DataSubsystem->GetAttributeValue(RandomEvent.Attribute);
	
	// 比较阈值
	if (CurrentAttributeValue < RandomEvent.Threshold)
	{
		return  false;
	}
	
	// 触发随机事件
	OnRandomEventTriggered.Broadcast(RandomEvent);
	return true;
}

TArray<UScheduleTaskDataAsset*> UScheduleSubsystem::DrawCandidateTasks()
{
	//TODO 这个抽取逻辑确实不会写，写个todo是给自己以后学习算法用
	TArray<UScheduleTaskDataAsset*> Result;

	// 辅助 Lambda：从任务池随机抽取 Count 个，不重复
	auto DrawFrom = [](TArray<UScheduleTaskDataAsset*>& Pool, int32 Count,
					   TArray<UScheduleTaskDataAsset*>& Out)
	{
		// 先做一次随机排列（Fisher-Yates shuffle）
		TArray<UScheduleTaskDataAsset*> Shuffled = Pool;
		for (int32 i = Shuffled.Num() - 1; i > 0; --i)
		{
			int32 j = FMath::RandRange(0, i);
			Shuffled.Swap(i, j);
		}
		// 取前 Count 个（不超过实际数量）
		int32 Take = FMath::Min(Count, Shuffled.Num());
		for (int32 i = 0; i < Take; ++i)
			Out.Add(Shuffled[i]);
	};

	// 候选列表构成：3甲 + 2乙 + 1丙 = 6个候选，玩家从中选5个放槽
	DrawFrom(AlphaPool, 3, Result);
	DrawFrom(BetaPool,  2, Result);
	DrawFrom(GammaPool, 1, Result);

	// 池子不足时做兜底：从所有池里补到至少1个
	if (Result.IsEmpty())
	{
		TArray<UScheduleTaskDataAsset*> AllTasks;
		AllTasks.Append(AlphaPool);
		AllTasks.Append(BetaPool);
		AllTasks.Append(GammaPool);

		if (!AllTasks.IsEmpty())
		{
			int32 Idx = FMath::RandRange(0, AllTasks.Num() - 1);
			Result.Add(AllTasks[Idx]);
		}
	}

	return Result;
}

UGameDataSubsystem* UScheduleSubsystem::GetGameDataSubsystem() const
{
	// 从GameInstance中获取Subsystem并返回
	UGameDataSubsystem* GameDataSubsystem = GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	return GameDataSubsystem;
}
