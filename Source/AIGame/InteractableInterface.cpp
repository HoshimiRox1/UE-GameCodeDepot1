// 交互接口模块：提供可交互接口的默认 C++ 行为


#include "InteractableInterface.h"
#include "GameFramework/Actor.h"

void IInteractableInterface::Interact_Implementation(AActor* Interactor)
{
}

FText IInteractableInterface::GetInteractPrompt_Implementation(AActor* Interactor) const
{
	return FText::FromString(TEXT("按 E 交互"));
}

bool IInteractableInterface::CanInteract_Implementation(AActor* Interactor) const
{
	return IsValid(Interactor);
}
