// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDataSubsystem.h"

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
