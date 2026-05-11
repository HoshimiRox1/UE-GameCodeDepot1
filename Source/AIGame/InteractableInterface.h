// 交互接口模块：定义所有可交互 Actor 必须暴露给主角调用的统一契约

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// 可被蓝图实现的交互接口类型
UINTERFACE(MinimalAPI, BlueprintType)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

// 可交互对象的接口函数集合
class AIGAME_API IInteractableInterface
{
	GENERATED_BODY()

public:
	// 执行交互对象自己的交互逻辑
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "交互")
	void Interact(AActor* Interactor);
	virtual void Interact_Implementation(AActor* Interactor);

	// 返回当前交互对象要显示给玩家的提示文本
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "交互")
	FText GetInteractPrompt(AActor* Interactor) const;
	virtual FText GetInteractPrompt_Implementation(AActor* Interactor) const;

	// 判断当前交互者是否允许触发交互
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "交互")
	bool CanInteract(AActor* Interactor) const;
	virtual bool CanInteract_Implementation(AActor* Interactor) const;
};
