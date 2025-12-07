# 第一章谜题实现指南（博物馆版）

**目标读者**: 集成指导 Manus、蓝图开发者  
**文档目的**: 提供详细的蓝图实现指南，确保谜题和交互的正确实现  
**最后更新**: 2025-12-07

---

## 📋 文档说明

本文档详细描述第一章"珠宝、埃及与音乐"中所有可交互对象的具体行为、状态机、事件流程和蓝图实现要点。每个交互都包含：

- **前置条件**: 触发交互所需的条件
- **交互流程**: 详细的步骤序列
- **状态变化**: 对象状态的转换
- **事件触发**: 需要广播的事件
- **视觉反馈**: UI和特效的表现
- **音效**: 需要播放的音频
- **蓝图实现**: 具体的实现建议

---

## 1. 场景管理器 (BP_Museum_Hall1_Manager)

### 基本信息

- **类型**: Actor
- **功能**: 管理整个展厅的状态、事件和流程
- **位置**: 场景根节点

### 状态机设计

```cpp
UENUM(BlueprintType)
enum class EHall1State : uint8
{
    Idle,                    // 初始状态
    JewelsWhispering,        // 珠宝低语中
    JewelsCaseOpened,        // 展柜已打开
    ScaleTested,             // 天平已测试
    DiaryRead,               // 日记已阅读
    MusicBoxUnlocked,        // 音乐盒已解锁
    NotesFlying,             // 音符飞舞中
    ScaleBalanced,           // 天平已平衡
    ConcertMemoryPlaying,    // 演唱会回忆播放中
    SarcophagusOpened,       // 石棺已打开
    MummyRevived,            // 木乃伊已复活
    CrowFed,                 // 乌鸦已喂食
    SymbolObtained,          // 符号已获得
    ChapterComplete          // 章节完成
};
```

### 关键事件

```cpp
// 事件委托定义
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJewelsCaseOpened);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScaleBalanced);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMusicBoxUnlocked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConcertMemoryComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMummyRevived);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSymbolObtained);

// 在蓝图中暴露
UPROPERTY(BlueprintAssignable, Category = "Events")
FOnJewelsCaseOpened OnJewelsCaseOpened;

UPROPERTY(BlueprintAssignable, Category = "Events")
FOnScaleBalanced OnScaleBalanced;

// ... 其他事件
```

### 蓝图实现要点

**Event BeginPlay**:
```
Event BeginPlay
├─ Set CurrentState = Idle
├─ Initialize All Interactive Objects
├─ Bind Event Listeners
└─ Play Opening Dialogue: CH1_Opening
```

**状态转换函数**:
```cpp
UFUNCTION(BlueprintCallable, Category = "State")
void TransitionToState(EHall1State NewState);
```

**保存/加载**:
```cpp
UFUNCTION(BlueprintCallable, Category = "Save")
void SaveProgress();

UFUNCTION(BlueprintCallable, Category = "Save")
void LoadProgress();
```

---

## 2. 卡地亚展柜 (BP_CartierCase)

### 基本信息

- **对象**: BP_CartierCase (继承自 AActor, 实现 IInteractable)
- **位置**: 展厅左侧
- **功能**: 展示三件珠宝，播放低语，打开后可拾取

### 状态机

```cpp
enum class ECartierCaseState : uint8
{
    Locked,          // 锁定状态
    Whispering,      // 低语播放中
    Opened,          // 已打开
    JewelTaken       // 珠宝已拾取
};
```

### 交互流程

#### 阶段1: 点击展柜

**触发**: 玩家点击展柜

**行为**:
1. 检查状态是否为 Locked
2. 播放点击音效: `SFX_Click_Jewel`
3. 展柜玻璃发光（材质参数动画）
4. 播放独白: `CH1_Jewels_Observe`
5. 转换状态为 Whispering
6. 开始珠宝低语序列

**蓝图实现**:
```
Event OnInteract()
├─ If (CurrentState == Locked)
│  ├─ Play Sound: SFX_Click_Jewel
│  ├─ Play Camera Animation: CameraZoom_To_Case
│  ├─ Animate Material: Glass_Glow (0 → 1, 1s)
│  ├─ Play Dialogue: CH1_Jewels_Observe
│  ├─ Set CurrentState = Whispering
│  └─ Call StartWhisperSequence()
└─ Else If (CurrentState == Opened)
   └─ Show Jewel Selection UI
```

#### 阶段2: 珠宝低语序列

**流程**:
1. 戒指闪烁 → 播放低语 → 冰霜特效
2. 等待3秒
3. 项链闪烁 → 播放低语 → 锁链特效
4. 等待3秒
5. 王冠闪烁 → 播放低语 → 枷锁特效
6. 等待2秒
7. 播放领悟独白
8. 展柜打开

**蓝图实现**:
```
Function StartWhisperSequence()
├─ Sequence Node
│  ├─ [0] Ring Whisper
│  │  ├─ Animate: Ring_Glow (闪烁)
│  │  ├─ Play Dialogue: CH1_Ring_Whisper
│  │  ├─ Spawn VFX: Frost around Ring
│  │  └─ Delay: 3s
│  ├─ [1] Necklace Whisper
│  │  ├─ Animate: Necklace_Glow (闪烁)
│  │  ├─ Play Dialogue: CH1_Necklace_Whisper
│  │  ├─ Spawn VFX: Chain around Necklace
│  │  └─ Delay: 3s
│  ├─ [2] Crown Whisper
│  │  ├─ Animate: Crown_Glow (闪烁)
│  │  ├─ Play Dialogue: CH1_Crown_Whisper
│  │  ├─ Spawn VFX: Shackle around Crown
│  │  └─ Delay: 2s
│  ├─ [3] Realization
│  │  ├─ Play Dialogue: CH1_Jewels_Realization
│  │  └─ Delay: 2s
│  └─ [4] Open Case
│     ├─ Call OpenCase()
│     └─ Broadcast Event: OnJewelsCaseOpened
```

