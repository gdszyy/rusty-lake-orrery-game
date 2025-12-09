# UI与通用资产清单

**项目**: 《锈湖：炼金术士的天体仪》  
**资产类型**: UI系统与通用美术资产  
**创建日期**: 2025-12-09  
**版本**: 1.0

---

## 概述

本文档记录了为《锈湖：炼金术士的天体仪》项目创建的所有UI与通用美术资产。所有资产均严格遵循锈湖系列的艺术风格，采用手绘版画质感、暗沉色调和做旧效果。

**艺术风格特征：**
- 色调：暗沉低饱和度（深棕、灰黑、铁锈色），强调色为血红、暗金
- 质感：手绘版画质感，线条带有自然瑕疵
- 主题：维多利亚时代、炼金术符号、神秘机械装置

**技术规范：**
- 所有需要透明背景的资产使用绿幕背景生成
- 纹理尺寸遵循2的幂次方原则
- 命名遵循项目标准前缀规范

---

## 1. 交互图标 (Interaction Icons)

### 1.1 通用交互图标

| 资源名称 | 描述 | 尺寸 | 路径 | 状态 |
|---------|------|------|------|------|
| `T_Icon_Eye` | 观察图标（眼睛），暗金色手绘风格 | 64x64 | `Content/Textures/UI/` | ✅ 完成 |
| `T_Icon_Hand` | 交互图标（手），指向手势 | 64x64 | `Content/Textures/UI/` | ✅ 完成 |
| `T_Icon_Gear` | 设置图标（齿轮），炼金术风格 | 64x64 | `Content/Textures/UI/` | ✅ 完成 |
| `T_Icon_Magnifier` | 放大镜图标，黄铜质感 | 64x64 | `Content/Textures/UI/` | ✅ 完成 |
| `T_Icon_ArrowLeft` | 返回箭头，手绘风格 | 64x64 | `Content/Textures/UI/` | ✅ 完成 |
| `T_Icon_ArrowDown` | 继续提示箭头 | 32x32 | `Content/Textures/UI/` | ✅ 完成 |

### 1.2 菜单入口图标

| 资源名称 | 描述 | 尺寸 | 路径 | 状态 |
|---------|------|------|------|------|
| `T_Icon_Inventory` | 背包图标（深褐色皮革包） | 80x80 | `Content/Textures/UI/` | ✅ 完成 |
| `T_Icon_Menu` | 菜单图标（炼金术齿轮） | 80x80 | `Content/Textures/UI/` | ✅ 完成 |

**技术说明：** 所有图标均使用绿幕背景生成，需要后期抠图处理以获得透明背景。

---

## 2. UI背景纹理 (UI Background Textures)

### 2.1 可平铺纹理

| 资源名称 | 描述 | 尺寸 | 路径 | 状态 |
|---------|------|------|------|------|
| `T_BG_Leather` | 深褐色皮革纹理，可平铺 | 512x512 | `Content/Textures/UI/` | ✅ 完成 |
| `T_BG_Parchment` | 米色羊皮纸纹理，带做旧效果 | 512x512 | `Content/Textures/UI/` | ✅ 完成 |
| `T_BG_Wood` | 深色木质纹理，可平铺 | 512x512 | `Content/Textures/UI/` | ✅ 完成 |
| `T_BG_Metal` | 黄铜金属纹理，带铜锈效果 | 512x512 | `Content/Textures/UI/` | ✅ 完成 |

**使用场景：** 这些纹理可用于UI窗口背景、对话框背景、菜单背景等。

---

## 3. 按钮元素 (Button Elements)

### 3.1 按钮状态

| 资源名称 | 描述 | 尺寸 | 路径 | 状态 |
|---------|------|------|------|------|
| `T_Button_Normal` | 按钮默认状态，深褐色木质/黄铜边框 | 400x80 | `Content/Textures/UI/` | ✅ 完成 |
| `T_Button_Hover` | 按钮悬停状态，带金色高光 | 400x80 | `Content/Textures/UI/` | ✅ 完成 |
| `T_Button_Pressed` | 按钮按下状态，更暗的阴影效果 | 400x80 | `Content/Textures/UI/` | ✅ 完成 |
| `T_Button_Disabled` | 按钮禁用状态，灰褐色去饱和 | 400x80 | `Content/Textures/UI/` | ✅ 完成 |

**设计特征：** 所有按钮均带有精美的炼金术风格边框，四角有神秘符号装饰。

---

## 4. 对话框元素 (Dialogue Box Elements)

### 4.1 对话框组件

| 资源名称 | 描述 | 尺寸 | 路径 | 状态 |
|---------|------|------|------|------|
| `T_DialogueBox_BG` | 对话框背景，羊皮纸质感 | 1728x240 | `Content/Textures/UI/` | ✅ 完成 |
| `T_DialogueBox_Border` | 对话框边框，炼金术符号装饰 | 1728x240 | `Content/Textures/UI/` | ✅ 完成 |

**使用说明：** 背景和边框可叠加使用，边框包含四角的神秘符号（杯子、太阳、天平、六芒星）。

---

## 5. 装饰元素 (Decorative Elements)

### 5.1 边框与符号

| 资源名称 | 描述 | 尺寸 | 路径 | 状态 |
|---------|------|------|------|------|
| `T_Border_Frame` | 方形装饰边框，炼金术符号 | 512x512 | `Content/Textures/UI/` | ✅ 完成 |
| `T_Symbol_Watermark` | 炼金术符号水印，淡淡的背景装饰 | 256x256 | `Content/Textures/UI/` | ✅ 完成 |

### 5.2 粒子纹理

