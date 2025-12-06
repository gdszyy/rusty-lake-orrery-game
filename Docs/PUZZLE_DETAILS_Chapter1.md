# 第一章交互细节实现文档

**目标读者**: 集成指导 Manus、蓝图开发者  
**文档目的**: 提供详细的蓝图实现指南，确保谜题和交互的正确实现  
**最后更新**: 2025-12-07

---

## 📋 文档说明

本文档详细描述第一章"乌鸦与钟楼"中所有可交互对象的具体行为、状态机、事件流程和蓝图实现要点。每个交互都包含：

- **前置条件**: 触发交互所需的条件
- **交互流程**: 详细的步骤序列
- **状态变化**: 对象状态的转换
- **事件触发**: 需要广播的事件
- **视觉反馈**: UI和特效的表现
- **音效**: 需要播放的音频
- **蓝图实现**: 具体的实现建议

---

## 1. 甲虫解剖谜题 (Beetle Dissection Puzzle)

### 基本信息

- **对象**: `BP_DeadBeetle` (继承自 `APuzzle_SequenceInteraction`)
- **位置**: 二层机械室地板
- **难度**: ★★★★☆
- **预计完成时间**: 2-3分钟

### 前置条件

- 玩家已进入二层机械室
- 玩家已发现死甲虫和手术工具

### 交互流程

#### 阶段1: 观察阶段

**触发**: 玩家点击死甲虫

**行为**:
1. 镜头缓慢推近到甲虫（使用 `Camera Zoom` 动画）
2. 甲虫放大显示，可以看到外壳的缝隙
3. 显示观察UI，包含"解剖"按钮和"返回"按钮
4. 播放独白：`CH1_DeadBeetle_Observe`

**蓝图实现**:
```
Event OnInteract()
├─ Play Camera Animation: CameraZoom_In
├─ Show Widget: WBP_BeetleObservation
│  ├─ Button: "解剖" -> OnClickDissect()
│  └─ Button: "返回" -> OnClickReturn()
└─ Play Dialogue: CH1_DeadBeetle_Observe
```

#### 阶段2: 解剖操作

**触发**: 玩家点击"解剖"按钮

**操作序列**:

| 步骤 | 操作 | 工具 | 动画 | 音效 | 独白 |
|------|------|------|------|------|------|
| 1 | 选择小刀 | 小刀 | 手拿起小刀 | SFX_PickupTool | CH1_Dissection_Start |
| 2 | 沿缝隙切开 | 小刀 | 刀刃沿缝隙移动 | SFX_CutShell | - |
| 3 | 选择镊子 | 镊子 | 手放下小刀，拿起镊子 | SFX_PickupTool | - |
| 4 | 揭开背壳 | 镊子 | 镊子夹住边缘，揭开 | SFX_LiftShell | - |
| 5 | 发现机械 | - | 镜头推近内部 | SFX_MechanicalReveal | CH1_Dissection_Reveal |
| 6 | 取出机械甲虫 | 镊子 | 镊子夹出机械甲虫 | SFX_ExtractMechanism | CH1_MechanicalBeetle_Get |

**状态机**:
```
State: Idle
  ├─ OnInteract() -> State: Observing
  
State: Observing
  ├─ OnClickDissect() -> State: Step1_SelectScalpel
  └─ OnClickReturn() -> State: Idle
  
State: Step1_SelectScalpel
  ├─ OnSelectTool(Scalpel) -> State: Step2_CutShell
  
State: Step2_CutShell
  ├─ OnCutComplete() -> State: Step3_SelectTweezers
  
State: Step3_SelectTweezers
  ├─ OnSelectTool(Tweezers) -> State: Step4_LiftShell
  
State: Step4_LiftShell
  ├─ OnLiftComplete() -> State: Step5_RevealMechanism
  
State: Step5_RevealMechanism
  ├─ OnRevealComplete() -> State: Step6_ExtractBeetle
  
State: Step6_ExtractBeetle
  ├─ OnExtractComplete() -> State: Completed
  
State: Completed
  ├─ Add Item: MechanicalBeetle
  ├─ Destroy Actor: BP_DeadBeetle
  └─ Play Camera Animation: CameraZoom_Out
```

**视觉反馈**:

- **切开时**: 外壳沿缝隙分离，露出内部结构
- **揭开时**: 背壳缓慢抬起，内部逐渐显露
- **发现机械时**: 镜头推近，内部的齿轮和发条清晰可见
  - 有机组织（血管、器官）已被机械零件替代
  - 血管变成铜线，器官变成齿轮
  - 使用材质混合展现有机与机械的融合
- **取出时**: 机械甲虫在镊子中微微震动

**错误处理**:

如果玩家点击错误的工具或错误的位置：
- 播放错误音效：`SFX_WrongAction`
- 显示提示文本："这样不对..."
- 不改变状态，等待正确操作

**蓝图实现要点**:

```cpp
// BP_DeadBeetle.h (C++ 基类)
UCLASS()
class APuzzle_SequenceInteraction : public AActor, public IInteractable
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FInteractionStep> Steps; // 操作步骤数组
    
    UPROPERTY(BlueprintReadOnly)
    int32 CurrentStep; // 当前步骤索引
    
    UFUNCTION(BlueprintCallable)
    void ExecuteStep(int32 StepIndex);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnStepCompleted(int32 StepIndex);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnAllStepsCompleted();
};

// 蓝图中
Event OnInteract()
├─ If (CurrentStep == 0)
│  └─ Show Observation UI
└─ Else
   └─ Continue Current Step

Event OnClickDissect()
├─ Hide Observation UI
├─ Show Dissection UI (with tool selection)
└─ Set CurrentStep = 1

Event OnSelectTool(ToolType)
├─ If (ToolType == ExpectedTool[CurrentStep])
│  ├─ Play Step Animation
│  ├─ Play Step Sound
│  ├─ Increment CurrentStep
│  └─ If (CurrentStep >= Steps.Num())
│     └─ Call OnAllStepsCompleted()
└─ Else
   └─ Play Error Feedback
```

---

## 2. 音乐盒序列谜题 (Music Box Sequence Puzzle)

### 基本信息

- **对象**: `BP_MusicBox` (继承自 `APuzzle_SequenceInput`)
- **位置**: 二层机械室
- **难度**: ★★★★☆
- **预计完成时间**: 3-5分钟

### 前置条件

- 玩家已获得音乐盒钥匙
- 玩家已通过望远镜看清乐谱

### 交互流程

#### 阶段1: 锁定状态

**触发**: 玩家点击音乐盒（未解锁时）

**行为**:
1. 显示"音乐盒被锁住了"提示
2. 播放独白：`CH1_MusicBox_Observe`
3. 如果玩家有钥匙，显示"使用钥匙"选项

**蓝图实现**:
```
Event OnInteract()
├─ If (bIsLocked)
│  ├─ Show Dialogue: "音乐盒被锁住了"
│  ├─ Play Dialogue: CH1_MusicBox_Observe
│  └─ If (Player Has MusicBoxKey)
│     └─ Show Option: "使用钥匙"
└─ Else
   └─ Open Music Box UI
```

#### 阶段2: 解锁

**触发**: 玩家选择"使用钥匙"

**行为**:
1. 播放钥匙插入动画
2. 播放解锁音效：`SFX_UnlockMusicBox`
3. 音乐盒盖子缓慢打开
4. 露出7个音符按钮和内部的机械鸟
5. 播放独白：`CH1_MusicBox_Unlock`
6. 从背包移除钥匙

**蓝图实现**:
```
Event OnUseKey()
├─ Play Animation: KeyInsert
├─ Play Sound: SFX_UnlockMusicBox
├─ Play Animation: LidOpen
├─ Set bIsLocked = false
├─ Remove Item: MusicBoxKey
├─ Play Dialogue: CH1_MusicBox_Unlock
└─ Show Music Box UI
```

#### 阶段3: 序列输入

**UI布局**:
```
┌─────────────────────────────────────┐
│        音乐盒                        │
│                                     │
│   [机械鸟动画区域]                   │
│   (根据输入做出反应)                 │
│                                     │
│   音符按钮:                          │
│   [Do] [Re] [Mi] [Fa] [Sol] [La] [Si]│
│                                     │
│   当前序列: ♪ ♪ ♪ _ _ _ _ _         │
│                                     │
│   [重置]              [返回]         │
└─────────────────────────────────────┘
```

**正确序列**: Mi - Sol - Mi - Do - Re - Mi - Re - Do

**操作流程**:

1. **玩家点击音符按钮**
   - 按钮短暂发光
   - 播放对应音符：`SFX_Note_[NoteName]`
   - 对应的齿轮转动一格
   - 机械鸟扇动一次翅膀
   - 序列显示区增加一个音符图标

2. **每次输入后验证**
   - 如果当前输入与正确序列的对应位置匹配：继续
   - 如果不匹配：触发错误反馈

3. **错误反馈**
   - 播放不和谐音：`SFX_WrongSequence`
   - 机械鸟剧烈挣扎，翅膀快速扇动
   - 齿轮卡顿，发出刺耳声音
   - 序列显示区清空
   - 播放独白：`CH1_MusicBox_Wrong`

4. **正确完成**
   - 播放完整旋律：`SFX_MusicBoxMelody`
   - 机械鸟的动作变得流畅，像真正在飞翔
   - 音乐盒底部打开，露出幻灯片
   - 触发小刚演唱会回忆过场动画

**状态机**:
```
State: Locked
  └─ OnUseKey() -> State: Unlocked
  
State: Unlocked
  └─ OnInteract() -> State: InputMode
  
State: InputMode
  ├─ OnNotePressed(Note)
  │  ├─ Add Note to CurrentSequence
  │  ├─ Play Note Sound
  │  ├─ Animate Mechanical Bird
  │  └─ Validate Sequence
  │     ├─ If (Correct so far)
  │     │  └─ If (Sequence Complete)
  │     │     └─ State: Completed
  │     │  └─ Else
  │     │     └─ Continue in InputMode
  │     └─ If (Wrong)
  │        ├─ Play Error Feedback
  │        └─ Reset CurrentSequence
  └─ OnReset()
     └─ Clear CurrentSequence
     
State: Completed
  ├─ Play Complete Melody
  ├─ Open Bottom Compartment
  ├─ Add Item: Slide
  ├─ Add Item: Note_1
  └─ Trigger Cutscene: Concert Memory
```

