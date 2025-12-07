// DialogueDataAsset.cpp

#include "DialogueDataAsset.h"
#include "Sound/SoundBase.h"

#if WITH_EDITOR
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#endif

UDialogueDataAsset::UDialogueDataAsset()
{
    bUseChinese = true;
}

bool UDialogueDataAsset::GetDialogueEntry(FName DialogueID, FDialogueEntry& OutEntry) const
{
    for (const FDialogueEntry& Entry : DialogueEntries)
    {
        if (Entry.DialogueID == DialogueID)
        {
            OutEntry = Entry;
            return true;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("DialogueDataAsset: Dialogue ID '%s' not found"), *DialogueID.ToString());
    return false;
}

bool UDialogueDataAsset::GetDialogueByTrigger(FName TriggerEvent, FDialogueEntry& OutEntry) const
{
    for (const FDialogueEntry& Entry : DialogueEntries)
    {
        if (Entry.TriggerEvent == TriggerEvent)
        {
            OutEntry = Entry;
            return true;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("DialogueDataAsset: Trigger event '%s' not found"), *TriggerEvent.ToString());
    return false;
}

FText UDialogueDataAsset::GetDisplayText(const FDialogueEntry& Entry) const
{
    return bUseChinese ? Entry.TextCN : Entry.TextEN;
}

#if WITH_EDITOR
bool UDialogueDataAsset::ImportFromCSV(const FString& CSVFilePath)
{
    // 读取CSV文件
    FString CSVContent;
    if (!FFileHelper::LoadFileToString(CSVContent, *CSVFilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("DialogueDataAsset: Failed to load CSV file: %s"), *CSVFilePath);
        return false;
    }

    // 解析CSV
    TArray<FString> Lines;
    CSVContent.ParseIntoArrayLines(Lines);

    if (Lines.Num() < 2)
    {
        UE_LOG(LogTemp, Error, TEXT("DialogueDataAsset: CSV file is empty or invalid"));
        return false;
    }

    // 清空现有数据
    DialogueEntries.Empty();

    // 跳过标题行,从第二行开始解析
    for (int32 i = 1; i < Lines.Num(); i++)
    {
        const FString& Line = Lines[i];
        
        // 跳过空行
        if (Line.TrimStartAndEnd().IsEmpty())
        {
            continue;
        }

        // 解析CSV行(使用逗号分隔)
        TArray<FString> Fields;
        Line.ParseIntoArray(Fields, TEXT(","), false);

        if (Fields.Num() < 8)
        {
            UE_LOG(LogTemp, Warning, TEXT("DialogueDataAsset: Invalid CSV line %d, skipping"), i + 1);
            continue;
        }

        // 创建对话条目
        FDialogueEntry Entry;
        Entry.DialogueID = FName(*Fields[0].TrimStartAndEnd());
        Entry.TriggerEvent = FName(*Fields[1].TrimStartAndEnd());
        
        // 解析说话者类型
        FString SpeakerStr = Fields[2].TrimStartAndEnd();
        if (SpeakerStr == TEXT("Narrator"))
        {
            Entry.SpeakerType = ESpeakerType::Narrator;
        }
        else if (SpeakerStr == TEXT("Player"))
        {
            Entry.SpeakerType = ESpeakerType::Player;
        }
        else if (SpeakerStr == TEXT("NPC"))
        {
            Entry.SpeakerType = ESpeakerType::NPC;
        }
        else if (SpeakerStr == TEXT("SoundEffect"))
        {
            Entry.SpeakerType = ESpeakerType::SoundEffect;
        }

        Entry.TextCN = FText::FromString(Fields[3].TrimStartAndEnd().Replace(TEXT("\""), TEXT("")));
        Entry.TextEN = FText::FromString(Fields[4].TrimStartAndEnd().Replace(TEXT("\""), TEXT("")));
        Entry.AudioPath = Fields[5].TrimStartAndEnd();
        
        // 解析持续时间
        FString DurationStr = Fields[6].TrimStartAndEnd();
        if (!DurationStr.IsEmpty() && DurationStr != TEXT("循环"))
        {
            Entry.Duration = FCString::Atof(*DurationStr);
        }

        // 下一条对话ID和选项
        if (Fields.Num() > 7)
        {
            Entry.NextDialogueID = FName(*Fields[7].TrimStartAndEnd());
        }
        if (Fields.Num() > 8)
        {
            Entry.ChoiceOptions = Fields[8].TrimStartAndEnd();
        }

        DialogueEntries.Add(Entry);
    }

    UE_LOG(LogTemp, Log, TEXT("DialogueDataAsset: Successfully imported %d dialogue entries from CSV"), DialogueEntries.Num());
    return true;
}
#endif