#### 阶段3: 展柜打开

**行为**:
1. 玻璃门向上滑动（动画）
2. 播放开锁音效
3. 三件珠宝可点击
4. 状态转换为 Opened

**蓝图实现**:
```
Function OpenCase()
├─ Play Animation: GlassDoor_SlideUp
├─ Play Sound: SFX_Case_Open
├─ Set Jewels Interactable = True
├─ Set CurrentState = Opened
└─ Zoom Out Camera
```

#### 阶段4: 拾取珠宝

**行为**:
1. 玩家点击任意一件珠宝
2. 珠宝飞向背包（动画）
3. 添加到背包
4. 其他珠宝变暗（不可拾取）
5. 状态转换为 JewelTaken

**蓝图实现**:
```
Function OnJewelClicked(JewelType)
├─ Play Animation: Jewel_FlyToInventory
├─ Add Item To Inventory: JewelType
├─ Disable Other Jewels
├─ Set CurrentState = JewelTaken
└─ Play Sound: SFX_Item_Get
```

### 视觉反馈设计

#### 材质参数

- **Glass_Glow**: 玻璃发光强度 (0-1)
- **Ring_Emissive**: 戒指自发光 (0-5)
- **Necklace_Emissive**: 项链自发光 (0-5)
- **Crown_Emissive**: 王冠自发光 (0-5)

#### 特效

- **VFX_Frost**: 冰霜粒子，从戒指向外扩散
- **VFX_Chain**: 锁链纹理，环绕项链
- **VFX_Shackle**: 枷锁纹理，束缚王冠

---

## 3. 埃及天平 (BP_EgyptianScale)

### 基本信息

- **对象**: BP_EgyptianScale
- **位置**: 展厅中央
- **功能**: 审判物品重量，平衡后触发事件

### 组件结构

```
BP_EgyptianScale
├─ StaticMesh: Scale_Base (天平底座)
├─ StaticMesh: Scale_LeftPan (左秤盘，可旋转)
├─ StaticMesh: Scale_RightPan (右秤盘，可旋转)
├─ StaticMesh: Scale_Feather (真理之羽，在右盘上)
├─ PhysicsConstraint: Scale_Pivot (物理约束，模拟平衡)
└─ ParticleSystem: VFX_GoldenLight (平衡时的金光)
```

### 状态机

```cpp
enum class EScaleState : uint8
{
    Idle,           // 空闲
    Testing,        // 测试中
    Imbalanced,     // 失衡
    Balancing,      // 平衡中
    Balanced        // 已平衡
};
```

### 交互流程

#### 阶段1: 观察天平

**触发**: 玩家点击天平

**行为**:
1. 镜头推近
2. 显示天平状态
3. 播放独白: `CH1_Scale_Observe`

**蓝图实现**:
```
Event OnInteract()
├─ Play Camera Animation: CameraZoom_To_Scale
├─ Show UI: Scale_Info
│  ├─ Left Pan: Empty
│  └─ Right Pan: Feather of Truth
└─ Play Dialogue: CH1_Scale_Observe
```

#### 阶段2: 放置珠宝（失败）

**触发**: 玩家将珠宝拖到左秤盘

**行为**:
1. 珠宝飞向左秤盘
2. 天平剧烈失衡（左侧下沉）
3. 播放失衡音效
4. 珠宝周围出现暗影
5. 天平拒绝，珠宝返回背包
6. 播放独白: `CH1_Scale_Imbalance`

**蓝图实现**:
```
Function OnItemPlaced(Item)
├─ If (Item is Jewel)
│  ├─ Animate: Jewel_FlyToLeftPan
│  ├─ Set LeftPan Weight = 1000 (很重)
│  ├─ Animate: LeftPan_Rotate (下沉45度)
│  ├─ Play Sound: SFX_Scale_Imbalance
│  ├─ Spawn VFX: DarkShadow around Jewel
│  ├─ Delay: 2s
│  ├─ Play Dialogue: CH1_Scale_Imbalance
│  ├─ Delay: 3s
│  ├─ Animate: Jewel_FlyBackToInventory
│  ├─ Return Item To Inventory
│  └─ Set CurrentState = Imbalanced
└─ Else If (Item is NoteFeather)
   └─ Call OnNoteFeatherPlaced()
```

#### 阶段3: 音符落下（成功）

**触发**: 音乐盒打开后，音符自动飞向天平

**行为**:
1. 7个音符依次飞向左秤盘
2. 每个音符落下时：
   - 播放对应音高
   - 天平逐渐平衡
3. 最后一个音符落下
4. 天平完全平衡
5. 金色光芒从中心升起
6. 广播事件: OnScaleBalanced

