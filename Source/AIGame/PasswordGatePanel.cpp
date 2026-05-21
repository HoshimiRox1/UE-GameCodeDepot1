// 密码门面板模块：实现按钮密码输入、校验与解锁广播

#include "PasswordGatePanel.h"

namespace PasswordGatePanelNames
{
	const FName BackspaceKey(TEXT("Backspace"));
	const FName ConfirmKey(TEXT("Confirm"));
	const FName ResetKey(TEXT("Reset"));
	const FName ClearKey(TEXT("Clear"));
	const FText ReadyStatus = NSLOCTEXT("PasswordGatePanel", "ReadyStatus", "请输入对应代码...");
	const FText FullStatus = NSLOCTEXT("PasswordGatePanel", "FullStatus", "代码已满");
	const FText ShortStatus = NSLOCTEXT("PasswordGatePanel", "ShortStatus", "代码长度不足");
	const FText WrongStatus = NSLOCTEXT("PasswordGatePanel", "WrongStatus", "代码错误");
	const FText GrantedStatus = NSLOCTEXT("PasswordGatePanel", "GrantedStatus", "ACCESS GRANTED");
}

void UPasswordGatePanel::InitPasswordGatePanel(AActor* InSourceDoorActor, const TArray<FName>& InRequiredCode)
{
	SourceDoorActor = InSourceDoorActor;
	RequiredCode = InRequiredCode;
	MaxInputLength = RequiredCode.Num();
	CurrentInput.Reset();
	bSolved = false;

	BroadcastInputPreview();
	BroadcastStatus(PasswordGatePanelNames::ReadyStatus);
}

void UPasswordGatePanel::InitSecurityDoorPanel(AActor* InSourceDoorActor, const TArray<FName>& InRequiredCode)
{
	InitPasswordGatePanel(InSourceDoorActor, InRequiredCode);
}

void UPasswordGatePanel::SubmitKey(FName ButtonID)
{
	if (bSolved)
	{
		return;
	}

	if (ButtonID == PasswordGatePanelNames::BackspaceKey)
	{
		SubmitBackspace();
		return;
	}

	if (ButtonID == PasswordGatePanelNames::ConfirmKey)
	{
		SubmitConfirm();
		return;
	}

	if (ButtonID == PasswordGatePanelNames::ResetKey || ButtonID == PasswordGatePanelNames::ClearKey)
	{
		ResetPasswordGateInput();
		return;
	}

	if (CurrentInput.Num() >= MaxInputLength)
	{
		BroadcastStatus(PasswordGatePanelNames::FullStatus);
		return;
	}

	CurrentInput.Add(ButtonID);
	BroadcastInputPreview();
}

void UPasswordGatePanel::SubmitBackspace()
{
	if (bSolved)
	{
		return;
	}

	if (!CurrentInput.IsEmpty())
	{
		CurrentInput.Pop(EAllowShrinking::No);
	}

	BroadcastInputPreview();
	BroadcastStatus(PasswordGatePanelNames::ReadyStatus);
}

void UPasswordGatePanel::SubmitConfirm()
{
	if (bSolved)
	{
		return;
	}

	if (CurrentInput.Num() != RequiredCode.Num())
	{
		BroadcastStatus(PasswordGatePanelNames::ShortStatus);
		return;
	}

	CheckCode();
}

void UPasswordGatePanel::ResetPasswordGateInput()
{
	if (bSolved)
	{
		return;
	}

	CurrentInput.Reset();
	BroadcastInputPreview();
	BroadcastStatus(PasswordGatePanelNames::ReadyStatus);
}

void UPasswordGatePanel::ResetSecurityDoorInput()
{
	ResetPasswordGateInput();
}

FText UPasswordGatePanel::GetInputPreviewText() const
{
	TArray<FString> Segments;
	Segments.Reserve(MaxInputLength);

	for (int32 Index = 0; Index < MaxInputLength; ++Index)
	{
		Segments.Add(Index < CurrentInput.Num() ? TEXT("●") : TEXT("_"));
	}

	return FText::FromString(FString::Join(Segments, TEXT(" ")));
}

int32 UPasswordGatePanel::GetCurrentInputLength() const
{
	return CurrentInput.Num();
}

bool UPasswordGatePanel::IsPasswordGateSolved() const
{
	return bSolved;
}

bool UPasswordGatePanel::IsSecurityDoorSolved() const
{
	return IsPasswordGateSolved();
}

void UPasswordGatePanel::CheckCode()
{
	if (bSolved)
	{
		return;
	}

	for (int32 Index = 0; Index < RequiredCode.Num(); ++Index)
	{
		if (CurrentInput[Index] != RequiredCode[Index])
		{
			BroadcastStatus(PasswordGatePanelNames::WrongStatus);
			BroadcastWrongCode();
			CurrentInput.Reset();
			BroadcastInputPreview();
			return;
		}
	}

	bSolved = true;
	BroadcastStatus(PasswordGatePanelNames::GrantedStatus);
	BroadcastSolved();
}

void UPasswordGatePanel::BroadcastInputPreview()
{
	const FText PreviewText = GetInputPreviewText();
	OnPasswordGateInputChanged.Broadcast(PreviewText);
	OnSecurityDoorInputChanged.Broadcast(PreviewText);
}

void UPasswordGatePanel::BroadcastStatus(const FText& StatusText)
{
	OnPasswordGateStatusChanged.Broadcast(StatusText);
	OnSecurityDoorStatusChanged.Broadcast(StatusText);
}

void UPasswordGatePanel::BroadcastSolved()
{
	OnPasswordGateSolved.Broadcast();
	OnSecurityDoorSolved.Broadcast();
}

void UPasswordGatePanel::BroadcastWrongCode()
{
	OnPasswordGateWrongCode.Broadcast();
	OnSecurityDoorWrongCode.Broadcast();
}
