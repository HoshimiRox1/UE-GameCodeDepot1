// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameTypes.h"
#include "ScheduleTaskDataAsset.generated.h"

/**
 * 
 */
// ── 随机事件结构体（目前为最简单形态：单属性阈值触发） ──────────────────────────────────
USTRUCT(BlueprintType)
struct FRandomEvent
{
	GENERATED_BODY()
	
	// 触发所需的属性类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "随机事件")
	EAttributeType Attribute = EAttributeType::Decision;
	
	// 触发所需的属性阈值
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "随机事件")
	int32 Threshold = 100;
	
	// 遍历属性列表检测是否满足条件
	bool bAllRequirementsFits = false;
	
	// 随机事件选项A
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "随机事件")
	FRandomEventOption OptionA;
	
	// 随机事件选项B
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "随机事件")
	FRandomEventOption OptionB;
	
	// 随机事件的描述文本
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "随机事件")
	FText EventDescription;
};

// ── 单个日程任务数据资产 ──────────────────────────────
UCLASS()
class AIGAME_API UScheduleTaskDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	// 任务名称
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "任务")
	FText TaskName;
	
	// 任务描述
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "任务")
	FText TaskDescription;
	
	// 任务类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "任务")
	ETaskCategory TaskCategory = ETaskCategory::Alpha;
	
	// 任务奖励
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "任务")
	FTaskReward Reward;
	
	// 是否携带随机事件
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "随机事件")
	bool bHasRandomEvent = false;
	
	// 随机事件数据(仅在bHasRandomEvent = true时有效)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "随机事件", meta = (EditCondition = "bHasRandomEvent"))
	FRandomEvent RandomEvent;
	
	// 任务配套的过场动画资产引用
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "过场动画")
	TSoftObjectPtr<UObject> CutSceneAsset;
	
	// 任务图标资产引用
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "图标")
	TSoftObjectPtr<UTexture2D> TaskIcon;
	
	// 任务资产的接口
	UFUNCTION()
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