**蓝图实现**:
```
Function OnNotesFly(Notes Array)
├─ For Each Note in Notes
│  ├─ Animate: Note_FlyToLeftPan (曲线路径)
│  ├─ Play Sound: Note_Pitch[Index]
│  ├─ Add Weight: LeftPan += 1/7
│  ├─ Animate: LeftPan_Rotate (逐渐上升)
│  └─ Delay: 0.5s
├─ When All Notes Landed
│  ├─ Set CurrentState = Balanced
│  ├─ Spawn VFX: VFX_GoldenLight (从中心升起)
│  ├─ Play Sound: SFX_Scale_Balanced
│  └─ Broadcast Event: OnScaleBalanced
```

#### 阶段4: 音符凝固

**触发**: 天平平衡后

**行为**:
1. 7个音符融合
2. 变成"音符羽毛"
3. 羽毛发光，可拾取

**蓝图实现**:
```
Function OnScaleBalanced()
├─ Delay: 2s
├─ Animate: Notes_Merge (7个音符向中心聚拢)
├─ Spawn Actor: BP_NoteFeather at LeftPan
├─ Destroy: All Note Actors
├─ Play VFX: Transformation_Flash
└─ Set NoteFeather Interactable = True
```

### 物理模拟

使用 Physics Constraint 模拟天平平衡：

```
Physics Constraint Settings:
- Angular Limits: Locked (除了Swing1)
- Swing1 Motion: Limited (-45° to +45°)
- Swing1 Spring: Enabled
  - Stiffness: 100
  - Damping: 10
```

**权重计算**:
- 真理之羽: 权重 = 1
- 珠宝: 权重 = 1000（远大于羽毛）
- 单个音符: 权重 = 1/7
- 7个音符总和: 权重 = 1（与羽毛平衡）

---

## 4. 音乐盒 (BP_MusicBox)

### 基本信息

- **对象**: BP_MusicBox
- **位置**: 展厅右侧
- **功能**: 密码解锁，播放音乐，释放音符

### 组件结构

```
BP_MusicBox
├─ StaticMesh: MusicBox_Base (底座)
├─ StaticMesh: MusicBox_Lid (盖子，可旋转)
├─ SkeletalMesh: MusicBox_Dancers (两个跳舞人偶)
├─ StaticMesh: MusicBox_Lock (密码锁)
├─ AudioComponent: MusicBox_Melody
└─ ParticleSystem: VFX_Notes (音符粒子)
```

### 状态机

```cpp
enum class EMusicBoxState : uint8
{
    Locked,         // 锁定
    Unlocking,      // 解锁中
    Opening,        // 开盖中
    Playing,        // 播放中
    NotesReleased   // 音符已释放
};
```

### 交互流程

#### 阶段1: 观察音乐盒

**触发**: 玩家点击音乐盒

**行为**:
1. 镜头推近
2. 显示密码锁（3位数字）
3. 播放独白: `CH1_MusicBox_Observe`

**蓝图实现**:
```
Event OnInteract()
├─ If (CurrentState == Locked)
│  ├─ Play Camera Animation: CameraZoom_To_MusicBox
│  ├─ Show Widget: WBP_PasswordLock
│  │  ├─ Input Field: 3 Digits
│  │  ├─ Button: Confirm
│  │  └─ Button: Cancel
│  └─ Play Dialogue: CH1_MusicBox_Observe
└─ Else If (CurrentState == Playing)
   └─ Show Optional Note Sequence Puzzle
```

#### 阶段2: 输入密码

**UI设计**:
```
┌─────────────────────────────┐
│      音乐盒密码锁            │
│                             │
│      [_] [_] [_]            │
│                             │
│   [1] [2] [3]               │
│   [4] [5] [6]               │
│   [7] [8] [9]               │
│        [0]                  │
│                             │
│   [确认]      [取消]         │
└─────────────────────────────┘
```

**蓝图实现**:
```
Widget: WBP_PasswordLock

Event OnNumberClicked(Number)
├─ If (CurrentDigits.Num() < 3)
│  ├─ Add Number to CurrentDigits
│  ├─ Update Display
│  └─ Play Sound: SFX_Button_Click
└─ Else
   └─ Play Sound: SFX_Error

Event OnConfirmClicked()
├─ Combine CurrentDigits to Password
├─ If (Password == 727)
│  ├─ Play Sound: SFX_Unlock
│  ├─ Close Widget
│  ├─ Call UnlockMusicBox()
│  └─ Play Dialogue: CH1_MusicBox_Unlock
└─ Else
   ├─ Play Sound: SFX_Password_Wrong
   ├─ Play Dialogue: CH1_Password_Wrong
   ├─ Shake Widget (错误反馈)
   └─ Clear CurrentDigits
```

#### 阶段3: 音乐盒打开

**触发**: 密码正确

**行为**:
1. 锁发光并消失
2. 盖子缓慢打开（5秒动画）
3. 内部人偶开始旋转跳舞
4. 播放音乐盒旋律
5. 状态转换为 Playing

**蓝图实现**:
```
Function UnlockMusicBox()
├─ Set CurrentState = Unlocking
├─ Animate Material: Lock_Glow (0 → 5, 1s)
├─ Delay: 1s
├─ Set Lock Visibility = False
├─ Set CurrentState = Opening
├─ Play Animation: Lid_Open (5s, Ease In-Out)
├─ Delay: 2s (盖子打开到一半时)
├─ Start Animation: Dancers_Rotate (循环)
├─ Play Audio: MusicBox_Melody (循环)
├─ Set CurrentState = Playing
└─ Delay: 3s, then Call ReleaseNotes()
```

#### 阶段4: 音符释放

