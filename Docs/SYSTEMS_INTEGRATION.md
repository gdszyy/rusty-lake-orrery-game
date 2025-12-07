# 系统集成文档

**项目**: Rusty Lake Orrery  
**版本**: 1.0  
**最后更新**: 2025-12-07

---

## 📋 概述

本文档描述了游戏中三个核心系统的实现和集成方式:
1. **DialogueSystem** - 对话系统
2. **UIManager** - UI管理系统
3. **PuzzleSystem** - 谜题系统

这些系统与现有的 **InteractableComponent** 和 **InventorySystem** 完美集成。

---

## 1. DialogueSystem (对话系统)

### 核心组件

#### UDialogueDataAsset
**文件**: `DialogueDataAsset.h/cpp`

**功能**:
- 存储对话数据(支持中英文双语)
- 从CSV文件导入对话
- 支持对话链和分支对话
- 音频资源管理

**数据结构**:
```cpp
struct FDialogueEntry
{
    FName DialogueID;           // 对话ID
    FName TriggerEvent;         // 触发事件
    ESpeakerType SpeakerType;   // 说话者类型
    FText TextCN;               // 中文文本
    FText TextEN;               // 英文文本
    FString AudioPath;          // 音频路径
    float Duration;             // 持续时间
    FName NextDialogueID;       // 下一条对话
    FString ChoiceOptions;      // 选项列表
};
```

**使用示例**:
```cpp
// 在编辑器中创建 DialogueDataAsset
// 从CSV导入: Content/Data/DT_Dialogue_Chapter1.csv

UDialogueDataAsset* DialogueData = ...;
FDialogueEntry Entry;
if (DialogueData->GetDialogueByTrigger("EnterHall1", Entry))
{
    // 使用对话数据
}
```

#### UDialogueComponent
**文件**: `DialogueComponent.h/cpp`

**功能**:
- 播放对话
- 文本逐字显示
- 音频播放
- 对话链自动播放
- 分支对话选择

**关键接口**:
```cpp
// 播放对话
bool PlayDialogue(FName DialogueID);
bool PlayDialogueByTrigger(FName TriggerEvent);

// 控制对话
void StopDialogue();
void SkipDialogue();
void PlayNextDialogue();
void SelectChoice(int32 ChoiceIndex);
```

**事件**:
```cpp
UPROPERTY(BlueprintAssignable)
FOnDialogueStarted OnDialogueStarted;

UPROPERTY(BlueprintAssignable)
FOnDialogueCompleted OnDialogueCompleted;

UPROPERTY(BlueprintAssignable)
FOnDialogueTextChanged OnDialogueTextChanged;

UPROPERTY(BlueprintAssignable)
FOnWaitingForChoice OnWaitingForChoice;
```

### 与 InteractableComponent 集成

**InteractableComponent.h** 已更新:
```cpp
// Observe 配置
UPROPERTY(EditAnywhere, Category = "Observe Config")
UDialogueDataAsset* ObserveDialogue;

UPROPERTY(EditAnywhere, Category = "Observe Config")
FName ObserveTriggerEvent;
```

**使用流程**:
1. 在 InteractableComponent 中设置 `ObserveDialogue` 和 `ObserveTriggerEvent`
2. 玩家点击 Observe 交互时,触发对话
3. DialogueComponent 播放对应的对话

**蓝图示例**:
```
BP_InteractableObject
├─ InteractableComponent
│  ├─ InteractionType = Observe
│  ├─ ObserveDialogue = DA_Chapter1_Dialogues
│  └─ ObserveTriggerEvent = "ObserveJewelCase"
└─ DialogueComponent (可选,用于直接播放)
```

---

## 2. UIManager (UI管理系统)

### 核心组件

#### AUIManager
**文件**: `UIManager.h/cpp`

**功能**:
- 统一管理所有UI Widget
- 单例模式访问
- UI显示/隐藏控制
- UI状态查询

