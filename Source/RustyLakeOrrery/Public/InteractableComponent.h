// InteractableComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

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
    
    /** 自定义：触发蓝图事件（仅在需要特殊逻辑时使用） */
    Custom UMETA(DisplayName = "Custom (Blueprint Event)")
};

/**
 * @brief 高度集成的可交互组件（黑盒设计）
 * 
 * 这是一个"开箱即用"的组件，将所有常见交互逻辑封装在C++内部。
 * 蓝图开发者只需：
 * 1. 将此组件添加到任何Actor
 * 2. 在细节面板中配置交互类型和相关参数
 * 3. 完成！无需编写任何蓝图代码
 * 
 * 设计原则：
 * - 配置驱动：通过枚举和数据资产配置行为
 * - 零代码使用：常见交互无需蓝图编程
 * - 高度集成：所有逻辑在C++内部自动完成
 * - 最小暴露：只暴露必要的配置参数
 */
UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class RUSTYLAKEORRERY_API UInteractableComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInteractableComponent();

    // ========================================================================
    // 核心配置参数（唯一需要在蓝图中设置的内容）
    // ========================================================================

    /** 交互类型：决定点击后的行为 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction Config")
    EInteractionType InteractionType = EInteractionType::Observe;

    /** 是否当前可交互（可在运行时动态修改） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Config")
    bool bIsInteractable = true;

    /** 交互提示文本（显示在UI上） */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction Config")
    FText InteractionPrompt = FText::FromString(TEXT("Interact"));

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
    // 内部函数（蓝图无需调用）
    // ========================================================================

    /**
     * @brief 执行交互（由InteractionComponent自动调用）
     * @param Interactor 发起交互的Actor
     */
    void ExecuteInteraction(AActor* Interactor);

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

protected:
    virtual void BeginPlay() override;

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

    /** 应用高亮效果 */
    void ApplyHighlight(bool bEnable);

    /** 缓存的网格体组件（用于高亮） */
    UPROPERTY()
    class UMeshComponent* CachedMeshComponent = nullptr;

    /** 是否当前处于聚焦状态 */
    bool bIsFocused = false;
};
