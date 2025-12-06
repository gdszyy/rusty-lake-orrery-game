// Interactable.cpp

#include "Interactable.h"

// ============================================================================
// IInteractable 接口默认实现
// ============================================================================

void IInteractable::OnInteract_Implementation(AActor* Interactor)
{
    // 默认实现为空,由蓝图或C++子类重载
    // TODO: 可以在这里添加默认的交互逻辑或日志输出
}

void IInteractable::OnBeginFocus_Implementation()
{
    // 默认实现为空,由蓝图或C++子类重载
    // TODO: 可以在这里添加默认的聚焦开始逻辑或日志输出
}

void IInteractable::OnEndFocus_Implementation()
{
    // 默认实现为空,由蓝图或C++子类重载
    // TODO: 可以在这里添加默认的聚焦结束逻辑或日志输出
}

bool IInteractable::OnUseItem_Implementation(UItemDataAsset* ItemData, AActor* Interactor)
{
    // 默认实现返回false,表示物品未被使用
    // TODO: 由蓝图或C++子类重载以实现具体的物品使用逻辑
    return false;
}

FText IInteractable::GetInteractionPrompt_Implementation() const
{
    // 默认返回通用的交互提示
    // TODO: 由蓝图或C++子类重载以返回具体的提示文本
    return FText::FromString(TEXT("Interact"));
}

bool IInteractable::CanInteract_Implementation() const
{
    // 默认返回true,表示对象可以被交互
    // TODO: 由蓝图或C++子类重载以实现动态的可交互性检查
    return true;
}
