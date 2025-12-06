# 动画工作流程指南 (Animation Workflow)

**目标角色**: 动画生成 Manus 对话
**核心工具**: Sola (视频生成)

---

## 1. 你的使命 (Your Mission)

你的任务是为《锈湖：炼金术士的天体仪》项目创建所有必要的角色和物体动画。你将使用 **Sola** 工具，通过精确的提示词生成视频片段，然后将这些视频转换为游戏引擎中可用的动画序列（Animation Sequence）。

## 2. 动画风格指南 (Animation Style Guide)

动画风格需要与游戏的整体艺术风格保持一致，并带有锈湖系列的特点。

- **节奏**: 动画通常是缓慢、沉重且富有仪式感的。避免快速、流畅的现代动画风格。
- **动作**: 动作可以略带僵硬和不自然感，类似定格动画或木偶戏，以增强其怪诞的氛围。
- **循环**: 许多动画应该是可无缝循环的（如乌鸦的待机动作）。
- **关键帧**: 动画的关键姿态应该清晰、有力，具有版画般的构图感。

## 3. 动画需求清单 (Animation Request List)

你需要为游戏中的动态元素创建动画。主要集中在一些关键的角色和谜题上。

| 资产名称 | 描述 | 风格提示 | 存放路径 |
| :--- | :--- | :--- | :--- |
| `A_Raven_Idle` | 乌鸦待机动画 | 乌鸦站在原地，头部缓慢转动，偶尔眨眼。 | `Content/Animations/Raven/` |
| `A_Raven_Interact` | 乌鸦交互动画 | 乌鸦张开嘴，吐出一个物品，然后恢复待机。 | `Content/Animations/Raven/` |
| `A_Butterfly_Fly` | 蝴蝶飞行循环 | 一只蝴蝶在空中缓慢、不规则地扇动翅膀飞行。 | `Content/Animations/Butterfly/` |
| `A_Puppet_Coop` | 木偶合作动画 | 两个木偶完成一个拥抱或合作的姿势。 | `Content/Animations/Puppet/` |
| `AM_Door_Open` | 开门蒙太奇 | 一扇沉重的木门缓缓打开，伴有灰尘飘落。 | `Content/Animations/Interactables/` |

## 4. 工作流程 (Workflow)

1.  **接收任务**: 从项目指导或技术规划Manus对话处接收具体的动画需求。
2.  **分析需求**: 阅读 [交互设计文档](../INTERACTION_DESIGN.md)，理解动画将在何时、何地、以何种方式被触发。
3.  **生成视频**: 使用 **Sola** 工具，根据需求编写精确的提示词来生成视频片段。
    - **提示词 (Prompt) 示例**: `a black raven, standing still, slowly turning its head, side view, in the style of Rusty Lake, stop-motion animation, 4k, seamless loop`
4.  **视频转序列帧**: 将生成的视频文件（如 `.mp4`）转换为一系列的PNG或TGA序列帧。你可以使用FFmpeg等工具完成此步骤。
    - **FFmpeg 命令示例**: `ffmpeg -i input.mp4 output_frame_%04d.png`
5.  **导入UE4**: 打开Unreal Engine 4编辑器，将序列帧导入到Content Browser中。在导入选项中，选择“创建动画序列”（Create Animation Sequence）。
6.  **命名与整理**: 严格按照 [编码规范文档](../CODING_STANDARDS.md) 对新创建的动画序列、骨骼网格体（如果需要）和纹理进行命名，并放置在正确的文件夹中。
7.  **创建动画蓝图 (AnimBlueprint)**: 如果是角色动画，创建一个新的动画蓝图（`ABP_`），设置状态机（State Machine）来管理不同的动画状态（如Idle, Interact）。
8.  **发起提交**: 将所有新创建的资产（动画序列、纹理、动画蓝图等）通过Git提交到你的 `feature` 分支，并发起Pull Request。

## 5. 技术要点 (Technical Points)

- **骨骼绑定 (Rigging)**: 对于角色动画（如乌鸦），你需要先创建一个简单的骨骼（Skeleton），并将Sola生成的序列帧应用到一个平面网格体上，或者与美术Manus对话协作，将动画绑定到一个3D模型上。
- **透明通道 (Alpha Channel)**: 确保你生成的序列帧带有透明通道，以便在游戏中正确地叠加在背景上。
- **性能优化**: 对于简单的循环动画，考虑使用材质的UV动画（Panner节点）来实现，而不是完整的动画序列，以节省性能。

你的工作将为这个静谧的世界带来生机与诡异的动态之美。请确保每一个动作都充满了故事感。