**机械鸟动画**:

机械鸟有三种状态：

| 状态 | 触发条件 | 动画 | 描述 |
|------|---------|------|------|
| **待机** | 无输入时 | 微微颤抖 | 翅膀轻微震动，眼睛闪烁 |
| **正常扇动** | 正确音符 | 流畅扇动 | 翅膀优雅地上下扇动一次 |
| **痛苦挣扎** | 错误音符 | 剧烈挣扎 | 翅膀快速、不规律地扇动，身体扭曲 |
| **自由飞翔** | 序列完成 | 持续飞翔 | 翅膀持续、流畅地扇动，像真正的鸟 |

**蓝图实现要点**:

```cpp
// BP_MusicBox.h (C++ 基类)
UCLASS()
class APuzzle_SequenceInput : public AActor, public IInteractable
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ENoteType> CorrectSequence; // 正确序列
    
    UPROPERTY(BlueprintReadOnly)
    TArray<ENoteType> CurrentSequence; // 当前输入序列
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsLocked;
    
    UFUNCTION(BlueprintCallable)
    void InputNote(ENoteType Note);
    
    UFUNCTION(BlueprintCallable)
    bool ValidateSequence();
    
    UFUNCTION(BlueprintCallable)
    void ResetSequence();
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnCorrectNote(int32 NoteIndex);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnWrongNote();
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnSequenceCompleted();
};

// 蓝图中
Event OnNotePressed(NoteType)
├─ Call InputNote(NoteType)
├─ Play Note Sound
├─ Animate Button (Highlight)
├─ Animate Mechanical Bird (Wing Flap)
├─ Update Sequence Display UI
└─ Call ValidateSequence()
   ├─ If (Correct so far)
   │  ├─ Call OnCorrectNote()
   │  └─ If (Sequence Complete)
   │     └─ Call OnSequenceCompleted()
   │        ├─ Play Complete Melody
   │        ├─ Open Bottom Compartment
   │        ├─ Add Items
   │        └─ Trigger Cutscene
   └─ Else (Wrong)
      ├─ Call OnWrongNote()
      ├─ Play Error Sound
      ├─ Animate Bird (Struggle)
      └─ Call ResetSequence()
```

---

## 3. 小刚演唱会回忆过场动画

### 基本信息

- **类型**: 过场动画 (Cutscene)
- **触发**: 完成音乐盒谜题
- **时长**: 约30秒
- **风格**: 超现实主义

### 动画序列

#### 镜头1: 进入回忆 (0-5秒)

**视觉**:
- 从音乐盒内部的视角，镜头缓慢拉远
- 音乐盒的机械结构逐渐模糊，变成光影
- 画面切换到演唱会现场

**音效**:
- 音乐盒旋律逐渐淡出
- 人群的欢呼声逐渐淡入
- 演唱会音乐开始

**独白**: `CH1_Concert_Memory_01` - "那个夜晚，音乐让时间凝固。"

**蓝图实现**:
```
Sequencer Timeline:
├─ 0s: Camera Start Position (Inside Music Box)
├─ 0-3s: Camera Dolly Out + Blur Transition
├─ 3-5s: Cross Fade to Concert Scene
├─ 2s: Play Dialogue: CH1_Concert_Memory_01
└─ 0-5s: Audio Crossfade (Music Box -> Concert)
```

#### 镜头2: 舞台异变 (5-12秒)

**视觉**:
- 舞台上的小刚正在唱歌
- 镜头推近，小刚的身形开始变化
- 头部逐渐变成乌鸦的头部
- 身体扩大，变成一只巨大的乌鸦
- 但依然在唱同样的歌

**音效**:
- 演唱会音乐持续
- 加入乌鸦的鸣叫声（与音乐融合）
- 人群的欢呼声变得空灵

**独白**: `CH1_Concert_Memory_02` - "在成千上万的人群中，我只看得到你。"

**蓝图实现**:
```
Sequencer Timeline:
├─ 5s: Camera Focus on Stage
├─ 5-9s: Morph Animation (Singer -> Giant Raven)
│  └─ Use Material Blend for Transformation
├─ 7s: Play Dialogue: CH1_Concert_Memory_02
├─ 5-12s: Audio Mix (Add Raven Caw, Ethereal Crowd)
└─ 9-12s: Camera Slow Zoom on Raven
```

#### 镜头3: 人群模糊 (12-18秒)

**视觉**:
- 镜头转向观众席
- 成千上万的观众，但所有人的脸都是模糊的
- 只有两个人（玩家和伴侣）是清晰的
- 他们站在人群中，牵着手

**音效**:
- 人群的欢呼声变成风声
- 演唱会音乐变成音乐盒旋律
- 心跳声开始出现

**蓝图实现**:
```
Sequencer Timeline:
├─ 12s: Camera Pan to Audience
├─ 12-15s: Apply Blur Post Process to All Except Two Characters
│  └─ Use Custom Depth Stencil for Selective Blur
├─ 15-18s: Camera Zoom on Two Characters
└─ 12-18s: Audio Crossfade (Concert -> Music Box + Heartbeat)
```

#### 镜头4: 光的聚焦 (18-23秒)

**视觉**:
- 舞台的灯光不再照在乌鸦上
- 所有的灯光聚焦在两个人牵着的手上
- 手发出温暖的金色光芒
- 音符从空中飘落，像雪花一样

**音效**:
- 音乐盒旋律达到高潮
- 音符飘落的"叮叮"声
- 心跳声与音乐同步

**独白**: `CH1_Concert_Memory_03` - "那一刻，我想让它永恒。"

**蓝图实现**:
```
Sequencer Timeline:
├─ 18s: Redirect Stage Lights to Characters' Hands
├─ 18-20s: Add Glow Effect to Hands
├─ 19s: Play Dialogue: CH1_Concert_Memory_03
├─ 20-23s: Spawn Note Particles (Falling like Snow)
│  └─ Use Niagara System for Note Particles
└─ 18-23s: Audio Peak (Music Box Melody + Heartbeat Sync)
```

#### 镜头5: 时间停滞 (23-27秒)

**视觉**:
- 整个场景突然静止
- 乌鸦、人群、飘落的音符都定格
- 只有两个人还在动
- 他们转头看向彼此

**音效**:
- 所有声音突然停止
- 只剩下两个人的呼吸声
- 微弱的心跳声

**蓝图实现**:
```
Sequencer Timeline:
├─ 23s: Pause All Animations (Except Two Characters)
├─ 23s: Stop All Audio (Except Breathing + Heartbeat)
├─ 23-25s: Two Characters Turn to Face Each Other
└─ 25-27s: Hold on Their Faces
```

#### 镜头6: 符号浮现 (27-30秒)

**视觉**:
- 从两人牵着的手中，浮现出音符符号1
- 符号发出温暖的金色光芒
- 符号缓慢旋转，上升
- 画面逐渐变白

**音效**:
- 和谐的音符和弦
- 光芒的"嗡嗡"声
- 音乐盒旋律的最后一个音符

**蓝图实现**:
```
Sequencer Timeline:
├─ 27s: Spawn Note Symbol 1 from Hands
├─ 27-29s: Symbol Float Up + Rotate
├─ 27-30s: Add Glow Effect to Symbol
├─ 29-30s: Fade to White
├─ 28s: Play Harmony Chord
└─ 30s: End Cutscene, Return to Game
```

### 过场动画后

**行为**:
1. 玩家回到二层机械室
2. 音乐盒底部已打开
3. 自动添加物品到背包：
   - 幻灯片 (`Slide`)
   - 音符符号1 (`Note_1`)
4. 播放独白：`CH1_Note1_Get` 和 `CH1_Slide_Get`

**蓝图实现**:
```
Event OnCutsceneEnd()
├─ Fade from White
├─ Return Camera to Player
├─ Set Music Box State: Completed
├─ Add Item: Slide
├─ Add Item: Note_1
├─ Play Dialogue: CH1_Note1_Get
└─ Play Dialogue: CH1_Slide_Get
```

---

## 4. 幻灯机焦距谜题 (Projector Focus Puzzle)

### 基本信息

- **对象**: `BP_Projector` (继承自 `APuzzle_Slider`)
- **位置**: 三层观景台
- **难度**: ★★★★☆
- **预计完成时间**: 2-4分钟

### 前置条件

- 玩家已获得幻灯片

### 交互流程

#### 阶段1: 空置状态

**触发**: 玩家点击幻灯机（无幻灯片时）

**行为**:
1. 显示"幻灯机缺少幻灯片"提示
2. 如果玩家有幻灯片，显示"放入幻灯片"选项

**蓝图实现**:
```
Event OnInteract()
├─ If (bHasSlide == false)
│  ├─ Show Dialogue: "幻灯机缺少幻灯片"
│  └─ If (Player Has Slide)
│     └─ Show Option: "放入幻灯片"
└─ Else
   └─ Open Projector UI
```

#### 阶段2: 放入幻灯片

**触发**: 玩家选择"放入幻灯片"或拖动幻灯片到幻灯机

**行为**:
1. 播放幻灯片插入动画
2. 播放音效：`SFX_InsertSlide`
3. 幻灯机自动打开，发出光束
4. 对面墙上出现投影，但画面模糊
5. 播放独白：`CH1_Projector_Blurry`
6. 从背包移除幻灯片

**蓝图实现**:
```
Event OnInsertSlide()
├─ Play Animation: SlideInsert
├─ Play Sound: SFX_InsertSlide
├─ Set bHasSlide = true
├─ Remove Item: Slide
├─ Activate Projector Light
├─ Show Projection on Wall (Blurry)
├─ Play Dialogue: CH1_Projector_Blurry
└─ Show Focus Adjustment UI
```

#### 阶段3: 焦距调整

