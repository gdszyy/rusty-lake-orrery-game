// RotationPuzzle.cpp

#include "RotationPuzzle.h"
#include "Components/SceneComponent.h"

ARotationPuzzle::ARotationPuzzle()
{
    PrimaryActorTick.bCanEverTick = true;

    TargetRotation = 0.0f;
    AngleTolerance = 5.0f;
    HoldTime = 0.5f;
    CurrentRotation = 0.0f;
    HoldTimer = 0.0f;
    bIsAtCorrectAngle = false;

    // 创建根组件
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);
}

void ARotationPuzzle::BeginPlay()
{
    Super::BeginPlay();

    // 如果没有指定可旋转组件,使用根组件
    if (!RotatableComponent)
    {
        RotatableComponent = GetRootComponent();
    }

    // 获取初始旋转
    if (RotatableComponent)
    {
        CurrentRotation = RotatableComponent->GetComponentRotation().Yaw;
    }

    UE_LOG(LogTemp, Log, TEXT("RotationPuzzle: Initialized with target %.2f, tolerance %.2f"), 
           TargetRotation, AngleTolerance);
}

void ARotationPuzzle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 只在激活状态下检查
    if (!IsActive())
    {
        return;
    }

    UpdateRotationState();

    // 如果在正确角度,增加计时器
    if (bIsAtCorrectAngle)
    {
        HoldTimer += DeltaTime;

        // 如果保持足够长时间,完成谜题
        if (HoldTimer >= HoldTime)
        {
            CompletePuzzle();
        }
    }
    else
    {
        // 重置计时器
        HoldTimer = 0.0f;
    }
}

void ARotationPuzzle::SetRotation(float NewRotation)
{
    if (!RotatableComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("RotationPuzzle: No rotatable component set"));
        return;
    }

    CurrentRotation = NormalizeAngle(NewRotation);

    // 应用旋转
    FRotator NewRotator = RotatableComponent->GetComponentRotation();
    NewRotator.Yaw = CurrentRotation;
    RotatableComponent->SetWorldRotation(NewRotator);

    // 触发旋转改变事件
    OnRotationChanged.Broadcast(CurrentRotation, TargetRotation);

    UE_LOG(LogTemp, Verbose, TEXT("RotationPuzzle: Rotation set to %.2f"), CurrentRotation);
}

void ARotationPuzzle::AddRotation(float DeltaRotation)
{
    SetRotation(CurrentRotation + DeltaRotation);
}

bool ARotationPuzzle::IsAtCorrectAngle() const
{
    return bIsAtCorrectAngle;
}

float ARotationPuzzle::GetAngleDifference() const
{
    float Diff = FMath::Abs(NormalizeAngle(CurrentRotation) - NormalizeAngle(TargetRotation));
    
    // 处理跨越0度的情况
    if (Diff > 180.0f)
    {
        Diff = 360.0f - Diff;
    }
    
    return Diff;
}

float ARotationPuzzle::GetProgress_Implementation() const
{
    if (IsCompleted())
    {
        return 1.0f;
    }

    if (!IsActive())
    {
        return 0.0f;
    }

    // 根据角度差计算进度
    float AngleDiff = GetAngleDifference();
    float MaxDiff = 180.0f; // 最大可能的角度差
    float Progress = 1.0f - (AngleDiff / MaxDiff);

    // 如果在正确角度,根据保持时间增加进度
    if (bIsAtCorrectAngle && HoldTime > 0.0f)
    {
        float HoldProgress = HoldTimer / HoldTime;
        Progress = FMath::Lerp(0.9f, 1.0f, HoldProgress);
    }

    return FMath::Clamp(Progress, 0.0f, 1.0f);
}

void ARotationPuzzle::OnPuzzleActivatedEvent_Implementation()
{
    Super::OnPuzzleActivatedEvent_Implementation();

    // 启用Tick
    SetActorTickEnabled(true);

    UE_LOG(LogTemp, Log, TEXT("RotationPuzzle: Activated"));
}

void ARotationPuzzle::OnPuzzleResetEvent_Implementation()
{
    Super::OnPuzzleResetEvent_Implementation();

    // 重置状态
    HoldTimer = 0.0f;
    bIsAtCorrectAngle = false;

    // 重置旋转到初始值(可选)
    // SetRotation(0.0f);

    UE_LOG(LogTemp, Log, TEXT("RotationPuzzle: Reset"));
}

void ARotationPuzzle::UpdateRotationState()
{
    // 获取当前旋转
    if (RotatableComponent)
    {
        CurrentRotation = RotatableComponent->GetComponentRotation().Yaw;
    }

    // 检查是否在正确角度
    float AngleDiff = GetAngleDifference();
    bool bWasAtCorrectAngle = bIsAtCorrectAngle;
    bIsAtCorrectAngle = (AngleDiff <= AngleTolerance);

    // 如果刚到达正确角度,触发事件
    if (bIsAtCorrectAngle && !bWasAtCorrectAngle)
    {
        OnCorrectAngleReached.Broadcast(CurrentRotation);
        UE_LOG(LogTemp, Log, TEXT("RotationPuzzle: Reached correct angle (diff: %.2f)"), AngleDiff);
    }
}

float ARotationPuzzle::NormalizeAngle(float Angle) const
{
    // 标准化到0-360范围
    Angle = FMath::Fmod(Angle, 360.0f);
    if (Angle < 0.0f)
    {
        Angle += 360.0f;
    }
    return Angle;
}
