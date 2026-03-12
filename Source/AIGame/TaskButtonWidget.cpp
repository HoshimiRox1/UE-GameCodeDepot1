// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskButtonWidget.h"
#include "ScheduleTaskDataAsset.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameDataSubsystem.h"

void UTaskButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 根据任务数据自动设置任务名称
	if (TaskName)
	{
		TaskName->SetText(TaskDataAsset->TaskName);
	}
	// 订阅任务按钮的点击事件
	if (TaskButton)
	{
		TaskButton->OnClicked.AddDynamic(this, &UTaskButtonWidget::OnTaskButtonClicked);
	}
}

void UTaskButtonWidget::OnTaskButtonClicked()
{
	// 检查任务数据非空
	if (not TaskDataAsset) return;
	
	// 获取 Subsystem
	UGameDataSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (Subsystem)
	{
		// 遍历任务数据所有结果，并调用子系统接口函数修改属性
		for (const auto& Modifier : TaskDataAsset->AttributeModifiers)
		{
			Subsystem->ModifyAttribute(Modifier.Key, Modifier.Value);
		}
		
		// TODO: 播放音效之类的
	}
}
