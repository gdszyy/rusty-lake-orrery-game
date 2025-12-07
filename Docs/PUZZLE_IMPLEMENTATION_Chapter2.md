# 第二章蓝图实现指南

**章节**: 第二章 - 植物、镜像与成长  
**引擎**: Unreal Engine 5  
**开发时间**: 5.5小时  
**最后更新**: 2025-12-07

---

## 📋 目录

1. [场景管理器](#场景管理器)
2. [温室区域对象](#温室区域对象)
3. [镜像区域对象](#镜像区域对象)
4. [特殊系统](#特殊系统)
5. [UI系统](#ui系统)
6. [性能优化](#性能优化)
7. [测试清单](#测试清单)

---

## 🎮 场景管理器

### BP_Museum_Hall2_Manager

**功能**: 管理第二展厅的所有状态、事件和流程

#### 组件结构

```
BP_Museum_Hall2_Manager
├── SceneRoot (DefaultSceneRoot)
├── CameraManager (CameraComponent)
├── AudioManager (AudioComponent)
└── LightingManager (LightComponent)
```

#### 变量定义

| 变量名 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| CurrentState | EHall2State | Idle | 当前场景状态 |
| PlantsBloomedCount | Integer | 0 | 已开花的植物数量 |
| IsSunflowerBloomed | Boolean | false | 向日葵是否开花 |
| IsWaterLilyBloomed | Boolean | false | 睡莲是否开花 |
| IsMoonflowerBloomed | Boolean | false | 月见草是否开花 |
| IsRoseBloomed | Boolean | false | 蔷薇是否开花 |
| IsMirrorPuzzleComplete | Boolean | false | 镜像谜题是否完成 |
| HasRedFlower | Boolean | false | 是否获得红色花朵 |
| HasBlueFlower | Boolean | false | 是否获得蓝色花朵 |
| HasPresentSymbol | Boolean | false | 是否获得现在之符 |
| RoseHintTimer | Float | 0.0 | 蔷薇提示计时器 |
| PlayerInGreenhouseArea | Boolean | true | 玩家是否在温室区域 |

#### 状态枚举 (EHall2State)

```cpp
enum class EHall2State : uint8
{
    Idle,                      // 空闲
    PlantingPhase,             // 种植阶段
    WaitingForRose,            // 等待蔷薇
    RoseHintShown,             // 蔷薇提示已显示
    MirrorPuzzleActive,        // 镜像谜题激活
    MirrorPuzzleComplete,      // 镜像谜题完成
    RoseBloomed,               // 蔷薇盛开
    FourFlowersBloomed,        // 四花齐放
    ButterflyActive,           // 蝴蝶激活
    TimeWebForming,            // 时间之网形成中
    SymbolFusing,              // 符号融合中
    ChapterComplete            // 章节完成
};
```

#### 核心事件

```cpp
// 事件声明
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlantBloomed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFourFlowersBloomed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMirrorPuzzleComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoseBloomed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPresentSymbolObtained);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChapterComplete);
```

#### 核心函数

**BeginPlay()**
```cpp
void BeginPlay()
{
    Super::BeginPlay();
    
    // 初始化状态
    CurrentState = EHall2State::Idle;
    
    // 播放开场独白
    PlayDialogue("DLG_C2_001");
    
    // 播放背景音乐
    PlayBGM("BGM_Hall2_Main");
    
    // 设置镜头
    SetupCamera();
    
    // 注册事件监听
    RegisterEventListeners();
}
```

**Tick(DeltaTime)**
```cpp
void Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 蔷薇提示计时器
    if (CurrentState == EHall2State::WaitingForRose)
    {
        RoseHintTimer += DeltaTime;
        
        if (RoseHintTimer >= 30.0f && CurrentState != EHall2State::RoseHintShown)
        {
            ShowRoseHint();
            CurrentState = EHall2State::RoseHintShown;
        }
    }
    
    // 检查玩家位置
    CheckPlayerArea();
}
```

**OnPlantBloomed(PlantType)**
```cpp
void OnPlantBloomed(EPlantType PlantType)
{
    // 增加计数
    PlantsBloomedCount++;
    
    // 更新对应植物状态
    switch(PlantType)
    {
        case EPlantType::Sunflower:
            IsSunflowerBloomed = true;
            PlayDialogue("DLG_C2_004");
            break;
        case EPlantType::WaterLily:
            IsWaterLilyBloomed = true;
            PlayDialogue("DLG_C2_005");
            break;
        case EPlantType::Moonflower:
            IsMoonflowerBloomed = true;
            PlayDialogue("DLG_C2_006");
            break;
        case EPlantType::Rose:
            IsRoseBloomed = true;
            TriggerAnjiMemory();
            break;
    }
    
    // 检查是否前三种都开花
    if (PlantsBloomedCount == 3 && !IsRoseBloomed)
    {
        CurrentState = EHall2State::WaitingForRose;
        RoseHintTimer = 0.0f;
    }
    
    // 检查是否四花齐放
    if (PlantsBloomedCount == 4)
    {
        TriggerFourFlowersBloomed();
    }
}
```

**TriggerFourFlowersBloomed()**
```cpp
void TriggerFourFlowersBloomed()
{
    CurrentState = EHall2State::FourFlowersBloomed;
    
    // 播放独白
    PlayDialogue("DLG_C2_010");
    
    // 触发四花齐放动画
    PlayFourFlowersAnimation();
    
    // 延迟后生成记忆碎片
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &AMuseumHall2Manager::SpawnMemoryCrystal, 3.0f, false);
    
    // 延迟后触发蝴蝶
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &AMuseumHall2Manager::SpawnButterfly, 5.0f, false);
}
```

**OnMirrorPuzzleComplete()**
```cpp
void OnMirrorPuzzleComplete()
{
    IsMirrorPuzzleComplete = true;
    CurrentState = EHall2State::MirrorPuzzleComplete;
    
    // 获得红色花朵
    HasRedFlower = true;
    PlayDialogue("DLG_C2_016");
    
    // 添加到背包
    AddItemToInventory("ITEM_C2_009");
    
    // 保存进度
    SaveProgress("MirrorPuzzleComplete");
}
```

**CheckPlayerArea()**
```cpp
void CheckPlayerArea()
{
    // 获取玩家位置
    FVector PlayerLocation = GetPlayerLocation();
    
    // 判断玩家在哪个区域
    bool WasInGreenhouse = PlayerInGreenhouseArea;
    PlayerInGreenhouseArea = (PlayerLocation.Y < 0); // 假设Y<0是温室
    
    // 如果从温室进入镜像区域
    if (WasInGreenhouse && !PlayerInGreenhouseArea)
    {
        OnEnterMirrorArea();
    }
    
    // 如果从镜像回到温室
    if (!WasInGreenhouse && PlayerInGreenhouseArea && IsMirrorPuzzleComplete && !IsRoseBloomed)
    {
        OnReturnToGreenhouse();
    }
}
```

**OnReturnToGreenhouse()**
```cpp
void OnReturnToGreenhouse()
{
    // 蔷薇自动盛开
    if (BP_Rose)
    {
        BP_Rose->BloomAutomatically();
    }
}
```

---

## 🌱 温室区域对象

### 1. BP_FlowerPot（花盆基类）

**功能**: 可种植植物的花盆

#### 组件结构

```
BP_FlowerPot
├── StaticMesh (FlowerPotMesh)
├── PlantSpawnPoint (SceneComponent)
├── InteractionBox (BoxCollision)
└── ParticleSystem (GrowthEffect)
```

#### 变量定义

| 变量名 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| PlantType | EPlantType | None | 花盆对应的植物类型 |
| CurrentState | EFlowerPotState | Empty | 花盆状态 |
| PlantActor | AActor* | nullptr | 当前种植的植物Actor |
| RequiredCondition | EGrowthCondition | None | 生长条件 |
| IsConditionMet | Boolean | false | 条件是否满足 |

#### 状态枚举

```cpp
enum class EFlowerPotState : uint8
{
    Empty,          // 空
    HasSeed,        // 有种子
    Growing,        // 生长中
    Bloomed         // 已开花
};

enum class EGrowthCondition : uint8
{
    Sunlight,       // 阳光
    Water,          // 水分
    Time,           // 时间
    Separation      // 分离
};
```

#### 核心函数

**PlantSeed(SeedType)**
```cpp
void PlantSeed(EPlantType SeedType)
{
    if (CurrentState != EFlowerPotState::Empty)
        return;
    
    // 生成种子Actor
    PlantActor = SpawnPlantActor(SeedType, EPlantState::Seed);
    
    // 更新状态
    CurrentState = EFlowerPotState::HasSeed;
    PlantType = SeedType;
    
    // 播放音效
    PlaySound("SFX_C2_001");
    
    // 设置生长条件
    SetGrowthCondition(SeedType);
}
```

**CheckGrowthCondition()**
```cpp
void CheckGrowthCondition()
{
    if (CurrentState != EFlowerPotState::HasSeed)
        return;
    
    if (IsConditionMet)
    {
        StartGrowth();
    }
}
```

**StartGrowth()**
```cpp
void StartGrowth()
{
    CurrentState = EFlowerPotState::Growing;
    
    // 播放生长音效
    PlaySound("SFX_C2_005");
    
    // 播放生长动画
    if (PlantActor)
    {
        PlantActor->PlayGrowthAnimation();
    }
    
    // 播放粒子效果
    ParticleSystem->Activate();
    
    // 3秒后开花
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &AFlowerPot::OnBloom, 3.0f, false);
}
```

**OnBloom()**
```cpp
void OnBloom()
{
    CurrentState = EFlowerPotState::Bloomed;
    
    // 播放开花音效
    PlaySound("SFX_C2_006");
    
    // 植物发光
    if (PlantActor)
    {
        PlantActor->SetGlowing(true);
    }
    
    // 通知管理器
    HallManager->OnPlantBloomed(PlantType);
}
```

---

### 2. BP_Plant（植物基类）

**功能**: 植物生长动画和状态管理

#### 组件结构

```
BP_Plant
├── SkeletalMesh (PlantMesh) 或 Sprite (PlantSprite)
├── PointLight (GlowLight)
└── ParticleSystem (BloomEffect)
```

#### 变量定义

| 变量名 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| CurrentFrame | Integer | 0 | 当前动画帧 |
| TotalFrames | Integer | 5 | 总帧数 |
| PlantSprites | Array<UTexture2D*> | - | 5帧精灵图 |
| IsGlowing | Boolean | false | 是否发光 |

#### 核心函数

**PlayGrowthAnimation()**
```cpp
void PlayGrowthAnimation()
{
    // 使用定时器播放帧动画
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &APlant::NextFrame, 0.6f, true);
}
```

**NextFrame()**
```cpp
void NextFrame()
{
    CurrentFrame++;
    
    if (CurrentFrame >= TotalFrames)
    {
        // 动画结束
        GetWorld()->GetTimerManager().ClearTimer(GrowthTimerHandle);
        OnGrowthComplete();
        return;
    }
    
    // 切换到下一帧
    if (PlantSprite)
    {
        PlantSprite->SetSprite(PlantSprites[CurrentFrame]);
    }
}
```

**SetGlowing(bool bGlow)**
```cpp
void SetGlowing(bool bGlow)
{
    IsGlowing = bGlow;
    
    if (bGlow)
    {
        GlowLight->SetIntensity(2.0f);
        BloomEffect->Activate();
    }
    else
    {
        GlowLight->SetIntensity(0.0f);
        BloomEffect->Deactivate();
    }
}
```

---

### 3. BP_Sunflower（向日葵）

**继承**: BP_Plant

#### 特殊变量

| 变量名 | 类型 | 说明 |
|--------|------|------|
| IsSunlightActive | Boolean | 天窗是否打开 |

#### 特殊函数

**OnSunlightActivated()**
```cpp
void OnSunlightActivated()
{
    IsSunlightActive = true;
    
    // 通知花盆条件满足
    OwnerPot->SetConditionMet(true);
}
```

---

### 4. BP_WaterLily（睡莲）

**继承**: BP_Plant

#### 特殊函数

**OnWatered()**
```cpp
void OnWatered()
{
    // 播放水波纹效果
    PlayWaterRipple();
    
    // 通知花盆条件满足
    OwnerPot->SetConditionMet(true);
}
```

---

### 5. BP_Moonflower（月见草）

**继承**: BP_Plant

#### 特殊变量

| 变量名 | 类型 | 说明 |
|--------|------|------|
| RequiredTimeOfDay | ETimeOfDay | 需要的时间（夜晚） |

#### 特殊函数

**OnTimeChanged(NewTime)**
```cpp
void OnTimeChanged(ETimeOfDay NewTime)
{
    if (NewTime == ETimeOfDay::Night)
    {
        // 通知花盆条件满足
        OwnerPot->SetConditionMet(true);
    }
}
```

---

### 6. BP_Rose（蔷薇）

**继承**: BP_Plant

#### 特殊变量

| 变量名 | 类型 | 说明 |
|--------|------|------|
| RequiresSeparation | Boolean | 需要玩家离开 |
| CanBloomAutomatically | Boolean | 可以自动盛开 |

#### 特殊函数

**CheckSeparationCondition()**
```cpp
void CheckSeparationCondition()
{
    // 检查玩家是否在镜像区域
    if (!HallManager->PlayerInGreenhouseArea && HallManager->IsMirrorPuzzleComplete)
    {
        CanBloomAutomatically = true;
    }
}
```

**BloomAutomatically()**
```cpp
void BloomAutomatically()
{
    if (!CanBloomAutomatically)
        return;
    
    // 自动开花
    OwnerPot->SetConditionMet(true);
    OwnerPot->StartGrowth();
}
```

---

### 7. BP_Skylight（天窗）

**功能**: 可开关的天窗，提供阳光

#### 组件结构

```
BP_Skylight
├── StaticMesh (SkylightFrame)
├── StaticMesh (SkylightGlass)
├── DirectionalLight (Sunlight)
├── InteractionBox (BoxCollision)
└── AudioComponent (OpenSound)
```

#### 变量定义

| 变量名 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| IsOpen | Boolean | false | 是否打开 |
| OpenAngle | Float | 45.0 | 打开角度 |

#### 核心函数

**OnInteract()**
```cpp
void OnInteract()
{
    if (IsOpen)
        return;
    
    // 打开天窗
    IsOpen = true;
    
    // 播放动画
    PlayOpenAnimation();
    
    // 播放音效
    PlaySound("SFX_C2_002");
    
    // 激活阳光
    Sunlight->SetIntensity(5.0f);
    
    // 通知向日葵
    NotifySunflower();
}
```

**PlayOpenAnimation()**
```cpp
void PlayOpenAnimation()
{
    // 使用Timeline旋转天窗
    FTimeline OpenTimeline;
    OpenTimeline.AddInterpFloat(OpenCurve, FOnTimelineFloat::CreateUObject(this, &ASkylight::UpdateRotation));
    OpenTimeline.Play();
}
```

---

### 8. BP_WateringCan（浇水壶）

**功能**: 可拾取的浇水壶，用于浇水

#### 组件结构

```
BP_WateringCan
├── StaticMesh (CanMesh)
├── InteractionBox (BoxCollision)
└── ParticleSystem (WaterEffect)
```

#### 变量定义

| 变量名 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| IsPickedUp | Boolean | false | 是否被拾取 |
| IsInUse | Boolean | false | 是否正在使用 |

#### 核心函数

**OnInteract()**
```cpp
void OnInteract()
{
    if (!IsPickedUp)
    {
        // 拾取
        PickUp();
    }
    else
    {
        // 使用（浇水）
        UseOnTarget();
    }
}
```

**UseOnTarget()**
```cpp
void UseOnTarget()
{
    // 射线检测目标
    FHitResult HitResult;
    if (LineTrace(HitResult))
    {
        // 检查是否是花盆
        AFlowerPot* Pot = Cast<AFlowerPot>(HitResult.GetActor());
        if (Pot && Pot->PlantType == EPlantType::WaterLily)
        {
            // 浇水
            Pot->SetConditionMet(true);
            
            // 播放音效和粒子
            PlaySound("SFX_C2_003");
            WaterEffect->Activate();
        }
    }
}
```

---

### 9. BP_Clock（时钟）

**功能**: 可调整时间的时钟

#### 组件结构

```
BP_Clock
├── StaticMesh (ClockBody)
├── StaticMesh (HourHand)
├── StaticMesh (MinuteHand)
├── InteractionBox (BoxCollision)
└── AudioComponent (TickSound)
```

#### 变量定义

| 变量名 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| CurrentTime | Float | 12.0 | 当前时间（小时） |
| TimeOfDay | ETimeOfDay | Day | 时间段 |
| IsAdjusting | Boolean | false | 是否正在调整 |

#### 核心函数

**OnInteract()**
```cpp
void OnInteract()
{
    // 打开时钟UI
    ShowClockUI();
}
```

**AdjustTime(NewTime)**
```cpp
void AdjustTime(float NewTime)
{
    CurrentTime = NewTime;
    
    // 更新指针旋转
    UpdateHands();
    
    // 更新时间段
    UpdateTimeOfDay();
    
    // 播放滴答声
    PlaySound("SFX_C2_004");
    
    // 通知月见草
    NotifyMoonflower();
}
```

**UpdateTimeOfDay()**
```cpp
void UpdateTimeOfDay()
{
    if (CurrentTime >= 6.0f && CurrentTime < 18.0f)
    {
        TimeOfDay = ETimeOfDay::Day;
        UpdateLighting(FLinearColor(1.0f, 0.9f, 0.7f)); // 暖黄色
    }
    else if (CurrentTime >= 18.0f && CurrentTime < 20.0f)
    {
        TimeOfDay = ETimeOfDay::Dusk;
        UpdateLighting(FLinearColor(1.0f, 0.5f, 0.3f)); // 橙红色
    }
    else
    {
        TimeOfDay = ETimeOfDay::Night;
        UpdateLighting(FLinearColor(0.3f, 0.3f, 0.5f)); // 深蓝色
    }
}
```

---

### 10. BP_AnjiDiary（安吉日记）

**功能**: 可阅读的日记，提供线索

#### 组件结构

```
BP_AnjiDiary
├── StaticMesh (DiaryMesh)
├── InteractionBox (BoxCollision)
└── TextRender (Highlight)
```

#### 变量定义

| 变量名 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| HasBeenRead | Boolean | false | 是否已阅读 |
| ShouldHighlight | Boolean | false | 是否高亮关键词 |

#### 核心函数

**OnInteract()**
```cpp
void OnInteract()
{
    // 显示日记UI
    ShowDiaryUI();
    
    // 播放独白
    PlayDialogue("DLG_C2_003");
    
    // 标记已读
    HasBeenRead = true;
    
    // 如果是第二次阅读（蔷薇提示后）
    if (ShouldHighlight)
    {
        HighlightKeywords();
    }
}
```

**HighlightKeywords()**
```cpp
void HighlightKeywords()
{
    // 高亮"分离"和"空间"
    HighlightText("分离", FLinearColor::Yellow);
    HighlightText("空间", FLinearColor::Yellow);
    
    // 短暂切向镜像区域
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &AAnjiDiary::PanCameraToMirror, 2.0f, false);
}
```

---

## 🪞 镜像区域对象

### 1. BP_BigMirror（大镜子）

**功能**: 可破碎的镜子，镜像人偶

#### 组件结构

```
BP_BigMirror
├── StaticMesh (MirrorFrame)
├── StaticMesh (MirrorGlass)
├── SceneCaptureComponent2D (MirrorCapture)
├── ParticleSystem (CrackEffect)
└── ParticleSystem (ShatterEffect)
```

#### 变量定义

| 变量名 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| IsCracked | Boolean | false | 是否有裂缝 |
| IsShattered | Boolean | false | 是否破碎 |
| CrackProgress | Float | 0.0 | 裂缝进度（0-1） |

#### 核心函数

**OnMechanismComplete(MechanismID)**
```cpp
void OnMechanismComplete(int32 MechanismID)
{
    CrackProgress += 0.5f; // 每个机关增加50%
    
    // 显示裂缝
    ShowCrack(CrackProgress);
    
    if (CrackProgress >= 1.0f)
    {
        // 破碎
        Shatter();
    }
}
```

**ShowCrack(Progress)**
```cpp
void ShowCrack(float Progress)
{
    // 更新裂缝材质
    MirrorGlass->SetScalarParameterValueOnMaterials("CrackAmount", Progress);
    
    // 播放裂缝效果
    CrackEffect->Activate();
    
    // 裂缝发光
    if (Progress >= 0.5f)
    {
        MirrorGlass->SetVectorParameterValueOnMaterials("CrackColor", FLinearColor::Yellow);
    }
}
```

**Shatter()**
```cpp
void Shatter()
{
    IsShattered = true;
    
    // 播放破碎音效
    PlaySound("SFX_C2_008");
    
    // 播放破碎动画
    PlayShatterAnimation();
    
    // 生成破碎粒子
    ShatterEffect->Activate();
    
    // 延迟后触发人偶融合
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &ABigMirror::TriggerPuppetFusion, 2.0f, false);
}
```

**TriggerPuppetFusion()**
```cpp
void TriggerPuppetFusion()
{
    // 通知人偶走出镜子
    if (RedPuppet && BluePuppet)
    {
        RedPuppet->WalkOut();
        BluePuppet->WalkOut();
    }
}
```

---

### 2. BP_Puppet（人偶基类）

**功能**: 可移动的镜像人偶

#### 组件结构

```
BP_Puppet
├── SkeletalMesh (PuppetMesh)
├── InteractionBox (BoxCollision)
├── PointLight (GlowLight)
└── ParticleSystem (TrailEffect)
```

#### 变量定义

| 变量名 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| PuppetColor | EPuppetColor | Red | 人偶颜色 |
| CurrentPosition | FVector | - | 当前位置 |
| TargetPosition | FVector | - | 目标位置 |
| IsMoving | Boolean | false | 是否移动中 |
| MirrorPuppet | ABP_Puppet* | nullptr | 镜像人偶引用 |

#### 核心函数

**OnInteract()**
```cpp
void OnInteract()
{
    // 高亮可移动
    SetGlowing(true);
}
```

**MoveTo(TargetPos)**
```cpp
void MoveTo(FVector TargetPos)
{
    TargetPosition = TargetPos;
    IsMoving = true;
    
    // 播放移动动画
    PlayMoveAnimation();
    
    // 显示轨迹
    TrailEffect->Activate();
    
    // 镜像人偶同步移动
    if (MirrorPuppet)
    {
        FVector MirroredPos = GetMirroredPosition(TargetPos);
        MirrorPuppet->MoveToMirrored(MirroredPos);
    }
}
```

**GetMirroredPosition(OriginalPos)**
```cpp
FVector GetMirroredPosition(FVector OriginalPos)
{
    // 以镜子中线为轴镜像
    FVector MirroredPos = OriginalPos;
    MirroredPos.Y = -OriginalPos.Y; // 假设Y轴为镜像轴
    return MirroredPos;
}
```

**WalkOut()**
```cpp
void WalkOut()
{
    // 从镜子中走出
    FVector OutPosition = GetOutPosition();
    MoveTo(OutPosition);
    
    // 到达后触发相遇
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &APuppet::OnMeet, 2.0f, false);
}
```

**OnMeet()**
```cpp
void OnMeet()
{
    if (PuppetColor == EPuppetColor::Red)
    {
        // 红色人偶触发拥抱
        TriggerHug();
    }
}
```

**TriggerHug()**
```cpp
void TriggerHug()
{
    // 播放拥抱动画
    PlayHugAnimation();
    
    // 延迟后融合
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &APuppet::Fuse, 1.0f, false);
}
```

**Fuse()**
```cpp
void Fuse()
{
    // 生成紫色人偶
    APuppet* PurplePuppet = SpawnPurplePuppet();
    
    // 播放融合音效
    PlaySound("SFX_C2_009");
    
    // 播放独白
    PlayDialogue("DLG_C2_015");
    
    // 隐藏红蓝人偶
    SetActorHiddenInGame(true);
    MirrorPuppet->SetActorHiddenInGame(true);
    
    // 2秒后分离
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &APuppet::Separate, 2.0f, false);
}
```

**Separate()**
```cpp
void Separate()
{
    // 销毁紫色人偶
    PurplePuppet->Destroy();
    
    // 显示红蓝人偶（手牵手）
    SetActorHiddenInGame(false);
    MirrorPuppet->SetActorHiddenInGame(false);
    
    // 播放手牵手动画
    PlayHandInHandAnimation();
    
    // 生成红色花朵
    SpawnRedFlower();
}
```

---

### 3. BP_SyncButton（同步按钮）

**功能**: 需要两个人偶同时按下的按钮

#### 组件结构

```
BP_SyncButton
├── StaticMesh (ButtonMesh)
├── InteractionBox (TriggerBox)
└── PointLight (IndicatorLight)
```

#### 变量定义

| 变量名 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| ButtonColor | EButtonColor | Red | 按钮颜色 |
| IsPressed | Boolean | false | 是否按下 |
| PairedButton | ABP_SyncButton* | nullptr | 配对按钮 |

#### 核心函数

**OnPuppetEnter(Puppet)**
```cpp
void OnPuppetEnter(APuppet* Puppet)
{
    // 检查颜色匹配
    if (Puppet->PuppetColor == ButtonColor)
    {
        Press();
    }
}
```

**Press()**
```cpp
void Press()
{
    IsPressed = true;
    
    // 播放音效
    PlaySound("SFX_C2_007");
    
    // 按钮下沉
    PlayPressAnimation();
    
    // 指示灯亮
    IndicatorLight->SetIntensity(5.0f);
    
    // 检查配对按钮
    CheckPairedButton();
}
```

**CheckPairedButton()**
```cpp
void CheckPairedButton()
{
    if (PairedButton && PairedButton->IsPressed)
    {
        // 两个按钮都按下
        OnBothPressed();
    }
}
```

**OnBothPressed()**
```cpp
void OnBothPressed()
{
    // 播放独白
    PlayDialogue("DLG_C2_013");
    
    // 通知镜子
    BigMirror->OnMechanismComplete(1);
    
    // 通知管理器
    HallManager->OnSyncButtonComplete();
}
```

---

### 4. BP_SeparatePath（分离路径）

**功能**: 两条不同的路径，最终汇合

#### 组件结构

```
BP_SeparatePath
├── SplineComponent (UpperPath)
├── SplineComponent (LowerPath)
├── SceneComponent (MeetPoint)
└── ParticleSystem (PathGlow)
```

#### 变量定义

| 变量名 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| IsUpperPathComplete | Boolean | false | 上路径是否完成 |
| IsLowerPathComplete | Boolean | false | 下路径是否完成 |

#### 核心函数

**OnPuppetClickPath(Puppet, PathType)**
```cpp
void OnPuppetClickPath(APuppet* Puppet, EPathType PathType)
{
    if (PathType == EPathType::Upper && Puppet->PuppetColor == EPuppetColor::Red)
    {
        // 红色人偶走上路
        Puppet->MoveAlongPath(UpperPath);
        IsUpperPathComplete = true;
    }
    else if (PathType == EPathType::Lower && Puppet->PuppetColor == EPuppetColor::Blue)
    {
        // 蓝色人偶走下路
        Puppet->MoveAlongPath(LowerPath);
        IsLowerPathComplete = true;
    }
    
    // 检查是否都完成
    CheckBothComplete();
}
```

**CheckBothComplete()**
```cpp
void CheckBothComplete()
{
    if (IsUpperPathComplete && IsLowerPathComplete)
    {
        // 播放独白
        PlayDialogue("DLG_C2_014");
        
        // 通知镜子
        BigMirror->OnMechanismComplete(2);
        
        // 通知管理器
        HallManager->OnSeparatePathComplete();
    }
}
```

---

### 5. BP_ItTakesTwo_Stele（双人成形碑文）

**功能**: 显示碑文内容

#### 核心函数

**OnInteract()**
```cpp
void OnInteract()
{
    // 显示碑文UI
    ShowSteleUI();
    
    // 播放独白
    PlayDialogue("DLG_C2_012");
    
    // 高亮关键词
    HighlightKeywords();
}
```

---

## 🦋 特殊系统

### 1. BP_Butterfly（蝴蝶系统）

**功能**: 自动访问四朵花和人偶，形成时间之网

#### 组件结构

```
BP_Butterfly
├── SkeletalMesh (ButterflyMesh)
├── SplineComponent (FlightPath)
├── ParticleSystem (TrailEffect)
└── AudioComponent (FlutterSound)
```

#### 变量定义

| 变量名 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| VisitTargets | Array<AActor*> | - | 访问目标列表 |
| CurrentTargetIndex | Integer | 0 | 当前目标索引 |
| IsFlying | Boolean | false | 是否飞行中 |
| FlightSpeed | Float | 300.0 | 飞行速度 |

#### 核心函数

**BeginPlay()**
```cpp
void BeginPlay()
{
    Super::BeginPlay();
    
    // 设置访问目标
    VisitTargets.Add(Sunflower);
    VisitTargets.Add(WaterLily);
    VisitTargets.Add(Moonflower);
    VisitTargets.Add(Rose);
    VisitTargets.Add(RedPuppet);
    VisitTargets.Add(BluePuppet);
    
    // 开始飞行
    StartFlying();
}
```

**StartFlying()**
```cpp
void StartFlying()
{
    IsFlying = true;
    
    // 播放飞行音效
    FlutterSound->Play();
    
    // 飞向第一个目标
    FlyToTarget(VisitTargets[0]);
}
```

**FlyToTarget(Target)**
```cpp
void FlyToTarget(AActor* Target)
{
    // 使用贝塞尔曲线飞行
    FVector Start = GetActorLocation();
    FVector End = Target->GetActorLocation();
    FVector Control = (Start + End) / 2.0f + FVector(0, 0, 200); // 中间点抬高
    
    // 使用Timeline飞行
    FTimeline FlightTimeline;
    FlightTimeline.AddInterpFloat(FlightCurve, FOnTimelineFloat::CreateUObject(this, &AButterfly::UpdatePosition, Start, Control, End));
    FlightTimeline.Play();
    
    // 显示轨迹
    TrailEffect->Activate();
}
```

**UpdatePosition(Value, Start, Control, End)**
```cpp
void UpdatePosition(float Value, FVector Start, FVector Control, FVector End)
{
    // 贝塞尔曲线插值
    FVector Position = BezierCurve(Start, Control, End, Value);
    SetActorLocation(Position);
}
```

**OnReachTarget(Target)**
```cpp
void OnReachTarget(AActor* Target)
{
    // 停留
    Land();
    
    // 目标发光
    Target->SetGlowing(true);
    
    // 显示日期
    ShowDate(Target);
    
    // 播放简短独白
    PlayShortDialogue(CurrentTargetIndex);
    
    // 1秒后飞向下一个目标
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &AButterfly::FlyToNextTarget, 1.0f, false);
}
```

**FlyToNextTarget()**
```cpp
void FlyToNextTarget()
{
    CurrentTargetIndex++;
    
    if (CurrentTargetIndex >= VisitTargets.Num())
    {
        // 所有目标访问完成
        OnAllTargetsVisited();
        return;
    }
    
    // 飞向下一个
    FlyToTarget(VisitTargets[CurrentTargetIndex]);
}
```

**OnAllTargetsVisited()**
```cpp
void OnAllTargetsVisited()
{
    // 播放独白
    PlayDialogue("DLG_C2_022");
    
    // 形成时间之网
    FormTimeWeb();
}
```

**FormTimeWeb()**
```cpp
void FormTimeWeb()
{
    // 创建连线
    for (int i = 0; i < VisitTargets.Num(); i++)
    {
        for (int j = i + 1; j < VisitTargets.Num(); j++)
        {
            CreateLine(VisitTargets[i], VisitTargets[j]);
        }
    }
    
    // 连线发光
    ActivateWebGlow();
    
    // 网络凝固成蓝色花朵
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &AButterfly::SpawnBlueFlower, 3.0f, false);
}
```

**SpawnBlueFlower()**
```cpp
void SpawnBlueFlower()
{
    // 生成蓝色花朵
    AActor* BlueFlower = SpawnActor(BP_BlueFlower, GetActorLocation());
    
    // 添加到背包
    HallManager->AddItemToInventory("ITEM_C2_010");
    
    // 触发符号融合
    HallManager->TriggerSymbolFusion();
}
```

---

### 2. BP_FourFlowersEffect（四花齐放效果）

**功能**: 四朵花同时发光，花瓣飘落融合

#### 组件结构

```
BP_FourFlowersEffect
├── ParticleSystem (PetalFall_1)
├── ParticleSystem (PetalFall_2)
├── ParticleSystem (PetalFall_3)
├── ParticleSystem (PetalFall_4)
├── ParticleSystem (PetalFusion)
└── AudioComponent (FusionSound)
```

#### 核心函数

**Play()**
```cpp
void Play()
{
    // 四朵花同时发光
    Sunflower->SetGlowing(true);
    WaterLily->SetGlowing(true);
    Moonflower->SetGlowing(true);
    Rose->SetGlowing(true);
    
    // 花瓣飘落
    PetalFall_1->Activate();
    PetalFall_2->Activate();
    PetalFall_3->Activate();
    PetalFall_4->Activate();
    
    // 2秒后融合
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &AFourFlowersEffect::FusePetals, 2.0f, false);
}
```

**FusePetals()**
```cpp
void FusePetals()
{
    // 花瓣在中央融合
    FVector CenterPos = GetCenterPosition();
    PetalFusion->SetWorldLocation(CenterPos);
    PetalFusion->Activate();
    
    // 播放融合音效
    FusionSound->Play();
    
    // 1秒后生成水晶
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &AFourFlowersEffect::SpawnCrystal, 1.0f, false);
}
```

**SpawnCrystal()**
```cpp
void SpawnCrystal()
{
    // 生成记忆碎片
    AActor* Crystal = SpawnActor(BP_MemoryCrystal, GetCenterPosition());
    
    // 添加到背包
    HallManager->AddItemToInventory("ITEM_C2_008");
}
```

---

### 3. BP_SymbolFusion（符号融合效果）

**功能**: 红蓝花朵融合成现在之符

#### 核心函数

**Play(RedFlower, BlueFlower)**
```cpp
void Play(AActor* RedFlower, AActor* BlueFlower)
{
    // 两朵花飞向中央
    RedFlower->FlyTo(CenterPosition);
    BlueFlower->FlyTo(CenterPosition);
    
    // 1秒后旋转交织
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &ASymbolFusion::Intertwine, 1.0f, false);
}
```

**Intertwine()**
```cpp
void Intertwine()
{
    // 旋转动画
    PlayRotationAnimation();
    
    // 播放独白
    PlayDialogue("DLG_C2_023");
    
    // 2秒后融合
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &ASymbolFusion::Fuse, 2.0f, false);
}
```

**Fuse()**
```cpp
void Fuse()
{
    // 销毁红蓝花朵
    RedFlower->Destroy();
    BlueFlower->Destroy();
    
    // 生成现在之符
    AActor* PresentSymbol = SpawnActor(BP_PresentSymbol, CenterPosition);
    
    // 符号发光
    PresentSymbol->SetGlowing(true);
    
    // 播放音效
    PlaySound("SFX_C2_011");
    
    // 添加到背包
    HallManager->AddItemToInventory("ITEM_C2_011");
    HallManager->HasPresentSymbol = true;
    
    // 触发章节完成
    HallManager->OnChapterComplete();
}
```

---

### 4. BP_AnjiMemory（安吉回忆过场）

**功能**: 分屏显示安吉分离与重逢

#### 组件结构

```
BP_AnjiMemory
├── WidgetComponent (MemoryUI)
├── AudioComponent (MemoryMusic)
└── CameraComponent (MemoryCamera)
```

#### 核心函数

**Play()**
```cpp
void Play()
{
    // 淡出当前场景
    FadeOut();
    
    // 1秒后显示回忆
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &AAnjiMemory::ShowMemory, 1.0f, false);
}
```

**ShowMemory()**
```cpp
void ShowMemory()
{
    // 显示分屏UI
    MemoryUI->SetVisibility(true);
    
    // 播放回忆音乐
    MemoryMusic->Play();
    
    // 播放独白序列
    PlayDialogueSequence();
    
    // 20秒后结束
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &AAnjiMemory::End, 20.0f, false);
}
```

**PlayDialogueSequence()**
```cpp
void PlayDialogueSequence()
{
    // 使用Timeline控制对话时机
    FTimeline DialogueTimeline;
    
    // 0秒: 分离
    DialogueTimeline.AddEvent(0.0f, FOnTimelineEvent::CreateUObject(this, &AAnjiMemory::PlayDialogue, "DLG_C2_025"));
    
    // 10秒: 孤独
    DialogueTimeline.AddEvent(10.0f, FOnTimelineEvent::CreateUObject(this, &AAnjiMemory::PlayDialogue, "DLG_C2_026"));
    
    // 15秒: 重逢
    DialogueTimeline.AddEvent(15.0f, FOnTimelineEvent::CreateUObject(this, &AAnjiMemory::PlayDialogue, "DLG_C2_027"));
    
    DialogueTimeline.Play();
}
```

**End()**
```cpp
void End()
{
    // 淡出回忆
    FadeOut();
    
    // 1秒后回到展厅
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, 
        &AAnjiMemory::ReturnToHall, 1.0f, false);
}
```

**ReturnToHall()**
```cpp
void ReturnToHall()
{
    // 隐藏UI
    MemoryUI->SetVisibility(false);
    
    // 淡入展厅
    FadeIn();
    
    // 蔷薇已盛开
    // （由管理器的OnReturnToGreenhouse处理）
}
```

---

## 🎨 UI系统

### 1. WBP_DiaryUI（日记UI）

**功能**: 显示日记内容

#### 组件

- TextBlock (DiaryText)
- Image (DiaryBackground)
- Button (CloseButton)

#### 核心函数

**ShowDiary(DiaryContent)**
```cpp
void ShowDiary(FString DiaryContent)
{
    DiaryText->SetText(FText::FromString(DiaryContent));
    SetVisibility(ESlateVisibility::Visible);
    
    // 暂停游戏
    UGameplayStatics::SetGamePaused(GetWorld(), true);
}
```

**HighlightKeyword(Keyword)**
```cpp
void HighlightKeyword(FString Keyword)
{
    // 使用富文本高亮
    FString HighlightedText = DiaryText->GetText().ToString();
    HighlightedText.ReplaceInline(*Keyword, *FString::Printf(TEXT("<span color=\"#FFFF00\">%s</>"), *Keyword));
    DiaryText->SetText(FText::FromString(HighlightedText));
}
```

---

### 2. WBP_ClockUI（时钟UI）

**功能**: 调整时间

#### 组件

- Slider (TimeSlider)
- TextBlock (TimeDisplay)
- Button (ConfirmButton)

#### 核心函数

**OnTimeSliderChanged(Value)**
```cpp
void OnTimeSliderChanged(float Value)
{
    // Value: 0-24
    float Hour = Value;
    
    // 更新显示
    TimeDisplay->SetText(FText::FromString(FString::Printf(TEXT("%02d:00"), (int)Hour)));
    
    // 实时更新时钟
    ClockActor->AdjustTime(Hour);
}
```

---

## ⚙️ 性能优化

### 1. 资源预加载

```cpp
void PreloadAssets()
{
    // 预加载植物精灵
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            FString Path = FString::Printf(TEXT("/Game/Art/Plants/Plant%d_Frame%d"), i, j);
            UTexture2D* Sprite = LoadObject<UTexture2D>(nullptr, *Path);
            PlantSprites[i][j] = Sprite;
        }
    }
    
    // 预加载音效
    PreloadSound("SFX_C2_001");
    PreloadSound("SFX_C2_002");
    // ...
}
```

### 2. 粒子优化

```cpp
void OptimizeParticles()
{
    // 限制粒子数量
    PetalFall->SetMaxParticleCount(100);
    
    // 使用LOD
    PetalFall->SetLODLevel(1);
    
    // 距离剔除
    PetalFall->SetCullDistance(2000.0f);
}
```

### 3. 动画优化

```cpp
void OptimizeAnimations()
{
    // 使用帧动画而非骨骼动画（植物）
    // 使用Timeline而非Tick（平滑移动）
    // 缓存贝塞尔曲线计算结果
}
```

### 4. 镜像优化

```cpp
void OptimizeMirror()
{
    // 降低SceneCaptureComponent2D分辨率
    MirrorCapture->TextureTarget->SizeX = 512;
    MirrorCapture->TextureTarget->SizeY = 512;
    
    // 限制捕获帧率
    MirrorCapture->SetCaptureEveryFrame(false);
    MirrorCapture->CaptureScene();
}
```

---

## ✅ 测试清单

### 功能测试

#### 温室区域
- [ ] 向日葵种植和生长正常
- [ ] 天窗开关正常，阳光激活向日葵
- [ ] 睡莲种植和浇水正常
- [ ] 浇水壶拾取和使用正常
- [ ] 月见草种植和时间调整正常
- [ ] 时钟UI正常，时间变化影响光照
- [ ] 蔷薇种植后不立即生长
- [ ] 蔷薇提示30秒后正常触发
- [ ] 日记阅读正常，关键词高亮
- [ ] 四花齐放动画正常
- [ ] 记忆碎片生成正常

#### 镜像区域
- [ ] 镜子反射正常
- [ ] 红蓝人偶可交互
- [ ] 同步按钮机制正常
- [ ] 人偶镜像移动正常
- [ ] 分离路径机制正常
- [ ] 镜子裂缝显示正常
- [ ] 镜子破碎动画正常
- [ ] 人偶融合与分离动画正常
- [ ] 红色花朵生成正常

#### 蝴蝶系统
- [ ] 蝴蝶自动访问四朵花
- [ ] 蝴蝶飞行轨迹正常
- [ ] 日期显示正常
- [ ] 时间之网形成正常
- [ ] 蓝色花朵生成正常

#### 符号融合
- [ ] 红蓝花朵飞向中央
- [ ] 旋转交织动画正常
- [ ] 现在之符生成正常
- [ ] 符号进入背包

#### 回忆过场
- [ ] 安吉回忆正常触发
- [ ] 分屏显示正常
- [ ] 对话时机正确
- [ ] 回忆结束后返回展厅
- [ ] 蔷薇已盛开

### 音频测试

- [ ] 所有音效正确播放
- [ ] 背景音乐循环正常
- [ ] 安吉回忆音乐正确触发
- [ ] 音效音量平衡合理
- [ ] 独白语音清晰（如有）
- [ ] 音效不重叠或冲突

### 视觉测试

- [ ] 植物生长动画流畅（5帧）
- [ ] 花瓣飘落粒子效果正常
- [ ] 镜子破碎效果正常
- [ ] 蝴蝶飞行轨迹发光
- [ ] 时间之网视觉效果正常
- [ ] 符号融合动画流畅
- [ ] 所有发光效果正常
- [ ] 光照变化（时间）正常
- [ ] UI显示正常

### 逻辑测试

- [ ] 无法跳过必要步骤
- [ ] 提示系统正确触发（30秒）
- [ ] 日记关键词正确高亮
- [ ] 镜像机制逻辑正确
- [ ] 蔷薇只在完成镜像后盛开
- [ ] 符号正确进入背包
- [ ] 保存点正确触发
- [ ] 状态转换正确
- [ ] 事件依赖正确

### 性能测试

- [ ] 帧率稳定（60fps）
- [ ] 粒子数量合理
- [ ] 内存占用正常
- [ ] 加载时间合理
- [ ] 无明显卡顿

### 叙事测试

- [ ] 所有独白正确触发
- [ ] 对话文本无错别字
- [ ] 情感节奏合理
- [ ] 安吉回忆与主线呼应
- [ ] 主题表达清晰
- [ ] 玩家能理解蔷薇的秘密

---

## 📝 开发注意事项

### 关键技术点

1. **植物生长**: 使用帧动画（Flipbook）而非骨骼动画
2. **蝴蝶飞行**: 使用贝塞尔曲线+Timeline
3. **镜像同步**: 实时计算镜像位置
4. **时间之网**: 使用LineRenderer或Spline连接节点
5. **粒子效果**: 花瓣飘落、镜子破碎、融合光效

### 蓝图最佳实践

1. **使用事件驱动**: 避免在Tick中轮询
2. **缓存引用**: 避免重复GetActor
3. **使用Timeline**: 平滑动画和插值
4. **使用枚举**: 管理状态
5. **使用接口**: 解耦对象交互

### 调试技巧

1. **Print String**: 输出关键变量
2. **Draw Debug**: 可视化射线、路径
3. **Breakpoint**: 断点调试蓝图
4. **Stat FPS**: 监控性能
5. **Log Category**: 分类日志

---

## 🎉 总结

第二章的实现重点：

1. **植物系统**: 4种植物，4种条件，清晰的逻辑
2. **蔷薇的秘密**: 打破常规，需要玩家"离开"
3. **镜像机制**: 同步与分离的双重体验
4. **蝴蝶系统**: 自动化流程，连接所有元素
5. **情感表达**: 通过动画、音效、独白传达主题

**核心体验**: 独立但相依，分离但不分开。✨

---

**开发时间**: 5.5小时  
**难度**: 中等  
**重点**: 蔷薇谜题的叙事设计 + 镜像机制的实现

祝开发顺利！🚀