**UI布局**:
```
┌─────────────────────────────────────┐
│        幻灯机焦距调整                 │
│                                     │
│   [投影预览区域]                     │
│   (实时显示投影效果)                 │
│                                     │
│   焦距:                              │
│   模糊 ◄─────●─────► 清晰           │
│        0%    75%   100%             │
│                                     │
│   [返回]                             │
└─────────────────────────────────────┘
```

**焦距阶段与视觉效果**:

| 焦距范围 | 画面内容 | 后期处理效果 | 独白 |
|---------|---------|-------------|------|
| **0-25%** | 完全模糊，只有色彩流动 | Gaussian Blur: 50 | - |
| **25-50%** | 理想化场景：阳光、花园 | Gaussian Blur: 25, Brightness +20% | - |
| **50-75%** | 植物生长和枯萎动画 | Gaussian Blur: 10, Time Dilation Effect | CH1_Projector_Adjusting |
| **75-100%** | 完全清晰：真实细节+超现实元素 | Gaussian Blur: 0, Sharpen Filter | CH1_Projector_Clear |

**超现实元素**（在75-100%清晰度时显示）:

1. **树根时钟**: 一棵树的根系延伸成时钟指针的形状
2. **蝴蝶刻度**: 巨大的蝴蝶飞过，翅膀上有钟表刻度
3. **影子融合**: 两个人的影子在地上融合成一棵树的形状
4. **倒置建筑**: 天空中有倒置的建筑
5. **密码数字**: 画面中央有三个发光的数字：**1 4 7**

**操作流程**:

1. **玩家拖动滑块**
   - 滑块值实时更新（0-100）
   - 投影的模糊度实时变化
   - 根据滑块值显示不同的画面内容

2. **到达75-100%区间**
   - 画面完全清晰
   - 超现实元素显现
   - 密码数字发光
   - 滑块自动锁定在最佳焦距（约85%）
   - 触发安吉百草园回忆过场动画

**状态机**:
```
State: Empty
  └─ OnInsertSlide() -> State: Blurry
  
State: Blurry
  └─ OnInteract() -> State: Adjusting
  
State: Adjusting
  ├─ OnSliderValueChanged(Value)
  │  ├─ Update Blur Amount
  │  ├─ Update Projection Content
  │  └─ If (Value >= 75)
  │     └─ State: Clear
  └─ OnReturn()
     └─ State: Blurry
     
State: Clear
  ├─ Lock Slider at Optimal Value
  ├─ Reveal Code: 147
  ├─ Add Item: Note_2
  └─ Trigger Cutscene: Garden Memory
  
State: Completed
  └─ Projection remains clear
```

**投影内容动画**:

在50-75%焦距范围内，投影中的植物应该有生长和枯萎的动画：

- 使用 **Flipbook Texture** 或 **Material Animation**
- 植物从种子→发芽→生长→开花→结果→枯萎→重生
- 循环播放，速度随焦距变化（焦距越高，速度越慢）

**蓝图实现要点**:

```cpp
// BP_Projector.h (C++ 基类)
UCLASS()
class APuzzle_Slider : public AActor, public IInteractable
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TargetValue; // 目标值（75-100）
    
    UPROPERTY(BlueprintReadOnly)
    float CurrentValue; // 当前滑块值
    
    UPROPERTY(BlueprintReadOnly)
    bool bHasSlide;
    
    UFUNCTION(BlueprintCallable)
    void SetSliderValue(float Value);
    
    UFUNCTION(BlueprintCallable)
    bool IsValueInTargetRange();
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnValueChanged(float Value);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnTargetReached();
};

// 蓝图中
Event OnSliderValueChanged(Value)
├─ Set CurrentValue = Value
├─ Call OnValueChanged(Value)
│  ├─ Update Post Process Blur Amount
│  │  └─ Blur = Lerp(50, 0, Value/100)
│  ├─ Update Projection Material
│  │  └─ Based on Value Range, Set Different Textures
│  └─ If (Value in 50-75 range)
│     └─ Play Plant Growth Animation
│        └─ Animation Speed = Lerp(2.0, 0.5, (Value-50)/25)
├─ If (Value >= 75)
│  ├─ Call OnTargetReached()
│  │  ├─ Lock Slider
│  │  ├─ Show Surreal Elements
│  │  ├─ Highlight Code: 147
│  │  ├─ Add Item: Note_2
│  │  └─ Trigger Cutscene
│  └─ Play Dialogue: CH1_Projector_Clear
└─ Else If (Value in 50-75)
   └─ Play Dialogue: CH1_Projector_Adjusting
```

**投影材质实现**:

创建一个动态材质实例，根据焦距值切换纹理和效果：

```
Material: M_ProjectionDynamic
├─ Parameter: FocusValue (0-100)
├─ Parameter: BlurAmount (0-50)
├─ Texture: T_Projection_Blurry (0-25%)
├─ Texture: T_Projection_Idealized (25-50%)
├─ Texture: T_Projection_Growing (50-75%, Animated)
├─ Texture: T_Projection_Clear (75-100%)
└─ Overlay: T_SurrealElements (75-100%, with glow)
   ├─ Tree Root Clock
   ├─ Butterfly with Clock Scales
   ├─ Merged Shadows
   └─ Code Numbers: 1 4 7 (Emissive)
```

---

## 5. 安吉百草园回忆过场动画

### 基本信息

- **类型**: 过场动画 (Cutscene)
- **触发**: 完成幻灯机谜题
- **时长**: 约30秒
- **风格**: 超现实主义 + 自然主义

### 动画序列

#### 镜头1: 进入投影 (0-5秒)

**视觉**:
- 镜头从玩家视角推向投影墙
- 穿过投影画面，进入回忆世界
- 画面从二维投影变成三维空间
- 出现在安吉百草园的小径上

**音效**:
- 幻灯机的运转声逐渐淡出
- 风声和鸟鸣逐渐淡入
- 植物生长的"沙沙"声

**独白**: `CH1_Garden_Memory_01` - "在那片绿意中，我们找到了片刻的宁静。"

**蓝图实现**:
```
Sequencer Timeline:
├─ 0s: Camera Start Position (Player View)
├─ 0-3s: Camera Dolly into Projection
├─ 2s: Transition from 2D to 3D (Shader Effect)
├─ 3-5s: Camera Emerge in Garden Scene
├─ 2s: Play Dialogue: CH1_Garden_Memory_01
└─ 0-5s: Audio Crossfade (Projector -> Nature)
```

#### 镜头2: 植物的生命 (5-12秒)

**视觉**:
- 镜头缓慢移动穿过花园
- 植物以可见的速度生长
  - 种子发芽
  - 茎叶伸展
  - 花朵开放
  - 果实成熟
  - 花瓣凋零
  - 种子落地
  - 新的植物长出
- 整个过程在几秒内完成，形成连续的循环

**音效**:
- 植物生长的加速音效
- 花朵开放的"啵"声
- 种子落地的"啪"声
- 持续的风声和鸟鸣

**独白**: `CH1_Garden_Memory_02` - "植物生长、枯萎、重生..."

**蓝图实现**:
```
Sequencer Timeline:
├─ 5s: Camera Slow Dolly through Garden
├─ 5-12s: Play Plant Growth Animations (Time Lapse)
│  ├─ Use Skeletal Mesh Animation for Plants
│  ├─ Or Use Morph Targets for Growth Stages
│  └─ Loop: Seed -> Grow -> Bloom -> Wither -> Seed
├─ 7s: Play Dialogue: CH1_Garden_Memory_02
└─ 5-12s: Audio Mix (Plant Sounds + Nature Ambience)
```

#### 镜头3: 时间的可视化 (12-18秒)

**视觉**:
- 镜头向下移动，聚焦地面
- 透过土壤，看到树根系统
- 树根延伸成时钟指针的形状
- 指针在地下缓慢旋转
- 根系的脉动与心跳同步

**音效**:
- 时钟指针的"滴答"声
- 心跳声开始出现
- 土壤中的"咕噜"声（根系生长）

**蓝图实现**:
```
Sequencer Timeline:
├─ 12s: Camera Tilt Down to Ground
├─ 12-14s: X-Ray Effect (See Through Soil)
│  └─ Use Custom Post Process Material
├─ 14-18s: Show Tree Root System as Clock Hands
│  ├─ Root Mesh Morphs into Clock Hand Shape
│  └─ Rotate Slowly (1 rotation per 10 seconds)
├─ 15-18s: Root Pulsation Synced with Heartbeat
└─ 12-18s: Audio Mix (Clock Tick + Heartbeat + Soil Sounds)
```

#### 镜头4: 蝴蝶的舞蹈 (18-23秒)

**视觉**:
- 镜头向上移动，回到地面
- 一只巨大的蝴蝶飞过
- 蝴蝶的翅膀上有钟表的刻度和指针
- 翅膀扇动时，刻度在转动
- 蝴蝶在花丛中盘旋

**音效**:
- 蝴蝶翅膀的"扑扑"声
- 翅膀上的刻度转动的"滴答"声
- 花朵摇曳的"沙沙"声

**独白**: `CH1_Garden_Memory_03` - "我终于明白，永恒不在于停止，而在于循环。"

**蓝图实现**:
```
Sequencer Timeline:
├─ 18s: Camera Tilt Up to Ground Level
├─ 18-20s: Spawn Giant Butterfly
│  ├─ Butterfly Scale: 5x Normal
│  └─ Wing Texture: Clock Face with Moving Hands
├─ 20-23s: Butterfly Flight Animation
│  └─ Follow Spline Path through Flowers
├─ 19s: Play Dialogue: CH1_Garden_Memory_03
└─ 18-23s: Audio Mix (Butterfly Wings + Clock Ticks + Flowers)
```

#### 镜头5: 影子的融合 (23-27秒)

**视觉**:
- 镜头移动到一片空地
- 两个人的身影出现（剪影）
- 他们站在一起，手牵手
- 阳光从背后照来，投下影子
- 两个人的影子在地上融合
- 融合的影子开始生长，变成一棵树的形状
- 树的影子逐渐长出枝叶

**音效**:
- 风声
- 树叶的"沙沙"声
- 心跳声持续

