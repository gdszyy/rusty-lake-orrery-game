// InteractionComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

/**
 * @brief 触控状态枚举
 */
UENUM()
enum class ETouchState : uint8
{
    None,
    Began,
    Moved,
    Ended
};

/**
 * @brief 玩家交互检测组件（黑盒设计 + 触控手势识别）
 * 
 * 附加在PlayerController上，自动处理所有交互检测和触发逻辑。
 * 支持Android触控手势识别（点击、滑动、旋转、长按）。
 * 
 * 蓝图开发者只需：
 * 1. 将此组件添加到BP_PlayerController
 * 2. 配置检测距离等基础参数
 * 3. 完成！组件会自动检测、高亮、触发交互
 * 
 * 设计原则：
 * - 全自动运行：无需蓝图编写任何逻辑
 * - 最小配置：只暴露必要的调试和配置参数
 * - 高度集成：与InteractableComponent无缝协作
 * - 触控优先：原生支持Android触控手势
 */
UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class RUSTYLAKEORRERY_API UInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInteractionComponent();

    // ========================================================================
    // 配置参数
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

    /** 是否启用触控手势调试信息 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Debug")
    bool bShowGestureDebug = false;

    // ========================================================================
    // 触控手势配置
    // ========================================================================

    /** 点击的最大移动距离（像素，超过此距离视为滑动而非点击） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Touch Settings")
    float TapMaxMovement = 20.0f;

    /** 点击的最大持续时间（秒，超过此时间视为长按而非点击） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Touch Settings")
    float TapMaxDuration = 0.3f;

    /** 旋转手势的灵敏度 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Touch Settings")
    float RotationSensitivity = 0.5f;

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

    /**
     * @brief 获取长按进度（0-1）
     * @return 长按进度，0表示未开始，1表示完成
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
    float GetLongPressProgress() const;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // ========================================================================
    // 内部实现（完全封装，蓝图不可见）
    // ========================================================================

    /** 执行射线检测 */
    void PerformInteractionTrace();

    /** 处理触控输入 */
    void HandleTouchInput(float DeltaTime);

    /** 更新聚焦状态 */
    void UpdateFocusedActor(AActor* NewFocusedActor);

    /** 绑定输入事件 */
    void BindInputActions();

    /** 处理触摸开始 */
    void OnTouchBegan(const FVector2D& TouchLocation);

    /** 处理触摸移动 */
    void OnTouchMoved(const FVector2D& TouchLocation);

    /** 处理触摸结束 */
    void OnTouchEnded(const FVector2D& TouchLocation);

    /** 识别手势类型 */
    void RecognizeGesture();

    /** 执行点击交互 */
    void ExecuteTapInteraction();

    /** 执行滑动交互 */
    void ExecuteSwipeInteraction();

    /** 从屏幕坐标执行射线检测 */
    bool TraceFromScreenPosition(const FVector2D& ScreenPosition, FHitResult& OutHitResult);

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

    // ========================================================================
    // 触控手势状态
    // ========================================================================

    /** 当前触控状态 */
    ETouchState CurrentTouchState = ETouchState::None;

    /** 触摸开始位置 */
    FVector2D TouchStartPosition = FVector2D::ZeroVector;

    /** 当前触摸位置 */
    FVector2D CurrentTouchPosition = FVector2D::ZeroVector;

    /** 触摸开始时间 */
    float TouchStartTime = 0.0f;

    /** 触摸持续时间 */
    float TouchDuration = 0.0f;

    /** 触摸移动的总距离 */
    float TouchTotalMovement = 0.0f;

    /** 上一帧的触摸位置（用于计算旋转） */
    FVector2D LastTouchPosition = FVector2D::ZeroVector;

    /** 是否正在处理旋转手势 */
    bool bIsRotating = false;

    /** 触摸开始时聚焦的Actor（用于确保整个手势作用于同一对象） */
    UPROPERTY()
    AActor* TouchStartFocusedActor = nullptr;

    /** 触摸开始时聚焦的InteractableComponent */
    UPROPERTY()
    class UInteractableComponent* TouchStartInteractableComponent = nullptr;
};
