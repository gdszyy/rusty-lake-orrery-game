# Rusty Lake Orrery - 资产清单

**生成日期**: 2025-12-09  
**美术风格**: Rusty Lake 手绘版画质感  
**资产总数**: 24项

---

## 开场序列资产 (Opening Sequence Assets)

### UI资产
| 文件名 | 路径 | 描述 | 尺寸 |
|--------|------|------|------|
| T_Logo_Game.png | Content/Textures/UI/ | 游戏Logo（中英文） | 1024x1024 |

### 背景资产
| 文件名 | 路径 | 描述 | 尺寸 |
|--------|------|------|------|
| BG_Laboratory.png | Content/Textures/Environment/ | 炼金术士实验室场景 | 1920x1080 |
| BG_Museum_Exterior.png | Content/Textures/Environment/ | 博物馆外观建筑 | 1920x1080 |
| BG_Corridor.png | Content/Textures/Environment/ | 博物馆走廊 | 1920x1080 |

### 动画资产
| 文件名 | 路径 | 描述 | 帧数 |
|--------|------|------|------|
| ANIM_Door_Open_Frame1.png | Content/Animations/ | 大门关闭状态 | 序列帧1/3 |
| ANIM_Door_Open_Frame2.png | Content/Animations/ | 大门半开状态 | 序列帧2/3 |
| ANIM_Door_Open_Frame3.png | Content/Animations/ | 大门完全打开 | 序列帧3/3 |

### 3D模型参考
| 文件名 | 路径 | 描述 | 备注 |
|--------|------|------|------|
| SM_Orrery_Real_Reference.png | Content/StaticMeshes/Props/ | 实物天体仪3D模型参考 | 带绿幕，需建模 |

---

## 第一章资产 (Chapter 1 Assets)

### 场景背景
| 文件名 | 路径 | 描述 | 尺寸 |
|--------|------|------|------|
| BG_Chapter1_Hall.png | Content/Textures/Environment/ | 第一展厅完整背景 | 1920x1080 |

### 场景3D模型参考
| 文件名 | 路径 | 描述 | 备注 |
|--------|------|------|------|
| SM_Chandelier.png | Content/StaticMeshes/Environment/ | 维多利亚吊灯 | 带绿幕，需建模 |
| SM_Sarcophagus.png | Content/StaticMeshes/Props/ | 埃及石棺 | 带绿幕，需建模 |
| SM_Scale_Egyptian.png | Content/StaticMeshes/Props/ | 埃及天平（含真理之羽） | 带绿幕，需建模 |

### 道具3D模型参考
| 文件名 | 路径 | 描述 | 备注 |
|--------|------|------|------|
| SM_Display_Case_Cartier.png | Content/StaticMeshes/Props/ | 卡地亚展柜 | 带绿幕，需建模 |
| SM_MusicBox.png | Content/StaticMeshes/Props/ | 音乐盒（带密码锁） | 带绿幕，需建模 |

### 物品图标
| 文件名 | 路径 | 描述 | 备注 |
|--------|------|------|------|
| T_Item_Ring.png | Content/Textures/Items/ | 卡地亚戒指 | 带绿幕，需抠图 |
| T_Item_Necklace.png | Content/Textures/Items/ | 卡地亚项链 | 带绿幕，需抠图 |
| T_Item_Crown.png | Content/Textures/Items/ | 卡地亚王冠 | 带绿幕，需抠图 |
| T_Item_TruthFeather.png | Content/Textures/Items/ | 真理之羽（Ma'at羽毛） | 带绿幕，需抠图 |
| T_Item_NoteFeather.png | Content/Textures/Items/ | 音符羽毛（彩虹色） | 带蓝幕，需抠图 |
| T_Item_PastSymbol.png | Content/Textures/Items/ | 过去之符（炼金术符号） | 带绿幕，需抠图 |

### 角色资产
| 文件名 | 路径 | 描述 | 备注 |
|--------|------|------|------|
| SM_Crow_Idle.png | Content/StaticMeshes/Props/ | 乌鸦待机姿态 | 带绿幕，需建模 |
| SM_Crow_TiltHead.png | Content/Animations/ | 乌鸦歪头姿态 | 带绿幕，动画帧 |
| SM_Crow_Eating.png | Content/Animations/ | 乌鸦啄食姿态 | 带绿幕，动画帧 |
| SM_Crow_WingsSpread.png | Content/Animations/ | 乌鸦展翅姿态 | 带绿幕，动画帧 |

---

## 艺术风格规范

### 色调方案
- **开场序列**: 幽蓝色 + 黑色（神秘感）
- **第一章展厅**: 暖金色（珠宝光泽）+ 深褐色（埃及古物）+ 幽蓝色（夜晚氛围）
- **强调色**: 纯白色（羽毛）、彩虹色（音符）、血红色（心脏）

### 质感特征
- 手绘版画质感，线条带有自然瑕疵
- 强烈的光影对比，聚光灯效果
- 维多利亚时代复古感
- 超现实主义元素

### 绿幕抠图规范
- **默认背景**: 纯绿色 (#00FF00)
- **包含绿色元素时**: 纯蓝色或品红色
- **严禁**: 发光、光晕、粒子溢出等超出主体的特效
- **后期处理**: 需使用Photoshop/GIMP进行抠图，导出为PNG透明背景

---

## 后续工作

### 需要完成的任务
1. **绿幕抠图**: 所有带绿幕/蓝幕的资产需要抠图处理，导出为透明PNG
2. **3D建模**: 所有标注"需建模"的参考图需要转换为3D模型（FBX格式）
3. **动画制作**: 
   - 大门开启动画（3帧序列）
   - 乌鸦动画循环（4个姿态）
   - 珠宝闪烁动画
   - 石棺开合动画
   - 音乐盒开盖动画
4. **特效制作**: 
   - 冰霜纹理（戒指）
   - 锁链纹理（项链）
   - 枷锁纹理（王冠）
   - 音符飞舞粒子
   - 羽毛发光效果
   - 心脏跳动效果

### 未包含的资产
根据设计文档，以下资产尚未生成（属于第二章、第三章或通用UI）：
- 第二章展厅资产
- 第三章核心大厅资产
- 结局序列资产
- 通用UI图标（眼睛、手、背包等）
- 音效和音乐资源

---

## 版权声明

所有资产均为AI生成，用于《锈湖：炼金术士的天体仪》项目。
- **艺术风格**: 致敬Rusty Lake系列
- **音乐**: 周传雄相关内容需获得授权
- **珠宝设计**: 卡地亚相关内容仅作艺术参考

---

**资产清单完成 ✓**
