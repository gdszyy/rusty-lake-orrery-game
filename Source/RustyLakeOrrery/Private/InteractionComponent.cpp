// InteractionComponent.cpp

#include "InteractionComponent.h"
#include "InteractableComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UInteractionComponent::UInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
    Super::BeginPlay();

    // 缓存PlayerController
    CachedPlayerController = Cast<APlayerController>(GetOwner());
    
    if (!CachedPlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("InteractionComponent: Must be attached to a PlayerController!"));
        return;
    }

    // 绑定输入
    BindInputActions();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 处理触控输入
    HandleTouchInput(DeltaTime);

    // 如果没有触摸输入，执行常规的射线检测（用于鼠标输入）
    if (CurrentTouchState == ETouchState::None)
    {
        PerformInteractionTrace();
    }
}

FText UInteractionComponent::GetCurrentPrompt() const
{
    if (CurrentInteractableComponent && CurrentInteractableComponent->CanInteract())
    {
        return CurrentInteractableComponent->GetPrompt();
    }
    
    return FText::GetEmpty();
}

float UInteractionComponent::GetLongPressProgress() const
{
    if (!TouchStartInteractableComponent)
    {
        return 0.0f;
    }

    // 获取长按所需时间
    // TODO: 从InteractableComponent获取LongPressDuration
    float RequiredDuration = 1.0f; // 默认值
    
    return FMath::Clamp(TouchDuration / RequiredDuration, 0.0f, 1.0f);
}

// ============================================================================
// 内部实现函数（完全封装的黑盒逻辑）
// ============================================================================

void UInteractionComponent::PerformInteractionTrace()
{
    if (!CachedPlayerController)
    {
        return;
    }

    // 获取鼠标位置
    float MouseX, MouseY;
    CachedPlayerController->GetMousePosition(MouseX, MouseY);

    FHitResult HitResult;
    bool bHit = TraceFromScreenPosition(FVector2D(MouseX, MouseY), HitResult);

    // 更新聚焦状态
    AActor* NewFocusedActor = nullptr;
    
    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            // 检查是否有InteractableComponent
            UInteractableComponent* InteractableComp = HitActor->FindComponentByClass<UInteractableComponent>();
            if (InteractableComp && InteractableComp->CanInteract())
            {
                NewFocusedActor = HitActor;
            }
        }
    }

    UpdateFocusedActor(NewFocusedActor);
}

void UInteractionComponent::HandleTouchInput(float DeltaTime)
{
    if (!CachedPlayerController)
    {
        return;
    }

    // 获取触摸输入状态
    float TouchX, TouchY;
    bool bIsTouching = false;
    CachedPlayerController->GetInputTouchState(ETouchIndex::Touch1, TouchX, TouchY, bIsTouching);

    FVector2D CurrentScreenPosition(TouchX, TouchY);

    // 状态机处理触摸事件
    if (bIsTouching)
    {
        if (CurrentTouchState == ETouchState::None)
        {
            // 触摸开始
            OnTouchBegan(CurrentScreenPosition);
        }
        else
        {
            // 触摸移动
            OnTouchMoved(CurrentScreenPosition);
            
            // 更新触摸持续时间
            TouchDuration += DeltaTime;
        }
    }
    else
    {
        if (CurrentTouchState != ETouchState::None)
        {
            // 触摸结束
            OnTouchEnded(CurrentScreenPosition);
        }
    }
}

void UInteractionComponent::OnTouchBegan(const FVector2D& TouchLocation)
{
    CurrentTouchState = ETouchState::Began;
    TouchStartPosition = TouchLocation;
    CurrentTouchPosition = TouchLocation;
    LastTouchPosition = TouchLocation;
    TouchStartTime = GetWorld()->GetTimeSeconds();
    TouchDuration = 0.0f;
    TouchTotalMovement = 0.0f;
    bIsRotating = false;

    // 执行射线检测，记录触摸开始时的聚焦对象
    FHitResult HitResult;
    if (TraceFromScreenPosition(TouchLocation, HitResult))
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            UInteractableComponent* InteractableComp = HitActor->FindComponentByClass<UInteractableComponent>();
            if (InteractableComp && InteractableComp->CanInteract())
            {
                TouchStartFocusedActor = HitActor;
                TouchStartInteractableComponent = InteractableComp;
                
                // 更新聚焦状态
                UpdateFocusedActor(HitActor);
                
                // 如果是长按模式，开始长按计时
                if (InteractableComp->InteractionMode == EInteractionMode::LongPress)
                {
                    InteractableComp->BeginLongPress();
                }
                
                if (bShowGestureDebug)
                {
                    UE_LOG(LogTemp, Log, TEXT("InteractionComponent: Touch began on %s"), *HitActor->GetName());
                }
            }
        }
    }
}

