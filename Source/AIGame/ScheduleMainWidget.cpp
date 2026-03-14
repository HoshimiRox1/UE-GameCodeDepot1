// Fill out your copyright notice in the Description page of Project Settings.


#include "ScheduleMainWidget.h"
#include "StatBarWidget.h"
#include "GameDataSubsystem.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void UScheduleMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 获取GameInstanceSubsystem
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		TObjectPtr<UGameDataSubsystem> DataSubsystem = GameInstance->GetSubsystem<UGameDataSubsystem>();
		
		// 订阅GameInstanceSubsystem的广播，属性改变时调用HandleAttributeChanged
		if (DataSubsystem)
		{
			DataSubsystem->OnAttributeChanged.AddDynamic(this, &UScheduleMainWidget::HandleAttributeChanged);			
			
			StatBar_Decision->OnValueUpdate(DataSubsystem->GetAttributeValue(EPlayerAttribute::Decision));
			StatBar_Social->OnValueUpdate(DataSubsystem->GetAttributeValue(EPlayerAttribute::Social));
			StatBar_Empathy->OnValueUpdate(DataSubsystem->GetAttributeValue(EPlayerAttribute::Empathy));
		}
	}
	
	// 『确认按钮』的逻辑
	if (ConfirmButton)
	{
		// 添加其点击事件到广播
		ConfirmButton->OnClicked.AddDynamic(this, &UScheduleMainWidget::OnConfirmButtonClicked);
		
		// 未选中任务时，禁用『确认按钮』
		ConfirmButton->SetIsEnabled(false);
	}
	
	// 『出发按钮』的逻辑
	if (DepartButton)
	{
		// 添加其点击事件到广播
		DepartButton->OnClicked.AddDynamic(this, &UScheduleMainWidget::OnDepartButtonClicked);
		
		// 未选中确认任务时，隐藏『出发按钮』
		DepartButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UScheduleMainWidget::HandleAttributeChanged(EPlayerAttribute AttributeType, float NewValue)
{
	switch (AttributeType)
	{
		case EPlayerAttribute::Decision:
			StatBar_Decision->OnValueUpdate(NewValue);
			break;
		case EPlayerAttribute::Social:
			StatBar_Social->OnValueUpdate(NewValue);
			break;
		case EPlayerAttribute::Empathy:
			StatBar_Empathy->OnValueUpdate(NewValue);
			break;
		default:
			break;
	}
}

void UScheduleMainWidget::OnConfirmButtonClicked()
{
}

void UScheduleMainWidget::OnDepartButtonClicked()
{
}