**触发**: 音乐播放3秒后

**行为**:
1. 7个音符从音乐盒中飞出
2. 音符在空中飞舞（3秒）
3. 播放独白: `CH1_Notes_Appear`
4. 音符自动飞向天平

**蓝图实现**:
```
Function ReleaseNotes()
├─ For i = 0 to 6
│  ├─ Spawn Actor: BP_MusicalNote at MusicBox Location
│  │  ├─ Set Note Type = NoteTypes[i]
│  │  ├─ Set Note Color = NoteColors[i]
│  │  └─ Set Note Pitch = NotePitches[i]
│  ├─ Animate: Note_FlyUp (向上飞出)
│  └─ Delay: 0.3s
├─ Play Dialogue: CH1_Notes_Appear
├─ Delay: 3s (让音符飞舞)
├─ For Each Note
│  └─ Call Note.FlyToScale()
└─ Set CurrentState = NotesReleased
```

### 可选谜题：音符序列

**触发**: 音乐盒打开后，玩家点击音符按钮

**UI设计**:
```
┌─────────────────────────────────┐
│        音乐盒音符谜题            │
│                                 │
│   [机械鸟动画区域]               │
│                                 │
│   音符按钮:                      │
│   [Do] [Re] [Mi] [Fa]           │
│   [Sol] [La] [Si]               │
│                                 │
│   当前序列: ♪ ♪ ♪ _ _ _ _ _     │
│                                 │
│   [重置]            [跳过]       │
└─────────────────────────────────┘
```

**正确序列**: Mi-Sol-Mi-Do-Re-Mi-Re-Do

**蓝图实现**:
```
Widget: WBP_NoteSequence

Variable: CorrectSequence = [Mi, Sol, Mi, Do, Re, Mi, Re, Do]
Variable: CurrentSequence = []

Event OnNotePressed(Note)
├─ Add Note to CurrentSequence
├─ Play Sound: Note_Pitch
├─ Animate: MechanicalBird_Wing (扇动)
├─ Update Display
├─ If (CurrentSequence.Num() == CorrectSequence.Num())
│  ├─ If (CurrentSequence == CorrectSequence)
│  │  ├─ Play Sound: SFX_Sequence_Correct
│  │  ├─ Play Animation: MechanicalBird_Fly (流畅飞翔)
│  │  ├─ Spawn VFX: Extra_Notes (额外特效)
│  │  ├─ Play Dialogue: CH1_Optional_Success
│  │  └─ Close Widget
│  └─ Else
│     ├─ Play Sound: SFX_Sequence_Wrong
│     ├─ Play Animation: MechanicalBird_Struggle (挣扎)
│     ├─ Clear CurrentSequence
│     └─ Shake Widget
└─ Else If (CurrentSequence[i] != CorrectSequence[i])
   ├─ Play Sound: SFX_Sequence_Wrong
   ├─ Play Animation: MechanicalBird_Struggle
   └─ Clear CurrentSequence

Event OnSkipClicked()
├─ Close Widget
└─ Continue Main Flow
```

---

## 5. 演唱会回忆过场 (BP_ConcertMemory_Cutscene)

### 基本信息

- **类型**: Level Sequence
- **触发**: 天平平衡后
- **时长**: 20秒
- **风格**: 超现实主义

### 场景构成

#### Scene 1: 舞台全景 (3秒)

**镜头**: 从远处推向舞台

**元素**:
- 舞台背景（静态图片）
- 周传雄剪影（Sprite）
- 聚光灯扫射（光束）
- 观众光点（粒子系统）

**蓝图实现**:
```
Sequence Track: Scene1
├─ Camera: Camera_Wide
│  └─ Movement: Dolly In (远 → 近, 3s)
├─ Image: BG_Stage
├─ Sprite: Jay_Silhouette
│  └─ Material: Emissive = 2
├─ Light: Spotlight_Sweep
│  └─ Rotation: 0° → 360° (循环)
└─ Particle: Audience_Dots
   └─ Spawn Rate: 1000/s
```

#### Scene 2: 人群中的两人 (4秒)

**镜头**: 推近到观众席

**元素**:
- 模糊的人群（背景）
- 两个清晰的人影（Sprite）
- 牵着的手（特写）
- 聚光灯照在手上

**蓝图实现**:
```
Sequence Track: Scene2
├─ Camera: Camera_Medium
│  └─ Movement: Push In to Couple
├─ Image: BG_BlurryCrowd
│  └─ Material: Blur Amount = 10
├─ Sprite: Couple_Clear
│  └─ Material: Emissive = 1
├─ Sprite: Hands_Holding (特写)
│  └─ Material: Emissive = 3
└─ Light: Spotlight_OnHands
   └─ Intensity: 0 → 10 (渐亮)
```

#### Scene 3: 舞台异变 (3秒)

**镜头**: 切回舞台

**元素**:
- 周传雄变形为乌鸦（Morph动画）
- 乌鸦展翅（骨骼动画）
- 依然在唱歌（音符从嘴中飞出）

**蓝图实现**:
```
Sequence Track: Scene3
├─ Camera: Camera_Stage
├─ Skeletal Mesh: Jay_ToCrow
│  └─ Morph Target: Human → Crow (3s)
├─ Animation: Crow_Singing
│  └─ Loop: True
└─ Particle: Notes_FromMouth
   └─ Spawn: Continuous
```

#### Scene 4: 音符雪 (4秒)

**镜头**: 仰视角度

