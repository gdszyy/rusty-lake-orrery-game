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
    PrimaryComponentTick.bCanEverTick = true;
}

void UInteractableComponent::BeginPlay()
{
    Super::BeginPlay();

    // 缓存网格体组件用于高亮效果和旋转
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

        // 保存初始旋转
        if (CachedMeshComponent)
        {
            InitialRotation = CachedMeshComponent->GetRelativeRotation();
        }
    }
}

void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 如果正在旋转且是RotateObject类型，检查目标角度
    if (bIsRotating && InteractionType == EInteractionType::RotateObject)
    {
        CheckTargetRotation();
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

        case EInteractionType::SwipeTrigger:
            HandleSwipeTriggerInteraction(Interactor);
            break;

        case EInteractionType::RotateObject:
            // 旋转类型不通过点击触发，而是通过BeginRotation/UpdateRotation
            break;

        case EInteractionType::Custom:
            // 触发蓝图自定义事件
            OnCustomInteract.Broadcast(Interactor);
            break;
    }
}

bool UInteractableComponent::HandleSwipe(const FVector2D& SwipeVector, AActor* Interactor)
{
    if (!bIsInteractable || !Interactor)
    {
        return false;
    }

    // 检查滑动方向是否有效
    if (!IsSwipeDirectionValid(SwipeVector))
    {
        return false;
    }

    // 执行交互
    ExecuteInteraction(Interactor);
    
    UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Swipe handled, direction: %s"), 
        *SwipeVector.ToString());
    
    return true;
}

void UInteractableComponent::BeginRotation()
{
    if (!bIsInteractable)
    {
        return;
    }

    bIsRotating = true;
    bTargetAngleReached = false;
    
    UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Rotation started"));
}

void UInteractableComponent::UpdateRotation(float DeltaRotation)
{
    if (!bIsRotating || !CachedMeshComponent)
    {
        return;
    }

    // 应用旋转灵敏度
    float AdjustedRotation = DeltaRotation * RotationSensitivity;

    // 累计旋转角度
    CurrentRotationAngle += AdjustedRotation;

    // 如果启用了角度限制，进行夹紧
    if (bClampRotation)
    {
        CurrentRotationAngle = FMath::Clamp(CurrentRotationAngle, MinRotationAngle, MaxRotationAngle);
    }
    else
    {
        // 归一化到0-360范围
        while (CurrentRotationAngle >= 360.0f) CurrentRotationAngle -= 360.0f;
        while (CurrentRotationAngle < 0.0f) CurrentRotationAngle += 360.0f;
    }

    // 应用旋转到网格体
    FRotator NewRotation = InitialRotation;
    FQuat RotationQuat = FQuat(RotationAxis.GetSafeNormal(), FMath::DegreesToRadians(CurrentRotationAngle));
    NewRotation = (RotationQuat * FQuat(InitialRotation)).Rotator();
    
    CachedMeshComponent->SetRelativeRotation(NewRotation);

    UE_LOG(LogTemp, Verbose, TEXT("InteractableComponent: Rotation updated to %.2f degrees"), CurrentRotationAngle);
}

void UInteractableComponent::EndRotation()
{
    if (!bIsRotating)
    {
        return;
    }

    bIsRotating = false;
    
    UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Rotation ended at %.2f degrees"), CurrentRotationAngle);
}

void UInteractableComponent::BeginLongPress()
{
    if (!bIsInteractable)
    {
        return;
    }

    bIsLongPressing = true;
    LongPressTimer = 0.0f;
    
    UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Long press started"));
}

bool UInteractableComponent::UpdateLongPress(float DeltaTime)
{
    if (!bIsLongPressing)
    {
        return false;
    }

    LongPressTimer += DeltaTime;

    if (LongPressTimer >= LongPressDuration)
    {
        // 长按完成
        bIsLongPressing = false;
        UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Long press completed"));
        return true;
    }

    return false;
}

