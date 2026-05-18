// 保险箱旋钮面板模块：实现控件蓝图继承使用的旋钮拖拽算法与解锁判定


#include "SafeBoxPanel.h"

void USafeBoxPanel::SetSafeDialConfig(float InTargetAngle, float InTolerance)
{
	TargetAngle = NormalizeToZero360(InTargetAngle);
	Tolerance = FMath::Max(0.f, InTolerance);
	bSolved = false;
}

void USafeBoxPanel::SetDialCenterInWidget(FVector2D InDialCenter)
{
	DialCenterInWidget = InDialCenter;
}

void USafeBoxPanel::BeginDialDrag(FVector2D MouseLocalPosition)
{
	bIsDragging = true;
	LastMouseAngle = CalculateMouseAngle(MouseLocalPosition);
}

void USafeBoxPanel::UpdateDialDrag(FVector2D MouseLocalPosition)
{
	if (!bIsDragging || bSolved)
	{
		return;
	}

	const float CurrentMouseAngle = CalculateMouseAngle(MouseLocalPosition);
	const float DeltaAngle = FMath::FindDeltaAngleDegrees(LastMouseAngle, CurrentMouseAngle);

	CurrentAngle = NormalizeToZero360(CurrentAngle + DeltaAngle);
	LastMouseAngle = CurrentMouseAngle;

	OnDialAngleChanged.Broadcast(CurrentAngle);
	CheckSolved();
}

void USafeBoxPanel::EndDialDrag()
{
	bIsDragging = false;
}

float USafeBoxPanel::GetCurrentAngle() const
{
	return CurrentAngle;
}

void USafeBoxPanel::ResetSafeDial()
{
	bIsDragging = false;
	bSolved = false;
	LastMouseAngle = 0.f;
	CurrentAngle = 0.f;

	OnDialAngleChanged.Broadcast(CurrentAngle);
}

float USafeBoxPanel::NormalizeToZero360(float Angle)
{
	float NormalizedAngle = FMath::Fmod(Angle, 360.f);
	if (NormalizedAngle < 0.f)
	{
		NormalizedAngle += 360.f;
	}

	return NormalizedAngle;
}

float USafeBoxPanel::CalculateMouseAngle(FVector2D MouseLocalPosition) const
{
	const FVector2D Direction = MouseLocalPosition - DialCenterInWidget;
	return FMath::RadiansToDegrees(FMath::Atan2(Direction.Y, Direction.X));
}

void USafeBoxPanel::CheckSolved()
{
	if (bSolved)
	{
		return;
	}

	const float AngleError = FMath::Abs(FMath::FindDeltaAngleDegrees(TargetAngle, CurrentAngle));
	if (AngleError <= Tolerance)
	{
		bSolved = true;
		bIsDragging = false;
		OnSafeDialSolved.Broadcast();
	}
}

