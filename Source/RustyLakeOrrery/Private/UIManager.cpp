// UIManager.cpp

#include "UIManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// 静态实例初始化
AUIManager* AUIManager::Instance = nullptr;

AUIManager::AUIManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AUIManager::BeginPlay()
{
    Super::BeginPlay();

    // 设置单例
    Instance = this;

    // 创建UI Widgets
    CreateUIWidgets();

    // 初始状态隐藏所有UI
    HideAllUI();

    UE_LOG(LogTemp, Log, TEXT("UIManager: Initialized"));
}

void AUIManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    // 清理单例
    if (Instance == this)
    {
        Instance = nullptr;
    }

    // 清理计时器
    GetWorld()->GetTimerManager().ClearTimer(HintTimerHandle);
}

void AUIManager::CreateUIWidgets()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("UIManager: Failed to get PlayerController"));
        return;
    }

    // 创建对话UI
    if (DialogueWidgetClass)
    {
        DialogueWidget = CreateWidget<UUserWidget>(PC, DialogueWidgetClass);
        if (DialogueWidget)
        {
            DialogueWidget->AddToViewport(10); // 较高的Z-Order
            DialogueWidget->SetVisibility(ESlateVisibility::Hidden);
            UE_LOG(LogTemp, Log, TEXT("UIManager: Created Dialogue Widget"));
        }
    }

    // 创建背包UI
    if (InventoryWidgetClass)
    {
        InventoryWidget = CreateWidget<UUserWidget>(PC, InventoryWidgetClass);
        if (InventoryWidget)
        {
            InventoryWidget->AddToViewport(20); // 更高的Z-Order
            InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
            UE_LOG(LogTemp, Log, TEXT("UIManager: Created Inventory Widget"));
        }
    }

    // 创建提示UI
    if (HintWidgetClass)
    {
        HintWidget = CreateWidget<UUserWidget>(PC, HintWidgetClass);
        if (HintWidget)
        {
            HintWidget->AddToViewport(30); // 最高的Z-Order
            HintWidget->SetVisibility(ESlateVisibility::Hidden);
            UE_LOG(LogTemp, Log, TEXT("UIManager: Created Hint Widget"));
        }
    }

    // 创建交互提示UI
    if (InteractionPromptWidgetClass)
    {
        InteractionPromptWidget = CreateWidget<UUserWidget>(PC, InteractionPromptWidgetClass);
        if (InteractionPromptWidget)
        {
            InteractionPromptWidget->AddToViewport(5); // 较低的Z-Order
            InteractionPromptWidget->SetVisibility(ESlateVisibility::Hidden);
            UE_LOG(LogTemp, Log, TEXT("UIManager: Created Interaction Prompt Widget"));
        }
    }
}

// ========================================================================
// 对话UI接口实现
// ========================================================================

void AUIManager::ShowDialogue(const FDialogueEntry& DialogueEntry)
{
    if (!DialogueWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("UIManager: Dialogue Widget not created"));
        return;
    }

    DialogueWidget->SetVisibility(ESlateVisibility::Visible);

    // TODO: 调用Widget的蓝图函数来设置对话内容
    // 这需要在Widget蓝图中实现对应的函数
    UE_LOG(LogTemp, Log, TEXT("UIManager: Showing dialogue: %s"), *DialogueEntry.DialogueID.ToString());
}

void AUIManager::UpdateDialogueText(const FText& DisplayText, float Progress)
{
    if (!DialogueWidget || DialogueWidget->GetVisibility() != ESlateVisibility::Visible)
    {
        return;
    }

    // TODO: 调用Widget的蓝图函数来更新文本
    // UE_LOG(LogTemp, Verbose, TEXT("UIManager: Updating dialogue text, progress: %.2f"), Progress);
}

void AUIManager::HideDialogue()
{
    if (DialogueWidget)
    {
        DialogueWidget->SetVisibility(ESlateVisibility::Hidden);
        UE_LOG(LogTemp, Log, TEXT("UIManager: Hiding dialogue"));
    }
}

void AUIManager::ShowDialogueChoices(const TArray<FString>& Choices)
{
    if (!DialogueWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("UIManager: Dialogue Widget not created"));
        return;
    }

    // TODO: 调用Widget的蓝图函数来显示选项
    UE_LOG(LogTemp, Log, TEXT("UIManager: Showing %d dialogue choices"), Choices.Num());
}

