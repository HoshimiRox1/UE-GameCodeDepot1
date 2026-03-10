// Fill out your copyright notice in the Description page of Project Settings.


#include "ScheduleMainWidget.h"
#include "StatBarWidget.h"
#include "GameDataSubsystem.h"
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