void UInteractionComponent::OnTouchMoved(const FVector2D& TouchLocation)
{
    CurrentTouchState = ETouchState::Moved;
    
    // 计算移动距离
    FVector2D Movement = TouchLocation - LastTouchPosition;
    float MovementDistance = Movement.Size();
    TouchTotalMovement += MovementDistance;
    
    CurrentTouchPosition = TouchLocation;

    if (!TouchStartInteractableComponent)
    {
        LastTouchPosition = TouchLocation;
        return;
    }

    // 根据交互模式处理移动
    switch (TouchStartInteractableComponent->InteractionMode)
    {
        case EInteractionMode::Tap:
            // 点击模式：如果移动距离超过阈值，取消点击
            if (TouchTotalMovement > TapMaxMovement)
            {
                TouchStartInteractableComponent = nullptr;
            }
            break;

        case EInteractionMode::Swipe:
            // 滑动模式：不需要特殊处理，在触摸结束时识别
            break;

        case EInteractionMode::Rotate:
            // 旋转模式：持续更新旋转
            if (!bIsRotating && TouchTotalMovement > TapMaxMovement)
            {
                // 开始旋转
                bIsRotating = true;
                TouchStartInteractableComponent->BeginRotation();
            }
            
            if (bIsRotating)
            {
                // 计算旋转增量（基于水平移动）
                float DeltaRotation = Movement.X * RotationSensitivity;
                TouchStartInteractableComponent->UpdateRotation(DeltaRotation);
            }
            break;

        case EInteractionMode::LongPress:
            // 长按模式：更新长按进度
            if (TouchStartInteractableComponent->UpdateLongPress(GetWorld()->GetDeltaSeconds()))
            {
                // 长按完成，执行交互
                TouchStartInteractableComponent->ExecuteInteraction(CachedPlayerController);
                TouchStartInteractableComponent = nullptr;
            }
            break;
    }

    LastTouchPosition = TouchLocation;
}

void UInteractionComponent::OnTouchEnded(const FVector2D& TouchLocation)
{
    if (bShowGestureDebug)
    {
        UE_LOG(LogTemp, Log, TEXT("InteractionComponent: Touch ended, Duration: %.2f, Movement: %.2f"), 
            TouchDuration, TouchTotalMovement);
    }

    // 识别手势并执行相应交互
    RecognizeGesture();

    // 重置状态
    CurrentTouchState = ETouchState::None;
    TouchStartFocusedActor = nullptr;
    
    if (TouchStartInteractableComponent)
    {
        // 如果正在旋转，结束旋转
        if (bIsRotating)
        {
            TouchStartInteractableComponent->EndRotation();
        }
        
        // 如果正在长按，取消长按
        TouchStartInteractableComponent->CancelLongPress();
        
        TouchStartInteractableComponent = nullptr;
    }
    
    bIsRotating = false;
}

void UInteractionComponent::RecognizeGesture()
{
    if (!TouchStartInteractableComponent)
    {
        return;
    }

    EInteractionMode Mode = TouchStartInteractableComponent->InteractionMode;

    switch (Mode)
    {
        case EInteractionMode::Tap:
            // 检查是否是有效的点击
            if (TouchTotalMovement <= TapMaxMovement && TouchDuration <= TapMaxDuration)
            {
                ExecuteTapInteraction();
            }
            break;

        case EInteractionMode::Swipe:
            // 检查是否是有效的滑动
            ExecuteSwipeInteraction();
            break;

        case EInteractionMode::Rotate:
            // 旋转在OnTouchMoved中持续处理，这里不需要额外操作
            break;

        case EInteractionMode::LongPress:
            // 长按在OnTouchMoved中处理，这里不需要额外操作
            break;
    }
}