**蓝图实现**:
```
Sequencer Timeline:
├─ 23s: Camera Move to Open Space
├─ 23-24s: Fade in Two Character Silhouettes
├─ 24-26s: Shadow Merge Animation
│  ├─ Use Dynamic Shadow Projection
│  └─ Morph Shadow Shape into Tree
├─ 26-27s: Tree Shadow Grows Branches
│  └─ Use Animated Texture for Shadow
└─ 23-27s: Audio Mix (Wind + Leaves + Heartbeat)
```

#### 镜头6: 树的开花与符号浮现 (27-30秒)

**视觉**:
- 镜头推近影子树
- 影子树上开出发光的花朵
- 每朵花都是一个回忆的画面（微型投影）
  - 一起吃饭的画面
  - 一起散步的画面
  - 一起看展览的画面
- 从树干中央，浮现出音符符号2
- 符号发出柔和的绿色光芒
- 符号缓慢旋转，上升
- 画面逐渐变白

**音效**:
- 花朵开放的"叮"声
- 和谐的音符和弦
- 光芒的"嗡嗡"声
- 风声的最后一阵

**蓝图实现**:
```
Sequencer Timeline:
├─ 27s: Camera Zoom on Shadow Tree
├─ 27-28s: Flowers Bloom on Tree
│  ├─ Use Particle System for Flower Bloom
│  └─ Each Flower shows a Memory Image (Decal)
├─ 28s: Spawn Note Symbol 2 from Tree Trunk
├─ 28-29s: Symbol Float Up + Rotate
├─ 28-30s: Add Glow Effect to Symbol (Green)
├─ 29-30s: Fade to White
├─ 28s: Play Harmony Chord
└─ 30s: End Cutscene, Return to Game
```

### 过场动画后

**行为**:
1. 玩家回到三层观景台
2. 投影保持清晰状态，密码147清晰可见
3. 自动添加物品到背包：
   - 音符符号2 (`Note_2`)
4. 播放独白：`CH1_Note2_Get` 和 `CH1_Code_Reveal`

**蓝图实现**:
```
Event OnCutsceneEnd()
├─ Fade from White
├─ Return Camera to Player
├─ Set Projector State: Completed
├─ Keep Projection Clear (with Code visible)
├─ Add Item: Note_2
├─ Play Dialogue: CH1_Note2_Get
└─ Play Dialogue: CH1_Code_Reveal
```

---

## 6. 齿轮拖放谜题 (Gear Drag-and-Drop Puzzle)

### 基本信息

- **对象**: `BP_GearMechanism` (继承自 `APuzzle_DragAndDrop`)
- **位置**: 底层大厅
- **难度**: ★★★☆☆
- **预计完成时间**: 2-3分钟

### 前置条件

- 玩家已获得小齿轮A和中齿轮B

### 装置结构

**三个轴位**:

| 轴位 | 符文标记 | 接受的齿轮 | 位置 |
|------|---------|-----------|------|
| **小轴** | "日常" | Gear_A (小齿轮) | 左侧 |
| **中轴** | "回忆" | Gear_B (中齿轮) | 中间 |
| **大轴** | "未来" | Gear_C (大齿轮) | 右侧 |

**初始状态**: 所有轴位都是空的，只有符文标记

### 交互流程

#### 阶段1: 观察装置

**触发**: 玩家点击齿轮装置

**行为**:
1. 镜头推近装置
2. 显示三个轴位和符文标记
3. 播放独白：`CH1_GearMechanism_Observe`
4. 如果玩家有齿轮，显示拖放提示

**蓝图实现**:
```
Event OnInteract()
├─ Play Camera Animation: ZoomToMechanism
├─ Show Mechanism UI
│  ├─ Display Three Shafts with Rune Labels
│  └─ If (Player Has Gears)
│     └─ Show Drag-Drop Hint
└─ Play Dialogue: CH1_GearMechanism_Observe
```

#### 阶段2: 拖放齿轮

**操作方式**:

1. **从背包拖动齿轮**
   - 玩家打开背包
   - 点击并拖动齿轮图标
   - 移动到装置的某个轴位上
   - 松开鼠标/手指

2. **轴位检测**
   - 当齿轮靠近轴位时，轴位高亮
   - 如果齿轮类型匹配，轴位发出绿光
   - 如果齿轮类型不匹配，轴位发出红光

**匹配规则**:

| 齿轮 | 正确轴位 | 错误轴位 |
|------|---------|---------|
| Gear_A (小) | 小轴 | 中轴、大轴 |
| Gear_B (中) | 中轴 | 小轴、大轴 |
| Gear_C (大) | 大轴 | 小轴、中轴 |

**正确放置**:

1. 齿轮卡入轴位，发出"咔嚓"声
2. 齿轮开始缓慢旋转
3. 符文发光
4. 从背包移除齿轮
5. 播放独白（根据齿轮类型）

**错误放置**:

1. 齿轮弹回背包
2. 播放错误音效：`SFX_WrongPlacement`
3. 轴位闪烁红光
4. 播放独白：`CH1_GearPuzzle_Wrong`

**状态机**:
```
State: Empty
  └─ OnInteract() -> State: DragDropMode
  
State: DragDropMode
  ├─ OnGearDragged(GearType, ShaftID)
  │  ├─ If (GearType matches ShaftID)
  │  │  ├─ Place Gear on Shaft
  │  │  ├─ Play Place Animation
  │  │  ├─ Remove Gear from Inventory
  │  │  └─ Check if All Gears Placed
  │  │     └─ If (Yes) -> State: AllGearsPlaced
  │  └─ Else
  │     ├─ Play Error Feedback
  │     └─ Return Gear to Inventory
  └─ OnReturn()
     └─ State: Empty
     
State: AllGearsPlaced
  ├─ All Gears Start Rotating
  ├─ Gears Engage with Each Other
  ├─ Play Mechanism Running Animation
  └─ Trigger: Drawer Opens -> State: Completed
  
State: Completed
  ├─ Drawer Open, Reveal Gear_C
  └─ Add Item: Gear_C (when player picks it up)
```

#### 阶段3: 联动运转

**触发**: 小齿轮和中齿轮都正确放置

**行为**:

1. **齿轮联动**:
   - 小齿轮开始旋转（速度：快）
   - 小齿轮带动中齿轮旋转（速度：中）
   - 两个齿轮的齿完美咬合

2. **机械运转**:
   - 装置内部的其他齿轮也开始转动
   - 发出机械运转的"咔咔"声
   - 符文全部发光

3. **抽屉弹出**:
   - 装置底部的隐藏抽屉缓慢弹出
   - 发出"咔嚓"的解锁声
   - 抽屉内露出大齿轮C

4. **独白**:
   - 播放：`CH1_GearPuzzle_Correct`

**蓝图实现要点**:

```cpp
// BP_GearMechanism.h (C++ 基类)
UCLASS()
class APuzzle_DragAndDrop : public AActor, public IInteractable
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FDropSlot> DropSlots; // 拖放槽位数组
    
    UPROPERTY(BlueprintReadOnly)
    TMap<int32, bool> SlotOccupied; // 槽位占用状态
    
    UFUNCTION(BlueprintCallable)
    bool CanAcceptItem(int32 SlotID, FName ItemID);
    
    UFUNCTION(BlueprintCallable)
    void PlaceItem(int32 SlotID, FName ItemID);
    
    UFUNCTION(BlueprintCallable)
    bool AreAllSlotsOccupied();
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnItemPlaced(int32 SlotID, FName ItemID);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnWrongPlacement(int32 SlotID, FName ItemID);
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnAllItemsPlaced();
};

// FDropSlot 结构体
USTRUCT(BlueprintType)
struct FDropSlot
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SlotID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName AcceptedItemID; // 接受的物品ID
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Location; // 槽位位置
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName RuneText; // 符文文本
};

// 蓝图中
Event OnGearDraggedToShaft(GearType, ShaftID)
├─ Call CanAcceptItem(ShaftID, GearType)
├─ If (Can Accept)
│  ├─ Call PlaceItem(ShaftID, GearType)
│  ├─ Call OnItemPlaced(ShaftID, GearType)
│  │  ├─ Play Gear Place Animation
│  │  ├─ Play Sound: SFX_GearPlace
│  │  ├─ Start Gear Rotation
│  │  ├─ Light Up Rune
│  │  └─ Remove Gear from Inventory
│  ├─ Call AreAllSlotsOccupied()
│  └─ If (All Occupied)
│     └─ Call OnAllItemsPlaced()
│        ├─ Engage Gears (Link Rotations)
│        ├─ Play Mechanism Running Animation
│        ├─ Play Sound: SFX_MechanismRunning
│        ├─ Play Dialogue: CH1_GearPuzzle_Correct
│        └─ Trigger Drawer Open
│           ├─ Play Animation: DrawerOpen
│           ├─ Play Sound: SFX_DrawerOpen
│           └─ Reveal Gear_C
└─ Else (Cannot Accept)
   ├─ Call OnWrongPlacement(ShaftID, GearType)
   │  ├─ Play Gear Bounce Back Animation
   │  ├─ Play Sound: SFX_WrongPlacement
   │  ├─ Flash Shaft Red
   │  └─ Play Dialogue: CH1_GearPuzzle_Wrong
   └─ Keep Gear in Inventory
```

**齿轮旋转同步**:

为了让齿轮看起来真实地咬合，需要同步它们的旋转：

```
// 齿轮旋转速度计算
// 基于齿轮大小的反比关系

Small Gear (Gear_A):
- Teeth Count: 12
- Rotation Speed: 60 RPM (1 rotation per second)

Medium Gear (Gear_B):
- Teeth Count: 24
- Rotation Speed: 30 RPM (0.5 rotation per second)
- Engaged with Small Gear

Large Gear (Gear_C):
- Teeth Count: 36
- Rotation Speed: 20 RPM (0.33 rotation per second)
- Will engage with Medium Gear (in clock puzzle)

// 蓝图实现
Event Tick(DeltaTime)
├─ If (Gears Are Engaged)
│  ├─ Rotate Small Gear: DeltaTime * 360 (degrees per second)
│  └─ Rotate Medium Gear: DeltaTime * 180 (half speed)
└─ Check Gear Teeth Alignment for Visual Accuracy
```

