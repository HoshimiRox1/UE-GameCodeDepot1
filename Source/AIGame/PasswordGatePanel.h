// 密码门面板模块：提供控件蓝图继承使用的按钮密码输入、校验与解锁广播

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PasswordGatePanel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPasswordGateSolvedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPasswordGateInputChangedSignature, FText, PreviewText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPasswordGateStatusChangedSignature, FText, StatusText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPasswordGateWrongCodeSignature);

// 密码门控件基类，负责按钮序列输入和密码判定
UCLASS(BlueprintType, Blueprintable)
class AIGAME_API UPasswordGatePanel : public UUserWidget
{
	GENERATED_BODY()

public:
	// 初始化密码门面板的来源 Actor 和正确密码序列
	UFUNCTION(BlueprintCallable, Category = "密码门")
	void InitPasswordGatePanel(AActor* InSourceDoorActor, const TArray<FName>& InRequiredCode);

	// 兼容文档里的旧命名，实际蓝图父类仍然选择 PasswordGatePanel
	UFUNCTION(BlueprintCallable, Category = "密码门")
	void InitSecurityDoorPanel(AActor* InSourceDoorActor, const TArray<FName>& InRequiredCode);

	// 提交一个按钮 ID，Backspace、Confirm、Reset 会在 C++ 内部分流
	UFUNCTION(BlueprintCallable, Category = "密码门")
	void SubmitKey(FName ButtonID);

	// 删除当前输入的最后一位
	UFUNCTION(BlueprintCallable, Category = "密码门")
	void SubmitBackspace();

	// 确认当前输入并触发密码校验
	UFUNCTION(BlueprintCallable, Category = "密码门")
	void SubmitConfirm();

	// 清空当前输入并刷新显示
	UFUNCTION(BlueprintCallable, Category = "密码门")
	void ResetPasswordGateInput();

	// 兼容文档里的旧命名
	UFUNCTION(BlueprintCallable, Category = "密码门")
	void ResetSecurityDoorInput();

	// 获取当前输入进度文本
	UFUNCTION(BlueprintPure, Category = "密码门")
	FText GetInputPreviewText() const;

	// 获取当前已经输入的按钮数量
	UFUNCTION(BlueprintPure, Category = "密码门")
	int32 GetCurrentInputLength() const;

	// 查询密码门是否已经解锁
	UFUNCTION(BlueprintPure, Category = "密码门")
	bool IsPasswordGateSolved() const;

	// 兼容文档里的旧命名
	UFUNCTION(BlueprintPure, Category = "密码门")
	bool IsSecurityDoorSolved() const;

	// 输入正确后通知外部 Actor 或 PlayerController 执行开门逻辑
	UPROPERTY(BlueprintAssignable, Category = "密码门")
	FPasswordGateSolvedSignature OnPasswordGateSolved;

	// 兼容文档里的旧事件命名
	UPROPERTY(BlueprintAssignable, Category = "密码门")
	FPasswordGateSolvedSignature OnSecurityDoorSolved;

	// 输入变化后通知控件蓝图刷新输入进度
	UPROPERTY(BlueprintAssignable, Category = "密码门")
	FPasswordGateInputChangedSignature OnPasswordGateInputChanged;

	// 兼容文档里的旧事件命名
	UPROPERTY(BlueprintAssignable, Category = "密码门")
	FPasswordGateInputChangedSignature OnSecurityDoorInputChanged;

	// 状态变化后通知控件蓝图刷新提示文本
	UPROPERTY(BlueprintAssignable, Category = "密码门")
	FPasswordGateStatusChangedSignature OnPasswordGateStatusChanged;

	// 兼容文档里的旧事件命名
	UPROPERTY(BlueprintAssignable, Category = "密码门")
	FPasswordGateStatusChangedSignature OnSecurityDoorStatusChanged;

	// 密码错误后通知控件蓝图播放错误反馈
	UPROPERTY(BlueprintAssignable, Category = "密码门")
	FPasswordGateWrongCodeSignature OnPasswordGateWrongCode;

	// 兼容文档里的旧事件命名
	UPROPERTY(BlueprintAssignable, Category = "密码门")
	FPasswordGateWrongCodeSignature OnSecurityDoorWrongCode;

protected:
	// 打开当前面板的场景 Actor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "密码门")
	TObjectPtr<AActor> SourceDoorActor = nullptr;

	// 正确密码序列，例如 K1, K5, K8, K3
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "密码门")
	TArray<FName> RequiredCode;

	// 玩家当前输入的按钮序列
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "密码门")
	TArray<FName> CurrentInput;

	// 最大输入长度，默认等于 RequiredCode 长度
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "密码门")
	int32 MaxInputLength = 0;

	// 当前面板是否已经完成解锁
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "密码门")
	bool bSolved = false;

private:
	// 检查当前输入是否等于正确密码
	void CheckCode();

	// 广播当前输入进度
	void BroadcastInputPreview();

	// 同时广播新旧两套状态事件名
	void BroadcastStatus(const FText& StatusText);

	// 同时广播新旧两套解锁事件名
	void BroadcastSolved();

	// 同时广播新旧两套错误事件名
	void BroadcastWrongCode();
};
