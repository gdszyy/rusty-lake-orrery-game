// DialogueDataAsset.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogueDataAsset.generated.h"

/**
 * @brief 对话说话者类型枚举
 */
UENUM(BlueprintType)
enum class ESpeakerType : uint8
{
    /** 旁白/叙述者 */
    Narrator UMETA(DisplayName = "Narrator"),
    
    /** 主角(玩家) */
    Player UMETA(DisplayName = "Player"),
    
    /** NPC角色 */
    NPC UMETA(DisplayName = "NPC"),
    
    /** 音效(非语音对话) */
    SoundEffect UMETA(DisplayName = "Sound Effect")
};

/**
 * @brief 单条对话数据结构
 */
USTRUCT(BlueprintType)
struct FDialogueEntry
{
    GENERATED_BODY()

    /** 对话ID(唯一标识符) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
    FName DialogueID;

    /** 触发事件名称 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
    FName TriggerEvent;

    /** 说话者类型 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
    ESpeakerType SpeakerType = ESpeakerType::Narrator;

    /** 对话文本(中文) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (MultiLine = true))
    FText TextCN;

    /** 对话文本(英文) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (MultiLine = true))
    FText TextEN;

    /** 音频路径 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
    FString AudioPath;

    /** 对话持续时间(秒) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
    float Duration = 3.0f;

    /** 下一条对话ID(用于对话链) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
    FName NextDialogueID;

    /** 选项列表(用于分支对话,逗号分隔) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
    FString ChoiceOptions;

    /** 音频资源(运行时加载) */
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Dialogue")
    class USoundBase* AudioAsset = nullptr;
};

/**
 * @brief 对话数据资产
 * 
 * 用于存储和管理游戏中的对话内容。支持：
 * - 多语言(中文/英文)
 * - 对话链(自动播放下一条)
 * - 分支对话(选项)
 * - 音频播放
 * - 从CSV导入
 * 
 * 使用方法：
 * 1. 在编辑器中创建 DialogueDataAsset
 * 2. 手动添加对话条目或从CSV导入
 * 3. 在 DialogueComponent 中引用此资产
 */
UCLASS(BlueprintType)
class RUSTYLAKEORRERY_API UDialogueDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UDialogueDataAsset();

    /** 所有对话条目 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
    TArray<FDialogueEntry> DialogueEntries;

    /** 是否使用中文(false则使用英文) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    bool bUseChinese = true;

    /**
     * @brief 根据对话ID获取对话条目
     * @param DialogueID 对话ID
     * @param OutEntry 输出的对话条目
     * @return 是否找到
     */
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    bool GetDialogueEntry(FName DialogueID, FDialogueEntry& OutEntry) const;

    /**
     * @brief 根据触发事件获取对话条目
     * @param TriggerEvent 触发事件名称
     * @param OutEntry 输出的对话条目
     * @return 是否找到
     */
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    bool GetDialogueByTrigger(FName TriggerEvent, FDialogueEntry& OutEntry) const;

    /**
     * @brief 获取对话的显示文本(根据当前语言)
     * @param Entry 对话条目
     * @return 显示文本
     */
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    FText GetDisplayText(const FDialogueEntry& Entry) const;

#if WITH_EDITOR
    /**
     * @brief 从CSV文件导入对话数据
     * @param CSVFilePath CSV文件路径
     * @return 是否导入成功
     */
    UFUNCTION(BlueprintCallable, Category = "Dialogue|Editor")
    bool ImportFromCSV(const FString& CSVFilePath);
#endif
};
