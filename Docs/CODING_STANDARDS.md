# 编码规范文档

**最后更新**: 2025-12-07
**负责人**: 主架构师 (Manus)

---

## 1. 概述 (Overview)

本规范旨在为《锈湖：炼金术士的天体仪》项目建立一套统一、清晰、可读性强的编码和内容创作标准。所有参与项目的Manus对话都必须严格遵守此规范，以确保代码质量、减少集成冲突并提高协作效率。

## 2. 通用命名规范 (General Naming Conventions)

所有资源和代码的命名都应使用**英文**，并遵循**清晰、简洁、一致**的原则。禁止使用拼音或无意义的缩写。

### 2.1 资源命名 (Asset Naming)

所有在Content Browser中创建的资源都必须添加指定的前缀，并使用**PascalCase**（大驼峰命名法）。

| 资源类型 | 前缀 | 示例 |
| :--- | :--- | :--- |
| **蓝图 (Blueprint)** | `BP_` | `BP_PlayerController` |
| **蓝图接口 (Blueprint Interface)** | `BPI_` | `BPI_Interactable` |
| **控件蓝图 (Widget Blueprint)** | `WBP_` | `WBP_MainMenu` |
| **材质 (Material)** | `M_` | `M_RustyMetal` |
| **材质实例 (Material Instance)** | `MI_` | `MI_RustyMetal_Polished` |
| **纹理 (Texture)** | `T_` | `T_Wood_D` (D: Diffuse/Albedo) |
| **法线贴图 (Normal Map)** | `T_` | `T_Wood_N` (N: Normal) |
| **遮罩纹理 (Mask Texture)** | `T_` | `T_Wood_M` (M: Mask - R:AO, G:Roughness, B:Metallic) |
| **静态网格体 (Static Mesh)** | `SM_` | `SM_ClockTower` |
| **骨骼网格体 (Skeletal Mesh)** | `SK_` | `SK_Raven` |
| **物理资产 (Physics Asset)** | `PHYS_` | `PHYS_Raven` |
| **动画序列 (Animation Sequence)** | `A_` | `A_Raven_Fly` |
| **混合空间 (Blend Space)** | `BS_` | `BS_Raven_Locomotion` |
| **动画蒙太奇 (Anim Montage)** | `AM_` | `AM_Raven_Interact` |
| **行为树 (Behavior Tree)** | `BT_` | `BT_Raven_AI` |
| **黑板 (Blackboard)** | `BB_` | `BB_Raven_AI`|
| **数据资产 (Data Asset)** | `DA_` | `DA_CrowbarItem` |
| **数据表 (Data Table)** | `DT_` | `DT_Dialogue_Chapter1` |
| **枚举 (Enum)** | `E` | `EPuzzleState` |
| **结构体 (Struct)** | `F` | `FItemInfo` |
| **音效 (Sound Wave)** | `S_` | `S_Door_Creak` |
| **音效队列 (Sound Cue)** | `SC_` | `SC_Door_Creak_Variant` |
| **关卡 (Level)** | `L_` | `L_MainMenu` |
| **粒子系统 (Particle System)** | `PS_` | `PS_DustMotes` |

### 2.2 文件夹结构 (Folder Structure)

Content Browser的文件夹结构应保持清晰、有序。所有资源都应放置在与其类型相对应的文件夹中。

```
Content/
├── Blueprints/
│   ├── Core/         (核心框架类)
│   ├── Puzzles/      (谜题相关)
│   └── Interactables/ (可交互对象)
├── Materials/
├── Textures/
├── Sounds/
├── UI/
│   ├── HUD/
│   └── Menus/
├── Animations/
│   └── Raven/
├── Maps/
│   ├── Main/         (主关卡)
│   └── Sublevels/    (子关卡)
└── Data/
    ├── Items/
    └── Dialogue/
```

## 3. C++ 编码规范

我们遵循Epic Games官方的 [C++编码标准](https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/DevelopmentSetup/CodingStandard/)，并在此基础上强调以下几点：

### 3.1 命名

