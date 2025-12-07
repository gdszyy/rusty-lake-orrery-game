# 《锈湖：炼金术士的天体仪》美术资产清单

**目标：** 罗列游戏第一至第三章所需的全部美术资产，包括场景、可交互物品及物品栏图标，并提供初步的风格提示。

---

## 第一章：博物馆第一展厅：时间的结晶

### 场景资产 (Scene Assets)

| 资产名称 | 描述 | 风格提示 |
| :--- | :--- | :--- |
| `BG_Chapter1_Museum_Base` | 博物馆展厅的基础背景，包含墙壁、地板、天花板和吊灯。墙上为展柜、石棺、音乐盒等预留了清晰的空位和轮廓。 | `2D side-scrolling game background`, `decaying museum hall`, `empty spaces for props`, `dramatic chiaroscuro lighting` |
| `BG_Chapter1_Museum_DoorOpen` | 展厅背景的变体，通往第二展厅的门是打开的。 | 基于 `BG_Chapter1_Museum_Base`，门洞处有深邃的黑暗。 |
| `Scene_Chapter1_Concert` | 回忆过场中的演唱会场景，包含舞台、周传雄剪影和前景的两个人影。 | `static scene`, `stage with spotlight`, `silhouettes of a singer and audience`, `two clear figures in foreground` |

### 可交互道具资产 (In-Scene Prop Assets)

| 资产名称 | 描述 | 风格提示 |
| :--- | :--- | :--- |
| `Prop_Ch1_CartierDisplay` | 卡地亚展柜，内含三件珠宝（戒指、项链、王冠）。 | `ornate glass display case`, `Victorian-era`, `contains three jewels`, `light source from top-left` |
| `Prop_Ch1_Jewel_Ring_FX` | 戒指低语时的冰霜特效。 | `frost effect`, `subtle animation`, `overlay on the ring` |
| `Prop_Ch1_Jewel_Necklace_FX` | 项链低语时的枷锁特效。 | `ethereal chains`, `subtle animation`, `overlay on the necklace` |
| `Prop_Ch1_Jewel_Crown_FX` | 王冠低语时的锁链特效。 | `heavy chains`, `subtle animation`, `overlay on the crown` |
| `Prop_Ch1_Sarcophagus_Closed` | 紧闭的埃及石棺，表面有象形文字。 | `ancient Egyptian sarcophagus`, `detailed hieroglyphs`, `stone texture` |
| `Prop_Ch1_Sarcophagus_Open` | 打开的石棺，内部是空的木乃伊。 | `open sarcophagus`, `mummy inside with an empty chest cavity` |
| `Prop_Ch1_Mummy_WithHeart` | 木乃伊胸腔中放入“音符羽毛”后的状态，心脏由音符构成并跳动。 | `mummy with a glowing heart made of musical notes`, `subtle pulsing animation` |
| `Prop_Ch1_MusicBox_Closed` | 带密码锁的精美音乐盒。 | `dark wood music box`, `tarnished brass fittings`, `combination lock` |
| `Prop_Ch1_MusicBox_Open` | 打开的音乐盒，内部有旋转的舞台和跳舞的小人偶。 | `open music box`, `rotating stage with two dancing figurines`, `animated` |
| `Prop_Ch1_BalanceScale` | 埃及审判天平，右侧已放置“真理之羽”。 | `Egyptian balance scale`, `brass material`, `a white feather on the right plate` |
| `Prop_Ch1_BalanceScale_Balanced` | 天平被音符平衡后的状态，发出金色光芒。 | `balanced scale`, `glowing musical notes on the left plate`, `radiating golden light` |
| `Prop_Ch1_Crow` | 栖息在横梁上的乌鸦，会有歪头等简单动画。 | `a black crow`, `perched on a beam`, `simple animation (head tilt, blink)` |
| `Prop_Ch1_AlchemistDiary` | 放在展品台上的炼金术士日记。 | `leather-bound diary`, `yellowed pages`, `placed on a display stand` |
| `Prop_Ch1_ConcertPoster` | 墙上贴着的周传雄演唱会海报，日期清晰可见。 | `vintage concert poster`, `blurry image of a singer`, `clear date: 7月27日` |
| `FX_Ch1_MusicalNotes` | 从音乐盒飞出的实体化音符，在空中飞舞。 | `glowing, translucent musical notes`, `butterfly-like flying animation`, `particle effect` |

### 物品栏资产 (Inventory Item Icons)