**元素**:
- 音符从天空飘落（粒子）
- 两人伸手接住（动画）
- 音符在手中发光融化

**蓝图实现**:
```
Sequence Track: Scene4
├─ Camera: Camera_LookUp
├─ Particle: Notes_Falling
│  ├─ Spawn Rate: 100/s
│  ├─ Velocity: Down
│  └─ Material: Glow + Rainbow
├─ Animation: Couple_ReachOut
└─ VFX: Note_Melt_InHands
   └─ Trigger: On Collision
```

#### Scene 5: 时间停滞 (3秒)

**镜头**: 固定

**元素**:
- 整个场景静止
- 只有两人还在动
- 周围变黑白，两人保持彩色
- 时间粒子停滞在空中

**蓝图实现**:
```
Sequence Track: Scene5
├─ Camera: Camera_Fixed
├─ Post Process: Saturation
│  └─ Desaturate: 0 → 1 (除了Couple)
├─ Time Dilation: 1 → 0.01 (慢动作)
├─ Particle: Time_Particles
│  └─ Freeze In Air
└─ Animation: Couple_SlowMotion
```

#### Scene 6: 符号浮现 (3秒)

**镜头**: 特写牵着的手

**元素**:
- 从手中浮现"过去之符"原型
- 符号旋转上升
- 发出金色光芒

**蓝图实现**:
```
Sequence Track: Scene6
├─ Camera: Camera_Closeup_Hands
├─ Spawn Actor: Symbol_Prototype
│  └─ Location: Between Hands
├─ Animation: Symbol_Rise
│  ├─ Rotation: 0° → 360° (3s)
│  └─ Position: Hands → Up (3s)
├─ Material: Symbol_Glow
│  └─ Emissive: 0 → 10
└─ Fade Out: 1s
```

### 转场回展厅

**蓝图实现**:
```
Sequence Track: Transition
├─ Fade Out: 1s (黑屏)
├─ Unload: Concert Scene
├─ Load: Museum Hall
├─ Fade In: 1s
└─ Broadcast Event: OnConcertMemoryComplete
```

### 音频设计

```
Audio Track:
├─ [0-15s] BGM: 周传雄《黄昏》片段
├─ [5-10s] SFX: 人群欢呼 (逐渐淡出)
├─ [10-15s] SFX: 音符飘落 "叮铃"
├─ [15-20s] SFX: 心跳声 (渐强)
└─ [18-20s] Fade Out All
```

---

## 6. 埃及石棺与木乃伊 (BP_Sarcophagus)

### 基本信息

- **对象**: BP_Sarcophagus
- **位置**: 展厅中央偏左
- **功能**: 天平平衡后打开，木乃伊复活

### 组件结构

```
BP_Sarcophagus
├─ StaticMesh: Sarcophagus_Base (底座)
├─ StaticMesh: Sarcophagus_Lid (盖子，可移动)
├─ SkeletalMesh: Mummy (木乃伊，骨骼动画)
├─ StaticMesh: Mummy_Chest (胸腔，可发光)
├─ Actor: BP_MummyNote (纸条)
└─ ParticleSystem: VFX_Heart_Beat (心跳粒子)
```

### 状态机

```cpp
enum class ESarcophagusState : uint8
{
    Closed,         // 封闭
    Opening,        // 开启中
    Opened,         // 已打开
    MummyReviving,  // 木乃伊复活中
    MummyRevived    // 木乃伊已复活
};
```

### 交互流程

#### 阶段1: 观察石棺（封闭状态）

**触发**: 玩家点击石棺

**行为**:
1. 镜头推近
2. 显示象形文字
3. 播放独白: `CH1_Sarcophagus_Observe`

**蓝图实现**:
```
Event OnInteract()
├─ If (CurrentState == Closed)
│  ├─ Play Camera Animation: CameraZoom_To_Sarcophagus
│  ├─ Show UI: Hieroglyphs_Display
│  └─ Play Dialogue: CH1_Sarcophagus_Observe
└─ Else If (CurrentState == Opened)
   └─ Show Mummy Interaction
```

#### 阶段2: 使用放大镜

**触发**: 玩家用放大镜观察象形文字

**行为**:
1. 象形文字发光
2. 显示翻译文本
3. 播放对话: `CH1_Hieroglyphs_Translation`

**蓝图实现**:
```
Function OnUseMagnifier()
├─ Animate Material: Hieroglyphs_Glow (0 → 3)
├─ Show Widget: WBP_Translation
│  └─ Text: CH1_Hieroglyphs_Translation
└─ Play Dialogue: CH1_Hieroglyphs_Translation
```

#### 阶段3: 自动打开

**触发**: 天平平衡后（监听事件）

**行为**:
1. 石棺震动
2. 盖子缓慢滑开（10秒）
3. 播放轰隆声
4. 尘埃飞扬
5. 露出木乃伊

**蓝图实现**:
```
Event OnScaleBalanced() [Bound to Manager Event]
├─ Set CurrentState = Opening
├─ Play Sound: SFX_Sarcophagus_Open
├─ Shake Actor (震动)
├─ Spawn VFX: Dust_Cloud
├─ Play Animation: Lid_SlideOpen (10s)
├─ Delay: 10s
├─ Set CurrentState = Opened
├─ Set Mummy Visible = True
└─ Set MummyNote Interactable = True
```

#### 阶段4: 阅读纸条

**触发**: 玩家点击木乃伊手中的纸条