**管理的UI**:
1. **DialogueWidget** - 对话UI
2. **InventoryWidget** - 背包UI
3. **HintWidget** - 提示UI
4. **InteractionPromptWidget** - 交互提示UI

**关键接口**:
```cpp
// 对话UI
void ShowDialogue(const FDialogueEntry& DialogueEntry);
void UpdateDialogueText(const FText& DisplayText, float Progress);
void HideDialogue();
void ShowDialogueChoices(const TArray<FString>& Choices);

// 背包UI
void ShowInventory();
void HideInventory();
void ToggleInventory();
void RefreshInventory();

// 提示UI
void ShowHint(const FText& HintText, float Duration = 3.0f);
void HideHint();

// 交互提示UI
void ShowInteractionPrompt(const FText& PromptText);
void HideInteractionPrompt();

// 通用
void HideAllUI();
bool IsAnyUIVisible() const;
```

**单例访问**:
```cpp
AUIManager* UIManager = AUIManager::GetUIManager(this);
if (UIManager)
{
    UIManager->ShowHint(FText::FromString("找到了线索!"), 3.0f);
}
```

### 使用流程

1. **在关卡中放置 UIManager**:
   - 创建 `BP_UIManager` (继承自 AUIManager)
   - 设置各个 Widget 类
   - 放置到关卡中

2. **创建 UI Widget**:
   - `WBP_Dialogue` - 对话UI
   - `WBP_Inventory` - 背包UI
   - `WBP_Hint` - 提示UI
   - `WBP_InteractionPrompt` - 交互提示UI

3. **在蓝图中使用**:
   ```
   Get UI Manager
   └─ Show Hint
      ├─ Hint Text: "解开谜题获得奖励"
      └─ Duration: 3.0
   ```

### 与 DialogueComponent 集成

**DialogueComponent** 应该与 **UIManager** 配合:
```cpp
// 在 DialogueComponent 中
void UDialogueComponent::PlayDialogueInternal(const FDialogueEntry& Entry)
{
    // ... 播放逻辑
    
    // 通知 UIManager 显示对话
    AUIManager* UIManager = AUIManager::GetUIManager(this);
    if (UIManager)
    {
        UIManager->ShowDialogue(Entry);
    }
    
    OnDialogueStarted.Broadcast(Entry);
}
```

---

## 3. PuzzleSystem (谜题系统)

### 核心组件

#### APuzzleBase
**文件**: `PuzzleBase.h/cpp`

**功能**:
- 谜题状态管理
- 提示系统
- 重置功能
- 奖励发放
- 事件广播

**状态枚举**:
```cpp
enum class EPuzzleState
{
    Inactive,   // 未激活
    Active,     // 已激活
    Solving,    // 解决中
    Completed,  // 已完成
    Failed      // 失败
};
```

**关键接口**:
```cpp
// 状态控制
void ActivatePuzzle();
void CompletePuzzle();
void FailPuzzle();
void ResetPuzzle();

// 提示系统
FText ShowNextHint();
FText GetCurrentHint() const;
bool HasMoreHints() const;

// 进度查询
float GetProgress() const;
bool IsCompleted() const;
bool IsActive() const;
```

**事件**:
```cpp
UPROPERTY(BlueprintAssignable)
FOnPuzzleActivated OnPuzzleActivated;

UPROPERTY(BlueprintAssignable)
FOnPuzzleCompleted OnPuzzleCompleted;

UPROPERTY(BlueprintAssignable)
FOnPuzzleFailed OnPuzzleFailed;

UPROPERTY(BlueprintAssignable)
FOnPuzzleReset OnPuzzleReset;

UPROPERTY(BlueprintAssignable)
FOnHintShown OnHintShown;
```

