// DialogueComponent.cpp

#include "DialogueComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

UDialogueComponent::UDialogueComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = false;

    bAutoPlayNext = true;
    bAllowSkip = true;
    TextDisplaySpeed = 20.0f;
    DialogueInterval = 0.5f;
}

void UDialogueComponent::BeginPlay()
{
    Super::BeginPlay();
    
    CurrentState = EDialogueState::Idle;
}

void UDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (CurrentState == EDialogueState::Playing)
    {
        // 更新文本显示
        UpdateTextDisplay(DeltaTime);

        // 更新对话计时器
        DialogueTimer += DeltaTime;

        // 检查对话是否完成
        if (DialogueTimer >= CurrentDialogue.Duration && TextProgress >= 1.0f)
        {
            CompleteCurrentDialogue();
        }
    }
    else if (bInInterval)
    {
        // 间隔计时
        IntervalTimer += DeltaTime;
        if (IntervalTimer >= DialogueInterval)
        {
            bInInterval = false;
            PlayNextDialogue();
        }
    }
}

bool UDialogueComponent::PlayDialogue(FName DialogueID)
{
    if (!DialogueDataAsset)
    {
        UE_LOG(LogTemp, Error, TEXT("DialogueComponent: DialogueDataAsset is not set"));
        return false;
    }

    FDialogueEntry Entry;
    if (!DialogueDataAsset->GetDialogueEntry(DialogueID, Entry))
    {
        return false;
    }

    PlayDialogueInternal(Entry);
    return true;
}

bool UDialogueComponent::PlayDialogueByTrigger(FName TriggerEvent)
{
    if (!DialogueDataAsset)
    {
        UE_LOG(LogTemp, Error, TEXT("DialogueComponent: DialogueDataAsset is not set"));
        return false;
    }

    FDialogueEntry Entry;
    if (!DialogueDataAsset->GetDialogueByTrigger(TriggerEvent, Entry))
    {
        return false;
    }

    PlayDialogueInternal(Entry);
    return true;
}

void UDialogueComponent::PlayDialogueInternal(const FDialogueEntry& Entry)
{
    // 停止当前对话
    StopDialogue();

    // 设置新对话
    CurrentDialogue = Entry;
    CurrentState = EDialogueState::Playing;
    TextProgress = 0.0f;
    DialogueTimer = 0.0f;

    // 启用Tick
    SetComponentTickEnabled(true);

    // 播放音频
    PlayAudio(Entry);

    // 触发事件
    OnDialogueStarted.Broadcast(Entry);

    UE_LOG(LogTemp, Log, TEXT("DialogueComponent: Started dialogue '%s'"), *Entry.DialogueID.ToString());
}

void UDialogueComponent::UpdateTextDisplay(float DeltaTime)
{
    if (TextDisplaySpeed <= 0.0f)
    {
        // 立即显示全部文本
        TextProgress = 1.0f;
    }
    else
    {
        // 逐字显示
        FText FullText = DialogueDataAsset->GetDisplayText(CurrentDialogue);
        int32 TotalChars = FullText.ToString().Len();
        
        if (TotalChars > 0)
        {
            float CharsPerSecond = TextDisplaySpeed;
            float ProgressDelta = (CharsPerSecond / TotalChars) * DeltaTime;
            TextProgress = FMath::Clamp(TextProgress + ProgressDelta, 0.0f, 1.0f);
        }
        else
        {
            TextProgress = 1.0f;
        }
    }

    // 触发文本更新事件
    FText DisplayText = GetCurrentDisplayText();
    OnDialogueTextChanged.Broadcast(DisplayText, TextProgress);
}

void UDialogueComponent::CompleteCurrentDialogue()
{
    // 触发完成事件
    OnDialogueCompleted.Broadcast(CurrentDialogue);

    UE_LOG(LogTemp, Log, TEXT("DialogueComponent: Completed dialogue '%s'"), *CurrentDialogue.DialogueID.ToString());

    // 检查是否有选项
    if (!CurrentDialogue.ChoiceOptions.IsEmpty())
    {
        // 进入等待选择状态
        CurrentState = EDialogueState::WaitingForInput;

        // 解析选项
        TArray<FString> Choices;
        CurrentDialogue.ChoiceOptions.ParseIntoArray(Choices, TEXT("|"), true);
        OnWaitingForChoice.Broadcast(Choices);
    }
    // 检查是否有下一条对话
    else if (bAutoPlayNext && !CurrentDialogue.NextDialogueID.IsNone())
    {
        // 进入间隔状态
        bInInterval = true;
        IntervalTimer = 0.0f;
        CurrentState = EDialogueState::Idle;
    }
    else
    {
        // 对话完全结束
        CurrentState = EDialogueState::Completed;
        SetComponentTickEnabled(false);
        StopAudio();
    }
}