**行为**:
1. 纸条发光
2. 显示文字
3. 播放对话: `CH1_MummyNote_Read`

**蓝图实现**:
```
BP_MummyNote: Event OnInteract()
├─ Animate Material: Note_Glow
├─ Show Widget: WBP_NoteText
│  └─ Text: "将真实的心脏放入胸腔..."
├─ Play Dialogue: CH1_MummyNote_Read
└─ Set Chest Glow = True (提示玩家)
```

#### 阶段5: 放置音符羽毛

**触发**: 玩家将音符羽毛拖到胸腔

**行为**:
1. 羽毛飞入胸腔
2. 羽毛发光变形
3. 变成跳动的心脏
4. 播放心跳音效

**蓝图实现**:
```
Function OnItemPlaced(Item)
├─ If (Item == NoteFeather)
│  ├─ Animate: Feather_FlyToChest
│  ├─ Delay: 1s
│  ├─ Spawn VFX: Transformation_Flash
│  ├─ Destroy: Feather Actor
│  ├─ Spawn Actor: BP_MusicalHeart at Chest
│  ├─ Play Animation: Heart_Beat (循环)
│  ├─ Play Sound: SFX_Heart_Beat (循环)
│  └─ Call ReviveMummy()
```

#### 阶段6: 木乃伊复活

**触发**: 心脏开始跳动

**行为**:
1. 木乃伊眼睛睁开（发光）
2. 木乃伊坐起（骨骼动画）
3. 木乃伊转头看向玩家
4. 播放对话: `CH1_Mummy_Revive`
5. 木乃伊递出真理之羽
6. 木乃伊躺回，石棺合上

**蓝图实现**:
```
Function ReviveMummy()
├─ Set CurrentState = MummyReviving
├─ Delay: 2s (心跳2秒)
├─ Play Animation: Mummy_EyesOpen
│  └─ Material: Eyes_Emissive (0 → 5)
├─ Delay: 1s
├─ Play Animation: Mummy_SitUp (5s)
├─ Delay: 3s
├─ Play Animation: Mummy_TurnHead (2s)
├─ Play Dialogue: CH1_Mummy_Revive
├─ Delay: 10s (对话时长)
├─ Spawn Actor: BP_TruthFeather at Mummy Hand
├─ Animate: Feather_FlyToPlayer
├─ Add Item To Inventory: TruthFeather
├─ Delay: 2s
├─ Play Animation: Mummy_LieDown (5s)
├─ Play Animation: Mummy_EyesClose
├─ Play Animation: Lid_Close (5s)
├─ Set CurrentState = MummyRevived
└─ Broadcast Event: OnMummyRevived
```

---

## 7. 乌鸦 (BP_Crow)

### 基本信息

- **对象**: BP_Crow
- **位置**: 横梁上
- **功能**: 接受真理之羽，给予过去之符

### 组件结构

```
BP_Crow
├─ SkeletalMesh: Crow_Body (骨骼动画)
├─ ParticleSystem: VFX_Feather_Glow (羽毛发光)
└─ AudioComponent: Crow_Caw (鸣叫音效)
```

### 状态机

```cpp
enum class ECrowState : uint8
{
    Idle,       // 空闲
    Observing,  // 观察中
    Waiting,    // 等待喂食
    Eating,     // 进食中
    Satisfied   // 满足
};
```

### 动画状态机

```
Idle Animation:
├─ Idle_Perch (栖息)
├─ Idle_HeadTurn (转头)
└─ Idle_Preen (理羽毛)

Observing Animation:
├─ Look_At_Scale
├─ Look_At_Sarcophagus
└─ Look_At_Player

Eating Animation:
├─ Peck_Down (低头啄食)
└─ Swallow (吞咽)

Satisfied Animation:
├─ Wings_Spread (展翅)
└─ Shake_Feathers (抖羽毛)
```

### 交互流程

#### 阶段1: 观察乌鸦

**触发**: 玩家点击乌鸦

**行为**:
1. 镜头推近
2. 乌鸦转头看向玩家
3. 播放鸣叫: "嘎——"
4. 播放独白: `CH1_Crow_Observe`

**蓝图实现**:
```
Event OnInteract()
├─ Play Camera Animation: CameraZoom_To_Crow
├─ Play Animation: Crow_HeadTurn_ToPlayer
├─ Play Sound: SFX_Crow_Caw
├─ Play Dialogue: CH1_Crow_Observe
└─ Set CurrentState = Observing
```

#### 阶段2: 乌鸦的眼神提示

**触发**: 木乃伊复活后（监听事件）

**行为**:
1. 乌鸦依次看向：天平 → 石棺 → 玩家
2. 眼神中有期待
3. 状态转换为 Waiting

**蓝图实现**:
```
Event OnMummyRevived() [Bound to Manager Event]
├─ Set CurrentState = Waiting
├─ Sequence:
│  ├─ [0] Look At Scale
│  │  ├─ Play Animation: Crow_LookAt_Scale
│  │  └─ Delay: 2s
│  ├─ [1] Look At Sarcophagus
│  │  ├─ Play Animation: Crow_LookAt_Sarcophagus
│  │  └─ Delay: 2s
│  └─ [2] Look At Player
│     ├─ Play Animation: Crow_LookAt_Player
│     └─ Material: Eyes_Emissive (增强)
```

#### 阶段3: 喂食乌鸦

**触发**: 玩家将真理之羽拖到乌鸦

