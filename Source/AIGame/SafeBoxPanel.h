// 保险箱旋钮面板模块：提供控件蓝图继承使用的旋钮拖拽算法与解锁判定

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SafeBoxPanel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSafeBoxDialAngleChangedSignature, float, CurrentAngle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSafeBoxDialSolvedSignature);

// 保险箱旋钮控件基类，负责角度计算和谜题判定
UCLASS(BlueprintType, Blueprintable)
class AIGAME_API USafeBoxPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	// 设置保险箱旋钮的目标角度和容差
	UFUNCTION(BlueprintCallable, Category = "保险箱旋钮")
	void SetSafeDialConfig(float InTargetAngle, float InTolerance);

	// 设置旋钮中心点在 Widget 本地空间中的坐标
	UFUNCTION(BlueprintCallable, Category = "保险箱旋钮")
	void SetDialCenterInWidget(FVector2D InDialCenter);

	// 开始拖拽旋钮并记录鼠标初始角度
	UFUNCTION(BlueprintCallable, Category = "保险箱旋钮")
	void BeginDialDrag(FVector2D MouseLocalPosition);

	// 根据鼠标当前位置更新旋钮角度
	UFUNCTION(BlueprintCallable, Category = "保险箱旋钮")
	void UpdateDialDrag(FVector2D MouseLocalPosition);

	// 结束当前旋钮拖拽
	UFUNCTION(BlueprintCallable, Category = "保险箱旋钮")
	void EndDialDrag();

	// 获取当前旋钮角度
	UFUNCTION(BlueprintPure, Category = "保险箱旋钮")
	float GetCurrentAngle() const;

	// 重置旋钮拖拽状态和当前角度
	UFUNCTION(BlueprintCallable, Category = "保险箱旋钮")
	void ResetSafeDial();

	// C++ 算出新角度后通知控件蓝图刷新旋钮显示
	UPROPERTY(BlueprintAssignable, Category = "保险箱旋钮")
	FSafeBoxDialAngleChangedSignature OnDialAngleChanged;

	// 当前角度进入目标容差后通知外部保险箱已解锁
	UPROPERTY(BlueprintAssignable, Category = "保险箱旋钮")
	FSafeBoxDialSolvedSignature OnSafeDialSolved;

protected:
	// 目标解锁角度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "保险箱旋钮")
	float TargetAngle = 135.f;

	// 允许的目标角度误差
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "保险箱旋钮")
	float Tolerance = 5.f;

	// 当前旋钮累计角度
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "保险箱旋钮")
	float CurrentAngle = 0.f;

	// 旋钮中心点在 Widget 本地空间中的坐标
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "保险箱旋钮")
	FVector2D DialCenterInWidget = FVector2D::ZeroVector;

	// 当前是否正在拖拽旋钮
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "保险箱旋钮")
	bool bIsDragging = false;

	// 谜题是否已经解锁
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "保险箱旋钮")
	bool bSolved = false;

private:
	float LastMouseAngle = 0.f;

	// 将任意角度归一化到 0 到 360 区间
	static float NormalizeToZero360(float Angle);

	// 计算鼠标位置相对旋钮中心的方向角度
	float CalculateMouseAngle(FVector2D MouseLocalPosition) const;

	// 检查当前角度是否已经满足解锁条件
	void CheckSolved();
};
