// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDataSubsystem.h"
#include "ScheduleTaskDataAsset.h"

void UGameDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

int32 UGameDataSubsystem::GetAttributeValue(EAttributeType Type) const
{
	// 根据Type返回对应属性
	switch (Type)
	{
	case EAttributeType::Decision:	return GameState.DecisionValue;
	case EAttributeType::Social:	return GameState.SocialValue;
	case EAttributeType::Empathy:	return GameState.EmpathyValue;
	default:						return 0;
	}
}

void UGameDataSubsystem::ApplyTaskReward(const FTaskReward& Reward)
{
	// 遍历任务奖励内的属性奖励，根据类型修改对应属性值
	for (const auto& AR : Reward.AttributeRewards)
	{
		switch (AR.Attribute)
		{
		case EAttributeType::Decision:
			GameState.DecisionValue += AR.Value;
			break;
		case  EAttributeType::Empathy:
			GameState.EmpathyValue += AR.Value;
			break;
		case EAttributeType::Social:
			GameState.SocialValue += AR.Value;
			break;
		}
		
		// 广播：属性变化
		OnAttributeChanged.Broadcast(AR.Attribute, GetAttributeValue(AR.Attribute));
	}
	
	// 修改其他单项属性
	GameState.ThoughtPoints += Reward.ThoughtPointReward;
	GameState.AnnieRapport += Reward.AnnieReward;
	GameState.Cash -= Reward.CashCost;
	
	// 广播：其他属性变化
	if (Reward.ThoughtPointReward != 0) OnThoughtPointsChanged.Broadcast(GameState.ThoughtPoints);
	if (Reward.AnnieReward != 0) OnAnnieRapportChanged.Broadcast(GameState.AnnieRapport);
	if (Reward.CashCost != 0) OnCashChanged.Broadcast(GameState.Cash);
}

void UGameDataSubsystem::AdvanceDate()
{
	GameState.CurrentDate.AdvanceDay();
	OnDateChanged.Broadcast(GameState.CurrentDate);
}


