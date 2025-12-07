// PuzzleComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PuzzleBase.h"
#include "PuzzleComponent.generated.h"

/**
 * @brief 谜题组件
 * 
 * 可以附加到任何Actor上,使其成为谜题的一部分。
 * 用于简单的谜题逻辑,不需要创建完整的APuzzleBase子类。
 * 
 * 特性:
 * - 简单的状态管理
 * - 条件检查
 * - 事件广播
 * - 与APuzzleBase集成
 * 
 * 使用场景:
 * - 谜题的子元素(如旋转谜题中的单个旋转物体)
 * - 简单的开关/按钮
 * - 谜题的触发器
 * 
 * 使用方法:
 * 1. 将此组件添加到Actor
 * 2. 设置关联的PuzzleActor
 * 3. 在蓝图中监听事件并实现逻辑
 */
UCLASS(ClassGroup=(Puzzle), meta=(BlueprintSpawnableComponent))
class RUSTYLAKEORRERY_API UPuzzleComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPuzzleComponent();

protected:
    virtual void BeginPlay() override;

public:
    // ========================================================================
    // 配置参数
    // ========================================================================

    /** 关联的谜题Actor */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Config")
    APuzzleBase* PuzzleActor = nullptr;

    /** 组件名称/ID */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle Config")
    FName ComponentID;

    /** 是否是必需的(必须完成才能解决谜题) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Config")
    bool bIsRequired = true;

    /** 是否已完成 */
    UPROPERTY(BlueprintReadWrite, Category = "Puzzle State")
    bool bIsCompleted = false;

    // ========================================================================
    // 委托事件
    // ========================================================================

    /** 组件激活事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPuzzleComponentActivated, UPuzzleComponent*, Component);
    UPROPERTY(BlueprintAssignable, Category = "Puzzle Events")
    FOnPuzzleComponentActivated OnPuzzleComponentActivated;

    /** 组件完成事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComponentCompleted, UPuzzleComponent*, Component);
    UPROPERTY(BlueprintAssignable, Category = "Puzzle Events")
    FOnComponentCompleted OnComponentCompleted;

    /** 组件重置事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComponentReset, UPuzzleComponent*, Component);
    UPROPERTY(BlueprintAssignable, Category = "Puzzle Events")
    FOnComponentReset OnComponentReset;

    // ========================================================================
    // 公共接口
    // ========================================================================

    /**
     * @brief 激活组件
     */
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void ActivateComponent();

    /**
     * @brief 完成组件
     */
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void CompleteComponent();

    /**
     * @brief 重置组件
     */
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void ResetComponent();

    /**
     * @brief 检查谜题是否完成(检查所有必需组件)
     */
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void CheckPuzzleCompletion();

    /**
     * @brief 获取关联的谜题Actor
     * @return 谜题Actor
     */
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    APuzzleBase* GetPuzzleActor() const { return PuzzleActor; }

    /**
     * @brief 设置关联的谜题Actor
     * @param NewPuzzleActor 新的谜题Actor
     */
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void SetPuzzleActor(APuzzleBase* NewPuzzleActor);
};
