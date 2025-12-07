# 美术工作流程指南 (Art Workflow) - V2.1

**文档版本**: 2.1  
**目标角色**: 美术资源设计师  
**核心工具**: AI 图像生成 (如 Midjourney, Stable Diffusion), Adobe Photoshop  
**最后更新**: 2025-12-07

---

## 1. 你的使命 (Your Mission)

你的核心使命是为《锈湖：炼金术士的天体仪》项目创作所有必要的2D美术资源。你将严格遵循项目的艺术风格，利用AI辅助生成与后期手动绘制相结合的工作流，高效产出高质量的场景、UI元素、道具图标和动画序列帧。

## 2. 艺术风格指南 (Art Style Guide)

你必须严格遵循 **“锈湖” (Rusty Lake)** 系列的标志性艺术风格。关键元素包括：

- **色调**: 以暗沉、低饱和度的颜色为主，如深棕、灰、黑、铁锈色。使用血红、湖蓝、暗金作为点缀色来突出关键元素。
- **质感**: 强烈的版画和手绘质感。线条不要求绝对平滑，可以有自然的瑕疵和粗糙感，多用交叉影线（cross-hatching）来表现阴影。
- **主题**: 维多利亚时代的复古感、超现实主义、怪诞的动植物组合、炼金术符号、机械装置。
- **光影**: 光影对比强烈，常用聚光灯效果来突出场景中的关键区域，大面积的暗部是常态。
- **参考**: 在开始创作前，请务必研究《锈湖》系列游戏（如《Rusty Lake Hotel》、《Cube Escape》系列）的截图和艺术设定。

## 3. 核心生成方法论

根据项目实践，我们确立了以“分层生成”为核心的方法论，以确保资产的灵活性和一致性。

### 3.1 场景与道具：完整生成 + 元素消除法

此方法为制作带有可交互道具的场景的标准流程。

1.  **生成完整场景**: 首先，生成一张包含所有静态和可交互元素的完整场景图。在提示词中需明确描述所有元素的位置、形状和光照关系。
2.  **消除可交互元素**: 使用图像编辑软件（如 Photoshop 的“内容感知填充”），从完整场景图中移除所有可交互的道具，生成纯净的“背景层”。
3.  **独立生成道具**: 为每个被移除的道具独立生成带透明背景的资产。生成时必须严格遵循原始场景的光照方向、透视和风格。
4.  **验证与融合**: 将独立生成的道具叠加回“背景层”进行验证，确保完美融合，没有“贴图感”。

### 3.2 多状态与动画：参考图生成法

对于需要多个状态（如开/关的盒子）或动画序列（如植物生长）的资产，采用此方法保证一致性。

1.  **生成基准状态/关键帧**: 首先生成该资产的初始状态（如关闭的盒子）或动画的起始/结束帧。
2.  **参考生成衍生状态**: 使用AI工具的“图像参考”功能，将基准状态的图片作为参考，在提示词中仅描述需要变化的部分（如“the same box, now OPEN”），并强调“保持完全相同的角度、尺寸和光照”。
3.  **生成中间帧**: 对于动画，可同时参考起始帧和结束帧，生成中间过渡帧，以确保动画的流畅性。

## 4. 标准化提示词体系

为保证风格统一，所有资产生成都必须基于以下核心风格基石，并根据资产类型添加相应模块。

- **核心风格基石 (Core Style Foundation)**
  ```
  In the distinct art style of the Rusty Lake series, a detailed hand-drawn illustration with a printmaking texture. The scene is characterized by a muted and desaturated color palette, dramatic chiaroscuro lighting, and a surreal, mysterious atmosphere. Etching-style cross-hatching creates deep shadows, set against a textured, slightly worn paper background.
  ```

- **场景类模块 (Scene Module)**
  - `2D side-scrolling game background of [scene description].`
  - `The scene features: [list of elements and their positions].`

- **道具类模块 (Prop Module)**
  - `a single object: [detailed prop description].`
  - `centered on a transparent background.`
  - `Game asset prop design with clear silhouette.`

- **图标类模块 (Icon Module)**
  - `Clean 2D icon of [item name].`
  - `minimal cross-hatching for texture.`
  - `simple and recognizable even at small sizes.`

## 5. 美术资产需求清单 (V2.1)

以下是根据第一至第三章游戏设计文档（Optimized版）梳理的详细美术资产需求清单。请严格按照此清单进行制作。

### 5.1 第一章：珠宝、埃及与音乐

**场景：博物馆第一展厅 - 时间的结晶**

