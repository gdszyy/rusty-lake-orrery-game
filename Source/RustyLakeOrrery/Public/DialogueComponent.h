// DialogueComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueDataAsset.h"
#include "DialogueComponent.generated.h"

/**
 * @brief 对话播放状态枚举
 */
UENUM(BlueprintType)
enum class EDialogueState : uint8
{
    /** 空闲,未播放对话 */
    Idle UMETA(DisplayName = "Idle"),
    
    /** 正在播放对话 */
    Playing UMETA(DisplayName = "Playing"),
    
    /** 等待用户输入(用于选项对话) */
    WaitingForInput UMETA(DisplayName = "Waiting For Input"),
    
    /** 对话已完成 */
    Completed UMETA(DisplayName = "Completed")
};

/**
 * @brief 对话组件
 * 
 * 负责播放和管理对话系统。支持：
 * - 自动播放对话链
 * - 音频播放
 * - 对话UI显示
 * - 分支对话(选项)
 * - 对话跳过
 * 
 * 使用方法：
 * 1. 将此组件添加到需要对话的Actor
 * 2. 设置 DialogueDataAsset
 * 3. 调用 PlayDialogue() 或 PlayDialogueByTrigger()
 * 
 * 事件：
 * - OnDialogueStarted: 对话开始时触发
 * - OnDialogueCompleted: 对话完成时触发
 * - OnDialogueTextChanged: 对话文本更新时触发
 */
UCLASS(ClassGroup=(Dialogue), meta=(BlueprintSpawnableComponent))
class RUSTYLAKEORRERY_API UDialogueComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UDialogueComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    // ========================================================================
    // 配置参数
    // ========================================================================

    /** 对话数据资产 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Config")
    UDialogueDataAsset* DialogueDataAsset = nullptr;

    /** 是否自动播放下一条对话 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Config")
    bool bAutoPlayNext = true;

    /** 是否允许跳过对话 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Config")
    bool bAllowSkip = true;

    /** 文本显示速度(字符/秒,0表示立即显示) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Config", meta = (ClampMin = "0.0"))
    float TextDisplaySpeed = 20.0f;

    /** 对话间隔时间(秒) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue Config", meta = (ClampMin = "0.0"))
    float DialogueInterval = 0.5f;

    // ========================================================================
    // 运行时状态
    // ========================================================================

    /** 当前对话状态 */
    UPROPERTY(BlueprintReadOnly, Category = "Dialogue State")
    EDialogueState CurrentState = EDialogueState::Idle;

    /** 当前播放的对话条目 */
    UPROPERTY(BlueprintReadOnly, Category = "Dialogue State")
    FDialogueEntry CurrentDialogue;

    /** 当前显示的文本进度(0-1) */
    UPROPERTY(BlueprintReadOnly, Category = "Dialogue State")
    float TextProgress = 0.0f;

    // ========================================================================
    // 委托事件
    // ========================================================================

    /** 对话开始事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueStarted, const FDialogueEntry&, Dialogue);
    UPROPERTY(BlueprintAssignable, Category = "Dialogue Events")
    FOnDialogueStarted OnDialogueStarted;

    /** 对话完成事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueCompleted, const FDialogueEntry&, Dialogue);
    UPROPERTY(BlueprintAssignable, Category = "Dialogue Events")
    FOnDialogueCompleted OnDialogueCompleted;

    /** 对话文本更新事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueTextChanged, const FText&, DisplayText, float, Progress);
    UPROPERTY(BlueprintAssignable, Category = "Dialogue Events")
    FOnDialogueTextChanged OnDialogueTextChanged;

    /** 等待用户选择事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaitingForChoice, const TArray<FString>&, Choices);
    UPROPERTY(BlueprintAssignable, Category = "Dialogue Events")
    FOnWaitingForChoice OnWaitingForChoice;

    // ========================================================================
    // 公共接口
    // ========================================================================

    /**
     * @brief 播放指定ID的对话
     * @param DialogueID 对话ID
     * @return 是否成功开始播放
     */
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    bool PlayDialogue(FName DialogueID);

    /**
     * @brief 根据触发事件播放对话
     * @param TriggerEvent 触发事件名称
     * @return 是否成功开始播放
     */
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    bool PlayDialogueByTrigger(FName TriggerEvent);

    /**
     * @brief 停止当前对话
     */
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void StopDialogue();

    /**
     * @brief 跳过当前对话(立即完成)
     */
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void SkipDialogue();

    /**
     * @brief 播放下一条对话(用于对话链)
     */
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void PlayNextDialogue();

    /**
     * @brief 选择对话选项
     * @param ChoiceIndex 选项索引
     */
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void SelectChoice(int32 ChoiceIndex);

    /**
     * @brief 获取当前显示的文本
     * @return 当前显示的文本
     */
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    FText GetCurrentDisplayText() const;

    /**
     * @brief 是否正在播放对话
     * @return 是否正在播放
     */
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    bool IsPlaying() const { return CurrentState == EDialogueState::Playing; }

private:
    /** 内部播放对话实现 */
    void PlayDialogueInternal(const FDialogueEntry& Entry);

    /** 更新文本显示 */
    void UpdateTextDisplay(float DeltaTime);

    /** 完成当前对话 */
    void CompleteCurrentDialogue();

    /** 播放音频 */
    void PlayAudio(const FDialogueEntry& Entry);

    /** 停止音频 */
    void StopAudio();

    /** 当前播放的音频组件 */
    UPROPERTY(Transient)
    class UAudioComponent* AudioComponent = nullptr;

    /** 对话计时器 */
    float DialogueTimer = 0.0f;

    /** 间隔计时器 */
    float IntervalTimer = 0.0f;

    /** 是否在间隔中 */
    bool bInInterval = false;
};
