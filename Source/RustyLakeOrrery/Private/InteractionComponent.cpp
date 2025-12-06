// InteractionComponent.cpp

#include "InteractionComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// ============================================================================
// 构造函数和生命周期
// ============================================================================

UInteractionComponent::UInteractionComponent()
{
    // 设置此组件每帧调用TickComponent()
    PrimaryComponentTick.bCanEverTick = true;

    // 设置默认值
    InteractionDistance = 5000.0f;
    InteractionTraceChannel = ECC_Visibility;
    bDrawDebugTrace = false;
    TraceMode = EInteractionTraceMode::ScreenCenter;
}

void UInteractionComponent::BeginPlay()
{
    Super::BeginPlay();

    // 缓存PlayerController引用
    CachedPlayerController = Cast<APlayerController>(GetOwner());
    
    if (!CachedPlayerController)
    {
        // TODO: 添加错误日志,此组件必须附加在PlayerController上
        UE_LOG(LogTemp, Error, TEXT("UInteractionComponent: Component must be attached to a PlayerController!"));
    }
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 每帧更新交互检测
    UpdateInteraction();
}

// ============================================================================
// 交互函数
// ============================================================================

void UInteractionComponent::UpdateInteraction()
{
    // TODO: 实现射线检测逻辑
    // 1. 调用PerformTrace()执行射线检测
    // 2. 检查命中的Actor是否实现IInteractable接口
    // 3. 调用UpdateFocusedActor()更新聚焦状态
    
    if (!CachedPlayerController)
    {
        return;
    }

    FHitResult HitResult;
    AActor* NewFocusedActor = nullptr;

    if (PerformTrace(HitResult))
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor && IsActorInteractable(HitActor))
        {
            // 检查对象是否可以被交互
            if (IInteractable::Execute_CanInteract(HitActor))
            {
                NewFocusedActor = HitActor;
            }
        }
    }

    // 更新聚焦状态
    UpdateFocusedActor(NewFocusedActor);
}

void UInteractionComponent::Interact()
{
    // TODO: 实现交互逻辑
    // 1. 检查是否有聚焦的对象
    // 2. 检查对象是否可以被交互
    // 3. 调用对象的OnInteract()函数
    // 4. 广播OnInteracted事件
    
    if (!FocusedActor)
    {
        return;
    }

    if (!IsActorInteractable(FocusedActor))
    {
        return;
    }

    if (!IInteractable::Execute_CanInteract(FocusedActor))
    {
        return;
    }

    // 调用交互函数
    IInteractable::Execute_OnInteract(FocusedActor, CachedPlayerController);

    // 广播交互事件
    OnInteracted.Broadcast(FocusedActor);
}

bool UInteractionComponent::UseItemOnFocusedActor(UItemDataAsset* ItemData)
{
    // TODO: 实现物品使用逻辑
    // 1. 检查是否有聚焦的对象和有效的物品数据
    // 2. 调用对象的OnUseItem()函数
    // 3. 根据返回值决定是否从背包中移除物品
    
    if (!FocusedActor || !ItemData)
    {
        return false;
    }

    if (!IsActorInteractable(FocusedActor))
    {
        return false;
    }

    // 调用物品使用函数
    bool bItemUsed = IInteractable::Execute_OnUseItem(FocusedActor, ItemData, CachedPlayerController);

    return bItemUsed;
}

void UInteractionComponent::SetFocusedActor(AActor* NewFocusedActor)
{
    // TODO: 实现手动设置聚焦对象的逻辑
    UpdateFocusedActor(NewFocusedActor);
}

// ============================================================================
// 内部辅助函数
// ============================================================================

bool UInteractionComponent::PerformTrace(FHitResult& OutHitResult)
{
    // TODO: 实现射线检测逻辑
    // 1. 调用GetTraceStartAndDirection()获取射线参数
    // 2. 计算射线终点
    // 3. 执行LineTraceSingleByChannel
    // 4. 如果启用了调试绘制,绘制射线和命中点
    
    if (!CachedPlayerController)
    {
        return false;
    }

    FVector StartLocation;
    FVector Direction;

    if (!GetTraceStartAndDirection(StartLocation, Direction))
    {
        return false;
    }

    // 计算射线终点
    FVector EndLocation = StartLocation + (Direction * InteractionDistance);

    // 执行射线检测
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(CachedPlayerController->GetPawn());

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        OutHitResult,
        StartLocation,
        EndLocation,
        InteractionTraceChannel,
        QueryParams
    );

    // 调试绘制
    if (bDrawDebugTrace)
    {
        FColor TraceColor = bHit ? FColor::Green : FColor::Red;
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, TraceColor, false, 0.0f, 0, 1.0f);
        
        if (bHit)
        {
            DrawDebugSphere(GetWorld(), OutHitResult.ImpactPoint, 10.0f, 12, FColor::Green, false, 0.0f);
        }
    }

    return bHit;
}

