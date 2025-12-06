# 游戏架构设计文档

**最后更新**: 2025-12-07
**负责人**: 主架构师 (Manus)

---

## 1. 概述 (Overview)

本文档定义了《锈湖：炼金术士的天体仪》项目的整体技术架构。该架构旨在创建一个模块化、可扩展且易于维护的框架，以支持游戏的核心玩法——点击式解谜，并确保C++底层与蓝图上层逻辑之间的清晰分离与高效协作。

## 2. 核心设计原则

- **模块化 (Modularity)**: 将游戏系统划分为多个高内聚、低耦合的独立模块（如交互系统、谜题系统、背包系统），每个模块负责单一的功能。
- **数据驱动 (Data-Driven)**: 游戏中的大部分内容（如物品信息、谜题配置、对话文本）将通过数据资产（Data Assets）和数据表（Data Tables）进行管理，方便策划和迭代，减少硬编码。
- **接口驱动 (Interface-Driven)**: 交互行为将通过接口（Interface）来定义，任何希望成为可交互对象的Actor只需实现该接口即可，实现高度解耦。
- **C++与蓝图分离 (C++/Blueprint Separation)**: C++负责构建稳定、高性能的核心系统和框架，而蓝图则用于实现具体的游戏逻辑、关卡设计和视觉表现，充分利用二者的优势。

## 3. 游戏框架 (Game Framework)

我们将使用Unreal Engine 4标准的游戏框架类，并进行自定义派生以满足项目需求。

| 框架类 | C++ 实现类 | 蓝图派生类 | 主要职责 |
| :--- | :--- | :--- | :--- |
| **Game Mode** | `ARustyLakeGameMode` | `BP_RustyLakeGameMode` | 管理游戏规则、章节进度、谜题状态和胜负条件。 |
| **Player Controller** | `ARustyLakePlayerController` | `BP_PlayerController` | 处理所有玩家输入（PC端鼠标点击、Android端触摸），执行射线检测以确定交互对象。 |
| **Pawn** | `APlayerPawn` | `BP_PlayerPawn` | 一个简单的Pawn，作为玩家在世界中的观察点，不包含移动逻辑，仅负责视角控制。 |
| **Game State** | `ARustyLakeGameState` | `BP_GameState` | 存储和同步全局游戏状态，如当前章节、已完成的关键事件等。 |
| **Player State** | `ARustyLakePlayerState` | `BP_PlayerState` | 存储玩家相关数据，如背包内容、已获得的线索等。 |

## 4. 模块化设计 (Modular Design)

游戏的核心功能将被拆分为以下几个C++模块。这些模块将作为插件（Plugin）或游戏模块（Game Module）存在，为上层蓝图提供强大的功能支持。

| 模块名称 | C++ 核心类 | 职责描述 |
| :--- | :--- | :--- |
| **CoreGame** | `UGameManager` | 管理游戏的主循环、章节切换、场景加载以及游戏的保存与读取。 |
| **InteractionSystem** | `UInteractionComponent`, `IInteractable` | 提供一个通用的交互框架。玩家通过`UInteractionComponent`发起交互，场景中的对象通过实现`IInteractable`接口来响应交互。 |
| **InventorySystem** | `UInventoryComponent`, `UItemDataAsset` | 管理玩家的背包。`UInventoryComponent`附加在PlayerState上，`UItemDataAsset`用于定义物品的属性。 |
| **PuzzleSystem** | `APuzzleBase`, `UPuzzleComponent` | 提供一个通用的谜题基类。所有具体的谜题（如音乐盒、幻灯机）都将继承自`APuzzleBase`，并通过事件委托（Delegates）来广播谜题的完成状态。 |
| **DialogueSystem** | `UDialogueComponent`, `UDialogueDataAsset` | 管理游戏中的对话、字幕和独白显示。文本内容由`UDialogueDataAsset`驱动。 |
| **UIManager** | `UUIManagerSubsystem` | 作为UI的中心管理器，负责创建、显示、隐藏和销毁所有UMG控件，并处理UI层级关系。 |

## 5. 关键系统详解

