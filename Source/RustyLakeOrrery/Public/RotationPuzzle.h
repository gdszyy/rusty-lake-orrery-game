// RotationPuzzle.h

#pragma once

#include "CoreMinimal.h"
#include "PuzzleBase.h"
#include "RotationPuzzle.generated.h"

/**
 * @brief 旋转谜题
 * 
 * 一个具体的谜题实现示例,要求玩家将物体旋转到正确的角度。
 * 
 * 特性:
 * - 支持多个旋转目标
 * - 角度容差设置
 * - 可视化反馈
 * - 自动检测完成
 * 
 * 使用场景:
 * - 旋转雕像/画框到正确角度
 * - 调整时钟指针
 * - 对齐符号/图案
 * 
 * 使用方法:
 * 1. 在关卡中放置 BP_RotationPuzzle
 * 2. 设置 TargetRotation 和 AngleTolerance
 * 3. 添加可旋转的组件或Actor
 * 4. 在蓝图中实现旋转交互逻辑
 */
UCLASS()
class RUSTYLAKEORRERY_API ARotationPuzzle : public APuzzleBase
{
    GENERATED_BODY()

public:
    ARotationPuzzle();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    // ========================================================================
    // 旋转谜题配置
    // ========================================================================

    /** 目标旋转角度(Yaw) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Puzzle")
    float TargetRotation = 0.0f;

    /** 角度容差(度) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Puzzle")
    float AngleTolerance = 5.0f;

    /** 需要保持正确角度的时间(秒) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Puzzle")
    float HoldTime = 0.5f;

    /** 可旋转的组件(如果为空,则旋转整个Actor) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation Puzzle")
    class USceneComponent* RotatableComponent = nullptr;

    // ========================================================================
    // 运行时状态
    // ========================================================================

    /** 当前旋转角度 */
    UPROPERTY(BlueprintReadOnly, Category = "Rotation Puzzle State")
    float CurrentRotation = 0.0f;

    /** 保持正确角度的计时器 */
    UPROPERTY(BlueprintReadOnly, Category = "Rotation Puzzle State")
    float HoldTimer = 0.0f;

    /** 是否在正确角度 */
    UPROPERTY(BlueprintReadOnly, Category = "Rotation Puzzle State")
    bool bIsAtCorrectAngle = false;

    // ========================================================================
    // 委托事件
    // ========================================================================

    /** 角度改变事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRotationChanged, float, CurrentAngle, float, TargetAngle);
    UPROPERTY(BlueprintAssignable, Category = "Rotation Puzzle Events")
    FOnRotationChanged OnRotationChanged;

    /** 到达正确角度事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCorrectAngleReached, float, Angle);
    UPROPERTY(BlueprintAssignable, Category = "Rotation Puzzle Events")
    FOnCorrectAngleReached OnCorrectAngleReached;

    // ========================================================================
    // 公共接口
    // ========================================================================

    /**
     * @brief 设置旋转角度
     * @param NewRotation 新的旋转角度(Yaw)
     */
    UFUNCTION(BlueprintCallable, Category = "Rotation Puzzle")
    void SetRotation(float NewRotation);

    /**
     * @brief 增加旋转角度
     * @param DeltaRotation 旋转增量
     */
    UFUNCTION(BlueprintCallable, Category = "Rotation Puzzle")
    void AddRotation(float DeltaRotation);

    /**
     * @brief 检查当前角度是否正确
     * @return 是否正确
     */
    UFUNCTION(BlueprintCallable, Category = "Rotation Puzzle")
    bool IsAtCorrectAngle() const;

    /**
     * @brief 获取与目标角度的差值
     * @return 角度差(度)
     */
    UFUNCTION(BlueprintCallable, Category = "Rotation Puzzle")
    float GetAngleDifference() const;

    // ========================================================================
    // 重写基类方法
    // ========================================================================

    virtual float GetProgress_Implementation() const override;
    virtual void OnPuzzleActivatedEvent_Implementation() override;
    virtual void OnPuzzleResetEvent_Implementation() override;

private:
    /** 更新旋转状态 */
    void UpdateRotationState();

    /** 标准化角度到0-360范围 */
    float NormalizeAngle(float Angle) const;
};