| 资产名称 | 描述 | 风格提示 |
| :--- | :--- | :--- |
| `Icon_Ch1_Magnifier` | 放大镜 | `clean 2D icon`, `brass magnifying glass`, `clear silhouette` |
| `Icon_Ch1_Jewel_Ring` | 卡地亚戒指 | `clean 2D icon`, `diamond ring`, `sparkling` |
| `Icon_Ch1_Jewel_Necklace` | 卡地亚项链 | `clean 2D icon`, `gemstone necklace`, `ornate` |
| `Icon_Ch1_Jewel_Crown` | 卡地亚王冠 | `clean 2D icon`, `golden crown with jewels`, `luxurious` |
| `Icon_Ch1_NoteFeather` | 音符羽毛 | `clean 2D icon`, `a feather made of solidified musical notes`, `glowing` |
| `Icon_Ch1_TruthFeather` | 真理之羽 | `clean 2D icon`, `a single pure white feather`, `simple and elegant` |
| `Icon_Ch1_PastSymbol` | 过去之符 | `clean 2D icon`, `alchemical symbol shaped like a crow feather`, `golden glow` |

---

## 第二章：博物馆第二展厅：生长的悖论

### 场景资产 (Scene Assets)

| 资产名称 | 描述 | 风格提示 |
| :--- | :--- | :--- |
| `BG_Chapter2_Museum_Base` | 第二展厅背景，左侧为温室区，右侧为镜像区。温室区有四个空花盆和天窗，镜像区有一面大镜子。 | `2D side-scrolling game background`, `split into two areas: a greenhouse and a mirror hall` |
| `BG_Chapter2_Museum_Night` | 第二展厅的夜晚版本，用于月见草生长。 | 基于 `BG_Chapter2_Museum_Base`，色调变为深蓝，窗外有月亮。 |
| `BG_Chapter2_Mirror_Broken` | 镜像区的大镜子破碎后的状态。 | `large mirror with a heart-shaped crack`, `shattered glass effect` |
| `Scene_Chapter2_Anji` | 回忆过场中的安吉场景，分屏显示云上草原和百草园。 | `split-screen static scene`, `left: grasslands`, `right: botanical garden` |

### 可交互道具资产 (In-Scene Prop Assets)

| 资产名称 | 描述 | 风格提示 |
| :--- | :--- | :--- |
| `Prop_Ch2_Plant_Sunflower` | 向日葵，包含种子、生长、开花三个状态。 | `sunflower`, `animated growth sequence (5 frames)`, `needs sunlight` |
| `Prop_Ch2_Plant_WaterLily` | 睡莲，包含种子、生长、开花三个状态。 | `water lily`, `animated growth sequence (5 frames)`, `needs water` |
| `Prop_Ch2_Plant_Moonflower` | 月见草，包含种子、生长、开花三个状态。 | `moonflower (Oenothera)`, `animated growth sequence (5 frames)`, `blooms at night` |
| `Prop_Ch2_Plant_Rose` | 蔷薇，包含种子、生长、开花三个状态。 | `rose`, `animated growth sequence (5 frames)`, `grows when player is away` |
| `Prop_Ch2_WateringCan` | 浇水壶。 | `exquisite watering can`, `game asset prop` |
| `Prop_Ch2_Clock` | 可调节时间的时钟。 | `wall clock with movable hands`, `Victorian style` |
| `Prop_Ch2_AnjiDiary` | 安吉日记。 | `diary with a plant-themed cover` |
| `Prop_Ch2_Mirror_Figurines` | 镜子内外的红色和蓝色人偶，可移动。 | `two small figurines (red and blue)`, `marionette style`, `mirrored movement` |
| `Prop_Ch2_Mirror_Buttons` | 镜像谜题中的同步按钮。 | `two pressure plates`, `glowing when pressed` |
| `FX_Ch2_Butterfly` | 连接温室和镜像的蝴蝶，有飞行轨迹。 | `a single glowing butterfly`, `animated flight path`, `particle trail` |
| `FX_Ch2_TimeWeb` | 蝴蝶飞行轨迹形成的时间之网。 | `a glowing web-like pattern`, `connecting flowers and figurines` |

### 物品栏资产 (Inventory Item Icons)

| 资产名称 | 描述 | 风格提示 |
| :--- | :--- | :--- |
| `Icon_Ch2_WateringCan` | 浇水壶 | `clean 2D icon`, `watering can`, `clear silhouette` |
| `Icon_Ch2_MemoryFragment2` | 记忆碎片2（彩色水晶） | `clean 2D icon`, `a colorful crystal`, `glowing` |
| `Icon_Ch2_RedFlower` | 红色花朵（现在之符的一半） | `clean 2D icon`, `a single red flower`, `stylized` |
| `Icon_Ch2_BlueFlower` | 蓝色花朵（现在之符的另一半） | `clean 2D icon`, `a single blue flower`, `stylized` |
| `Icon_Ch2_PresentSymbol` | 现在之符 | `clean 2D icon`, `two intertwined flowers (red and blue)`, `alchemical symbol` |