- **类名**: 使用 `U` (UObject派生), `A` (AActor派生), `F` (Struct) 作为前缀，并使用PascalCase。例如 `class UInventoryComponent : public UActorComponent`。
- **函数名**: 使用PascalCase。例如 `void UpdateInteraction();`。
- **变量名**: 使用PascalCase。例如 `float InteractionDistance;`。
- **私有变量**: 可以在前面加上 `b` (bool), `In` (输入参数), `Out` (输出参数) 等前缀。例如 `bool bIsInteracting;`。

### 3.2 格式

- **大括号**: 每个大括号 `{}` 都必须另起一行。
- **缩进**: 使用4个空格进行缩进，而不是Tab。
- **指针与引用**: `*` 和 `&` 符号应紧跟在变量类型后面，而不是变量名前面。例如 `FString& MyString` 而不是 `FString &MyString`。

### 3.3 注释

- **所有公共API都必须有注释**。包括类、函数、属性和枚举。
- 使用 `/** */` 格式为函数和类添加详细的多行注释，以便生成文档。
- 使用 `//` 为代码块或单行代码添加简短的解释。

```cpp
/**
 * @brief 负责处理玩家与世界中可交互对象的交互。
 * 每帧从玩家视角发射射线，检测可交互对象。
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUSTYLAKEORRERY_API UInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    /** 交互检测的最大距离 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    float InteractionDistance = 200.0f;

    /**
     * @brief 执行一次交互检测。
     * 通常在Tick中被调用。
     */
    void UpdateInteraction();

private:
    // 当前聚焦的可交互对象
    AActor* FocusedActor = nullptr;
};
```

### 3.4 UPROPERTY 和 UFUNCTION 宏

- 所有暴露给蓝图或需要被引擎管理的属性和函数都必须使用正确的宏。
- **分类 (Category)**: 必须为所有 `UPROPERTY` 和 `UFUNCTION` 指定一个有意义的 `Category`，以便在编辑器中进行分组。
- **访问权限**: 明确指定 `BlueprintReadOnly` 或 `BlueprintReadWrite`。

## 4. 蓝图编码规范

### 4.1 可读性

- **保持整洁**: 蓝图图表必须保持整洁、有序。使用对齐工具和重新路由节点（Reroute Node）来避免连线交叉。
- **注释**: 为复杂的逻辑块添加注释框（Comment Box），解释其功能。
- **函数化**: 将可复用的逻辑封装成函数（Function）或宏（Macro）。一个独立的图表不应过长，超过20个节点的逻辑就应考虑封装。

### 4.2 命名

- **变量名**: 使用PascalCase，并保持清晰简洁。例如 `IsDoorOpen`。
- **函数名**: 使用动词开头的PascalCase。例如 `OpenDoor`。
- **事件分发器 (Event Dispatchers)**: 使用 `On` 前缀。例如 `OnPuzzleSolved`。

### 4.3 性能

- **避免在Tick中执行复杂操作**: 只有绝对必要时才使用 `Event Tick`。优先使用事件驱动（Event-Driven）的逻辑。
- **软引用 (Soft References)**: 对于不常加载的大型资源（如音效、关卡），使用软对象引用（Soft Object Pointers）或TSoftClassPtr，避免不必要的内存占用。
- **蓝图原生化 (Nativization)**: 对于性能瓶颈严重的蓝图，考虑将其转换为C++代码。

## 5. 版本控制 (Version Control)

- **Git LFS**: 本项目将使用Git LFS (Large File Storage) 来管理大型二进制文件（如`.uasset`, `.umap`）。
- **提交信息 (Commit Messages)**: 提交信息必须清晰、有意义。遵循以下格式：
  ```
  <类型>: <简短描述>
  
  [可选] 详细描述...
  ```
  - **类型**: `feat` (新功能), `fix` (bug修复), `docs` (文档), `style` (格式), `refactor` (重构), `test` (测试), `chore` (构建或工具)。
  - **示例**: `feat: Implement basic inventory system component`
- **分支策略**: 
  - `main`: 主分支，始终保持可发布状态。
  - `develop`: 开发主分支，所有功能分支从这里创建。
  - `feature/<feature-name>`: 功能开发分支。
  - `fix/<bug-name>`: Bug修复分支。

所有Manus对话在开始工作前，都必须从`develop`分支创建自己的`feature`分支。开发完成后，发起一个Pull Request到`develop`分支，并由项目指导进行代码审查（Code Review）。