---

## 7. 钟表旋转谜题 (Clock Rotation Puzzle)

### 基本信息

- **对象**: `BP_GreatClock` (继承自 `APuzzle_Rotatable`)
- **位置**: 顶层钟塔
- **难度**: ★★★★☆
- **预计完成时间**: 3-5分钟

### 前置条件

- 玩家已获得大齿轮C
- 玩家已知道密码147

### 交互流程

#### 阶段1: 检查大钟

**触发**: 玩家点击大钟（未安装齿轮C时）

**行为**:
1. 镜头推近大钟
2. 显示大钟的正面：三个指针定格在12:00
3. 播放独白："大钟停止了...它在等待什么？"
4. 提示玩家检查大钟背面

**蓝图实现**:
```
Event OnInteract()
├─ If (bGearCInstalled == false)
│  ├─ Play Camera Animation: ZoomToClock
│  ├─ Show Clock Face (Hands at 12:00)
│  └─ Show Hint: "检查大钟背面"
└─ Else
   └─ Open Clock Adjustment UI
```

#### 阶段2: 安装大齿轮

**触发**: 玩家检查大钟背面，并拖动大齿轮C

**行为**:

1. **背面视图**:
   - 镜头移动到大钟背面
   - 显示复杂的机械装置
   - 有一个空的轴位，明显缺少一个大齿轮
   - 周围有其他小齿轮，但都静止不动

2. **安装齿轮**:
   - 玩家从背包拖动大齿轮C到空轴位
   - 齿轮卡入，发出洪亮的"咔嚓"声
   - 大齿轮开始缓慢旋转
   - 带动周围的小齿轮一起转动
   - 整个机械装置"苏醒"

3. **独白**:
   - 播放：`CH1_Clock_Install`
   - 播放：`CH1_Clock_Ready`

4. **指针激活**:
   - 镜头回到大钟正面
   - 三个指针开始微微震动
   - 指针变为可交互状态（高亮）

**蓝图实现**:
```
Event OnCheckClockBack()
├─ Play Camera Animation: MoveToClock Back
├─ Show Clock Mechanism (with empty shaft)
└─ If (Player Has Gear_C)
   └─ Show Drag-Drop Hint

Event OnInstallGearC()
├─ Play Gear Install Animation
├─ Play Sound: SFX_GearInstall_Heavy
├─ Set bGearCInstalled = true
├─ Remove Item: Gear_C
├─ Start Mechanism Animation (All Gears Rotate)
├─ Play Sound: SFX_ClockMechanismStart
├─ Play Dialogue: CH1_Clock_Install
├─ Wait 2 seconds
├─ Play Camera Animation: MoveToClockFront
├─ Activate Clock Hands (Make Interactable)
├─ Play Hand Trembling Animation
└─ Play Dialogue: CH1_Clock_Ready
```

#### 阶段3: 调整指针

**UI布局**:
```
┌─────────────────────────────────────┐
│           大钟                       │
│                                     │
│          12                         │
│       11    1                       │
│    10         2                     │
│   9            3                    │
│    8         4                      │
│       7    5                        │
│          6                          │
│                                     │
│   当前时间: 12:00:00                 │
│   目标时间: ??:??:??                 │
│                                     │
│   [返回]                             │
└─────────────────────────────────────┘
```

**三个指针**:

| 指针 | 颜色 | 长度 | 旋转速度 | 目标位置 |
|------|------|------|---------|---------|
| **时针** | 金色 | 短 | 慢 | 1点 (30°) |
| **分针** | 银色 | 中 | 中 | 47分 (282°) |
| **秒针** | 红色 | 长 | 快 | 0秒 (0°) |

**操作方式**:

1. **选择指针**:
   - 玩家点击某个指针
   - 该指针高亮（发光）
   - 其他指针变暗

2. **旋转指针**:
   - 玩家拖动鼠标/手指
   - 指针跟随鼠标/手指旋转
   - 实时显示当前时间
   - 播放"滴答"声（每经过一个刻度）

3. **释放指针**:
   - 玩家松开鼠标/手指
   - 指针锁定在当前位置
   - 取消高亮

4. **验证时间**:
   - 当三个指针都设置完毕
   - 自动验证时间是否正确
   - 如果正确：触发大钟激活
   - 如果错误：指针震动，提示错误

**正确时间**: 1:47:00

- 时针：30° (1点)
- 分针：282° (47分)
- 秒针：0° (0秒)

**状态机**:
```
State: MechanismIncomplete
  └─ OnInstallGearC() -> State: ReadyToAdjust
  
State: ReadyToAdjust
  └─ OnInteract() -> State: AdjustingHands
  
State: AdjustingHands
  ├─ OnHandSelected(HandType)
  │  └─ Set SelectedHand = HandType
  ├─ OnHandRotated(Angle)
  │  ├─ Rotate SelectedHand to Angle
  │  ├─ Update Time Display
  │  └─ Play Tick Sound
  ├─ OnHandReleased()
  │  ├─ Lock Hand at Current Angle
  │  └─ Validate Time
  │     ├─ If (Time == 1:47:00)
  │     │  └─ State: Activated
  │     └─ Else
  │        └─ Play Error Feedback
  └─ OnReturn()
     └─ State: ReadyToAdjust
     
State: Activated
  ├─ Lock All Hands
  ├─ Play Clock Chime (12 times)
  ├─ Start Pendulum Swinging
  ├─ Trigger Ritual Sequence
  └─ Add Item: Note_3
  
State: Completed
  └─ Clock continues to run
```

**旋转计算**:

```cpp
// 根据鼠标/触摸位置计算指针角度

FVector2D ClockCenter = GetClockCenterScreenPosition();
FVector2D MousePosition = GetMouseScreenPosition();

FVector2D Direction = MousePosition - ClockCenter;
float Angle = FMath::Atan2(Direction.Y, Direction.X);
Angle = FMath::RadiansToDegrees(Angle);

// 调整角度，使12点为0度
Angle = (Angle + 90.0f);
if (Angle < 0) Angle += 360.0f;

// 根据指针类型，对齐到刻度
if (HandType == EHandType::Hour)
{
    // 时针：对齐到小时（30度一格）
    Angle = FMath::RoundToInt(Angle / 30.0f) * 30.0f;
}
else if (HandType == EHandType::Minute)
{
    // 分针：对齐到分钟（6度一格）
    Angle = FMath::RoundToInt(Angle / 6.0f) * 6.0f;
}
else if (HandType == EHandType::Second)
{
    // 秒针：对齐到秒（6度一格）
    Angle = FMath::RoundToInt(Angle / 6.0f) * 6.0f;
}

return Angle;
```

**时间验证**:

```cpp
bool ValidateTime()
{
    float HourAngle = GetHandAngle(EHandType::Hour);
    float MinuteAngle = GetHandAngle(EHandType::Minute);
    float SecondAngle = GetHandAngle(EHandType::Second);
    
    // 目标时间：1:47:00
    float TargetHourAngle = 30.0f;    // 1点 = 30度
    float TargetMinuteAngle = 282.0f; // 47分 = 47 * 6 = 282度
    float TargetSecondAngle = 0.0f;   // 0秒 = 0度
    
    // 允许小误差（±3度）
    float Tolerance = 3.0f;
    
    bool bHourCorrect = FMath::Abs(HourAngle - TargetHourAngle) <= Tolerance;
    bool bMinuteCorrect = FMath::Abs(MinuteAngle - TargetMinuteAngle) <= Tolerance;
    bool bSecondCorrect = FMath::Abs(SecondAngle - TargetSecondAngle) <= Tolerance;
    
    return (bHourCorrect && bMinuteCorrect && bSecondCorrect);
}
```

**蓝图实现要点**:

```
Event OnHandSelected(HandType)
├─ Set SelectedHand = HandType
├─ Highlight Selected Hand
└─ Dim Other Hands

Event Tick(DeltaTime)
├─ If (SelectedHand != None && IsMouseDown)
│  ├─ Calculate Angle from Mouse Position
│  ├─ Rotate SelectedHand to Angle
│  ├─ Update Time Display
│  └─ If (Angle crossed a tick mark)
│     └─ Play Sound: SFX_ClockTick
└─ Else
   └─ Do Nothing

Event OnMouseReleased()
├─ Lock SelectedHand at Current Angle
├─ Set SelectedHand = None
├─ Call ValidateTime()
└─ If (Time is Correct)
   └─ Call OnClockActivated()

Event OnClockActivated()
├─ Lock All Hands
├─ Play Sound: SFX_ClockChime (12 times, loop)
├─ Play Animation: PendulumStart
├─ Play Dialogue: CH1_Clock_Activated
├─ Wait for Chimes to Finish
└─ Trigger Ritual Sequence
```

---

## 8. 大钟仪式序列 (Clock Ritual Sequence)

### 基本信息

- **类型**: 脚本序列 (Scripted Sequence)
- **触发**: 完成钟表谜题
- **时长**: 约40秒
- **风格**: 宏大、仪式感

### 序列步骤

#### 步骤1: 钟声响起 (0-12秒)

**视觉**:
- 大钟的钟锤开始摆动
- 钟锤敲击钟体，发出洪亮的钟声
- 整个钟楼随着钟声震动
- 灰尘从天花板落下
- 窗户的玻璃震动

**音效**:
- 洪亮的钟声（12次，每次间隔1秒）
- 钟楼震动的"隆隆"声
- 玻璃震动的"叮当"声

**独白**:
- 播放：`CH1_Clock_Ritual_01` - "时间再次流动，心脏再次跳动。"

**蓝图实现**:
```
Sequencer Timeline:
├─ 0s: Start Pendulum Swing Animation
├─ 0-12s: Play Clock Chime Sound (12 times, 1 second apart)
│  └─ For Each Chime:
│     ├─ Trigger Camera Shake
│     ├─ Spawn Dust Particles
│     └─ Vibrate Windows
├─ 2s: Play Dialogue: CH1_Clock_Ritual_01
└─ 0-12s: Audio Mix (Chime + Rumble + Glass)
```

