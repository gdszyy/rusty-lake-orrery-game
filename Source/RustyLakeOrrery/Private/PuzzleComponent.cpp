// PuzzleComponent.cpp

#include "PuzzleComponent.h"

UPuzzleComponent::UPuzzleComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    bIsRequired = true;
    bIsCompleted = false;
}

void UPuzzleComponent::BeginPlay()
{
    Super::BeginPlay();

    // 如果没有设置ComponentID,使用组件名称
    if (ComponentID.IsNone())
    {
        ComponentID = FName(*GetName());
    }

    UE_LOG(LogTemp, Log, TEXT("PuzzleComponent: '%s' initialized"), *ComponentID.ToString());
}

void UPuzzleComponent::ActivateComponent()
{
    if (bIsCompleted)
    {
        UE_LOG(LogTemp, Warning, TEXT("PuzzleComponent: '%s' is already completed"), *ComponentID.ToString());
        return;
    }

    // 触发激活事件
    OnComponentActivated.Broadcast(this);

    UE_LOG(LogTemp, Log, TEXT("PuzzleComponent: '%s' activated"), *ComponentID.ToString());
}

void UPuzzleComponent::CompleteComponent()
{
    if (bIsCompleted)
    {
        UE_LOG(LogTemp, Warning, TEXT("PuzzleComponent: '%s' is already completed"), *ComponentID.ToString());
        return;
    }

    bIsCompleted = true;

    // 触发完成事件
    OnComponentCompleted.Broadcast(this);

    UE_LOG(LogTemp, Log, TEXT("PuzzleComponent: '%s' completed"), *ComponentID.ToString());

    // 检查整个谜题是否完成
    CheckPuzzleCompletion();
}

void UPuzzleComponent::ResetComponent()
{
    bIsCompleted = false;

    // 触发重置事件
    OnComponentReset.Broadcast(this);

    UE_LOG(LogTemp, Log, TEXT("PuzzleComponent: '%s' reset"), *ComponentID.ToString());
}

void UPuzzleComponent::CheckPuzzleCompletion()
{
    if (!PuzzleActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("PuzzleComponent: No PuzzleActor set for '%s'"), *ComponentID.ToString());
        return;
    }

    // 如果谜题已经完成,不需要再检查
    if (PuzzleActor->IsCompleted())
    {
        return;
    }

    // 获取所有PuzzleComponent
    AActor* Owner = PuzzleActor;
    TArray<UPuzzleComponent*> AllComponents;
    Owner->GetComponents<UPuzzleComponent>(AllComponents);

    // 检查所有必需组件是否都完成
    bool bAllCompleted = true;
    int32 CompletedCount = 0;
    int32 RequiredCount = 0;

    for (UPuzzleComponent* Component : AllComponents)
    {
        if (Component && Component->bIsRequired)
        {
            RequiredCount++;
            if (Component->bIsCompleted)
            {
                CompletedCount++;
            }
            else
            {
                bAllCompleted = false;
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("PuzzleComponent: Puzzle completion check - %d/%d required components completed"), 
           CompletedCount, RequiredCount);

    // 如果所有必需组件都完成,完成谜题
    if (bAllCompleted && RequiredCount > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("PuzzleComponent: All required components completed, completing puzzle"));
        PuzzleActor->CompletePuzzle();
    }
}

void UPuzzleComponent::SetPuzzleActor(APuzzleBase* NewPuzzleActor)
{
    PuzzleActor = NewPuzzleActor;
    UE_LOG(LogTemp, Log, TEXT("PuzzleComponent: '%s' linked to puzzle '%s'"), 
           *ComponentID.ToString(), 
           PuzzleActor ? *PuzzleActor->PuzzleName.ToString() : TEXT("None"));
}
