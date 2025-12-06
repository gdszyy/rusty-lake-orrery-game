// InteractableComponent.cpp

#include "InteractableComponent.h"
#include "Components/MeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UInteractableComponent::UInteractableComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInteractableComponent::BeginPlay()
{
    Super::BeginPlay();

    // 缓存网格体组件用于高亮效果
    AActor* Owner = GetOwner();
    if (Owner)
    {
        // 优先查找StaticMeshComponent
        CachedMeshComponent = Owner->FindComponentByClass<UStaticMeshComponent>();
        
        // 如果没有找到，尝试SkeletalMeshComponent
        if (!CachedMeshComponent)
        {
            CachedMeshComponent = Owner->FindComponentByClass<USkeletalMeshComponent>();
        }
    }
}

// ============================================================================
// 公共接口函数（由InteractionComponent调用）
// ============================================================================

void UInteractableComponent::ExecuteInteraction(AActor* Interactor)
{
    if (!bIsInteractable || !Interactor)
    {
        return;
    }

    // 根据交互类型分发到对应的处理函数
    switch (InteractionType)
    {
        case EInteractionType::Observe:
            HandleObserveInteraction(Interactor);
            break;

        case EInteractionType::Pickup:
            HandlePickupInteraction(Interactor);
            break;

        case EInteractionType::Navigate:
            HandleNavigateInteraction(Interactor);
            break;

        case EInteractionType::UseItem:
            HandleUseItemInteraction(Interactor);
            break;

        case EInteractionType::TriggerPuzzle:
            HandlePuzzleInteraction(Interactor);
            break;

        case EInteractionType::Custom:
            // 触发蓝图自定义事件
            OnCustomInteract.Broadcast(Interactor);
            break;
    }
}

void UInteractableComponent::BeginFocus()
{
    if (bIsFocused)
    {
        return;
    }

    bIsFocused = true;
    
    // 应用高亮效果
    if (bEnableHighlight)
    {
        ApplyHighlight(true);
    }
}

void UInteractableComponent::EndFocus()
{
    if (!bIsFocused)
    {
        return;
    }

    bIsFocused = false;
    
    // 移除高亮效果
    if (bEnableHighlight)
    {
        ApplyHighlight(false);
    }
}

// ============================================================================
// 内部交互处理函数（完全封装的黑盒逻辑）
// ============================================================================

void UInteractableComponent::HandleObserveInteraction(AActor* Interactor)
{
    // TODO: 集成DialogueSystem
    // 优先使用ObserveDialogue数据资产，如果没有则使用ObserveText
    
    if (ObserveDialogue)
    {
        // TODO: 调用DialogueComponent播放对话
        // UDialogueComponent* DialogueComp = Interactor->FindComponentByClass<UDialogueComponent>();
        // if (DialogueComp)
        // {
        //     DialogueComp->StartDialogue(ObserveDialogue);
        // }
        
        UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Playing dialogue asset (TODO: Integrate DialogueSystem)"));
    }
    else if (!ObserveText.IsEmpty())
    {
        // TODO: 显示简单文本UI
        UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Observe text: %s"), *ObserveText.ToString());
    }
}

void UInteractableComponent::HandlePickupInteraction(AActor* Interactor)
{
    if (!PickupItemData)
    {
        UE_LOG(LogTemp, Warning, TEXT("InteractableComponent: Pickup interaction but no ItemData assigned!"));
        return;
    }

    // TODO: 集成InventorySystem
    // UInventoryComponent* InventoryComp = Interactor->FindComponentByClass<UInventoryComponent>();
    // if (InventoryComp)
    // {
    //     bool bAdded = InventoryComp->AddItem(PickupItemData);
    //     if (!bAdded)
    //     {
    //         // 背包已满
    //         return;
    //     }
    // }

    UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Picked up item (TODO: Integrate InventorySystem)"));

    // 播放拾取音效
    if (PickupSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);
    }

    // 销毁Actor
    if (bDestroyAfterPickup)
    {
        AActor* Owner = GetOwner();
        if (Owner)
        {
            Owner->Destroy();
        }
    }
}