#### 步骤2: 窗户依次亮起 (12-20秒)

**视觉**:
- 镜头拉远，显示整个钟楼外观
- 钟楼的窗户从底层到顶层依次亮起
- 每层窗户亮起时，发出"叮"的声音
- 窗户内闪现该层的回忆画面

**窗户亮起顺序**:

| 时间 | 楼层 | 窗户内容 | 音效 |
|------|------|---------|------|
| 12s | 底层 | 齿轮转动的画面 | 叮 + 齿轮声 |
| 14s | 二层 | 音乐盒和演唱会灯光 | 叮 + 音乐片段 |
| 16s | 三层 | 幻灯机投影和植物 | 叮 + 风声 |
| 18s | 顶层 | 大钟和乌鸦剪影 | 叮 + 乌鸦鸣叫 |

**音效**:
- 窗户亮起的"叮"声
- 每层对应的环境音（齿轮、音乐、风、乌鸦）
- 钟声逐渐淡化为心跳声

**独白**:
- 播放：`CH1_Clock_Ritual_02` - "我曾害怕失去，所以试图冻结。"

**蓝图实现**:
```
Sequencer Timeline:
├─ 12s: Camera Dolly Out (Show Full Tower)
├─ 12s: Light Up Floor 1 Window
│  ├─ Play Sound: SFX_WindowLight
│  ├─ Show Memory Image: Gears
│  └─ Play Sound: SFX_Gears_Short
├─ 14s: Light Up Floor 2 Window
│  ├─ Play Sound: SFX_WindowLight
│  ├─ Show Memory Image: Music Box + Concert
│  └─ Play Sound: SFX_Music_Short
├─ 16s: Light Up Floor 3 Window
│  ├─ Play Sound: SFX_WindowLight
│  ├─ Show Memory Image: Projector + Garden
│  └─ Play Sound: SFX_Wind_Short
├─ 18s: Light Up Floor 4 Window
│  ├─ Play Sound: SFX_WindowLight
│  ├─ Show Memory Image: Clock + Raven
│  └─ Play Sound: SFX_RavenCaw_Short
├─ 14s: Play Dialogue: CH1_Clock_Ritual_02
└─ 12-20s: Audio Crossfade (Chime -> Heartbeat)
```

#### 步骤3: 乌鸦飞翔 (20-28秒)

**视觉**:
- 乌鸦从顶层窗户飞出
- 围绕钟楼盘旋而上
- 飞行轨迹形成螺旋形
- 镜头跟随乌鸦的飞行
- 乌鸦越飞越高，最终飞向天空
- 乌鸦消失在云层中

**音效**:
- 乌鸦的鸣叫声（悠长、自由）
- 翅膀扇动的声音
- 风声
- 心跳声持续

**独白**:
- 播放：`CH1_Clock_Ritual_03` - "但现在我明白，真正的永恒，是在流动中始终选择彼此。"

**蓝图实现**:
```
Sequencer Timeline:
├─ 20s: Spawn Raven at Top Window
├─ 20-28s: Raven Flight Animation
│  ├─ Follow Spline Path (Spiral Up)
│  └─ Camera Follow Raven
├─ 22s: Play Dialogue: CH1_Clock_Ritual_03
├─ 27s: Raven Fade Out (Fly into Clouds)
└─ 20-28s: Audio Mix (Raven Caw + Wings + Wind + Heartbeat)
```

#### 步骤4: 时间流动 (28-35秒)

**视觉**:
- 镜头回到钟楼外观
- 天空开始变化
- 永恒的黄昏逐渐消散
- 光线变化：黄昏 → 夜晚 → 黎明
- 星星出现又消失
- 太阳从地平线升起
- 钟楼沐浴在晨光中

**音效**:
- 时间流逝的"嗖嗖"声
- 风声
- 晨鸟的鸣叫
- 心跳声逐渐平静

**蓝图实现**:
```
Sequencer Timeline:
├─ 28s: Camera Return to Tower View
├─ 28-35s: Sky Time Lapse
│  ├─ 28-30s: Dusk -> Night (Stars Appear)
│  ├─ 30-32s: Night (Stars Twinkle)
│  ├─ 32-35s: Night -> Dawn (Sun Rises)
│  └─ Use Directional Light Rotation + Sky Sphere
├─ 33s: Spawn Morning Birds (Flying)
└─ 28-35s: Audio Mix (Time Whoosh + Wind + Birds + Heartbeat Calm)
```

#### 步骤5: 符号浮现 (35-40秒)

**视觉**:
- 镜头推近大钟
- 从大钟的中心，浮现出音符符号3
- 符号发出耀眼的白色光芒
- 符号缓慢旋转，上升
- 符号飞向玩家

**音效**:
- 和谐的音符和弦（最高潮）
- 光芒的"嗡嗡"声
- 钟声的最后一个余音
- 心跳声与钟声重叠

**蓝图实现**:
```
Sequencer Timeline:
├─ 35s: Camera Zoom to Clock Face
├─ 35-37s: Spawn Note Symbol 3 from Clock Center
├─ 37-39s: Symbol Float Up + Rotate
├─ 37-40s: Add Glow Effect to Symbol (White, Intense)
├─ 39-40s: Symbol Fly to Player (Add to Inventory)
├─ 36s: Play Harmony Chord (Climax)
└─ 40s: End Sequence, Return Control to Player
```

### 序列结束后

**行为**:
1. 玩家重新获得控制权
2. 大钟持续摆动（背景动画）
3. 钟楼的窗户保持明亮
4. 天空是黎明状态
5. 乌鸦已经飞走
6. 自动添加物品到背包：
   - 音符符号3 (`Note_3`)
7. 播放独白：`CH1_Note3_Get`

**蓝图实现**:
```
Event OnRitualSequenceEnd()
├─ Return Camera Control to Player
├─ Set Clock State: Running (Pendulum continues)
├─ Set Tower State: Illuminated (Windows stay lit)
├─ Set Sky State: Dawn
├─ Remove Raven from Scene
├─ Add Item: Note_3
├─ Play Dialogue: CH1_Note3_Get
└─ Check if Player Has All 3 Note Symbols
   └─ If (Yes)
      └─ Trigger: Three Symbols Combination
```

---

## 9. 三符号组合 (Three Symbols Combination)

### 基本信息

- **类型**: 自动触发序列
- **触发**: 玩家收集齐三个音符符号
- **时长**: 约25秒
- **风格**: 仪式感、象征性

### 触发条件检查

```cpp
// 在玩家获得任何音符符号时检查
Event OnItemAdded(ItemID)
├─ If (ItemID is Note Symbol)
│  ├─ Check Inventory for All Three Symbols
│  │  ├─ Has Note_1?
│  │  ├─ Has Note_2?
│  │  └─ Has Note_3?
│  └─ If (Has All Three)
│     └─ Trigger: Symbol Combination Sequence
└─ Else
   └─ Do Nothing
```

### 组合动画序列

#### 步骤1: 符号浮现 (0-5秒)

**视觉**:
- 画面暗下来，进入一个抽象的空间
- 玩家的背包打开（自动）
- 三个音符符号从背包中飞出
- 符号悬浮在空中，围成一个三角形
- 每个符号发出各自的光芒：
  - 符号1：温暖的金色
  - 符号2：柔和的绿色
  - 符号3：耀眼的白色

**音效**:
- 符号飞出的"嗖嗖"声
- 三个不同音高的和弦音（对应三个符号）
- 空灵的背景音

**独白**:
- 播放：`CH1_Combine_Start` - "三个符号...听觉、视觉、时间。让它们融合。"

**蓝图实现**:
```
Sequencer Timeline:
├─ 0s: Fade to Abstract Space (Dark Background)
├─ 0-1s: Open Inventory UI (Automatic)
├─ 1-3s: Spawn Three Note Symbols from Inventory
│  ├─ Note_1 flies out (Gold Glow)
│  ├─ Note_2 flies out (Green Glow)
│  └─ Note_3 flies out (White Glow)
├─ 3-5s: Symbols Arrange in Triangle Formation
├─ 2s: Play Dialogue: CH1_Combine_Start
├─ 1-5s: Play Harmony (Three Notes)
└─ 0-5s: Audio Mix (Whoosh + Harmony + Ambient)
```

#### 步骤2: 围绕旋转 (5-10秒)

**视觉**:
- 三个符号开始围绕一个中心点旋转
- 旋转速度逐渐加快
- 符号的轨迹留下发光的轨迹线
- 轨迹线形成一个圆环的形状
- 中心点开始发光

**音效**:
- 旋转的"嗡嗡"声（音高逐渐升高）
- 三个和弦音持续，音量增强
- 空间共鸣的效果

**蓝图实现**:
```
Sequencer Timeline:
├─ 5s: Symbols Start Rotating Around Center
├─ 5-10s: Increase Rotation Speed
│  └─ Speed: Lerp from 30 RPM to 120 RPM
├─ 5-10s: Draw Trail Behind Symbols
│  └─ Use Niagara Ribbon System
├─ 7-10s: Center Point Glow Increases
└─ 5-10s: Audio Mix (Hum Pitch Up + Harmony Louder)
```

#### 步骤3: 形成圆环 (10-15秒)

**视觉**:
- 符号的旋转速度达到最快
- 符号模糊成光环
- 光环凝固，形成一个实体的圆环
- 圆环上浮现出精细的纹理和符文
- 圆环缓慢旋转

**音效**:
- 凝固的"嗡"声（长音）
- 圆环锁定的"咔嚓"声
- 符文浮现的"叮叮"声

**蓝图实现**:
```
Sequencer Timeline:
├─ 10s: Symbols Blur into Light Ring
├─ 10-12s: Ring Solidifies
│  └─ Morph from Blurred Light to Solid Ring
├─ 12-15s: Ring Texture Appears
│  ├─ Reveal Alchemical Patterns
│  └─ Reveal Rune Inscriptions
├─ 13-15s: Ring Slow Rotation
└─ 10-15s: Audio Mix (Solidify Hum + Lock Click + Rune Dings)
```