void UInteractableComponent::CancelLongPress()
{
    if (!bIsLongPressing)
    {
        return;
    }

    bIsLongPressing = false;
    LongPressTimer = 0.0f;
    
    UE_LOG(LogTemp, Verbose, TEXT("InteractableComponent: Long press cancelled"));
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

    // 如果正在长按，取消它
    if (bIsLongPressing)
    {
        CancelLongPress();
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
    UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Navigating to level: %s (TODO: Integrate CoreGame)"), *TargetLevelName.ToString());
}

void UInteractableComponent::HandleUseItemInteraction(AActor* Interactor)
{
    if (!RequiredItemData)
    {
        UE_LOG(LogTemp, Warning, TEXT("InteractableComponent: UseItem interaction but no RequiredItemData assigned!"));
        return;
    }

    // TODO: 集成InventorySystem检查物品
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
    UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Triggered puzzle (TODO: Integrate PuzzleSystem)"));
}

void UInteractableComponent::HandleSwipeTriggerInteraction(AActor* Interactor)
{
    // 显示滑动触发文本
    if (!OnSwipeText.IsEmpty())
    {
        // TODO: 显示UI文本
        UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Swipe trigger text: %s"), *OnSwipeText.ToString());
    }

    // 可以在这里触发其他逻辑，如播放动画、触发事件等
}

// ============================================================================
// 视觉反馈和辅助函数
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

        UE_LOG(LogTemp, Verbose, TEXT("InteractableComponent: Highlight enabled"));
    }
    else
    {
        // 禁用自定义深度渲染
        CachedMeshComponent->SetRenderCustomDepth(false);
        
        UE_LOG(LogTemp, Verbose, TEXT("InteractableComponent: Highlight disabled"));
    }
}

bool UInteractableComponent::IsSwipeDirectionValid(const FVector2D& SwipeVector) const
{
    // 检查滑动距离是否足够
    float SwipeDistance = SwipeVector.Size();
    if (SwipeDistance < MinSwipeDistance)
    {
        return false;
    }

    // 如果允许任意方向，直接返回true
    if (RequiredSwipeDirection == ESwipeDirection::Any)
    {
        return true;
    }

    // 计算滑动角度（相对于水平轴）
    float SwipeAngle = FMath::Atan2(SwipeVector.Y, SwipeVector.X) * 180.0f / PI;
    
    // 归一化到0-360范围
    if (SwipeAngle < 0) SwipeAngle += 360.0f;

    // 定义各方向的目标角度
    float TargetAngle = 0.0f;
    switch (RequiredSwipeDirection)
    {
        case ESwipeDirection::Right:
            TargetAngle = 0.0f;
            break;
        case ESwipeDirection::Up:
            TargetAngle = 90.0f;
            break;
        case ESwipeDirection::Left:
            TargetAngle = 180.0f;
            break;
        case ESwipeDirection::Down:
            TargetAngle = 270.0f;
            break;
        default:
            return true;
    }

    // 计算角度差
    float AngleDiff = FMath::Abs(SwipeAngle - TargetAngle);
    
    // 处理跨越0度的情况（例如350度和10度之间的差值）
    if (AngleDiff > 180.0f)
    {
        AngleDiff = 360.0f - AngleDiff;
    }

    // 检查是否在容差范围内
    bool bIsValid = AngleDiff <= SwipeAngleTolerance;

    UE_LOG(LogTemp, Verbose, TEXT("InteractableComponent: Swipe angle: %.2f, Target: %.2f, Diff: %.2f, Valid: %s"),
        SwipeAngle, TargetAngle, AngleDiff, bIsValid ? TEXT("Yes") : TEXT("No"));

    return bIsValid;
}

void UInteractableComponent::CheckTargetRotation()
{
    // 如果没有设置目标角度，或已经触发过，则不检查
    if (TargetRotationAngle < 0.0f || bTargetAngleReached)
    {
        return;
    }

    // 计算当前角度与目标角度的差值
    float AngleDiff = FMath::Abs(CurrentRotationAngle - TargetRotationAngle);
    
    // 处理跨越0度的情况
    if (AngleDiff > 180.0f)
    {
        AngleDiff = 360.0f - AngleDiff;
    }

    // 检查是否在容差范围内
    if (AngleDiff <= AngleTolerance)
    {
        bTargetAngleReached = true;
        OnTargetRotationReached.Broadcast(CurrentRotationAngle);
        
        UE_LOG(LogTemp, Log, TEXT("InteractableComponent: Target rotation reached! Current: %.2f, Target: %.2f"),
            CurrentRotationAngle, TargetRotationAngle);
    }
}