**蓝图可重写事件**:
```cpp
UFUNCTION(BlueprintNativeEvent)
void OnPuzzleActivated();

UFUNCTION(BlueprintNativeEvent)
void OnPuzzleSolved();

UFUNCTION(BlueprintNativeEvent)
void OnPuzzleFailedEvent();

UFUNCTION(BlueprintNativeEvent)
void OnPuzzleResetEvent();
```

#### UPuzzleComponent
**文件**: `PuzzleComponent.h/cpp`

**功能**:
- 谜题子元素管理
- 自动检测谜题完成
- 与 APuzzleBase 集成

**使用场景**:
- 谜题的多个部分(如4个按钮需要按正确顺序)
- 简单的开关/触发器
- 谜题进度跟踪

**关键接口**:
```cpp
void ActivateComponent();
void CompleteComponent();
void ResetComponent();
void CheckPuzzleCompletion();
```

**使用示例**:
```cpp
// 在谜题Actor上添加多个 PuzzleComponent
BP_MusicBoxPuzzle (APuzzleBase)
├─ PuzzleComponent_Note1 (bIsRequired = true)
├─ PuzzleComponent_Note2 (bIsRequired = true)
├─ PuzzleComponent_Note3 (bIsRequired = true)
└─ PuzzleComponent_Note4 (bIsRequired = false)

// 当所有必需组件完成时,自动完成谜题
```

#### ARotationPuzzle
**文件**: `RotationPuzzle.h/cpp`

**功能**:
- 旋转谜题的具体实现
- 角度检测
- 自动完成检测

**配置**:
```cpp
UPROPERTY(EditAnywhere)
float TargetRotation = 0.0f;        // 目标角度

UPROPERTY(EditAnywhere)
float AngleTolerance = 5.0f;        // 容差

UPROPERTY(EditAnywhere)
float HoldTime = 0.5f;              // 保持时间
```

**使用示例**:
```
BP_ClockPuzzle (继承自 ARotationPuzzle)
├─ TargetRotation = 90.0
├─ AngleTolerance = 5.0
├─ HoldTime = 0.5
└─ RewardItem = DA_Item_ClockKey
```

### 与 InteractableComponent 集成

**InteractableComponent.h** 已更新:
```cpp
// TriggerPuzzle 配置
UPROPERTY(EditAnywhere, Category = "Puzzle Config")
APuzzleBase* TargetPuzzle;

UPROPERTY(EditAnywhere, Category = "Puzzle Config")
bool bShowPuzzleHint = true;
```

**使用流程**:
1. 创建谜题Actor (如 `BP_MusicBoxPuzzle`)
2. 在 InteractableComponent 中设置 `TargetPuzzle`
3. 玩家点击交互时,激活谜题

**蓝图示例**:
```
BP_MusicBoxButton
├─ InteractableComponent
│  ├─ InteractionType = TriggerPuzzle
│  ├─ TargetPuzzle = BP_MusicBoxPuzzle
│  └─ bShowPuzzleHint = true
└─ StaticMeshComponent
```

**交互流程**:
```
玩家点击按钮
    ↓
InteractableComponent.Interact()
    ↓
检查 InteractionType == TriggerPuzzle
    ↓
TargetPuzzle->ActivatePuzzle()
    ↓
显示谜题提示 (如果 bShowPuzzleHint = true)
    ↓
玩家解决谜题
    ↓
TargetPuzzle->CompletePuzzle()
    ↓
OnPuzzleCompleted 事件触发
    ↓
发放奖励物品
```

---

## 4. 系统集成示例

### 示例1: 音乐盒谜题 (第一章)

**场景设置**:
```
BP_MusicBoxPuzzle (APuzzleBase)
├─ PuzzleName = "音乐盒旋律"
├─ PuzzleDescription = "弹奏正确的旋律解锁音乐盒"
├─ HintTexts = ["墙上的画框中有线索", "旋律是 Mi-Sol-Mi-Do-Re-Mi-Re-Do"]
├─ RewardItem = DA_Item_MemoryFragment1
└─ 7个音符按钮 (每个都有 InteractableComponent)

BP_MusicNote_Mi (InteractableComponent)
├─ InteractionType = TriggerPuzzle
├─ TargetPuzzle = BP_MusicBoxPuzzle
└─ OnInteracted: 检查序列是否正确

BP_PaintingClue (InteractableComponent)
├─ InteractionType = Observe
├─ ObserveDialogue = DA_Chapter1_Dialogues
└─ ObserveTriggerEvent = "ObservePainting"
```