#### 步骤4: 365刻度浮现 (15-20秒)

**视觉**:
- 圆环上开始浮现小刻度
- 刻度从0开始，逐渐增加
- 最终形成365个均匀分布的刻度
- 刻度以顺时针方向依次浮现
- 每个刻度浮现时发出微弱的光

**音效**:
- 刻度浮现的"滴答"声（快速，像时钟）
- 计数的声音（1, 2, 3... 加速到365）
- 背景的心跳声

**独白**:
- 播放：`CH1_Combine_365` - "365个刻度...一年的每一天。每一天都是我们故事的一部分。"

**蓝图实现**:
```
Sequencer Timeline:
├─ 15s: Start Spawning Tick Marks on Ring
├─ 15-19s: Spawn 365 Ticks (Clockwise)
│  ├─ Spawn Rate: 365 ticks in 4 seconds = ~91 ticks/second
│  └─ Each Tick: Small Glow on Appear
├─ 16s: Play Dialogue: CH1_Combine_365
├─ 15-19s: Play Tick Sounds (Fast, like Clock)
└─ 15-20s: Audio Mix (Ticks + Counting + Heartbeat)
```

#### 步骤5: 重要日子发光 (20-23秒)

**视觉**:
- 圆环上的某些刻度开始发光
- 这些刻度代表重要的日子
- 发光的刻度位置：
  - 第1个刻度：相识（金色）
  - 第107个刻度：第一次旅行（蓝色）
  - 第147个刻度：某个重要时刻（红色）
  - 第365个刻度：一周年（白色，最亮）
- 发光的刻度之间有光线连接

**音效**:
- 刻度发光的"叮"声（和谐音）
- 光线连接的"嗡"声
- 温暖的和弦

**蓝图实现**:
```
Sequencer Timeline:
├─ 20s: Highlight Important Ticks
│  ├─ Tick 1: Glow Gold
│  ├─ Tick 107: Glow Blue
│  ├─ Tick 147: Glow Red
│  └─ Tick 365: Glow White (Brightest)
├─ 20-22s: Draw Light Connections Between Highlighted Ticks
│  └─ Use Line Renderer or Beam Particles
├─ 20-23s: Play Harmony Chords (Warm)
└─ 20-23s: Audio Mix (Ding + Hum + Chords)
```

#### 步骤6: 符号融合与锁定 (23-25秒)

**视觉**:
- 三个原始符号再次显现（在圆环中心）
- 符号快速旋转，融合成一个复杂的炼金术符号
- 融合后的符号扩展，与圆环合并
- 整个结构锁定，形成最终的外层环符号
- 符号发出耀眼的光芒
- 光芒逐渐收敛，符号飞向玩家

**音效**:
- 融合的"嗡"声（洪亮）
- 锁定的"咔嚓"声（沉重）
- 最终的和弦（完满）

**蓝图实现**:
```
Sequencer Timeline:
├─ 23s: Show Three Original Symbols in Center
├─ 23-24s: Symbols Spin and Merge
│  └─ Morph into Complex Alchemical Symbol
├─ 24s: Merged Symbol Expands to Fill Ring
├─ 24-25s: Structure Locks (Final Form)
│  └─ Outer Ring Symbol Complete
├─ 24-25s: Intense Glow
├─ 25s: Symbol Flies to Player (Add to Inventory)
├─ 23-25s: Play Final Harmony (Climax)
└─ 25s: Fade Back to Game World
```

### 组合完成后

**行为**:
1. 玩家回到游戏世界（当前所在位置）
2. 从背包移除三个音符符号
3. 添加外层环符号到背包
4. 播放独白：`CH1_OuterRing_Get`
5. 自动触发炼金术士最后一页日记

**蓝图实现**:
```
Event OnCombinationEnd()
├─ Fade from Abstract Space to Game World
├─ Return Camera Control to Player
├─ Remove Items: Note_1, Note_2, Note_3
├─ Add Item: OuterRingSymbol
├─ Play Dialogue: CH1_OuterRing_Get
├─ Wait 3 seconds
└─ Trigger: Final Diary Appears
```

---

## 10. 炼金术士最后一页日记

### 触发

当玩家获得外层环符号后，自动触发。

### 行为

1. **日记浮现**:
   - 一页发光的纸页从空中缓缓降落
   - 纸页落在玩家面前
   - 自动打开阅读界面

2. **阅读日记**:
   - 显示炼金术士日记的最后一页
   - 文字逐渐浮现（打字机效果）
   - 播放独白：`CH1_DiaryFinal_Read`

3. **日记内容**:

> "1870年12月31日——一年后
>
> 我终于明白了。
>
> 我曾试图用齿轮囚禁时间，用音符封存瞬间，用光影重现过去。
>
> 但真正的永恒，不在于停止，而在于流动中的回归。
>
> 大钟必须摆动，乌鸦必须飞翔，记忆必须被允许变化。
>
> 因为**真正的永恒，是接纳每一个瞬间都会消逝，却依然选择珍惜**。
>
> 这就是我在这座钟楼里学到的：
>
> **爱不是冻结时间，而是在时间的流动中，始终选择彼此。**
>
> 现在，我要让大钟重新摆动。
>
> 让时间流动，让生命继续。
>
> 而那些美好的记忆，将不再被囚禁，而是成为守护我们的符号。
>
> ——一位曾试图囚禁永恒，最终学会放手的炼金术士"

4. **日记收起**:
   - 玩家关闭日记
   - 日记变成物品，添加到背包
   - 播放独白：`CH1_Ending_01` 和 `CH1_Ending_02`

5. **章节完成**:
   - 显示"第一章完成"提示
   - 解锁第二章

**蓝图实现**:
```
Event OnGetOuterRingSymbol()
├─ Wait 3 seconds
├─ Spawn Final Diary Page (Floating in Air)
├─ Play Animation: DiaryFloat Down
├─ Auto Open Diary Reading UI
├─ Play Typewriter Effect for Text
├─ Play Dialogue: CH1_DiaryFinal_Read (Voice Over)
├─ Wait for Player to Close Diary
├─ Add Item: Diary_Final
├─ Play Dialogue: CH1_Ending_01
├─ Wait 2 seconds
├─ Play Dialogue: CH1_Ending_02
├─ Wait 2 seconds
├─ Show Chapter Complete UI
└─ Unlock Chapter 2
```

---

## 11. 其他交互对象

### 11.1 乌鸦 (Raven)

**对象**: `BP_Raven`  
**位置**: 顶层钟塔

#### 交互状态

| 状态 | 触发条件 | 行为 |
|------|---------|------|
| **警惕** | 初始状态 | 乌鸦栖息在横梁上，眼睛跟随玩家 |
| **等待** | 玩家观察乌鸦 | 乌鸦看向玩家，又看向某处（暗示需要喂食） |
| **接受** | 玩家用机械甲虫喂食 | 乌鸦啄食甲虫，掉落钥匙 |
| **满足** | 喂食后 | 乌鸦平静地栖息，不再交互 |
| **飞走** | 大钟激活后 | 乌鸦飞向天空，离开钟楼 |

#### 眼睛跟随

```cpp
// 乌鸦的眼睛始终看向玩家
Event Tick(DeltaTime)
├─ If (State == Vigilant || State == Waiting)
│  ├─ Get Player Location
│  ├─ Calculate Direction to Player
│  ├─ Rotate Raven Head towards Player (Smooth)
│  └─ Blink Occasionally (Random Interval)
└─ Else
   └─ Do Nothing
```

#### 喂食动画

```
Event OnFeedBeetle()
├─ Play Animation: RavenLookDown
├─ Wait 0.5 seconds
├─ Play Animation: RavenPeck (Loop 3 times)
├─ Play Sound: SFX_RavenEat
├─ Destroy Mechanical Beetle
├─ Wait 1 second
├─ Play Animation: RavenSatisfied
├─ Play Sound: SFX_RavenCaw_Satisfied
├─ Spawn Music Box Key (Drop from Beak)
├─ Play Sound: SFX_KeyDrop
└─ Set State: Satisfied
```

### 11.2 望远镜 (Telescope)

**对象**: `BP_Telescope`  
**位置**: 三层观景台

#### 交互行为

1. **点击望远镜**:
   - 镜头切换到望远镜视角
   - 显示望远镜UI（圆形视野）
   - 视野聚焦在二层墙壁

2. **观察乐谱**:
   - 二层墙上的乐谱在望远镜中清晰可见
   - 乐谱显示音符序列：Mi-Sol-Mi-Do-Re-Mi-Re-Do
   - 播放独白：`CH1_Telescope_Use`

3. **退出望远镜**:
   - 玩家点击"返回"
   - 镜头切换回玩家视角

**蓝图实现**:
```
Event OnInteract()
├─ Save Player Camera Transform
├─ Switch to Telescope Camera
├─ Show Telescope UI (Circular Viewport)
├─ Focus on Floor 2 Wall (Sheet Music)
├─ Play Dialogue: CH1_Telescope_Use
└─ Wait for Player to Exit

Event OnExitTelescope()
├─ Hide Telescope UI
├─ Restore Player Camera Transform
└─ Return Control to Player
```

### 11.3 炼金术士日记 (Alchemist's Diary)

**对象**: `BP_DiaryPage` (各楼层)

#### 交互行为

1. **点击日记**:
   - 日记放大显示
   - 显示日记内容（文字）
   - 播放独白（对应的日记朗读）

2. **收集日记**:
   - 日记自动添加到背包
   - 可以在背包中重新阅读

**蓝图实现**:
```
Event OnInteract()
├─ Play Camera Animation: ZoomToDiary
├─ Show Diary Reading UI
│  └─ Display Diary Text
├─ Play Dialogue: Corresponding Diary Reading
├─ Wait for Player to Close
├─ Add Item: Diary Page
└─ Destroy Actor: BP_DiaryPage
```

---

## 12. 全局事件系统

为了确保各个谜题和交互之间的正确协调，需要一个全局事件系统。

### 事件类型

