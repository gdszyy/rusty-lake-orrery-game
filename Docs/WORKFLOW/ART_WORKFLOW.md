# 美术工作流程指南 (Art Workflow) - V2

**文档版本**: 2.0  
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

## 5. 美术资产需求清单 (V2.0)

以下是根据第一至第三章游戏设计文档（Optimized版）梳理的详细美术资产需求清单。请严格按照此清单进行制作。

### 5.1 第一章：珠宝、埃及与音乐

**场景：博物馆第一展厅 - 时间的结晶**

#### 场景资产

| 资产编号 | 资产名称 | 描述 | 优先级 |
|---|---|---|---|
| BG_Ch1_001 | BG_Museum_Hall1 | 展厅背景全景，包含大理石地板、木质护墙板、古典吊灯等元素 | P0 |

#### 可交互物品与道具

| 资产编号 | 资产名称 | 描述 | 状态/帧数 | 优先级 |
|---|---|---|---|---|
| OBJ_Ch1_001 | OBJ_Cartier_Case | 卡地亚展柜 | 2个状态 (开/关) | P0 |
| OBJ_Ch1_002 | OBJ_Ring / Necklace / Crown | 三件珠宝（戒指、项链、王冠） | 各3帧闪烁动画 | P0 |
| OBJ_Ch1_003 | OBJ_Sarcophagus | 埃及石棺 | 5帧开合动画 | P0 |
| OBJ_Ch1_004 | OBJ_Mummy | 木乃伊 | 8帧复活动画 | P1 |
| OBJ_Ch1_005 | OBJ_Scale | 埃及审判天平 | 5帧平衡动画 | P0 |
| OBJ_Ch1_006 | OBJ_MusicBox | 音乐盒 | 10帧开盖+旋转动画 | P0 |
| OBJ_Ch1_007 | OBJ_Crow | 乌鸦 | 8帧 (idle+啄食) | P1 |
| OBJ_Ch1_008 | OBJ_Poster_Concert | 演唱会海报 | 2个状态 (模糊/清晰) | P1 |
| ITEM_Ch1_001 | ITEM_Magnifier | 放大镜 | 道具+图标 | P0 |
| ITEM_Ch1_002 | ITEM_Note_Feather | 音符羽毛 | 道具+图标 | P0 |
| ITEM_Ch1_003 | ITEM_Truth_Feather | 真理之羽 | 道具+图标 | P0 |
| ITEM_Ch1_004 | ITEM_Symbol_Past | 过去之符 | 图标 | P0 |

### 5.2 第二章：成长与分离

**场景：博物馆第二展厅 - 生长的悖论**

#### 场景资产

| 资产编号 | 资产名称 | 描述 | 优先级 |
|---|---|---|---|
| BG_Ch2_001 | BG_Museum_Hall2 | 展厅背景全景，包含左侧温室区域和右侧镜像区域 | P0 |

#### 可交互物品与道具

| 资产编号 | 资产名称 | 描述 | 状态/帧数 | 优先级 |
|---|---|---|---|---|
| OBJ_Ch2_001 | PLANT_Sunflower | 向日葵 | 5帧生长动画 | P0 |
| OBJ_Ch2_002 | PLANT_WaterLily | 睡莲 | 5帧生长动画 | P0 |
| OBJ_Ch2_003 | PLANT_MoonFlower | 月见草 | 5帧生长动画 | P0 |
| OBJ_Ch2_004 | PLANT_Rose | 蔷薇 | 5帧生长动画 | P0 |
| OBJ_Ch2_005 | OBJ_Clock | 时钟 | 交互（可拖动指针） | P0 |
| OBJ_Ch2_006 | OBJ_BigMirror | 大镜子 | 5帧破裂动画 | P0 |
| OBJ_Ch2_007 | CHAR_RedPuppet / BluePuppet | 红/蓝人偶 | 各3帧移动动画 | P0 |
| OBJ_Ch2_008 | CHAR_Butterfly | 蝴蝶 | 骨骼动画 | P0 |
| ITEM_Ch2_001 | ITEM_Flower_Red / Blue | 红/蓝花朵 | 图标 | P0 |
| ITEM_Ch2_002 | ITEM_Symbol_Present | 现在之符 | 图标 | P0 |

### 5.3 第三章：流动的永恒

**场景：博物馆核心大厅 - 时间的交响**

#### 场景资产

| 资产编号 | 资产名称 | 描述 | 优先级 |
|---|---|---|---|
| BG_Ch3_001 | BG_Museum_Core_Hall | 核心大厅背景全景（宽场景） | P0 |

#### 可交互物品与道具

| 资产编号 | 资产名称 | 描述 | 状态/帧数 | 优先级 |
|---|---|---|---|---|
| OBJ_Ch3_001 | OBJ_Painting_Future | 未来画作 | 静态+动画（流动的星空） | P0 |
| OBJ_Ch3_002 | OBJ_CraneMachine | 娃娃机 | 动画（爪子抓取） | P0 |
| OBJ_Ch3_003 | OBJ_TicketMachine | 彩票兑换机 | 交互界面 | P1 |
| OBJ_Ch3_004 | OBJ_Orrery | 天体仪核心 | 动画（齿轮旋转、刻度点亮） | P0 |
| OBJ_Ch3_005 | OBJ_Clock_Main | 天体仪大钟 | 交互（可拖动指针） | P0 |
| ITEM_Ch3_001 | ITEM_Gear_Copper / Silver / Gold | 铜/银/金齿轮 | 图标 | P0 |
| ITEM_Ch3_002 | ITEM_Palette_Eternal | 永恒调色盘 | 图标 | P0 |
| ITEM_Ch3_003 | ITEM_Symbol_Future | 未来之符 | 图标 | P0 |
| ITEM_Ch3_004 | ITEM_Symbol_Eternal | 永恒之符（最终形态） | 图标 | P0 |

*注：以上为各章节核心资产节选，详细清单请参考 `art_assets_detailed_list.md`。*

## 6. 工作流程与交付标准

1.  **领取任务**: 从 `art_assets_detailed_list.md` 中按优先级（P0 > P1 > P2）领取资产制作任务。
2.  **分析需求**: 仔细阅读设计文档，理解资源将被用于何种场景和交互。
3.  **生成与创作**: 遵循第3节的“核心生成方法论”，使用AI工具生成基础素材，并结合手动绘制进行精修。
4.  **格式与命名**: 所有资产需保存为带透明背景的 `.png` 格式。严格按照 `{类型}_{章节}_{逻辑名称}_{状态}.png` (例如 `OBJ_Ch1_MusicBox_Open.png`) 的规范进行命名。
5.  **提交资源**: 将最终的资源文件放置在 `Content/` 目录下对应的文件夹中（如 `Content/Textures/Chapter1/Objects/`）。
6.  **发起PR**: 将新创建的资源文件和更新的文档通过Git提交到你的 `feature/art-ui-v2` 分支，并发起Pull Request至 `develop` 分支。

## 7. 质量标准 (Quality Standards)

- **一致性**: 所有资源必须在风格、光照、透视上保持高度一致。
- **清晰度**: 即使是图标，也应保证在不同尺寸下都具有良好的可读性。
- **优化**: 纹理尺寸应遵循2的幂次方原则（如 512x512, 1024x1024），并根据资源的重要性和使用场景选择合适的尺寸，以优化性能。

请开始你的创作，为这个神秘的锈湖世界注入生命。
