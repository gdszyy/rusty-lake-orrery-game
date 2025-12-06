// Interactable.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

/**
 * @brief UObject包装类,用于Unreal的反射系统。
 * 所有可交互对象的蓝图接口基类。
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractable : public UInterface
{
    GENERATED_BODY()
};

/**
 * @brief 所有可交互对象必须实现的接口。
 * 
 * 此接口定义了游戏中所有可交互对象的核心行为。任何希望响应玩家交互的Actor
 * 都应该实现这个接口。交互系统通过射线检测来识别实现了此接口的对象,并调用
 * 相应的交互函数。
 * 
 * 典型的交互流程:
 * 1. 玩家视线移动到对象上 -> OnBeginFocus() 被调用
 * 2. 玩家点击对象 -> OnInteract() 被调用
 * 3. 玩家视线离开对象 -> OnEndFocus() 被调用
 * 
 * @see UInteractionComponent
 */
class RUSTYLAKEORRERY_API IInteractable
{
    GENERATED_BODY()

public:
    /**
     * @brief 当玩家点击或触摸此对象时调用。
     * 
     * 这是主要的交互函数,应该在此实现对象被交互时的核心逻辑,例如:
     * - 播放交互动画
     * - 打开UI界面
     * - 将物品添加到背包
     * - 触发谜题事件
     * - 播放对话或独白
     * 
     * @param Interactor 发起交互的Actor,通常是玩家的PlayerController或Pawn。
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void OnInteract(AActor* Interactor);

    /**
     * @brief 当玩家的视线开始聚焦于此对象时调用。
     * 
     * 此函数用于提供视觉反馈,让玩家知道对象是可交互的。典型的实现包括:
     * - 显示高亮轮廓(通过后期处理或材质)
     * - 改变鼠标光标样式
     * - 显示交互提示UI
     * - 播放聚焦音效
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void OnBeginFocus();

    /**
     * @brief 当玩家的视线离开此对象时调用。
     * 
     * 此函数用于清理OnBeginFocus()中设置的视觉效果。典型的实现包括:
     * - 移除高亮轮廓
     * - 恢复默认鼠标光标
     * - 隐藏交互提示UI
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void OnEndFocus();

    /**
     * @brief 当玩家尝试在此对象上使用背包中的物品时调用。
     * 
     * 此函数用于实现"物品使用"交互,例如用钥匙开锁、用幻灯片放入幻灯机等。
     * 对象应该检查传入的物品是否是它所期望的,如果匹配则执行相应逻辑并返回true,
     * 否则返回false。
     * 
     * @param ItemData 玩家尝试使用的物品数据资产。
     * @param Interactor 发起交互的Actor,通常是玩家的PlayerController或Pawn。
     * @return 如果物品被成功使用则返回true,否则返回false。
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    bool OnUseItem(class UItemDataAsset* ItemData, AActor* Interactor);

    /**
     * @brief 获取此对象的交互提示文本。
     * 
     * 此函数用于在UI中显示交互提示,例如"打开门"、"拾取钥匙"、"观察画作"等。
     * 返回的文本应该简洁明了,描述玩家点击后会发生什么。
     * 
     * @return 交互提示文本。
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    FText GetInteractionPrompt() const;

    /**
     * @brief 检查此对象当前是否可以被交互。
     * 
     * 此函数用于动态控制对象的可交互状态。例如,一个门可能在某些条件下
     * (如谜题未解决)不可交互。如果返回false,交互组件将不会调用OnInteract()。
     * 
     * @return 如果对象可以被交互则返回true,否则返回false。
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    bool CanInteract() const;
};
