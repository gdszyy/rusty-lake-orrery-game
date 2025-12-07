// UIManager.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DialogueDataAsset.h"
#include "ItemDataAsset.h"
#include "UIManager.generated.h"

/**
 * @brief UI管理器
 * 
 * 负责管理游戏中所有UI的显示和隐藏,包括:
 * - 对话UI
 * - 背包UI
 * - 提示UI
 * - 交互提示UI
 * 
 * 设计为单例模式,通过 GetUIManager() 获取实例
 * 
 * 使用方法:
 * 1. 在关卡中放置 BP_UIManager
 * 2. 设置UI Widget类
 * 3. 通过静态函数或蓝图调用显示/隐藏UI
 */
UCLASS()
class RUSTYLAKEORRERY_API AUIManager : public AActor
{
    GENERATED_BODY()

public:
    AUIManager();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    // ========================================================================
    // UI Widget 类配置
    // ========================================================================

    /** 对话UI Widget类 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    TSubclassOf<class UUserWidget> DialogueWidgetClass;

    /** 背包UI Widget类 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    TSubclassOf<class UUserWidget> InventoryWidgetClass;

    /** 提示UI Widget类 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    TSubclassOf<class UUserWidget> HintWidgetClass;

    /** 交互提示UI Widget类 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    TSubclassOf<class UUserWidget> InteractionPromptWidgetClass;

    // ========================================================================
    // UI Widget 实例
    // ========================================================================

    /** 对话UI实例 */
    UPROPERTY(BlueprintReadOnly, Category = "UI State")
    class UUserWidget* DialogueWidget = nullptr;

    /** 背包UI实例 */
    UPROPERTY(BlueprintReadOnly, Category = "UI State")
    class UUserWidget* InventoryWidget = nullptr;

    /** 提示UI实例 */
    UPROPERTY(BlueprintReadOnly, Category = "UI State")
    class UUserWidget* HintWidget = nullptr;

    /** 交互提示UI实例 */
    UPROPERTY(BlueprintReadOnly, Category = "UI State")
    class UUserWidget* InteractionPromptWidget = nullptr;

    // ========================================================================
    // 对话UI接口
    // ========================================================================

    /**
     * @brief 显示对话UI
     * @param DialogueEntry 对话内容
     */
    UFUNCTION(BlueprintCallable, Category = "UI|Dialogue")
    void ShowDialogue(const FDialogueEntry& DialogueEntry);

    /**
     * @brief 更新对话文本
     * @param DisplayText 显示的文本
     * @param Progress 文本显示进度(0-1)
     */
    UFUNCTION(BlueprintCallable, Category = "UI|Dialogue")
    void UpdateDialogueText(const FText& DisplayText, float Progress);

    /**
     * @brief 隐藏对话UI
     */
    UFUNCTION(BlueprintCallable, Category = "UI|Dialogue")
    void HideDialogue();

    /**
     * @brief 显示对话选项
     * @param Choices 选项列表
     */
    UFUNCTION(BlueprintCallable, Category = "UI|Dialogue")
    void ShowDialogueChoices(const TArray<FString>& Choices);

    // ========================================================================
    // 背包UI接口
    // ========================================================================

    /**
     * @brief 显示背包UI
     */
    UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
    void ShowInventory();

    /**
     * @brief 隐藏背包UI
     */
    UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
    void HideInventory();

    /**
     * @brief 切换背包UI显示状态
     */
    UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
    void ToggleInventory();

    /**
     * @brief 刷新背包UI(更新物品列表)
     */
    UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
    void RefreshInventory();

    // ========================================================================
    // 提示UI接口
    // ========================================================================

    /**
     * @brief 显示提示信息
     * @param HintText 提示文本
     * @param Duration 显示时长(秒,0表示一直显示)
     */
    UFUNCTION(BlueprintCallable, Category = "UI|Hint")
    void ShowHint(const FText& HintText, float Duration = 3.0f);

    /**
     * @brief 隐藏提示UI
     */
    UFUNCTION(BlueprintCallable, Category = "UI|Hint")
    void HideHint();

    // ========================================================================
    // 交互提示UI接口
    // ========================================================================

    /**
     * @brief 显示交互提示
     * @param PromptText 提示文本
     */
    UFUNCTION(BlueprintCallable, Category = "UI|Interaction")
    void ShowInteractionPrompt(const FText& PromptText);

    /**
     * @brief 隐藏交互提示
     */
    UFUNCTION(BlueprintCallable, Category = "UI|Interaction")
    void HideInteractionPrompt();

    // ========================================================================
    // 通用UI接口
    // ========================================================================

    /**
     * @brief 隐藏所有UI
     */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void HideAllUI();

    /**
     * @brief 是否有任何UI正在显示
     * @return 是否有UI显示
     */
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsAnyUIVisible() const;

    // ========================================================================
    // 单例访问
    // ========================================================================

    /**
     * @brief 获取UIManager实例
     * @param WorldContext 世界上下文对象
     * @return UIManager实例
     */
    UFUNCTION(BlueprintCallable, Category = "UI", meta = (WorldContext = "WorldContext"))
    static AUIManager* GetUIManager(const UObject* WorldContext);

private:
    /** 创建UI Widget */
    void CreateUIWidgets();

    /** 提示计时器句柄 */
    FTimerHandle HintTimerHandle;

    /** 单例实例 */
    static AUIManager* Instance;
};