void UDialogueComponent::StopDialogue()
{
    if (CurrentState == EDialogueState::Idle)
    {
        return;
    }

    CurrentState = EDialogueState::Idle;
    TextProgress = 0.0f;
    DialogueTimer = 0.0f;
    bInInterval = false;
    
    SetComponentTickEnabled(false);
    StopAudio();

    UE_LOG(LogTemp, Log, TEXT("DialogueComponent: Stopped dialogue"));
}

void UDialogueComponent::SkipDialogue()
{
    if (!bAllowSkip || CurrentState != EDialogueState::Playing)
    {
        return;
    }

    // 立即完成文本显示
    TextProgress = 1.0f;
    DialogueTimer = CurrentDialogue.Duration;

    // 更新显示
    FText DisplayText = GetCurrentDisplayText();
    OnDialogueTextChanged.Broadcast(DisplayText, TextProgress);

    // 完成对话
    CompleteCurrentDialogue();

    UE_LOG(LogTemp, Log, TEXT("DialogueComponent: Skipped dialogue"));
}

void UDialogueComponent::PlayNextDialogue()
{
    if (CurrentDialogue.NextDialogueID.IsNone())
    {
        CurrentState = EDialogueState::Completed;
        SetComponentTickEnabled(false);
        return;
    }

    PlayDialogue(CurrentDialogue.NextDialogueID);
}

void UDialogueComponent::SelectChoice(int32 ChoiceIndex)
{
    if (CurrentState != EDialogueState::WaitingForInput)
    {
        UE_LOG(LogTemp, Warning, TEXT("DialogueComponent: Not waiting for choice"));
        return;
    }

    // 解析选项
    TArray<FString> Choices;
    CurrentDialogue.ChoiceOptions.ParseIntoArray(Choices, TEXT("|"), true);

    if (!Choices.IsValidIndex(ChoiceIndex))
    {
        UE_LOG(LogTemp, Error, TEXT("DialogueComponent: Invalid choice index %d"), ChoiceIndex);
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("DialogueComponent: Selected choice %d: %s"), ChoiceIndex, *Choices[ChoiceIndex]);

    // TODO: 根据选择播放不同的对话分支
    // 这里需要扩展DialogueDataAsset来支持分支对话
    
    // 暂时直接完成对话
    CurrentState = EDialogueState::Completed;
    SetComponentTickEnabled(false);
}

FText UDialogueComponent::GetCurrentDisplayText() const
{
    if (!DialogueDataAsset)
    {
        return FText::GetEmpty();
    }

    FText FullText = DialogueDataAsset->GetDisplayText(CurrentDialogue);
    
    if (TextProgress >= 1.0f)
    {
        return FullText;
    }

    // 返回部分文本
    FString FullString = FullText.ToString();
    int32 CharCount = FMath::FloorToInt(FullString.Len() * TextProgress);
    FString PartialString = FullString.Left(CharCount);
    
    return FText::FromString(PartialString);
}

void UDialogueComponent::PlayAudio(const FDialogueEntry& Entry)
{
    // 如果没有音频路径,跳过
    if (Entry.AudioPath.IsEmpty())
    {
        return;
    }

    // TODO: 加载音频资源
    // 这需要实现异步资源加载
    // 暂时使用日志输出
    UE_LOG(LogTemp, Log, TEXT("DialogueComponent: Would play audio: %s"), *Entry.AudioPath);

    // 示例代码(需要音频资源已加载):
    /*
    if (Entry.AudioAsset)
    {
        if (!AudioComponent)
        {
            AudioComponent = UGameplayStatics::SpawnSound2D(this, Entry.AudioAsset);
        }
        else
        {
            AudioComponent->SetSound(Entry.AudioAsset);
            AudioComponent->Play();
        }
    }
    */
}

void UDialogueComponent::StopAudio()
{
    if (AudioComponent && AudioComponent->IsPlaying())
    {
        AudioComponent->Stop();
    }
}