void UInteractionComponent::ExecuteTapInteraction()
{
    if (!TouchStartInteractableComponent)
    {
        return;
    }

    TouchStartInteractableComponent->ExecuteInteraction(CachedPlayerController);
    
    if (bShowGestureDebug)
    {
        UE_LOG(LogTemp, Log, TEXT("InteractionComponent: Tap interaction executed"));
    }
}

void UInteractionComponent::ExecuteSwipeInteraction()
{
    if (!TouchStartInteractableComponent)
    {
        return;
    }

    // 计算滑动向量
    FVector2D SwipeVector = CurrentTouchPosition - TouchStartPosition;
    
    // 尝试处理滑动
    bool bSwipeHandled = TouchStartInteractableComponent->HandleSwipe(SwipeVector, CachedPlayerController);
    
    if (bShowGestureDebug)
    {
        UE_LOG(LogTemp, Log, TEXT("InteractionComponent: Swipe interaction, Vector: %s, Handled: %s"),
            *SwipeVector.ToString(), bSwipeHandled ? TEXT("Yes") : TEXT("No"));
    }
}

bool UInteractionComponent::TraceFromScreenPosition(const FVector2D& ScreenPosition, FHitResult& OutHitResult)
{
    if (!CachedPlayerController)
    {
        return false;
    }

    // 将屏幕坐标转换为世界射线
    FVector WorldLocation, WorldDirection;
    bool bSuccess = CachedPlayerController->DeprojectScreenPositionToWorld(
        ScreenPosition.X,
        ScreenPosition.Y,
        WorldLocation,
        WorldDirection
    );

    if (!bSuccess)
    {
        return false;
    }

    // 计算射线终点
    FVector TraceEnd = WorldLocation + (WorldDirection * InteractionDistance);

    // 执行射线检测
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(CachedPlayerController->GetPawn());

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        OutHitResult,
        WorldLocation,
        TraceEnd,
        TraceChannel,
        QueryParams
    );

    // 调试可视化
    if (bShowDebugTrace)
    {
        FColor TraceColor = bHit ? FColor::Green : FColor::Red;
        DrawDebugLine(GetWorld(), WorldLocation, TraceEnd, TraceColor, false, 0.0f, 0, 1.0f);
        
        if (bHit)
        {
            DrawDebugSphere(GetWorld(), OutHitResult.ImpactPoint, 10.0f, 12, FColor::Green, false, 0.0f);
        }
    }

    return bHit;
}

void UInteractionComponent::UpdateFocusedActor(AActor* NewFocusedActor)
{
    if (CurrentFocusedActor == NewFocusedActor)
    {
        // 聚焦未改变
        return;
    }

    // 结束旧对象的聚焦
    if (CurrentInteractableComponent)
    {
        CurrentInteractableComponent->EndFocus();
    }

    // 更新聚焦对象
    CurrentFocusedActor = NewFocusedActor;
    CurrentInteractableComponent = nullptr;

    // 开始新对象的聚焦
    if (CurrentFocusedActor)
    {
        CurrentInteractableComponent = CurrentFocusedActor->FindComponentByClass<UInteractableComponent>();
        if (CurrentInteractableComponent)
        {
            CurrentInteractableComponent->BeginFocus();
            
            if (bShowGestureDebug)
            {
                UE_LOG(LogTemp, Verbose, TEXT("InteractionComponent: Focused on %s"), *CurrentFocusedActor->GetName());
            }
        }
    }
}

void UInteractionComponent::BindInputActions()
{
    if (bInputBound || !CachedPlayerController)
    {
        return;
    }

    // 对于触控游戏，主要依赖触摸输入，不需要绑定额外的输入动作
    // 触摸输入通过GetInputTouchState在Tick中自动处理
    
    // 如果需要支持鼠标点击（PC测试），可以在这里绑定
    // TODO: 根据项目需求决定是否绑定鼠标输入
    
    bInputBound = true;
    UE_LOG(LogTemp, Log, TEXT("InteractionComponent: Touch input system initialized"));
}