**流程**:
1. 玩家点击画框 → 显示对话线索
2. 玩家点击音符按钮 → 触发谜题检查
3. 序列正确 → 完成谜题 → 获得记忆碎片1

### 示例2: 植物培育谜题 (第二章)

**场景设置**:
```
BP_PlantPuzzle (APuzzleBase)
├─ PuzzleName = "植物培育"
├─ HintTexts = ["日记中有植物生长的秘密"]
├─ RewardItem = DA_Item_RedFlower
└─ 4个花盆组件

BP_Sunflower (PuzzleComponent)
├─ ComponentID = "Sunflower"
├─ bIsRequired = true
└─ PuzzleActor = BP_PlantPuzzle

BP_Skylight (InteractableComponent)
├─ InteractionType = TriggerPuzzle
├─ TargetPuzzle = BP_PlantPuzzle
└─ OnInteracted: 打开天窗 → 向日葵生长

BP_Diary (InteractableComponent)
├─ InteractionType = Observe
├─ ObserveDialogue = DA_Chapter2_Dialogues
└─ ObserveTriggerEvent = "ReadDiary"
```

**流程**:
1. 玩家阅读日记 → 了解植物需求
2. 玩家打开天窗 → 向日葵生长 → PuzzleComponent 完成
3. 玩家浇水 → 睡莲生长 → PuzzleComponent 完成
4. 所有组件完成 → 谜题完成 → 获得红色花朵

### 示例3: 旋转谜题

**场景设置**:
```
BP_ClockPuzzle (ARotationPuzzle)
├─ TargetRotation = 90.0
├─ AngleTolerance = 5.0
├─ HoldTime = 0.5
└─ RewardItem = DA_Item_ClockKey

BP_ClockHand (InteractableComponent)
├─ InteractionType = RotateObject
├─ RotationSpeed = 45.0
└─ TargetRotationAngle = 90.0
```

**流程**:
1. 玩家点击时钟指针 → 开始旋转
2. 旋转到90度 → 保持0.5秒
3. 谜题完成 → 获得时钟钥匙

---

## 5. 数据资产管理

### 对话数据

**CSV格式** (`Content/Data/DT_Dialogue_Chapter1.csv`):
```csv
DialogueID,TriggerEvent,SpeakerType,Text_CN,Text_EN,AudioPath,Duration,NextDialogueID,ChoiceOptions
DLG_C1_001,EnterHall1,Narrator,这是第一展厅...,This is the first hall...,/Game/Audio/Dialogue/C1/DLG_C1_001.wav,8,,
DLG_C1_002,ObserveJewelCase,Narrator,珠宝在低语...,The jewels whisper...,/Game/Audio/Dialogue/C1/DLG_C1_002.wav,5,DLG_C1_003,
```

**导入方法**:
1. 在编辑器中创建 `DialogueDataAsset`
2. 调用 `ImportFromCSV()` 函数
3. 指定CSV文件路径

### 物品数据

**CSV格式** (`Content/Data/DT_Items_Chapter1.csv`):
```csv
ItemID,ItemName,ItemDescription,ItemType,IconPath,IsStackable,MaxStackSize
ITEM_C1_001,记忆碎片1,音乐盒中的记忆,QuestItem,/Game/UI/Icons/Memory1.png,false,1
ITEM_C1_002,乌鸦羽毛,黑色的羽毛,QuestItem,/Game/UI/Icons/Feather.png,false,1
```

---

## 6. 蓝图接口