// ========================================================================
// 背包UI接口实现
// ========================================================================

void AUIManager::ShowInventory()
{
    if (!InventoryWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("UIManager: Inventory Widget not created"));
        return;
    }

    InventoryWidget->SetVisibility(ESlateVisibility::Visible);
    RefreshInventory();
    UE_LOG(LogTemp, Log, TEXT("UIManager: Showing inventory"));
}

void AUIManager::HideInventory()
{
    if (InventoryWidget)
    {
        InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
        UE_LOG(LogTemp, Log, TEXT("UIManager: Hiding inventory"));
    }
}

void AUIManager::ToggleInventory()
{
    if (!InventoryWidget)
    {
        return;
    }

    if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        HideInventory();
    }
    else
    {
        ShowInventory();
    }
}

void AUIManager::RefreshInventory()
{
    if (!InventoryWidget)
    {
        return;
    }

    // TODO: 调用Widget的蓝图函数来刷新背包内容
    // 需要从InventoryComponent获取物品列表
    UE_LOG(LogTemp, Log, TEXT("UIManager: Refreshing inventory"));
}

// ========================================================================
// 提示UI接口实现
// ========================================================================

void AUIManager::ShowHint(const FText& HintText, float Duration)
{
    if (!HintWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("UIManager: Hint Widget not created"));
        return;
    }

    HintWidget->SetVisibility(ESlateVisibility::Visible);

    // TODO: 调用Widget的蓝图函数来设置提示文本
    UE_LOG(LogTemp, Log, TEXT("UIManager: Showing hint: %s"), *HintText.ToString());

    // 清除之前的计时器
    GetWorld()->GetTimerManager().ClearTimer(HintTimerHandle);

    // 如果有持续时间,设置自动隐藏
    if (Duration > 0.0f)
    {
        GetWorld()->GetTimerManager().SetTimer(
            HintTimerHandle,
            this,
            &AUIManager::HideHint,
            Duration,
            false
        );
    }
}

void AUIManager::HideHint()
{
    if (HintWidget)
    {
        HintWidget->SetVisibility(ESlateVisibility::Hidden);
        UE_LOG(LogTemp, Log, TEXT("UIManager: Hiding hint"));
    }

    // 清除计时器
    GetWorld()->GetTimerManager().ClearTimer(HintTimerHandle);
}

// ========================================================================
// 交互提示UI接口实现
// ========================================================================

void AUIManager::ShowInteractionPrompt(const FText& PromptText)
{
    if (!InteractionPromptWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("UIManager: Interaction Prompt Widget not created"));
        return;
    }

    InteractionPromptWidget->SetVisibility(ESlateVisibility::Visible);

    // TODO: 调用Widget的蓝图函数来设置提示文本
    UE_LOG(LogTemp, Log, TEXT("UIManager: Showing interaction prompt: %s"), *PromptText.ToString());
}

void AUIManager::HideInteractionPrompt()
{
    if (InteractionPromptWidget)
    {
        InteractionPromptWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

// ========================================================================
// 通用UI接口实现
// ========================================================================

void AUIManager::HideAllUI()
{
    HideDialogue();
    HideInventory();
    HideHint();
    HideInteractionPrompt();

    UE_LOG(LogTemp, Log, TEXT("UIManager: Hiding all UI"));
}

bool AUIManager::IsAnyUIVisible() const
{
    bool bAnyVisible = false;

    if (DialogueWidget && DialogueWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        bAnyVisible = true;
    }

    if (InventoryWidget && InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        bAnyVisible = true;
    }

    if (HintWidget && HintWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        bAnyVisible = true;
    }

    return bAnyVisible;
}

// ========================================================================
// 单例访问
// ========================================================================

AUIManager* AUIManager::GetUIManager(const UObject* WorldContext)
{
    if (Instance)
    {
        return Instance;
    }

    // 尝试在世界中查找UIManager
    if (WorldContext)
    {
        UWorld* World = WorldContext->GetWorld();
        if (World)
        {
            TArray<AActor*> FoundActors;
            UGameplayStatics::GetAllActorsOfClass(World, AUIManager::StaticClass(), FoundActors);
            
            if (FoundActors.Num() > 0)
            {
                Instance = Cast<AUIManager>(FoundActors[0]);
                return Instance;
            }
        }
    }

    UE_LOG(LogTemp, Error, TEXT("UIManager: No UIManager instance found in the world"));
    return nullptr;
}