| 资产编号 | 资产名称 | 描述 | 状态/帧数 | 优先级 | 复用状态 |
|---|---|---|---|---|---|
| BG_Ch1_001 | BG_Museum_Hall1 | 展厅背景全景 | P0 | ✅ 已有基底 |
| OBJ_Ch1_001 | OBJ_Cartier_Case | 卡地亚展柜 | 2个状态 (开/关) | P0 | ⚠️ 仅有关闭状态 |
| OBJ_Ch1_002 | OBJ_Ring / Necklace / Crown | 三件珠宝 | 各3帧闪烁动画 | P0 | ❌ 待制作 |
| OBJ_Ch1_003 | OBJ_Sarcophagus | 埃及石棺 | 5帧开合动画 | P0 | ⚠️ 仅有关闭状态 |
| OBJ_Ch1_004 | OBJ_Mummy | 木乃伊 | 8帧复活动画 | P1 | ❌ 待制作 |
| OBJ_Ch1_005 | OBJ_Scale | 埃及审判天平 | 5帧平衡动画 | P0 | ✅ 可直接复用 |
| OBJ_Ch1_006 | OBJ_MusicBox | 音乐盒 | 10帧开盖+旋转动画 | P0 | ⚠️ 仅有关闭状态 |
| OBJ_Ch1_007 | OBJ_Crow | 乌鸦 | 8帧 (idle+啄食) | P1 | ✅ 可作为基准帧 |
| OBJ_Ch1_008 | OBJ_Poster_Concert | 演唱会海报 | 2个状态 (模糊/清晰) | P1 | ❌ 待制作 |
| ITEM_Ch1_001 | ITEM_Magnifier | 放大镜 | 道具+图标 | P0 | ✅ 可直接复用 |
| ITEM_Ch1_002 | ITEM_Note_Feather | 音符羽毛 | 道具+图标 | P0 | ✅ 可直接复用 |
| ITEM_Ch1_003 | ITEM_Truth_Feather | 真理之羽 | 道具+图标 | P0 | ✅ 可直接复用 |
| ITEM_Ch1_004 | ITEM_Symbol_Past | 过去之符 | 图标 | P0 | ✅ 可直接复用 |

### 5.2 第二章：成长与分离

**场景：博物馆第二展厅 - 生长的悖论**

| 资产编号 | 资产名称 | 描述 | 状态/帧数 | 优先级 | 复用状态 |
|---|---|---|---|---|---|
| BG_Ch2_001 | BG_Museum_Hall2 | 展厅背景全景 | P0 | ❌ 待制作 |
| OBJ_Ch2_001 | PLANT_Sunflower | 向日葵 | 5帧生长动画 | P0 | ❌ 待制作 |
| OBJ_Ch2_002 | PLANT_WaterLily | 睡莲 | 5帧生长动画 | P0 | ❌ 待制作 |
| OBJ_Ch2_003 | PLANT_MoonFlower | 月见草 | 5帧生长动画 | P0 | ❌ 待制作 |
| OBJ_Ch2_004 | PLANT_Rose | 蔷薇 | 5帧生长动画 | P0 | ❌ 待制作 |
| OBJ_Ch2_005 | OBJ_Clock | 时钟 | 交互 | P0 | ❌ 待制作 |
| OBJ_Ch2_006 | OBJ_BigMirror | 大镜子 | 5帧破裂动画 | P0 | ❌ 待制作 |
| OBJ_Ch2_007 | CHAR_RedPuppet / BluePuppet | 红/蓝人偶 | 各3帧移动动画 | P0 | ❌ 待制作 |
| OBJ_Ch2_008 | CHAR_Butterfly | 蝴蝶 | 骨骼动画 | P0 | ❌ 待制作 |
| ITEM_Ch2_001 | ITEM_Flower_Red / Blue | 红/蓝花朵 | 图标 | P0 | ❌ 待制作 |
| ITEM_Ch2_002 | ITEM_Symbol_Present | 现在之符 | 图标 | P0 | 🎨 可参考过去之符 |

### 5.3 第三章：流动的永恒

**场景：博物馆核心大厅 - 时间的交响**