---

## 第三章：博物馆核心大厅：时间的交响

### 场景资产 (Scene Assets)

| 资产名称 | 描述 | 风格提示 |
| :--- | :--- | :--- |
| `BG_Chapter3_Museum_Base` | 核心大厅背景，中央是天体仪和下方的大钟，背景墙为画作预留了四个空画框。 | `grand hall background`, `a large orrery in the center`, `four empty picture frames on the wall` |
| `BG_Chapter3_Museum_Night` | 核心大厅的夜晚版本，用于最终仪式。 | 基于 `BG_Chapter3_Museum_Base`，色调变为深夜，窗外有星空。 |

### 可交互道具资产 (In-Scene Prop Assets)

| 资产名称 | 描述 | 风格提示 |
| :--- | :--- | :--- |
| `Prop_Ch3_Painting_1` | 《承诺的肖像》画作。 | `painting in the style of Ilya Repin`, `a portrait of two people`, `can talk` |
| `Prop_Ch3_Painting_2` | 《音乐会的狂欢》画作。 | `painting in the style of Ilya Repin`, `a concert scene`, `can talk` |
| `Prop_Ch3_Painting_3` | 《博物馆的历史》画作。 | `painting in the style of Ilya Repin`, `a collage of all previous game elements`, `can talk` |
| `Prop_Ch3_Painting_4_Blank` | 空白画布。 | `a blank canvas on an easel` |
| `Prop_Ch3_Painting_4_Finished` | 绘制完成的《未来》画作。 | `a finished painting of an orrery and two figures`, `rainbow colors` |
| `Prop_Ch3_Orrery` | 天体仪，有三个符号插槽，365个刻度。 | `a complex mechanical orrery`, `brass and gold`, `three symbol slots`, `365 glowing scales` |
| `Prop_Ch3_Orrery_Active` | 放置所有符号并激活后的天体仪，开始旋转。 | `animated rotating orrery`, `all scales lit`, `glowing symbols` |
| `Prop_Ch3_GrandClock` | 天体仪下方的大钟，时针、分针、秒针可动。 | `a grand clock face`, `movable hands`, `integrated with the orrery` |
| `Prop_Ch3_Palette` | 调色盘，可收集三种颜色并混合成彩虹色。 | `an artist's palette`, `can show different color combinations` |
| `Prop_Ch3_ConductorsBaton` | 指挥棒。 | `a conductor's baton`, `slender and elegant` |
| `Prop_Ch3_BeijingDiary` | 北京日记。 | `diary with a cover related to art or Beijing` |
| `FX_Ch3_SymbolFusion` | 过去、现在、未来三个符号融合成“永恒之符”的动画。 | `complex fusion animation`, `three symbols swirling and merging`, `bright rainbow light` |
| `FX_Ch3_BellChimes` | 12次钟声敲响时的视觉特效，每次对应一个月的故事。 | `visual effect for each chime`, `glowing numbers (1-12)`, `short text display` |

### 物品栏资产 (Inventory Item Icons)

| 资产名称 | 描述 | 风格提示 |
| :--- | :--- | :--- |
| `Icon_Ch3_Color_Gold` | 从画作中提取的金色颜料。 | `clean 2D icon`, `a blob of golden paint` |
| `Icon_Ch3_Color_Blue` | 从画作中提取的蓝色颜料。 | `clean 2D icon`, `a blob of blue paint` |
| `Icon_Ch3_Color_Red` | 从画作中提取的红色颜料。 | `clean 2D icon`, `a blob of red paint` |
| `Icon_Ch3_RainbowBrush` | 混合了三种颜色的彩虹画笔。 | `clean 2D icon`, `a paintbrush with rainbow-colored paint on its tip` |
| `Icon_Ch3_FutureSymbol` | 未来之符（彩虹画笔形状）。 | `clean 2D icon`, `alchemical symbol shaped like a rainbow paintbrush` |
| `Icon_Ch3_ConductorsBaton` | 指挥棒。 | `clean 2D icon`, `a conductor's baton`, `clear silhouette` |
| `Icon_Ch3_EternalSymbol` | 永恒之符。 | `clean 2D icon`, `the final fused symbol`, `heart, ring, and three elements` |
