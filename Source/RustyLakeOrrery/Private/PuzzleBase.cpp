// PuzzleBase.cpp

#include "PuzzleBase.h"
#include "ItemDataAsset.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

APuzzleBase::APuzzleBase()
{
    PrimaryActorTick.bCanEverTick = false;

    bAutoActivate = false;
    bAllowReset = true;
    bHasHints = true;
}

void APuzzleBase::BeginPlay()
{
    Super::BeginPlay();

    CurrentState = EPuzzleState::Inactive;
    CurrentHintIndex = 0;

    if (bAutoActivate)
    {
        ActivatePuzzle();
    }

    UE_LOG(LogTemp, Log, TEXT("PuzzleBase: '%s' initialized"), *PuzzleName.ToString());
}

void APuzzleBase::ActivatePuzzle()
{
    if (CurrentState != EPuzzleState::Inactive)
    {
        UE_LOG(LogTemp, Warning, TEXT("PuzzleBase: Puzzle '%s' is already activated"), *PuzzleName.ToString());
        return;
    }

    CurrentState = EPuzzleState::Active;
    StartTime = GetWorld()->GetTimeSeconds();

    // 触发激活事件
    OnPuzzleActivated.Broadcast(this);
    OnPuzzleActivatedEvent_Implementation();

    UE_LOG(LogTemp, Log, TEXT("PuzzleBase: Puzzle '%s' activated"), *PuzzleName.ToString());
}

void APuzzleBase::CompletePuzzle()
{
    if (CurrentState == EPuzzleState::Completed)
    {
        UE_LOG(LogTemp, Warning, TEXT("PuzzleBase: Puzzle '%s' is already completed"), *PuzzleName.ToString());
        return;
    }

    CurrentState = EPuzzleState::Completed;
    CompletionTime = GetWorld()->GetTimeSeconds() - StartTime;

    // 给予奖励
    GiveReward();

    // 触发完成事件
    OnPuzzleCompleted.Broadcast(this);
    OnPuzzleSolved_Implementation();

    UE_LOG(LogTemp, Log, TEXT("PuzzleBase: Puzzle '%s' completed in %.2f seconds"), 
           *PuzzleName.ToString(), CompletionTime);
}

void APuzzleBase::FailPuzzle()
{
    if (CurrentState == EPuzzleState::Completed)
    {
        UE_LOG(LogTemp, Warning, TEXT("PuzzleBase: Cannot fail completed puzzle '%s'"), *PuzzleName.ToString());
        return;
    }

    CurrentState = EPuzzleState::Failed;

    // 触发失败事件
    OnPuzzleFailed.Broadcast(this);
    OnPuzzleFailedEvent_Implementation();

    UE_LOG(LogTemp, Log, TEXT("PuzzleBase: Puzzle '%s' failed"), *PuzzleName.ToString());
}

void APuzzleBase::ResetPuzzle()
{
    if (!bAllowReset)
    {
        UE_LOG(LogTemp, Warning, TEXT("PuzzleBase: Puzzle '%s' does not allow reset"), *PuzzleName.ToString());
        return;
    }

    CurrentState = EPuzzleState::Inactive;
    CurrentHintIndex = 0;
    StartTime = 0.0f;
    CompletionTime = 0.0f;

    // 触发重置事件
    OnPuzzleReset.Broadcast(this);
    OnPuzzleResetEvent_Implementation();

    UE_LOG(LogTemp, Log, TEXT("PuzzleBase: Puzzle '%s' reset"), *PuzzleName.ToString());
}

FText APuzzleBase::ShowNextHint()
{
    if (!bHasHints || HintTexts.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("PuzzleBase: Puzzle '%s' has no hints"), *PuzzleName.ToString());
        return FText::GetEmpty();
    }

    if (CurrentHintIndex >= HintTexts.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("PuzzleBase: No more hints for puzzle '%s'"), *PuzzleName.ToString());
        return FText::GetEmpty();
    }

    FText HintText = HintTexts[CurrentHintIndex];
    CurrentHintIndex++;

    // 触发提示事件
    OnHintShown.Broadcast(this, HintText);

    UE_LOG(LogTemp, Log, TEXT("PuzzleBase: Showing hint %d/%d for puzzle '%s'"), 
           CurrentHintIndex, HintTexts.Num(), *PuzzleName.ToString());

    return HintText;
}

FText APuzzleBase::GetCurrentHint() const
{
    if (!bHasHints || HintTexts.Num() == 0 || CurrentHintIndex == 0)
    {
        return FText::GetEmpty();
    }

    int32 Index = FMath::Clamp(CurrentHintIndex - 1, 0, HintTexts.Num() - 1);
    return HintTexts[Index];
}

bool APuzzleBase::HasMoreHints() const
{
    return bHasHints && CurrentHintIndex < HintTexts.Num();
}

float APuzzleBase::GetProgress_Implementation() const
{
    // 默认实现:根据状态返回进度
    switch (CurrentState)
    {
        case EPuzzleState::Inactive:
            return 0.0f;
        case EPuzzleState::Active:
        case EPuzzleState::Solving:
            return 0.5f;
        case EPuzzleState::Completed:
            return 1.0f;
        case EPuzzleState::Failed:
            return 0.0f;
        default:
            return 0.0f;
    }
}

void APuzzleBase::OnPuzzleActivatedEvent_Implementation()
{
    // 默认实现为空,子类可以重写
    UE_LOG(LogTemp, Log, TEXT("PuzzleBase: OnPuzzleActivatedEvent called for '%s'"), *PuzzleName.ToString());
}

void APuzzleBase::OnPuzzleSolved_Implementation()
{
    // 默认实现为空,子类可以重写
    UE_LOG(LogTemp, Log, TEXT("PuzzleBase: OnPuzzleSolved called for '%s'"), *PuzzleName.ToString());
}

void APuzzleBase::OnPuzzleFailedEvent_Implementation()
{
    // 默认实现为空,子类可以重写
    UE_LOG(LogTemp, Log, TEXT("PuzzleBase: OnPuzzleFailedEvent called for '%s'"), *PuzzleName.ToString());
}

void APuzzleBase::OnPuzzleResetEvent_Implementation()
{
    // 默认实现为空,子类可以重写
    UE_LOG(LogTemp, Log, TEXT("PuzzleBase: OnPuzzleResetEvent called for '%s'"), *PuzzleName.ToString());
}

void APuzzleBase::GiveReward()
{
    if (!RewardItem)
    {
        return;
    }

    // 查找玩家的InventoryComponent
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!PlayerPawn)
    {
        UE_LOG(LogTemp, Error, TEXT("PuzzleBase: Failed to get player pawn"));
        return;
    }

    UInventoryComponent* Inventory = PlayerPawn->FindComponentByClass<UInventoryComponent>();
    if (!Inventory)
    {
        UE_LOG(LogTemp, Error, TEXT("PuzzleBase: Player has no InventoryComponent"));
        return;
    }

    // 添加奖励物品
    if (Inventory->AddItem(RewardItem))
    {
        UE_LOG(LogTemp, Log, TEXT("PuzzleBase: Gave reward item '%s' to player"), 
               *RewardItem->ItemName.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PuzzleBase: Failed to give reward item '%s'"), 
               *RewardItem->ItemName.ToString());
    }
}