**行为**:
1. 羽毛飞向乌鸦
2. 乌鸦低头啄食
3. 羽毛在嘴中发光消失
4. 乌鸦满足鸣叫
5. 状态转换为 Eating

**蓝图实现**:
```
Function OnItemPlaced(Item)
├─ If (Item == TruthFeather)
│  ├─ Set CurrentState = Eating
│  ├─ Animate: Feather_FlyToCrow
│  ├─ Play Animation: Crow_PeckDown
│  ├─ Delay: 1s
│  ├─ Attach Feather To Beak
│  ├─ Animate Material: Feather_Glow (0 → 10)
│  ├─ Delay: 1s
│  ├─ Destroy: Feather Actor
│  ├─ Play Animation: Crow_Swallow
│  ├─ Play Sound: SFX_Crow_Satisfied
│  ├─ Play Dialogue: CH1_Crow_Satisfied
│  └─ Call GiveSymbol()
```

#### 阶段4: 给予符号

**触发**: 乌鸦吃完羽毛

**行为**:
1. 乌鸦展开翅膀
2. 从翅膀下掉落发光的羽毛
3. 羽毛飘落到地面
4. 状态转换为 Satisfied

**蓝图实现**:
```
Function GiveSymbol()
├─ Set CurrentState = Satisfied
├─ Play Animation: Crow_WingsSpread
├─ Delay: 1s
├─ Spawn Actor: BP_CrowFeather at Wing Location
├─ Animate: Feather_Fall (物理模拟)
├─ On Feather Landed:
│  ├─ Set Feather Interactable = True
│  └─ Spawn VFX: Landing_Glow
```

#### 阶段5: 拾取羽毛，变成符号

**触发**: 玩家点击地面上的羽毛

**行为**:
1. 羽毛飞起
2. 变形成"过去之符"
3. 符号旋转，发光
4. 播放独白: `CH1_PastSymbol_Get`
5. 添加到背包

**蓝图实现**:
```
BP_CrowFeather: Event OnInteract()
├─ Play Camera Animation: CameraZoom_To_Feather
├─ Animate: Feather_RiseUp
├─ Delay: 1s
├─ Spawn VFX: Transformation_Flash
├─ Morph: Feather → Symbol (2s)
├─ Animate: Symbol_Rotate (0° → 360°, 2s)
├─ Material: Symbol_Emissive (0 → 10)
├─ Play Sound: SFX_Symbol_Appear
├─ Play Dialogue: CH1_PastSymbol_Get
├─ Delay: 5s (独白时长)
├─ Animate: Symbol_FlyToInventory
├─ Add Item To Inventory: PastSymbol
├─ Destroy: Self
└─ Broadcast Event: OnSymbolObtained
```

---

## 8. 其他交互对象

### 8.1 放大镜 (BP_Magnifier)

**功能**: 观察细节

**蓝图实现**:
```
BP_Magnifier: Event OnInteract()
├─ Add Item To Inventory: Magnifier
├─ Play Sound: SFX_Item_Get
└─ Destroy: Self

Player Controller: Function UseMagnifier(Target)
├─ If (Target has IMagnifiable Interface)
│  ├─ Show Widget: WBP_MagnifierView
│  │  ├─ Render Target: Magnified View
│  │  └─ Zoom Level: 2x
│  └─ Call Target.OnMagnified()
```

### 8.2 炼金术士日记 (BP_AlchemistDiary)

**功能**: 提供线索

**蓝图实现**:
```
BP_AlchemistDiary: Event OnInteract()
├─ Play Camera Animation: CameraZoom_To_Diary
├─ Show Widget: WBP_DiaryPage
│  ├─ Background: Yellowed Paper
│  ├─ Text: CH1_Diary_Read (手写字体)
│  ├─ Highlight: "音乐盒" (发光)
│  └─ Button: Close
├─ Play Dialogue: CH1_Diary_Read
└─ On Close:
   ├─ Zoom Out Camera
   └─ Highlight MusicBox in Scene (微光)
```

### 8.3 演唱会海报 (BP_ConcertPoster)

**功能**: 提供密码线索

**蓝图实现**:
```
BP_ConcertPoster: Event OnInteract()
├─ If (Player Has Magnifier)
│  ├─ Show Widget: WBP_PosterView
│  │  ├─ Image: Blurry Poster
│  │  ├─ Button: Use Magnifier
│  │  └─ Button: Close
│  └─ On Use Magnifier:
│     ├─ Animate: Poster_Sharpen (模糊 → 清晰)
│     ├─ Highlight: "7月27日" (金色)
│     └─ Play Sound: SFX_Clue_Found
└─ Else
   └─ Show Dialogue: "太模糊了,需要放大镜"
```

### 8.4 展厅门 (BP_HallDoor)

**功能**: 通往第二章

**蓝图实现**:
```
BP_HallDoor: Event OnSymbolObtained() [Bound to Manager Event]
├─ Set CurrentState = Unlocked
├─ Animate Material: Door_Glow (0 → 3)
├─ Play Sound: SFX_Door_Unlock
└─ Set Interactable = True

Event OnInteract()
├─ If (CurrentState == Unlocked)
│  ├─ Play Dialogue: CH1_Ending_1
│  ├─ Delay: 4s
│  ├─ Play Dialogue: CH1_Ending_2
│  ├─ Delay: 5s
│  ├─ Play Animation: Door_Open
│  ├─ Fade Out Screen
│  ├─ Show Chapter Complete UI
│  └─ Load Next Level: Museum_Hall2
```

