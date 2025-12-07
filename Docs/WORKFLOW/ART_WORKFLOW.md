# 美术与动画工作流程指南 (Art & Animation Workflow)

**最后更新**: 2025-12-07
**负责人**: 美术资源设计师 (Manus)
**核心工具**: NanoBanana Pro (图像生成), 图像编辑软件 (用于后期融合)

---

## 1. 核心使命 (Core Mission)

作为项目的美术资源设计师，你的核心使命是为《锈湖：炼金术士的天体仪》梳理并创作所有必要的2D美术资源及对应的逐帧动画。你将严格遵循项目的统一艺术风格，使用AI辅助生成与后期处理相结合的工作流程，高效地产出高质量的场景、道具、图标和动画序列，并确保它们在视觉和功能上与游戏设计无缝集成。

## 2. 艺术风格指南 (Art Style Guide)

所有美术资产必须严格遵循“锈湖”系列标志性的艺术风格。这是一种融合了手绘、版画质感、超现实主义和维多利亚时代美学的独特风格。

### 2.1 核心风格基石 (Core Style Foundation)

为了确保所有资产的风格统一，所有AI生成任务都必须基于以下核心风格提示词进行构建。这是保证视觉一致性的基石。

> **核心风格提示词 (Base Style Prompt):**
> `In the distinct art style of the Rusty Lake series, a detailed hand-drawn illustration with a printmaking texture. The scene is characterized by a muted and desaturated color palette, dramatic chiaroscuro lighting, and a surreal, mysterious atmosphere. Etching-style cross-hatching creates deep shadows, set against a textured, slightly worn paper background.`

### 2.2 模块化提示词库

在核心基石之上，我们已经构建了一个模块化的提示词库，用于生成不同类型的资产（如场景、道具、图标）。在开始具体工作前，请务必详细阅读该文档。

> **详细指南**: [美术风格核心提示词 (ART_STYLE_PROMPTS.md)](../ART_STYLE_PROMPTS.md)

## 3. 核心工作流程：分层生成与视觉融合

为了在实现复杂交互（如拾取物品、状态改变）的同时保证视觉的统一性，我们采用**“分层生成 + 视觉融合”**的核心工作流程。**严禁将场景与可交互物品在单张图片中一次性生成。**

关于此工作流程的详细评估与理论基础，请参考 [Manus+NanoBanana Pro工作流程评估文档](../MANUS_NANOBANANA_WORKFLOW_EVALUATION.md)。

### 3.1 第一步：生成场景底图 (Generate Scene Base Layer)

此阶段的目标是创建不包含任何可交互元素的“舞台”。

- **策略**: 在提示词中明确要求为可交互物品**预留空位**（例如，`an empty display case area`, `a blank wooden table`）。
- **光照**: 在此阶段就确定场景的主光源方向和强度，并以此为后续所有道具的光照基准。

### 3.2 第二步：生成可交互道具 (Generate Interactive Props)

此阶段独立生成需要在场景中进行交互的物品。

- **策略**: 每次只生成一个道具，使用中性或透明背景。在提示词中明确光照方向，使其与场景底图的光源**严格保持一致**。
- **多状态**: 如果一个道具有多种状态（如打开/关闭的音乐盒），需要为每种状态单独生成一张资产图，并确保它们的尺寸、角度和位置完全一致。

### 3.3 第三步：生成物品栏图标 (Generate Inventory Icons)

此阶段生成玩家拾取后在UI中显示的物品图标。

- **策略**: 图标应在保持核心风格的同时，追求**简洁和高辨识度**。使用透明背景，并确保其轮廓在小尺寸下依然清晰。

### 3.4 第四步：后期融合与交付 (Post-Fusion & Delivery)

此阶段是确保视觉统一性的关键。

- **整合**: 在图像编辑软件中，将生成的可交互道具图层叠加到场景底图上进行预览。
- **调整**: 使用色阶、曲线、投影、纹理叠加等手段，微调道具的光影和色调，使其完美融入背景。
- **交付**: **分别交付**场景底图和各个独立的道具资产图层（PNG格式），而不是合并后的单张图片。

## 4. 动画制作需求 (Animation Requirements)

游戏中的动画将采用简单的**逐帧动画**（Frame-by-Frame Animation）形式。美术资源设计师负责提供动画所需的每一帧序列图。

- **原则**: 动画应简洁而有效，用于增强交互反馈和氛围感，而非复杂的角色动画。
- **示例**: 
  - 乌鸦的歪头/眨眼 (2-3帧)
  - 植物的生长过程 (3-5帧)
  - 音乐盒的旋转舞台 (4-8帧循环)
  - 天体仪的旋转 (8-12帧循环)
- **命名**: 动画序列帧应在资产名后加上 `_Anim_` 和帧序号，例如 `Prop_Ch1_Crow_Anim_01.png`, `Prop_Ch1_Crow_Anim_02.png`。

## 5. 资产需求与命名规范 (Asset Requirements & Naming Conventions)

### 5.1 完整资产清单

游戏第一至第三章所需的全部美术资产已在以下文档中详细罗列。所有美术制作任务都应以此清单为准。

> **完整清单**: [美术资产清单 (ART_ASSET_LIST.md)](../ART_ASSET_LIST.md)

### 5.2 命名规范

所有美术资产必须严格遵循统一的命名规范，以便于引擎集成和项目管理。规范格式如下：

`{类型}_{章节}_{逻辑名称}_{状态/序号}.png`

- **类型 (Type)**: `BG` (背景), `Prop` (道具), `Icon` (图标), `FX` (特效), `Scene` (过场静态场景)。
- **章节 (Chapter)**: `Ch1`, `Ch2`, `Ch3`。
- **逻辑名称 (Logical Name)**: 资产的逻辑描述，使用帕斯卡命名法 (PascalCase)，例如 `MusicBox`, `BalanceScale`。
- **状态/序号 (State/Index)**: 可选项，用于描述多状态或动画序列，例如 `Open`, `Closed`, `Anim_01`。

**示例:**
- `BG_Chapter1_Museum_Base.png`
- `Prop_Ch1_MusicBox_Open.png`
- `Icon_Ch2_WateringCan.png`
- `Prop_Ch1_Crow_Anim_01.png`

## 6. 质量标准 (Quality Standards)

- **一致性**: 所有资源在风格、光照、色调上必须保持高度一致。
- **清晰度**: 即使是图标，也应保证在不同尺寸下都具有良好的可读性。
- **优化**: 纹理尺寸应遵循2的幂次方原则（如 512x512, 1024x1024），并根据资源的重要性和使用场景选择合适的尺寸。
- **融合感**: 最终交付的独立道具在与背景合成时，不能有明显的“贴图感”。
