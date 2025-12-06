// InteractionComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interactable.h"
#include "InteractionComponent.generated.h"

/**
 * @brief 负责处理玩家与世界中可交互对象的交互。
 * 
 * 此组件应该附加在PlayerController上。它每帧从玩家的视角发射射线,
 * 检测玩家正在看向的对象。如果检测到的对象实现了IInteractable接口,
 * 则调用相应的聚焦和交互函数。
 * 
 * 核心功能:
 * - 射线检测: 从屏幕中心或鼠标位置发射射线,检测可交互对象
 * - 聚焦管理: 跟踪当前聚焦的对象,并在聚焦状态改变时调用相应的接口函数
 * - 交互触发: 响应玩家输入(点击/触摸),调用当前聚焦对象的OnInteract函数
 * - 物品使用: 支持从背包拖动物品到场景对象上的交互
 * 
 * 使用方法:
 * 1. 将此组件添加到PlayerController的蓝图中
 * 2. 在项目设置中绑定"Interact"输入动作
 * 3. 确保场景中的可交互对象实现了IInteractable接口
 * 
 * @see IInteractable
 * @see UInventoryComponent
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUSTYLAKEORRERY_API UInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // ========================================================================
    // 构造函数和生命周期
    // ========================================================================

    /**
     * @brief 构造函数。
     * 设置组件的默认值和初始化状态。
     */
    UInteractionComponent();

    /**
     * @brief 组件开始播放时调用。
     * 用于初始化组件的运行时状态。
     */
    virtual void BeginPlay() override;

    /**
     * @brief 每帧调用。
     * 执行射线检测和聚焦状态更新。
     * 
     * @param DeltaTime 自上一帧以来经过的时间。
     * @param TickType Tick的类型。
     * @param ThisTickFunction 此组件的Tick函数信息。
     */
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // ========================================================================
    // 交互配置参数
    // ========================================================================

    /**
     * @brief 交互检测的最大距离(单位:厘米)。
     * 
     * 只有在此距离范围内的对象才能被检测和交互。
     * 对于点击式解谜游戏,这个值通常设置得较大,以允许玩家与远处的对象交互。
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Settings")
    float InteractionDistance = 5000.0f;

    /**
     * @brief 射线检测的碰撞通道。
     * 
     * 定义射线检测时使用的碰撞通道。可交互对象应该在此通道上响应。
     * 建议在项目设置中创建一个自定义的"Interactable"碰撞通道。
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Settings")
    TEnumAsByte<ECollisionChannel> InteractionTraceChannel = ECC_Visibility;

    /**
     * @brief 是否启用调试绘制。
     * 
     * 如果为true,将在场景中绘制射线和命中点,方便调试交互系统。
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Debug")
    bool bDrawDebugTrace = false;

    /**
     * @brief 射线检测的起点模式。
     * 
     * - ScreenCenter: 从屏幕中心发射射线(适用于固定视角的点击式游戏)
     * - MouseCursor: 从鼠标光标位置发射射线(适用于PC平台)
     * - TouchLocation: 从触摸位置发射射线(适用于移动平台)
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Settings")
    TEnumAsByte<EInteractionTraceMode> TraceMode = EInteractionTraceMode::ScreenCenter;

    // ========================================================================
    // 交互状态
    // ========================================================================

    /**
     * @brief 获取当前聚焦的可交互对象。
     * 
     * @return 当前聚焦的Actor,如果没有聚焦任何对象则返回nullptr。
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
    AActor* GetFocusedActor() const { return FocusedActor; }

    /**
     * @brief 检查当前是否有聚焦的对象。
     * 
     * @return 如果有聚焦的对象则返回true,否则返回false。
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
    bool HasFocusedActor() const { return FocusedActor != nullptr; }

    // ========================================================================
    // 交互函数
    // ========================================================================

    /**
     * @brief 执行一次交互检测。
     * 
     * 从玩家视角发射射线,检测可交互对象并更新聚焦状态。
     * 此函数通常在TickComponent中被自动调用,但也可以手动调用以强制更新。
     */
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void UpdateInteraction();

    /**
     * @brief 与当前聚焦的对象进行交互。
     * 
     * 如果当前有聚焦的对象且该对象可以被交互,则调用其OnInteract函数。
     * 此函数应该绑定到玩家的"Interact"输入动作(如鼠标左键、触摸点击)。
     */
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void Interact();

    /**
     * @brief 在当前聚焦的对象上使用指定的物品。
     * 
     * 此函数用于实现"物品使用"交互,例如从背包拖动物品到场景对象上。
     * 如果当前有聚焦的对象且该对象接受此物品,则调用其OnUseItem函数。
     * 
     * @param ItemData 要使用的物品数据资产。
     * @return 如果物品被成功使用则返回true,否则返回false。
     */
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    bool UseItemOnFocusedActor(class UItemDataAsset* ItemData);

    /**
     * @brief 手动设置聚焦的对象。
     * 
     * 此函数用于在某些特殊情况下手动控制聚焦状态,例如在UI中点击物品后
     * 需要高亮场景中的目标对象。
     * 
     * @param NewFocusedActor 要聚焦的Actor,如果为nullptr则清除当前聚焦。
     */
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void SetFocusedActor(AActor* NewFocusedActor);

    // ========================================================================
    // 事件委托
    // ========================================================================

    /**
     * @brief 当聚焦的对象发生改变时广播。
     * 
     * @param NewFocusedActor 新聚焦的Actor,如果失去聚焦则为nullptr。
     * @param OldFocusedActor 之前聚焦的Actor,如果之前没有聚焦则为nullptr。
     */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFocusChanged, AActor*, NewFocusedActor, AActor*, OldFocusedActor);
    UPROPERTY(BlueprintAssignable, Category = "Interaction|Events")
    FOnFocusChanged OnFocusChanged;

    /**
     * @brief 当玩家与对象交互时广播。
     * 
     * @param InteractedActor 被交互的Actor。
     */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteracted, AActor*, InteractedActor);
    UPROPERTY(BlueprintAssignable, Category = "Interaction|Events")
    FOnInteracted OnInteracted;

