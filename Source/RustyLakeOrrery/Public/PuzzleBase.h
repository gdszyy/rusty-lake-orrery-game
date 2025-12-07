// PuzzleBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleBase.generated.h"

/**
 * @brief 谜题状态枚举
 */
UENUM(BlueprintType)
enum class EPuzzleState : uint8
{
    /** 未激活 */
    Inactive UMETA(DisplayName = "Inactive"),
    
    /** 已激活,等待解决 */
    Active UMETA(DisplayName = "Active"),
    
    /** 正在解决中 */
    Solving UMETA(DisplayName = "Solving"),
    
    /** 已完成 */
    Completed UMETA(DisplayName = "Completed"),
    
    /** 失败(可选,某些谜题可能有失败状态) */
    Failed UMETA(DisplayName = "Failed")
};

/**
 * @brief 谜题基类
 * 
 * 所有谜题的抽象基类,提供通用的谜题接口和状态管理。
 * 
 * 特性:
 * - 状态管理(未激活/激活/解决中/完成/失败)
 * - 提示系统
 * - 重置功能
 * - 事件广播
 * - 保存/加载支持
 * 
 * 使用方法:
 * 1. 继承此类创建具体的谜题类(如 ARotationPuzzle)
 * 2. 重写 ActivatePuzzle_Implementation() 和 OnPuzzleSolved_Implementation()
 * 3. 在蓝图中实现具体的谜题逻辑
 * 
 * 常见谜题类型:
 * - 旋转谜题(RotationPuzzle): 旋转物体到正确角度
 * - 序列谜题(SequencePuzzle): 按正确顺序点击
 * - 组合谜题(CombinationPuzzle): 组合多个元素
 * - 物理谜题(PhysicsPuzzle): 利用物理规则
 */
UCLASS(Abstract, Blueprintable)
class RUSTYLAKEORRERY_API APuzzleBase : public AActor
{
    GENERATED_BODY()

public:
    APuzzleBase();

protected:
    virtual void BeginPlay() override;

public:
    // ========================================================================
    // 谜题配置
    // ========================================================================

    /** 谜题名称 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle Config")
    FText PuzzleName;

    /** 谜题描述 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle Config", meta = (MultiLine = true))
    FText PuzzleDescription;

    /** 是否在BeginPlay时自动激活 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Config")
    bool bAutoActivate = false;

    /** 是否允许重置 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Config")
    bool bAllowReset = true;

    /** 是否有提示系统 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Config")
    bool bHasHints = true;

    /** 提示文本数组 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle Config", meta = (EditCondition = "bHasHints"))
    TArray<FText> HintTexts;

    /** 完成谜题后的奖励物品 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle Config")
    class UItemDataAsset* RewardItem = nullptr;

    // ========================================================================
    // 运行时状态
    // ========================================================================

    /** 当前谜题状态 */
    UPROPERTY(BlueprintReadOnly, Category = "Puzzle State")
    EPuzzleState CurrentState = EPuzzleState::Inactive;

    /** 当前提示索引 */
    UPROPERTY(BlueprintReadOnly, Category = "Puzzle State")
    int32 CurrentHintIndex = 0;

    /** 谜题开始时间 */
    UPROPERTY(BlueprintReadOnly, Category = "Puzzle State")
    float StartTime = 0.0f;

    /** 谜题完成时间 */
    UPROPERTY(BlueprintReadOnly, Category = "Puzzle State")
    float CompletionTime = 0.0f;

    // ========================================================================
    // 委托事件
    // ========================================================================

    /** 谜题激活事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPuzzleActivated, APuzzleBase*, Puzzle);
    UPROPERTY(BlueprintAssignable, Category = "Puzzle Events")
    FOnPuzzleActivated OnPuzzleActivated;

    /** 谜题完成事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPuzzleCompleted, APuzzleBase*, Puzzle);
    UPROPERTY(BlueprintAssignable, Category = "Puzzle Events")
    FOnPuzzleCompleted OnPuzzleCompleted;

    /** 谜题失败事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPuzzleFailed, APuzzleBase*, Puzzle);
    UPROPERTY(BlueprintAssignable, Category = "Puzzle Events")
    FOnPuzzleFailed OnPuzzleFailed;

    /** 谜题重置事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPuzzleReset, APuzzleBase*, Puzzle);
    UPROPERTY(BlueprintAssignable, Category = "Puzzle Events")
    FOnPuzzleReset OnPuzzleReset;

    /** 提示显示事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHintShown, APuzzleBase*, Puzzle, const FText&, HintText);
    UPROPERTY(BlueprintAssignable, Category = "Puzzle Events")
    FOnHintShown OnHintShown;

    // ========================================================================
    // 公共接口
    // ========================================================================

    /**
     * @brief 激活谜题
     */
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void ActivatePuzzle();

    /**
     * @brief 完成谜题
     */
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void CompletePuzzle();

    /**
     * @brief 谜题失败
     */
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void FailPuzzle();

    /**
     * @brief 重置谜题
     */
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void ResetPuzzle();

    /**
     * @brief 显示下一条提示
     * @return 提示文本,如果没有更多提示则返回空
     */
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    FText ShowNextHint();

    /**
     * @brief 获取当前提示
     * @return 当前提示文本
     */
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    FText GetCurrentHint() const;

    /**
     * @brief 是否还有更多提示
     * @return 是否有更多提示
     */
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    bool HasMoreHints() const;

    /**
     * @brief 获取谜题进度(0-1)
     * @return 进度值
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Puzzle")
    float GetProgress() const;

    /**
     * @brief 是否已完成
     * @return 是否完成
     */
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    bool IsCompleted() const { return CurrentState == EPuzzleState::Completed; }

    /**
     * @brief 是否已激活
     * @return 是否激活
     */
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    bool IsActive() const { return CurrentState == EPuzzleState::Active || CurrentState == EPuzzleState::Solving; }

    // ========================================================================
    // 蓝图可重写事件
    // ========================================================================

    /**
     * @brief 激活谜题时调用(蓝图可重写)
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Puzzle")
    void OnPuzzleActivatedEvent();

    /**
     * @brief 谜题完成时调用(蓝图可重写)
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Puzzle")
    void OnPuzzleSolved();

    /**
     * @brief 谜题失败时调用(蓝图可重写)
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Puzzle")
    void OnPuzzleFailedEvent();

    /**
     * @brief 谜题重置时调用(蓝图可重写)
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Puzzle")
    void OnPuzzleResetEvent();

protected:
    /** 给予奖励物品 */
    void GiveReward();
};
