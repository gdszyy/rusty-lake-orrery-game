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

    // 每帧执行交互检测
    PerformInteractionTrace();
}

FText UInteractionComponent::GetCurrentPrompt() const
{
    if (CurrentInteractableComponent && CurrentInteractableComponent->CanInteract())
    {
        return CurrentInteractableComponent->GetPrompt();
    }
    
    return FText::GetEmpty();
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

    // 获取屏幕中心位置（2D游戏通常使用屏幕中心或鼠标位置）
    int32 ViewportSizeX, ViewportSizeY;
    CachedPlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

    // 使用鼠标位置进行射线检测（2D点击游戏的标准做法）
    float MouseX, MouseY;
    CachedPlayerController->GetMousePosition(MouseX, MouseY);

    // 将屏幕坐标转换为世界射线
    FVector WorldLocation, WorldDirection;
    bool bSuccess = CachedPlayerController->DeprojectScreenPositionToWorld(
        MouseX,
        MouseY,
        WorldLocation,
        WorldDirection
    );

    if (!bSuccess)
    {
        return;
    }

    // 计算射线终点
    FVector TraceEnd = WorldLocation + (WorldDirection * InteractionDistance);

    // 执行射线检测
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(CachedPlayerController->GetPawn());

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
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
            DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 12, FColor::Green, false, 0.0f);
        }
    }

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

void UInteractionComponent::HandleInteractionInput()
{
    if (!CurrentInteractableComponent || !CurrentFocusedActor)
    {
        return;
    }

    if (!CurrentInteractableComponent->CanInteract())
    {
        return;
    }

    // 执行交互
    CurrentInteractableComponent->ExecuteInteraction(CachedPlayerController);
    
    UE_LOG(LogTemp, Log, TEXT("InteractionComponent: Interaction executed on %s"), *CurrentFocusedActor->GetName());
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
            UE_LOG(LogTemp, Verbose, TEXT("InteractionComponent: Focused on %s"), *CurrentFocusedActor->GetName());
        }
    }
}

void UInteractionComponent::BindInputActions()
{
    if (bInputBound || !CachedPlayerController)
    {
        return;
    }

    // 绑定鼠标左键点击到交互函数
    // 注意：这里使用InputComponent绑定，需要在PlayerController的SetupInputComponent中配置
    // 或者使用增强输入系统（Enhanced Input System）
    
    // TODO: 根据项目的输入系统进行绑定
    // 方案1：传统输入系统
    // CachedPlayerController->InputComponent->BindAction("Interact", IE_Pressed, this, &UInteractionComponent::HandleInteractionInput);
    
    // 方案2：在PlayerController的蓝图中手动绑定
    // 在BP_PlayerController的事件图表中：
    // Event Left Mouse Button -> Call HandleInteractionInput on InteractionComponent
    
    // 临时方案：使用鼠标左键的原始绑定
    if (CachedPlayerController->InputComponent)
    {
        CachedPlayerController->InputComponent->BindAction(
            "Interact",
            IE_Pressed,
            this,
            &UInteractionComponent::HandleInteractionInput
        );
        
        bInputBound = true;
        UE_LOG(LogTemp, Log, TEXT("InteractionComponent: Input binding successful"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("InteractionComponent: InputComponent not available, input binding failed"));
    }
}