---

## 9. UI系统

### 9.1 对话系统 (WBP_DialogueBox)

**设计**:
```
┌─────────────────────────────────────┐
│                                     │
│  [说话者头像]  [说话者名称]         │
│                                     │
│  "对话文本内容..."                  │
│                                     │
│              [继续] (可选)           │
└─────────────────────────────────────┘
```

**蓝图实现**:
```
Widget: WBP_DialogueBox

Function ShowDialogue(DialogueData)
├─ Set Speaker Name: DialogueData.Speaker
├─ Set Speaker Portrait: DialogueData.Portrait
├─ Set Dialogue Text: DialogueData.Text
├─ Play Audio: DialogueData.AudioPath
├─ If (DialogueData.Type == Monologue)
│  ├─ Auto Close After Duration
│  └─ Hide Continue Button
└─ Else
   ├─ Show Continue Button
   └─ Wait For Player Input
```

### 9.2 提示系统 (WBP_HintSystem)

**设计**:
```
┌─────────────────────────────────────┐
│  💡 需要帮助吗?                      │
│                                     │
│  [查看提示] [我再想想]               │
└─────────────────────────────────────┘
```

**蓝图实现**:
```
Widget: WBP_HintSystem

Variable: CurrentPuzzle
Variable: HintLevel = 0
Variable: TimeStuck = 0

Event Tick
├─ If (Player Is Stuck)
│  ├─ TimeStuck += DeltaTime
│  └─ If (TimeStuck > 30s && HintLevel == 0)
│     └─ Show Hint Offer
```

### 9.3 背包系统 (WBP_Inventory)

**设计**:
```
┌─────────────────────────────────────┐
│          背包                        │
│                                     │
│  [物品1] [物品2] [物品3] ...        │
│                                     │
│  物品名称: XXX                       │
│  物品描述: ...                       │
│                                     │
│  [使用] [检查] [关闭]                │
└─────────────────────────────────────┘
```

**蓝图实现**:
```
Widget: WBP_Inventory

Function AddItem(Item)
├─ Add Item to Items Array
├─ Create Item Icon Widget
├─ Add to Inventory Grid
└─ Play Animation: Item_FlyIn

Function UseItem(Item)
├─ If (Player Is Targeting Object)
│  ├─ Call Object.OnItemUsed(Item)
│  └─ If (Item Is Consumed)
│     └─ Remove Item from Inventory
└─ Else
   └─ Show Message: "选择一个目标"
```

---

## 10. 性能优化建议

### 10.1 资源加载

```cpp
// 预加载关键资源
void AMuseumHall1Manager::BeginPlay()
{
    Super::BeginPlay();
    
    // 预加载过场动画
    PreloadAsset("/Game/Sequences/ConcertMemory");
    
    // 预加载音频
    PreloadAsset("/Game/Audio/BGM/MusicBox_Melody");
    
    // 预加载粒子系统
    PreloadAsset("/Game/VFX/Notes_Flying");
}
```

### 10.2 粒子优化

- **音符粒子**: 限制最大数量为50
- **尘埃粒子**: 使用GPU粒子
- **光束**: 使用简单的Mesh代替粒子

### 10.3 动画优化

- **木乃伊复活**: 使用LOD，远处简化骨骼
- **乌鸦动画**: 使用动画蓝图的状态机
- **人偶旋转**: 使用简单的旋转组件，不用骨骼动画

---

## 11. 测试检查清单

### 11.1 功能测试

- [ ] 所有可交互对象都能正常点击
- [ ] 放大镜能正确使用在所有目标上
- [ ] 密码输入正确/错误都有反馈
- [ ] 物品能正确拖放到目标位置
- [ ] 珠宝放在天平上会被拒绝
- [ ] 音符能自动飞向天平并平衡
- [ ] 演唱会过场正常播放
- [ ] 石棺在正确时机打开
- [ ] 木乃伊复活动画流畅
- [ ] 乌鸦能接受真理之羽
- [ ] 符号正确生成并添加到背包
- [ ] 展厅门在获得符号后打开
- [ ] 章节正确结束并过渡到第二章

### 11.2 音频测试

- [ ] 所有对话音频正常播放
- [ ] 背景音乐循环正常
- [ ] 音效与动作同步
- [ ] 音乐盒旋律正确
- [ ] 演唱会音乐正常
- [ ] 心跳声与动画同步

### 11.3 视觉测试

- [ ] 所有材质正确显示
- [ ] 发光效果正常
- [ ] 粒子系统正常
- [ ] 动画流畅无卡顿
- [ ] 镜头移动平滑
- [ ] UI显示正确

### 11.4 逻辑测试

- [ ] 谜题必须按正确顺序完成
- [ ] 跳过可选内容不影响主线
- [ ] 物品使用逻辑正确
- [ ] 事件触发条件正确
- [ ] 状态转换正确
- [ ] 保存/加载功能正常

---

## ✨ 总结

这份实现指南提供了：

1. **详细的蓝图结构**: 每个对象的组件和状态机
2. **完整的交互流程**: 从触发到完成的每一步
3. **具体的代码示例**: 伪代码展示实现逻辑
4. **视觉和音频设计**: 特效、动画、音效的详细说明
5. **性能优化建议**: 确保流畅运行
6. **测试检查清单**: 确保质量

开发者可以直接基于这份文档，在Unreal Engine中实现第一章的所有内容。✨
