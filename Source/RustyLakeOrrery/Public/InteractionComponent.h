// InteractionComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

/**
 * @brief 玩家交互检测组件（黑盒设计）
 * 
 * 附加在PlayerController上，自动处理所有交互检测和触发逻辑。
 * 蓝图开发者只需：
 * 1. 将此组件添加到BP_PlayerController
 * 2. 配置检测距离等基础参数
 * 3. 完成！组件会自动检测、高亮、触发交互
 * 
 * 设计原则：
 * - 全自动运行：无需蓝图编写任何逻辑
 * - 最小配置：只暴露必要的调试和配置参数
 * - 高度集成：与InteractableComponent无缝协作
 */
UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class RUSTYLAKEORRERY_API UInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInteractionComponent();

    // ========================================================================
    // 配置参数（仅需配置这些）
    // ========================================================================

    /** 交互检测的最大距离（2D游戏通常设置较大值） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Settings")
    float InteractionDistance = 10000.0f;

    /** 射线检测使用的碰撞通道 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Settings")
    TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

    /** 是否启用调试可视化（开发时使用） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Debug")
    bool bShowDebugTrace = false;

    // ========================================================================
    // 只读状态（供UI系统读取）
    // ========================================================================

    /**
     * @brief 获取当前聚焦的Actor
     * @return 当前聚焦的Actor，如果没有则返回nullptr
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
    AActor* GetFocusedActor() const { return CurrentFocusedActor; }

    /**
     * @brief 获取当前聚焦对象的交互提示文本
     * @return 交互提示文本，如果没有聚焦对象则返回空文本
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
    FText GetCurrentPrompt() const;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // ========================================================================
    // 内部实现（完全封装，蓝图不可见）
    // ========================================================================

    /** 执行射线检测 */
    void PerformInteractionTrace();

    /** 处理玩家点击输入 */
    void HandleInteractionInput();

    /** 更新聚焦状态 */
    void UpdateFocusedActor(AActor* NewFocusedActor);

    /** 绑定输入事件 */
    void BindInputActions();

    /** 当前聚焦的Actor */
    UPROPERTY()
    AActor* CurrentFocusedActor = nullptr;

    /** 当前聚焦的InteractableComponent */
    UPROPERTY()
    class UInteractableComponent* CurrentInteractableComponent = nullptr;

    /** 缓存的PlayerController */
    UPROPERTY()
    class APlayerController* CachedPlayerController = nullptr;

    /** 输入是否已绑定 */
    bool bInputBound = false;
};
