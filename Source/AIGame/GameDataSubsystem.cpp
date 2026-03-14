// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDataSubsystem.h"
#include "ScheduleTaskDataAsset.h"

void UGameDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// 初始化默认属性数值
	PlayerStats.Add(EPlayerAttribute::Decision, 0.0f);
	PlayerStats.Add(EPlayerAttribute::Social, 0.0f);
	PlayerStats.Add(EPlayerAttribute::Empathy, 0.0f);
	PlayerStats.Add(EPlayerAttribute::Cash, 100.0f);
	PlayerStats.Add(EPlayerAttribute::DailyMood, 1.0f);
}

void UGameDataSubsystem::ModifyAttribute(EPlayerAttribute Type, float Amount)
{
	if (PlayerStats.Contains(Type))
	{
		PlayerStats[Type] += Amount;
		
		// 将心情值钳制在0-1
		if (Type == EPlayerAttribute::DailyMood)
		{
			PlayerStats[Type] = FMath::Clamp(PlayerStats[Type], 0.0f, 1.0f);
		}
		
		// 广播属性修改
		OnAttributeChanged.Broadcast(Type, PlayerStats[Type]);
	}
}

void UGameDataSubsystem::ResetDailyStats()
{
	ModifyAttribute(EPlayerAttribute::DailyMood, 1.0f - PlayerStats[EPlayerAttribute::DailyMood]);
	
}

float UGameDataSubsystem::GetAttributeValue(EPlayerAttribute Attribute) const
{
	return PlayerStats.Contains(Attribute) ? PlayerStats[Attribute] : 0.0f;
}

void UGameDataSubsystem::SelectTask(UScheduleTaskDataAsset* NewTask)
{
}

void UGameDataSubsystem::ComfirmSelectedTask()
{
}

void UGameDataSubsystem::ExecuteToday()
{
	if (not SelectedTask) return;
	
	// 修改属性
	for (const auto& Modifier : SelectedTask->AttributeModifiers)
	{
		ModifyAttribute(Modifier.Key, Modifier.Value);
	}
	
	// 清空选中任务
	SelectedTask = nullptr;
	
	// 当前日子++
	CurrentDate++;
	
	// 广播事件——新的一天，刷新ScheduleMain
	// TODO:但其实这里有BUG，就是ModifyAttribute()本来就会广播导致UI刷新，这里广播其实没太大用处，后面重整逻辑
	OnNewDayStarted.Broadcast();
}
