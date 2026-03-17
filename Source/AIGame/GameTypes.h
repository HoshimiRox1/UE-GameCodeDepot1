// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
/**
 * 
 */

// 角色属性类型枚举
UENUM(BlueprintType)
enum class EAttributeType : uint8
{
	Decision	UMETA(DisplayName = "决策"),
	Social		UMETA(DisplayName = "社交"),
	Empathy		UMETA(DisplayName = "共情"),
};

// 任务项目类型枚举
UENUM(BlueprintType)
enum class ETaskCategory : uint8
{
	Alpha		UMETA(DisplayName = "项目甲-单属性+思考"),
	Beta		UMETA(DisplayName = "项目乙-双属性"),
	Gamma		UMETA(DisplayName = "项目丙-全属性-默契"),
	Delta		UMETA(DisplayName = "项目丁-特殊效果"),
};

// 单条任务奖励结构体
USTRUCT(BlueprintType)
struct FAttributeReward
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "任务奖励")
	EAttributeType Attribute = EAttributeType::Decision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "任务奖励")
	int32 Value = 0;
};

// ── 任务奖励结构体（一个任务的全部奖励） ──────────────
USTRUCT(BlueprintType)
struct FTaskReward
{
	GENERATED_BODY()
	
	// 属性加成列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "任务奖励")
	TArray<FAttributeReward> AttributeRewards;
	
	// 思考点加成--甲
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "任务奖励")
	int32 ThoughtPointReward = 0;
	
	// 与安妮的默契加成--丙
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "任务奖励")
	int32 AnnieReward = 0;
	
	// 资金变化--消耗
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "任务奖励")
	int32 CashCost = 0;
};

// ── 随机事件选项结构体 ─────────────────────────────────
USTRUCT(BlueprintType)
struct FRamdomEventOption
{
	GENERATED_BODY()
	
	// 选项名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "随机事件")
	FText OptionName;
	
	// 奖励
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "随机事件")
	FTaskReward Reward;;
};

// ── 游戏日期结构体 ─────────────────────────────────────
USTRUCT(BlueprintType)
struct FGameDate
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "游戏日期")
	int32 Year = 2077;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "游戏日期")
	int32 Month = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "游戏日期")
	int32 Week = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "游戏日期")
	int32 Day = 11;
	
	// 日期流转，自动处理周数，月份和年份进位
	void AdvanceDay();
	
	// 根据月份获取天数
	int32 GetDaysInMonth() const;
	
	// 转换为字符串
	FString ToString() const;
};

//	── 游戏整体状态（搁置字段保留接口，逻辑不实现） ─────
USTRUCT(BlueprintType)
struct FGameState
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "整体游戏状态")
	int32 DecisionValue = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "整体游戏状态")
	int32 SocialValue = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "整体游戏状态")
	int32 EmpathyValue = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "整体游戏状态")
	int32 ThoughtPoints = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "整体游戏状态")
	int32 Cash = 500;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "整体游戏状态")
	int32 AnnieRapport = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "整体游戏状态")
	int32 Mood = 0;
	
	FGameDate CurrentDate;
};


class AIGAME_API GameTypes
{
public:
	GameTypes();
	~GameTypes();
};