bool UInteractionComponent::IsActorInteractable(AActor* Actor) const
{
    // TODO: 实现接口检查逻辑
    // 检查Actor是否实现了IInteractable接口
    
    if (!Actor)
    {
        return false;
    }

    return Actor->GetClass()->ImplementsInterface(UInteractable::StaticClass());
}

void UInteractionComponent::UpdateFocusedActor(AActor* NewFocusedActor)
{
    // TODO: 实现聚焦状态更新逻辑
    // 1. 检查聚焦对象是否发生改变
    // 2. 如果改变,调用旧对象的OnEndFocus()
    // 3. 调用新对象的OnBeginFocus()
    // 4. 更新FocusedActor变量
    // 5. 广播OnFocusChanged事件
    
    if (FocusedActor == NewFocusedActor)
    {
        // 聚焦对象未改变,无需更新
        return;
    }

    AActor* OldFocusedActor = FocusedActor;

    // 调用旧对象的OnEndFocus
    if (OldFocusedActor && IsActorInteractable(OldFocusedActor))
    {
        IInteractable::Execute_OnEndFocus(OldFocusedActor);
    }

    // 更新聚焦对象
    FocusedActor = NewFocusedActor;

    // 调用新对象的OnBeginFocus
    if (FocusedActor && IsActorInteractable(FocusedActor))
    {
        IInteractable::Execute_OnBeginFocus(FocusedActor);
    }

    // 广播聚焦改变事件
    OnFocusChanged.Broadcast(FocusedActor, OldFocusedActor);
}

bool UInteractionComponent::GetTraceStartAndDirection(FVector& OutStartLocation, FVector& OutDirection) const
{
    // TODO: 实现射线起点和方向计算逻辑
    // 根据TraceMode的不同,使用不同的方法获取射线参数
    // - ScreenCenter: 使用DeprojectScreenPositionToWorld,传入屏幕中心坐标
    // - MouseCursor: 使用GetMousePosition和DeprojectScreenPositionToWorld
    // - TouchLocation: 使用GetInputTouchState和DeprojectScreenPositionToWorld
    
    if (!CachedPlayerController)
    {
        return false;
    }

    int32 ViewportSizeX, ViewportSizeY;
    CachedPlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

    float ScreenX = 0.0f;
    float ScreenY = 0.0f;

    switch (TraceMode)
    {
        case EInteractionTraceMode::ScreenCenter:
        {
            // 使用屏幕中心
            ScreenX = ViewportSizeX * 0.5f;
            ScreenY = ViewportSizeY * 0.5f;
            break;
        }
        case EInteractionTraceMode::MouseCursor:
        {
            // 使用鼠标位置
            CachedPlayerController->GetMousePosition(ScreenX, ScreenY);
            break;
        }
        case EInteractionTraceMode::TouchLocation:
        {
            // 使用触摸位置
            float TouchX, TouchY;
            bool bIsTouching = false;
            CachedPlayerController->GetInputTouchState(ETouchIndex::Touch1, TouchX, TouchY, bIsTouching);
            
            if (bIsTouching)
            {
                ScreenX = TouchX;
                ScreenY = TouchY;
            }
            else
            {
                // 如果没有触摸输入,使用屏幕中心
                ScreenX = ViewportSizeX * 0.5f;
                ScreenY = ViewportSizeY * 0.5f;
            }
            break;
        }
    }

    // 将屏幕坐标转换为世界坐标
    FVector WorldLocation;
    FVector WorldDirection;
    bool bSuccess = CachedPlayerController->DeprojectScreenPositionToWorld(
        ScreenX,
        ScreenY,
        WorldLocation,
        WorldDirection
    );

    if (bSuccess)
    {
        OutStartLocation = WorldLocation;
        OutDirection = WorldDirection;
    }

    return bSuccess;
}