| 资产编号 | 资产名称 | 描述 | 状态/帧数 | 优先级 | 复用状态 |
|---|---|---|---|---|---|
| BG_Ch3_001 | BG_Museum_Core_Hall | 核心大厅背景全景 | P0 | ❌ 待制作 |
| OBJ_Ch3_001 | OBJ_Painting_Future | 未来画作 | 静态+动画 | P0 | ❌ 待制作 |
| OBJ_Ch3_002 | OBJ_CraneMachine | 娃娃机 | 动画 | P0 | ❌ 待制作 |
| OBJ_Ch3_003 | OBJ_TicketMachine | 彩票兑换机 | 交互界面 | P1 | ❌ 待制作 |
| OBJ_Ch3_004 | OBJ_Orrery | 天体仪核心 | 动画 | P0 | ❌ 待制作 |
| OBJ_Ch3_005 | OBJ_Clock_Main | 天体仪大钟 | 交互 | P0 | ❌ 待制作 |
| ITEM_Ch3_001 | ITEM_Gear_Copper / Silver / Gold | 铜/银/金齿轮 | 图标 | P0 | ❌ 待制作 |
| ITEM_Ch3_002 | ITEM_Palette_Eternal | 永恒调色盘 | 图标 | P0 | ❌ 待制作 |
| ITEM_Ch3_003 | ITEM_Symbol_Future | 未来之符 | 图标 | P0 | 🎨 可参考过去之符 |
| ITEM_Ch3_004 | ITEM_Symbol_Eternal | 永恒之符 | 图标 | P0 | 🎨 可参考过去之符 |

*注：详细清单请参考 `Docs/ART_ASSETS_LIST.md`。*

## 6. 资产复用指南 (Asset Reuse Guide)

为最大化效率并保证项目风格的绝对一致性，**在创建任何新资产之前，必须优先检查是否已有可复用的资源**。

### 6.1 可复用资产来源

项目的 `feature/art-ui` 分支已完成第一章部分核心资产的制作，这些资产质量优秀，可直接复用或作为创作新资产的基准。

- **位置**: `feature/art-ui` 分支下的 `Content/Art/Chapter1/` 目录。
- **状态**: 第一章约50%的核心资产已完成，特别是所有物品栏图标（Icons）均已制作完毕。

### 6.2 复用策略

1.  **直接复用 (Direct Reuse)**:
    - **第一章物品栏图标**: 全部7个图标已完成，可直接用于游戏。
    - **第一章部分道具**: 如放大镜、天平、乌鸦、日记本等静态道具可直接使用。

2.  **作为基准状态 (Use as Base State)**:
    - 对于需要多状态的道具，已有的资产可作为“状态1”或“基准帧”。
    - **示例**: 使用已有的 `Prop_Ch1_MusicBox_Closed.png` 作为参考图，生成其“打开”状态，确保尺寸、光照、风格完全一致。

3.  **作为风格参考 (Use as Style Reference)**:
    - 所有在 `feature/art-ui` 分支中的资产都应被视为后续所有章节资产创作的“黄金标准”。
    - 在为第二、三章创作新资产时，务必参考第一章的资产，以确保色调、质感、光影和轮廓风格的统一。
    - **示例**: 在创作第二章的“现在之符”时，应使用第一章的 `Icon_Ch1_PastSymbol.png` 作为风格和结构参考。

## 7. 工作流程与交付标准

1.  **检查与复用**: 在领取任务前，首先对照第6节的“资产复用指南”，确认该资产是否已有可复用的版本或基准。
2.  **领取任务**: 从 `Docs/ART_ASSETS_LIST.md` 中按优先级（P0 > P1 > P2）领取资产制作任务。
3.  **分析需求**: 仔细阅读设计文档，理解资源将被用于何种场景和交互。
4.  **生成与创作**: 遵循第3节的“核心生成方法论”，使用AI工具生成基础素材，并结合手动绘制进行精修。
5.  **格式与命名**: 所有资产需保存为带透明背景的 `.png` 格式。严格按照 `{类型}_{章节}_{逻辑名称}_{状态}.png` (例如 `OBJ_Ch1_MusicBox_Open.png`) 的规范进行命名。
6.  **提交资源**: 将最终的资源文件放置在 `Content/` 目录下对应的文件夹中（如 `Content/Textures/Chapter1/Objects/`）。
7.  **发起PR**: 将新创建的资源文件和更新的文档通过Git提交到你的 `feature/art-ui-v2` 分支，并发起Pull Request至 `develop` 分支。

## 8. 质量标准 (Quality Standards)

- **一致性**: 所有资源必须在风格、光照、透视上保持高度一致。
- **清晰度**: 即使是图标，也应保证在不同尺寸下都具有良好的可读性。
- **优化**: 纹理尺寸应遵循2的幂次方原则（如 512x512, 1024x1024），并根据资源的重要性和使用场景选择合适的尺寸，以优化性能。

请开始你的创作，为这个神秘的锈湖世界注入生命。
