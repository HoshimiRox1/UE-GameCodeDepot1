// 主角模块：处理玩家移动输入与交互输入入口

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class EMoveDirection : uint8
{
	RIGHT	UMETA(DisplayName = "右边"),
	LEFT	UMETA(DisplayName = "左边")
};


/**
 * 
 */
UCLASS(BlueprintType)
class AIGAME_API AMainCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// ---输入资产--------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "输入")
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "输入")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "输入")
	UInputAction* InteractAction;
	
	// ---回调函数--------------
	// 处理移动输入
	void Move(const FInputActionValue& Value);
	
	// 处理交互输入
	void Interact(const FInputActionValue& Value);

	// 设置当前可交互目标
	UFUNCTION(BlueprintCallable, Category = "交互")
	void SetCurrentInteractTarget(AActor* TargetActor);

	// 清除当前可交互目标
	UFUNCTION(BlueprintCallable, Category = "交互")
	void ClearCurrentInteractTarget(AActor* TargetActor);

	// 获取当前交互目标的提示文本
	UFUNCTION(BlueprintCallable, Category = "交互")
	FText GetCurrentInteractPrompt();

	// ---交互对象--------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "交互")
	AActor* CurrentInteractTarget = nullptr;
	
	// ---2.5D移动x轴限制移动变量--------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "移动")
	float MinY = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "移动")
	float MaxY = 2000.f;
};