### 5.1 交互系统 (Interaction System)

- **交互接口 (`IInteractable`)**: 定义了一系列交互函数，如 `OnInteract()`, `OnBeginFocus()`, `OnEndFocus()`。
- **交互组件 (`UInteractionComponent`)**: 附加在PlayerController上，每帧从鼠标或触摸点发射射线，检测指向的Actor是否实现了`IInteractable`接口。如果实现，则调用相应的聚焦或交互函数。
- **交互流程**: 
  1. `PlayerController` 接收输入。
  2. `InteractionComponent` 发射射线，命中Actor。
  3. 检查Actor是否实现 `IInteractable` 接口。
  4. 如果是，调用 `OnInteract()` 函数。
  5. Actor内部实现具体的交互逻辑（如播放动画、打开UI、放入背包）。

### 5.2 谜题系统 (Puzzle System)

- **谜题基类 (`APuzzleBase`)**: 这是一个抽象的Actor基类，包含了谜题的基本状态（`Locked`, `InProgress`, `Solved`）和通用逻辑。
- **状态管理**: 每个谜题内部通过一个枚举（Enum）来管理其当前状态。
- **完成通知**: 当谜题被解决时，它会广播一个多播委托（Multicast Delegate），其他关心此事件的系统（如GameMode或其他谜题）可以绑定该委托来触发后续逻辑。
- **蓝图实现**: 具体的谜题逻辑将在继承自`APuzzleBase`的蓝图中实现，例如`BP_MusicBoxPuzzle`。

### 5.3 UI架构 (UI Architecture)

- **UI管理器 (`UUIManagerSubsystem`)**: 这是一个游戏实例子系统（Game Instance Subsystem），全局唯一。它负责管理所有UI的生命周期。
- **UI层级**: 定义了不同的UI层级，如`Game`（游戏主界面）、`Menu`（菜单）、`Popup`（弹出窗口），确保UI按正确的顺序显示。
- **核心控件**: 
  - `WBP_MainHUD`: 游戏主HUD，包含指向场景中可交互对象的提示。
  - `WBP_Inventory`: 背包UI，显示玩家拥有的物品。
  - `WBP_Dialogue`: 对话框UI。
  - `WBP_PuzzleView`: 用于展示特写谜题的UI容器。

### 5.4 数据管理 (Data Management)

- **数据资产 (`UDataAsset`)**: 用于定义复杂但单一的数据集合，如`UItemDataAsset`用于定义一个物品的所有属性（名称、描述、图标、对应的Actor类等）。
- **数据表 (`UDataTable`)**: 用于管理大量的、结构相同的行数据，如所有对话文本、关卡配置等。可以使用CSV文件进行外部编辑，方便导入导出。
- **存档系统 (`USaveGame`)**: 创建一个`URustyLakeSaveGame`类，继承自`USaveGame`。其中包含了需要被保存的所有变量（如玩家背包内容、谜题完成状态、当前所在关卡等）。游戏的保存和读取将通过序列化这个对象来完成。

## 6. 蓝图与C++协作策略

- **C++ 职责**: 
  - 定义所有核心数据结构（Structs, Enums）。
  - 构建所有核心系统和框架（如上述六大模块）。
  - 处理性能敏感的操作。
  - 提供可供蓝图扩展的基类。

- **蓝图 职责**:
  - 继承自C++基类，实现具体的游戏对象（如`BP_MusicBoxPuzzle`继承自`APuzzleBase`）。
  - 编排关卡逻辑和事件序列。
  - 创建和设计所有UI控件（UMG）。
  - 快速迭代和原型验证。

- **通信方式**: 
  - C++函数通过 `UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintNativeEvent)` 宏暴露给蓝图。
  - C++属性通过 `UPROPERTY(BlueprintReadOnly, BlueprintReadWrite)` 宏暴露给蓝图。
  - 蓝图通过函数重载（Function Overrides）和事件分发器（Event Dispatchers）与C++通信。

通过这种方式，我们可以确保项目的稳定性和性能底座，同时给予策划和设计师在蓝图中自由发挥创意的最大自由度。
