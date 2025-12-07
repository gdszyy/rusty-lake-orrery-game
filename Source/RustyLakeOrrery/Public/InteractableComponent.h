// InteractableComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

/**
 * @brief 交互模式枚举
 * 定义对象响应的输入手势类型
 */
UENUM(BlueprintType)
enum class EInteractionMode : uint8
{
    /** 点击/触摸：单次点击触发 */
    Tap UMETA(DisplayName = "Tap"),
    
    /** 滑动：需要在对象上滑动特定方向 */
    Swipe UMETA(DisplayName = "Swipe"),
    
    /** 旋转：持续触摸并移动来旋转对象 */
    Rotate UMETA(DisplayName = "Rotate"),
    
    /** 长按：需要按住一段时间 */
    LongPress UMETA(DisplayName = "Long Press")
};

/**
 * @brief 滑动方向枚举
 */
UENUM(BlueprintType)
enum class ESwipeDirection : uint8
{
    /** 向上滑动 */
    Up UMETA(DisplayName = "Up"),
    
    /** 向下滑动 */
    Down UMETA(DisplayName = "Down"),
    
    /** 向左滑动 */
    Left UMETA(DisplayName = "Left"),
    
    /** 向右滑动 */
    Right UMETA(DisplayName = "Right"),
    
    /** 任意方向 */
    Any UMETA(DisplayName = "Any Direction")
};

/**
 * @brief 交互类型枚举
 * 定义所有内置的交互行为类型，C++内部自动处理对应逻辑
 */
UENUM(BlueprintType)
enum class EInteractionType : uint8
{
    /** 观察：显示描述文本或对话 */
    Observe UMETA(DisplayName = "Observe"),
    
    /** 拾取：将物品添加到背包并销毁Actor */
    Pickup UMETA(DisplayName = "Pickup Item"),
    
    /** 场景切换：加载新场景或切换视角 */
    Navigate UMETA(DisplayName = "Navigate Scene"),
    
    /** 使用物品：需要特定物品才能触发 */
    UseItem UMETA(DisplayName = "Use Item"),
    
    /** 触发谜题：激活关联的谜题Actor */
    TriggerPuzzle UMETA(DisplayName = "Trigger Puzzle"),
    
    /** 旋转物体：持续旋转对象（如钟表指针、齿轮） */
    RotateObject UMETA(DisplayName = "Rotate Object"),
    
    /** 滑动触发：通过滑动手势触发事件 */
    SwipeTrigger UMETA(DisplayName = "Swipe Trigger"),
    
    /** 自定义：触发蓝图事件（仅在需要特殊逻辑时使用） */
    Custom UMETA(DisplayName = "Custom (Blueprint Event)")
};

/**
 * @brief 高度集成的可交互组件（黑盒设计 + 触控手势支持）
 * 
 * 这是一个"开箱即用"的组件，将所有常见交互逻辑封装在C++内部。
 * 支持Android触控手势（点击、滑动、旋转、长按）。
 * 
 * 蓝图开发者只需：
 * 1. 将此组件添加到任何Actor
 * 2. 在细节面板中配置交互模式和交互类型
 * 3. 完成！无需编写任何蓝图代码
 * 
 * 设计原则：
 * - 配置驱动：通过枚举和数据资产配置行为
 * - 零代码使用：常见交互无需蓝图编程
 * - 高度集成：所有逻辑在C++内部自动完成
 * - 触控优先：原生支持Android触控手势
 */
UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class RUSTYLAKEORRERY_API UInteractableComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInteractableComponent();

    // ========================================================================
    // 核心配置参数
    // ========================================================================

    /** 交互模式：决定需要什么样的输入手势 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction Config")
    EInteractionMode InteractionMode = EInteractionMode::Tap;

    /** 交互类型：决定交互后的行为 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction Config")
    EInteractionType InteractionType = EInteractionType::Observe;

    /** 是否当前可交互（可在运行时动态修改） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Config")
    bool bIsInteractable = true;

    /** 交互提示文本（显示在UI上） */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction Config")
    FText InteractionPrompt = FText::FromString(TEXT("Interact"));

    // ========================================================================
    // 滑动手势配置（InteractionMode = Swipe）
    // ========================================================================

    /** 需要的滑动方向 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swipe Config", meta = (EditCondition = "InteractionMode == EInteractionMode::Swipe", EditConditionHides))
    ESwipeDirection RequiredSwipeDirection = ESwipeDirection::Any;

    /** 滑动方向的角度容差（度数，0-90） */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swipe Config", meta = (EditCondition = "InteractionMode == EInteractionMode::Swipe", EditConditionHides, ClampMin = "0.0", ClampMax = "90.0"))
    float SwipeAngleTolerance = 30.0f;

    /** 最小滑动距离（像素，低于此距离不算有效滑动） */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swipe Config", meta = (EditCondition = "InteractionMode == EInteractionMode::Swipe", EditConditionHides, ClampMin = "10.0"))
    float MinSwipeDistance = 50.0f;

    // ========================================================================
    // 旋转手势配置（InteractionMode = Rotate）
    // ========================================================================

    /** 旋转灵敏度（值越大旋转越快） */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotate Config", meta = (EditCondition = "InteractionMode == EInteractionMode::Rotate", EditConditionHides, ClampMin = "0.1", ClampMax = "10.0"))
    float RotationSensitivity = 1.0f;

    /** 旋转轴（Local Space） */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotate Config", meta = (EditCondition = "InteractionMode == EInteractionMode::Rotate", EditConditionHides))
    FVector RotationAxis = FVector(0, 0, 1);

    /** 是否限制旋转角度范围 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotate Config", meta = (EditCondition = "InteractionMode == EInteractionMode::Rotate", EditConditionHides))
    bool bClampRotation = false;

    /** 最小旋转角度（度数） */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotate Config", meta = (EditCondition = "InteractionMode == EInteractionMode::Rotate && bClampRotation", EditConditionHides))
    float MinRotationAngle = 0.0f;

    /** 最大旋转角度（度数） */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotate Config", meta = (EditCondition = "InteractionMode == EInteractionMode::Rotate && bClampRotation", EditConditionHides))
    float MaxRotationAngle = 360.0f;

    // ========================================================================
    // 长按手势配置（InteractionMode = LongPress）
    // ========================================================================

    /** 长按所需时间（秒） */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LongPress Config", meta = (EditCondition = "InteractionMode == EInteractionMode::LongPress", EditConditionHides, ClampMin = "0.1"))
    float LongPressDuration = 1.0f;

    // ========================================================================
    // 观察类型配置（InteractionType = Observe）
    // ========================================================================

    /** 观察时显示的对话或描述文本 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Observe Config", meta = (MultiLine = true, EditCondition = "InteractionType == EInteractionType::Observe", EditConditionHides))
    FText ObserveText;

    /** 观察时播放的对话数据资产（可选，优先于ObserveText） */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Observe Config", meta = (EditCondition = "InteractionType == EInteractionType::Observe", EditConditionHides))
    class UDialogueDataAsset* ObserveDialogue = nullptr;

    // ========================================================================
    // 拾取类型配置（InteractionType = Pickup）
    // ========================================================================

    /** 拾取的物品数据资产 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Config", meta = (EditCondition = "InteractionType == EInteractionType::Pickup", EditConditionHides))
    class UItemDataAsset* PickupItemData = nullptr;

    /** 拾取后是否销毁此Actor */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Config", meta = (EditCondition = "InteractionType == EInteractionType::Pickup", EditConditionHides))
    bool bDestroyAfterPickup = true;

    /** 拾取时播放的音效 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Config", meta = (EditCondition = "InteractionType == EInteractionType::Pickup", EditConditionHides))
    class USoundBase* PickupSound = nullptr;

    // ========================================================================
    // 场景切换配置（InteractionType = Navigate）
    // ========================================================================

    /** 要加载的关卡名称 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Navigate Config", meta = (EditCondition = "InteractionType == EInteractionType::Navigate", EditConditionHides))
    FName TargetLevelName;

    /** 切换时的过渡时间（秒） */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Navigate Config", meta = (EditCondition = "InteractionType == EInteractionType::Navigate", EditConditionHides))
    float TransitionDuration = 1.0f;

    // ========================================================================
    // 使用物品配置（InteractionType = UseItem）
    // ========================================================================

    /** 需要的物品数据资产 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UseItem Config", meta = (EditCondition = "InteractionType == EInteractionType::UseItem", EditConditionHides))
    class UItemDataAsset* RequiredItemData = nullptr;

    /** 使用物品后是否从背包移除 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UseItem Config", meta = (EditCondition = "InteractionType == EInteractionType::UseItem", EditConditionHides))
    bool bConsumeItem = true;

    /** 使用物品成功后显示的文本 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UseItem Config", meta = (MultiLine = true, EditCondition = "InteractionType == EInteractionType::UseItem", EditConditionHides))
    FText OnItemUsedText;

    // ========================================================================
    // 谜题触发配置（InteractionType = TriggerPuzzle）
    // ========================================================================

    /** 要激活的谜题Actor引用 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle Config", meta = (EditCondition = "InteractionType == EInteractionType::TriggerPuzzle", EditConditionHides))
    class APuzzleBase* TargetPuzzle = nullptr;

    // ========================================================================
    // 旋转物体配置（InteractionType = RotateObject）
    // ========================================================================

    /** 旋转到目标角度后触发的事件 */
    UPROPERTY(BlueprintAssignable, Category = "Rotate Events", meta = (EditCondition = "InteractionType == EInteractionType::RotateObject", EditConditionHides))
    FRotationReachedDelegate OnTargetRotationReached;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRotationReachedDelegate, float, CurrentAngle);

    /** 目标旋转角度（达到此角度时触发事件，-1表示不检测） */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotate Object Config", meta = (EditCondition = "InteractionType == EInteractionType::RotateObject", EditConditionHides))
    float TargetRotationAngle = -1.0f;

    /** 目标角度的容差范围（度数） */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotate Object Config", meta = (EditCondition = "InteractionType == EInteractionType::RotateObject && TargetRotationAngle >= 0", EditConditionHides, ClampMin = "0.1"))
    float AngleTolerance = 5.0f;

    // ========================================================================
    // 滑动触发配置（InteractionType = SwipeTrigger）
    // ========================================================================

    /** 滑动触发后显示的文本 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swipe Trigger Config", meta = (MultiLine = true, EditCondition = "InteractionType == EInteractionType::SwipeTrigger", EditConditionHides))
    FText OnSwipeText;

    // ========================================================================
    // 视觉反馈配置
    // ========================================================================

    /** 聚焦时的高亮颜色 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Feedback")
    FLinearColor HighlightColor = FLinearColor::Yellow;

    /** 聚焦时的高亮强度 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Feedback", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float HighlightIntensity = 3.0f;

    /** 是否启用高亮效果 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Feedback")
    bool bEnableHighlight = true;

    // ========================================================================
    // 自定义事件（仅在InteractionType = Custom时使用）
    // ========================================================================

    /**
     * @brief 自定义交互事件
     * 仅在InteractionType设置为Custom时触发，用于极少数需要特殊逻辑的情况
     * @param Interactor 发起交互的Actor（通常是PlayerController）
     */
    UPROPERTY(BlueprintAssignable, Category = "Custom Events")
    FInteractionDelegate OnCustomInteract;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionDelegate, AActor*, Interactor);

    // ========================================================================
    // 公共接口函数（由InteractionComponent调用）
    // ========================================================================

    /**
     * @brief 执行点击/触摸交互（由InteractionComponent自动调用）
     * @param Interactor 发起交互的Actor
     */
    void ExecuteInteraction(AActor* Interactor);

    /**
     * @brief 处理滑动手势（由InteractionComponent自动调用）
     * @param SwipeVector 滑动向量（屏幕空间）
     * @param Interactor 发起交互的Actor
     * @return 如果滑动被成功处理返回true
     */
    bool HandleSwipe(const FVector2D& SwipeVector, AActor* Interactor);

    /**
     * @brief 开始旋转交互（由InteractionComponent自动调用）
     */
    void BeginRotation();

    /**
     * @brief 更新旋转（由InteractionComponent每帧调用）
     * @param DeltaRotation 旋转增量（度数）
     */
    void UpdateRotation(float DeltaRotation);

    /**
     * @brief 结束旋转交互（由InteractionComponent自动调用）
     */
    void EndRotation();

    /**
     * @brief 开始长按计时（由InteractionComponent自动调用）
     */
    void BeginLongPress();

    /**
     * @brief 更新长按计时（由InteractionComponent每帧调用）
     * @param DeltaTime 时间增量
     * @return 如果长按完成返回true
     */
    bool UpdateLongPress(float DeltaTime);

    /**
     * @brief 取消长按（由InteractionComponent自动调用）
     */
    void CancelLongPress();

    /**
     * @brief 开始聚焦（由InteractionComponent自动调用）
     */
    void BeginFocus();

    /**
     * @brief 结束聚焦（由InteractionComponent自动调用）
     */
    void EndFocus();

    /**
     * @brief 检查是否可交互
     * @return 如果当前可交互返回true
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
    bool CanInteract() const { return bIsInteractable; }

    /**
     * @brief 获取交互提示文本
     * @return 交互提示文本
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
    FText GetPrompt() const { return InteractionPrompt; }

    /**
     * @brief 获取当前旋转角度（仅用于RotateObject类型）
     * @return 当前旋转角度
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
    float GetCurrentRotationAngle() const { return CurrentRotationAngle; }

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // ========================================================================
    // 内部实现函数（完全封装，蓝图不可见）
    // ========================================================================

    /** 处理观察交互 */
    void HandleObserveInteraction(AActor* Interactor);

    /** 处理拾取交互 */
    void HandlePickupInteraction(AActor* Interactor);

    /** 处理场景切换交互 */
    void HandleNavigateInteraction(AActor* Interactor);

    /** 处理使用物品交互 */
    void HandleUseItemInteraction(AActor* Interactor);

    /** 处理谜题触发交互 */
    void HandlePuzzleInteraction(AActor* Interactor);

    /** 处理滑动触发交互 */
    void HandleSwipeTriggerInteraction(AActor* Interactor);

    /** 应用高亮效果 */
    void ApplyHighlight(bool bEnable);

    /** 检查滑动方向是否匹配 */
    bool IsSwipeDirectionValid(const FVector2D& SwipeVector) const;

    /** 检查是否达到目标旋转角度 */
    void CheckTargetRotation();

    /** 缓存的网格体组件（用于高亮和旋转） */
    UPROPERTY()
    class UMeshComponent* CachedMeshComponent = nullptr;

    /** 是否当前处于聚焦状态 */
    bool bIsFocused = false;

    /** 是否正在旋转 */
    bool bIsRotating = false;

    /** 当前旋转角度（累计） */
    float CurrentRotationAngle = 0.0f;

    /** 初始旋转（用于恢复） */
    FRotator InitialRotation;

    /** 长按计时器 */
    float LongPressTimer = 0.0f;

    /** 是否正在长按 */
    bool bIsLongPressing = false;

    /** 是否已触发目标角度事件 */
    bool bTargetAngleReached = false;
};
