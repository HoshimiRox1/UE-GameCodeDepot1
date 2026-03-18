// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTypes.h"

void FGameDate::AdvanceDay()
{
	Day += 1;
	
	// 超出当前月的天数就进位
	if (Day > GetDaysInMonth())
	{
		Day = 1;
		Month += 1;
		
		// 超出12,月份进位
		if (Month > 12)
		{
			Month = 1;
			Year += 1;
		}
	}
}

int32 FGameDate::GetDaysInMonth() const
{
	static int32 DaysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	// 处理数组下标和实际月份的偏移
	return DaysInMonth[Month - 1];
}

FString FGameDate::ToString() const
{
	return FString::Printf(TEXT("%d年%d月%d日"), Year, Month, Day);
}

GameTypes::GameTypes()
{
}

GameTypes::~GameTypes()
{
}