```cpp
// 事件枚举
UENUM(BlueprintType)
enum class EChapter1Event : uint8
{
    // 物品获得事件
    E_GotMechanicalBeetle,
    E_GotMusicBoxKey,
    E_GotSlide,
    E_GotGearA,
    E_GotGearB,
    E_GotGearC,
    E_GotNote1,
    E_GotNote2,
    E_GotNote3,
    E_GotOuterRingSymbol,
    
    // 谜题完成事件
    E_BeetleDissected,
    E_MusicBoxUnlocked,
    E_MusicBoxSolved,
    E_ProjectorActivated,
    E_ProjectorSolved,
    E_GearMechanismSolved,
    E_ClockGearInstalled,
    E_ClockSolved,
    E_SymbolsCombined,
    
    // 剧情事件
    E_CrowFed,
    E_TelescopeUsed,
    E_ConcertMemoryViewed,
    E_GardenMemoryViewed,
    E_ClockRitualCompleted,
    E_FinalDiaryRead,
    
    // 章节事件
    E_Chapter1Started,
    E_Chapter1Completed
};
```

### 事件管理器

```cpp
// 事件管理器（单例）
UCLASS()
class UChapter1EventManager : public UObject
{
    UPROPERTY()
    TMap<EChapter1Event, bool> EventFlags;
    
    UFUNCTION(BlueprintCallable)
    void TriggerEvent(EChapter1Event Event);
    
    UFUNCTION(BlueprintCallable)
    bool HasEventOccurred(EChapter1Event Event);
    
    UFUNCTION(BlueprintCallable)
    void ResetAllEvents();
    
    // 事件委托
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventTriggered, EChapter1Event, Event);
    UPROPERTY(BlueprintAssignable)
    FOnEventTriggered OnEventTriggered;
};
```

### 使用示例

```
// 在任何Actor中
Event OnItemPickedUp(ItemID)
├─ If (ItemID == "MechanicalBeetle")
│  └─ Call EventManager->TriggerEvent(E_GotMechanicalBeetle)
└─ Else If (ItemID == "Note_1")
   └─ Call EventManager->TriggerEvent(E_GotNote1)

// 监听事件
Event BeginPlay()
└─ Bind to EventManager->OnEventTriggered

Event OnEventTriggered(Event)
├─ If (Event == E_GotNote1 || Event == E_GotNote2 || Event == E_GotNote3)
│  └─ Check if All Three Notes Collected
│     └─ If (Yes)
│        └─ Trigger Symbol Combination
└─ Else
   └─ Handle Other Events
```

---

## 13. 保存与读取

### 需要保存的数据

```cpp
USTRUCT(BlueprintType)
struct FChapter1SaveData
{
    // 谜题状态
    UPROPERTY()
    bool bBeetleDissected;
    
    UPROPERTY()
    bool bMusicBoxUnlocked;
    
    UPROPERTY()
    bool bMusicBoxSolved;
    
    UPROPERTY()
    bool bProjectorActivated;
    
    UPROPERTY()
    bool bProjectorSolved;
    
    UPROPERTY()
    bool bGearMechanismSolved;
    
    UPROPERTY()
    bool bClockGearInstalled;
    
    UPROPERTY()
    bool bClockSolved;
    
    UPROPERTY()
    bool bSymbolsCombined;
    
    // 物品收集状态
    UPROPERTY()
    TArray<FName> CollectedItems;
    
    // 事件标记
    UPROPERTY()
    TMap<EChapter1Event, bool> EventFlags;
    
    // 玩家位置
    UPROPERTY()
    FTransform PlayerTransform;
};
```

### 保存与读取实现

```
// 保存游戏
Event SaveGame()
├─ Create Save Game Object
├─ Fill Chapter1SaveData
│  ├─ Save Puzzle States
│  ├─ Save Collected Items
│  ├─ Save Event Flags
│  └─ Save Player Transform
└─ Call UGameplayStatics::SaveGameToSlot()

// 读取游戏
Event LoadGame()
├─ Call UGameplayStatics::LoadGameFromSlot()
├─ If (Save Exists)
│  ├─ Read Chapter1SaveData
│  ├─ Restore Puzzle States
│  ├─ Restore Collected Items
│  ├─ Restore Event Flags
│  └─ Restore Player Transform
└─ Else
   └─ Start New Game
```

---

## 14. 性能优化建议

### 14.1 资源加载

- 使用 **Level Streaming** 分层加载钟楼的四层
- 玩家在某一层时，只加载相邻的层
- 使用 **LOD (Level of Detail)** 系统优化模型
- 使用 **Texture Streaming** 优化纹理内存

### 14.2 动画优化

- 过场动画使用 **Sequencer**，而非实时计算
- 复杂的粒子效果使用 **Niagara**，并设置合理的粒子数量限制
- 植物生长动画使用 **Flipbook Texture** 而非骨骼动画

### 14.3 音效优化

- 使用 **Sound Cue** 管理音效变体
- 使用 **Attenuation** 设置音效的空间衰减
- 背景音乐使用 **Streaming** 加载，而非全部加载到内存

### 14.4 蓝图优化

- 避免在 **Tick** 中进行复杂计算
- 使用 **Event Dispatcher** 而非轮询检测状态变化
- 使用 **Timer** 替代 **Delay** 节点（在循环中）

---

## 15. 测试检查清单

### 谜题功能测试

- [ ] 甲虫解剖谜题
  - [ ] 所有操作步骤正确执行
  - [ ] 错误操作有正确反馈
  - [ ] 完成后正确获得机械甲虫

- [ ] 音乐盒谜题
  - [ ] 钥匙正确解锁音乐盒
  - [ ] 错误序列有正确反馈
  - [ ] 正确序列触发过场动画
  - [ ] 完成后获得幻灯片和符号1

- [ ] 幻灯机谜题
  - [ ] 幻灯片正确放入
  - [ ] 焦距调整实时生效
  - [ ] 不同焦距显示不同内容
  - [ ] 完成后触发过场动画
  - [ ] 完成后获得符号2和密码

- [ ] 齿轮拖放谜题
  - [ ] 正确放置齿轮有反馈
  - [ ] 错误放置齿轮弹回
  - [ ] 所有齿轮正确后联动运转
  - [ ] 完成后获得大齿轮

- [ ] 钟表旋转谜题
  - [ ] 大齿轮正确安装
  - [ ] 指针可以正确旋转
  - [ ] 错误时间有反馈
  - [ ] 正确时间触发仪式序列
  - [ ] 完成后获得符号3

- [ ] 三符号组合
  - [ ] 收集齐三个符号自动触发
  - [ ] 组合动画正确播放
  - [ ] 完成后获得外层环符号

### 过场动画测试

- [ ] 小刚演唱会回忆
  - [ ] 所有镜头正确切换
  - [ ] 超现实元素正确显示
  - [ ] 音效和独白同步

- [ ] 安吉百草园回忆
  - [ ] 所有镜头正确切换
  - [ ] 植物生长动画流畅
  - [ ] 超现实元素正确显示

- [ ] 大钟仪式序列
  - [ ] 钟声正确播放12次
  - [ ] 窗户依次亮起
  - [ ] 乌鸦飞翔路径正确
  - [ ] 时间流逝效果正确

### 交互测试

- [ ] 乌鸦交互
  - [ ] 眼睛跟随玩家
  - [ ] 喂食动画正确
  - [ ] 掉落钥匙

- [ ] 望远镜交互
  - [ ] 视角切换正确
  - [ ] 可以看清乐谱

- [ ] 日记交互
  - [ ] 可以阅读所有日记
  - [ ] 日记添加到背包

### 音效测试

- [ ] 所有交互音效正确播放
- [ ] 环境音效正确循环
- [ ] 独白音效与字幕同步
- [ ] 音乐正确淡入淡出

### 性能测试

- [ ] 帧率稳定在目标值以上
- [ ] 内存使用在合理范围
- [ ] 加载时间可接受
- [ ] 无明显卡顿

### 存档测试

- [ ] 可以正确保存进度
- [ ] 可以正确读取进度
- [ ] 读取后状态正确恢复

---

## 16. 已知问题与解决方案

### 问题1: 齿轮旋转不同步

**现象**: 齿轮拖放谜题中，齿轮的旋转速度不匹配，看起来不真实。

**解决方案**: 
- 根据齿轮的齿数计算正确的速度比
- 使用 `Gear Ratio = Teeth_Driven / Teeth_Driver`
- 确保齿轮的齿对齐

### 问题2: 过场动画中断

**现象**: 过场动画播放时，玩家按键可能中断动画。

**解决方案**:
- 在过场动画开始时，禁用玩家输入
- 使用 `Set Input Mode UI Only` 或 `Disable Input`
- 动画结束后恢复输入

### 问题3: 音效重叠

**现象**: 多个音效同时播放时，声音混乱。

**解决方案**:
- 使用 **Sound Class** 管理音效分类
- 设置合理的 **Priority** 和 **Concurrency**
- 使用 **Audio Mixer** 调整音量平衡

### 问题4: 内存泄漏

**现象**: 长时间游玩后，内存占用持续增加。

**解决方案**:
- 确保所有动态生成的Actor在不需要时销毁
- 使用 **Object Pool** 管理频繁创建/销毁的对象
- 定期使用 **Garbage Collection**

---

## 17. 总结

本文档详细描述了第一章"乌鸦与钟楼"中所有谜题和交互的实现细节。每个谜题都包含：

- 完整的交互流程
- 详细的状态机
- 具体的蓝图实现建议
- 视觉和音效反馈
- 错误处理机制

在实现时，请严格按照本文档的规范进行，确保：

1. **一致性**: 所有交互遵循相同的设计模式
2. **可扩展性**: 代码结构便于后续章节的开发
3. **可维护性**: 清晰的注释和模块化的结构
4. **性能**: 优化资源加载和运行时性能
5. **体验**: 流畅的交互和丰富的反馈

如有任何疑问或需要澄清的地方，请参考：
- `Chapter1_PuzzleChart.md` - 谜题依赖图
- `Chapter1_Metaphor_System.md` - 隐喻体系
- `INTERACTION_DESIGN.md` - 交互设计总纲
- `MODULES.md` - C++模块说明

祝开发顺利！🎮✨