protected:
    // ========================================================================
    // 内部辅助函数
    // ========================================================================

    /**
     * @brief 执行射线检测。
     * 
     * 根据当前的TraceMode,从屏幕中心、鼠标位置或触摸位置发射射线,
     * 检测场景中的对象。
     * 
     * @param OutHitResult 输出参数,包含射线命中的详细信息。
     * @return 如果射线命中了任何对象则返回true,否则返回false。
     */
    bool PerformTrace(FHitResult& OutHitResult);

    /**
     * @brief 检查给定的Actor是否实现了IInteractable接口。
     * 
     * @param Actor 要检查的Actor。
     * @return 如果Actor实现了IInteractable接口则返回true,否则返回false。
     */
    bool IsActorInteractable(AActor* Actor) const;

    /**
     * @brief 更新聚焦状态。
     * 
     * 将聚焦从旧对象切换到新对象,并调用相应的OnBeginFocus和OnEndFocus函数。
     * 
     * @param NewFocusedActor 新聚焦的Actor,如果为nullptr则表示失去聚焦。
     */
    void UpdateFocusedActor(AActor* NewFocusedActor);

    /**
     * @brief 获取射线检测的起点和方向。
     * 
     * 根据当前的TraceMode,计算射线的起点和方向。
     * 
     * @param OutStartLocation 输出参数,射线的起点。
     * @param OutDirection 输出参数,射线的方向(单位向量)。
     * @return 如果成功获取射线参数则返回true,否则返回false。
     */
    bool GetTraceStartAndDirection(FVector& OutStartLocation, FVector& OutDirection) const;

private:
    // ========================================================================
    // 私有成员变量
    // ========================================================================

    /**
     * @brief 当前聚焦的可交互对象。
     * 
     * 此变量跟踪玩家当前正在看向的可交互对象。当玩家的视线移动时,
     * 此变量会被UpdateInteraction函数更新。
     */
    UPROPERTY()
    AActor* FocusedActor = nullptr;

    /**
     * @brief 缓存的PlayerController引用。
     * 
     * 在BeginPlay时获取并缓存,以避免每帧都进行查找。
     */
    UPROPERTY()
    class APlayerController* CachedPlayerController = nullptr;
};

/**
 * @brief 射线检测的起点模式枚举。
 */
UENUM(BlueprintType)
enum EInteractionTraceMode
{
    /** 从屏幕中心发射射线 */
    ScreenCenter UMETA(DisplayName = "Screen Center"),
    
    /** 从鼠标光标位置发射射线 */
    MouseCursor UMETA(DisplayName = "Mouse Cursor"),
    
    /** 从触摸位置发射射线 */
    TouchLocation UMETA(DisplayName = "Touch Location")
};