void UInteractableComponent::HandleNavigateInteraction(AActor* Interactor)
{
    if (TargetLevelName.IsNone())
    {
        UE_LOG(LogTemp, Warning, TEXT("InteractableComponent: Navigate interaction but no TargetLevelName assigned!"));
        return;
    }

    // TODO: 集成CoreGame的关卡加载系统
    // UGameManager* GameManager = UGameInstance::GetSubsystem<UGameManager>();
    // if (GameManager)
    // {
    //     GameManager->LoadChapter(TargetLevelName);
    // }

    UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Navigating to level: %s (TODO: Integrate CoreGame)"), *TargetLevelName.ToString());

    // 临时实现：直接加载关卡
    // UGameplayStatics::OpenLevel(GetWorld(), TargetLevelName);
}

void UInteractableComponent::HandleUseItemInteraction(AActor* Interactor)
{
    if (!RequiredItemData)
    {
        UE_LOG(LogTemp, Warning, TEXT("InteractableComponent: UseItem interaction but no RequiredItemData assigned!"));
        return;
    }

    // TODO: 集成InventorySystem检查物品
    // UInventoryComponent* InventoryComp = Interactor->FindComponentByClass<UInventoryComponent>();
    // if (InventoryComp)
    // {
    //     if (!InventoryComp->HasItem(RequiredItemData))
    //     {
    //         // 玩家没有所需物品
    //         UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Player doesn't have required item"));
    //         return;
    //     }
    //
    //     // 使用物品
    //     if (bConsumeItem)
    //     {
    //         InventoryComp->RemoveItem(RequiredItemData);
    //     }
    // }

    UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Used item (TODO: Integrate InventorySystem)"));

    // 显示成功文本
    if (!OnItemUsedText.IsEmpty())
    {
        // TODO: 显示UI文本
        UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Item used text: %s"), *OnItemUsedText.ToString());
    }

    // 使用物品后可以禁用交互
    bIsInteractable = false;
}

void UInteractableComponent::HandlePuzzleInteraction(AActor* Interactor)
{
    if (!TargetPuzzle)
    {
        UE_LOG(LogTemp, Warning, TEXT("InteractableComponent: TriggerPuzzle interaction but no TargetPuzzle assigned!"));
        return;
    }

    // TODO: 调用谜题的激活函数
    // TargetPuzzle->Activate();

    UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Triggered puzzle (TODO: Integrate PuzzleSystem)"));
}

// ============================================================================
// 视觉反馈函数
// ============================================================================

void UInteractableComponent::ApplyHighlight(bool bEnable)
{
    if (!CachedMeshComponent)
    {
        return;
    }

    if (bEnable)
    {
        // 启用自定义深度渲染（用于后期处理高亮）
        CachedMeshComponent->SetRenderCustomDepth(true);
        CachedMeshComponent->SetCustomDepthStencilValue(1);

        // TODO: 可以通过后期处理材质读取CustomDepth并应用高亮效果
        // 或者直接修改材质参数
        // UMaterialInstanceDynamic* DynMat = CachedMeshComponent->CreateDynamicMaterialInstance(0);
        // if (DynMat)
        // {
        //     DynMat->SetVectorParameterValue(FName("HighlightColor"), HighlightColor);
        //     DynMat->SetScalarParameterValue(FName("HighlightIntensity"), HighlightIntensity);
        // }

        UE_LOG(LogTemp, Verbose, TEXT("InteractableComponent: Highlight enabled"));
    }
    else
    {
        // 禁用自定义深度渲染
        CachedMeshComponent->SetRenderCustomDepth(false);
        
        UE_LOG(LogTemp, Verbose, TEXT("InteractableComponent: Highlight disabled"));
    }
}
