// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameTypes.h"
#include "ScheduleTaskDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class AIGAME_API UScheduleTaskDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "任务")
	FText TaskName;
	
	// 任务结果的属性修改列表
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "任务结果")
	TMap<EPlayerAttribute, float> AttributeModifiers;
};
