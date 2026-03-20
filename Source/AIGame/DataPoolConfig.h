// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataPoolConfig.generated.h"

class UScheduleTaskDataAsset;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AIGAME_API UDataPoolConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "任务池")
	TArray<UScheduleTaskDataAsset*> AlphaTasks;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "任务池")
	TArray<UScheduleTaskDataAsset*> BetaTasks;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "任务池")
	TArray<UScheduleTaskDataAsset*> GammaTasks;
};