| 资源名称 | 描述 | 尺寸 | 路径 | 状态 |
|---------|------|------|------|------|
| `T_Particle_Star` | 星尘粒子，金黄色八角星 | 64x64 | `Content/Textures/UI/` | ✅ 完成 |
| `T_Particle_Glow` | 光晕粒子，温暖金色光球 | 128x128 | `Content/Textures/UI/` | ✅ 完成 |

---

## 6. 通用特效纹理 (VFX Textures)

### 6.1 粒子特效

| 资源名称 | 描述 | 尺寸 | 路径 | 状态 |
|---------|------|------|------|------|
| `T_VFX_Spark` | 火花粒子，金橙色 | 32x32 | `Content/Textures/VFX/` | ✅ 完成 |
| `T_VFX_StarDust` | 星尘云团粒子 | 64x64 | `Content/Textures/VFX/` | ✅ 完成 |
| `T_VFX_Glow` | （同上T_Particle_Glow） | 128x128 | `Content/Textures/UI/` | ✅ 完成 |

### 6.2 特殊效果

| 资源名称 | 描述 | 尺寸 | 路径 | 状态 |
|---------|------|------|------|------|
| `T_VFX_AlchemyCircle` | 炼金术魔法阵，复杂几何图案 | 512x512 | `Content/Textures/VFX/` | ✅ 完成 |

**设计亮点：** 炼金术法阵包含同心圆、五角星、六芒星、黄道十二宫符号、炼金术元素符号等，是锈湖风格的典型代表。

---

## 7. 字体资源 (Fonts)

### 7.1 字体文件

| 资源名称 | 字体名称 | 用途 | 路径 | 状态 |
|---------|---------|------|------|------|
| `Font_Main_Regular.ttf` | Cormorant Garamond | 正文、对话 | `Content/Fonts/` | ✅ 完成 |
| `Font_Title.ttf` | Uncial Antiqua | 标题、符号 | `Content/Fonts/` | ✅ 完成 |
| `Font_Handwriting.ttf` | Kalam | 日记、笔记 | `Content/Fonts/` | ✅ 完成 |

**字体说明：**
- **Cormorant Garamond**: 优雅的衬线字体，适合正文和对话，符合维多利亚时代美学
- **Uncial Antiqua**: 古典手写风格，适合标题和神秘符号
- **Kalam**: 自然手写风格，适合日记和笔记内容

**许可证：** 所有字体均为开源字体，遵循SIL Open Font License (OFL)。

---

## 8. 资产统计

### 8.1 完成情况

| 类别 | 数量 | 状态 |
|------|------|------|
| 交互图标 | 8 | ✅ 100% |
| UI背景纹理 | 4 | ✅ 100% |
| 按钮元素 | 4 | ✅ 100% |
| 对话框元素 | 2 | ✅ 100% |
| 装饰元素 | 4 | ✅ 100% |
| 特效纹理 | 4 | ✅ 100% |
| 字体资源 | 3 | ✅ 100% |
| **总计** | **29** | **✅ 100%** |

### 8.2 文件大小统计

```
Content/Textures/UI/     : 25 个PNG文件
Content/Textures/VFX/    : 3 个PNG文件
Content/Fonts/           : 3 个TTF文件 (约1.5MB)
```

---

## 9. 后期处理说明

### 9.1 绿幕抠图

所有带绿幕背景的资产需要进行后期抠图处理：

1. 使用图像处理工具（Photoshop、GIMP等）
2. 使用魔棒工具或色彩范围选择工具选中绿幕背景
3. 删除背景，保留主体
4. 检查边缘是否有绿边残留，使用去边工具清除
5. 导出为PNG格式（保留透明通道）

**需要抠图的资产列表：**
- 所有图标（T_Icon_*）
- 按钮元素（T_Button_*）
- 对话框边框（T_DialogueBox_Border）
- 装饰边框（T_Border_Frame）
- 所有粒子纹理（T_Particle_*, T_VFX_*）

### 9.2 质量检查

在使用资产前，请确保：
- ✅ 背景颜色纯净（无渐变、无杂色）
- ✅ 主体边缘清晰（无模糊、无绿边）
- ✅ 风格一致性（符合锈湖美学）
- ✅ 尺寸正确（符合规范要求）

---

## 10. 使用指南

### 10.1 UI窗口示例

```
背景层：T_BG_Parchment（羊皮纸纹理）
边框层：T_Border_Frame（装饰边框）
按钮：T_Button_Normal/Hover/Pressed/Disabled
图标：T_Icon_Eye, T_Icon_Hand等
```

### 10.2 对话框示例

```
背景层：T_DialogueBox_BG
边框层：T_DialogueBox_Border
字体：Font_Main_Regular.ttf
```

### 10.3 粒子特效示例

```
星尘效果：T_VFX_StarDust + T_Particle_Star
光晕效果：T_Particle_Glow
火花效果：T_VFX_Spark
魔法阵：T_VFX_AlchemyCircle（可旋转动画）
```

---

## 11. 版本历史

| 版本 | 日期 | 说明 |
|------|------|------|
| 1.0 | 2025-12-09 | 初始版本，完成所有核心UI与通用资产 |

---

## 12. 致谢

**艺术风格参考：** 锈湖系列游戏（Rusty Lake Hotel、Cube Escape系列）  
**生成工具：** NanoBananaPro  
**字体来源：** Google Fonts (Cormorant Garamond, Uncial Antiqua, Kalam)  
**许可证：** 所有自制资产遵循项目开源协议，字体遵循SIL OFL

---

**文档维护者：** UI与通用资产美术师  
**最后更新：** 2025-12-09