### DialogueComponent 蓝图接口

**事件**:
- `On Dialogue Started` - 对话开始
- `On Dialogue Completed` - 对话完成
- `On Dialogue Text Changed` - 文本更新
- `On Waiting For Choice` - 等待选择

**函数**:
- `Play Dialogue` - 播放对话
- `Play Dialogue By Trigger` - 根据触发事件播放
- `Stop Dialogue` - 停止对话
- `Skip Dialogue` - 跳过对话
- `Select Choice` - 选择选项

### UIManager 蓝图接口

**函数**:
- `Get UI Manager` - 获取单例
- `Show Dialogue` - 显示对话UI
- `Show Inventory` - 显示背包UI
- `Show Hint` - 显示提示
- `Show Interaction Prompt` - 显示交互提示
- `Hide All UI` - 隐藏所有UI

### PuzzleBase 蓝图接口

**事件**:
- `On Puzzle Activated` - 谜题激活
- `On Puzzle Solved` - 谜题完成
- `On Puzzle Failed` - 谜题失败
- `On Puzzle Reset` - 谜题重置

**函数**:
- `Activate Puzzle` - 激活谜题
- `Complete Puzzle` - 完成谜题
- `Show Next Hint` - 显示下一条提示
- `Get Progress` - 获取进度

---

## 7. 开发建议

### 创建新谜题的步骤

1. **继承 APuzzleBase**:
   ```cpp
   UCLASS()
   class AMyPuzzle : public APuzzleBase
   {
       GENERATED_BODY()
       
       // 实现谜题逻辑
   };
   ```

2. **重写关键函数**:
   ```cpp
   virtual void OnPuzzleActivated_Implementation() override;
   virtual void OnPuzzleSolved_Implementation() override;
   virtual float GetProgress_Implementation() const override;
   ```

3. **创建蓝图类**:
   - 在编辑器中创建 `BP_MyPuzzle`
   - 设置 PuzzleName, Description, Hints
   - 设置 RewardItem

4. **添加交互**:
   - 创建交互对象 (按钮/开关等)
   - 添加 InteractableComponent
   - 设置 TargetPuzzle = BP_MyPuzzle

### 调试技巧

1. **启用详细日志**:
   ```cpp
   UE_LOG(LogTemp, Log, TEXT("Puzzle: %s"), *Message);
   ```

2. **使用蓝图调试**:
   - 在事件上设置断点
   - 查看变量值
   - 单步执行

3. **测试谜题**:
   ```cpp
   // 在控制台执行
   DebugPuzzle.Activate BP_MyPuzzle
   DebugPuzzle.Complete BP_MyPuzzle
   DebugPuzzle.Reset BP_MyPuzzle
   ```

---

## 8. 性能优化

### 对话系统优化

- 音频资源异步加载
- 对话文本缓存
- UI更新频率限制

### UI管理优化

- Widget池化
- 按需创建/销毁
- Z-Order优化

### 谜题系统优化

- 只在激活状态下Tick
- 事件驱动而非轮询
- 状态缓存

---

## 9. 常见问题

### Q: 对话不显示?
A: 检查:
1. DialogueDataAsset 是否正确设置
2. UIManager 是否在场景中
3. DialogueWidget 是否正确创建

### Q: 谜题无法完成?
A: 检查:
1. PuzzleComponent 的 bIsRequired 设置
2. 是否调用了 CompleteComponent()
3. PuzzleActor 引用是否正确

### Q: UI层级错误?
A: 调整 AddToViewport() 的 ZOrder 参数

---

## 10. 总结

三个系统的集成提供了完整的游戏玩法支持:

- **DialogueSystem**: 叙事和剧情推进
- **UIManager**: 统一的UI管理和显示
- **PuzzleSystem**: 灵活的谜题设计框架

所有系统都与 **InteractableComponent** 和 **InventorySystem** 无缝集成,提供了强大而灵活的游戏开发基础。
