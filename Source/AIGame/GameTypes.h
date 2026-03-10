// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
/**
 * 
 */

// 角色属性   
UENUM(BlueprintType)
enum class EPlayerAttribute : uint8
{
	Decision	UMETA(DisplayName = "决策"),
	Social		UMETA(DisplayName = "社交"),
	Empathy		UMETA(DisplayName = "共情"),
	Cash		UMETA(DisplayName = "现金"),
	DailyMood		UMETA(DisplayName = "当日心情"),
};

class AIGAME_API GameTypes
{
public:
	GameTypes();
	~GameTypes();
};
