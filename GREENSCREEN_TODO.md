# 绿幕转换待办清单

## 已完成 (14/121)

### 第一章图标 (3/7)
- ✅ ITEM_Crown_Icon.png
- ✅ ITEM_Symbol_Past.png
- ✅ ITEM_Truth_Feather_Icon.png
- ⏳ ITEM_Magnifier_Icon.png（需重试）
- ⏳ ITEM_Necklace_Icon.png（需重试）
- ⏳ ITEM_Note_Feather_Icon.png（需重试）
- ⏳ ITEM_Ring_Icon.png（需重试）

### 第一章道具 (9/24)
- ✅ ITEM_Feather_Note.png
- ✅ ITEM_Feather_Truth.png
- ✅ ITEM_Magnifier_Prop.png
- ✅ OBJ_Crown_Base.png
- ✅ OBJ_Necklace_Base.png
- ✅ OBJ_Mummy_Lying.png
- ✅ OBJ_Crow_Idle.png
- ✅ OBJ_Door_Locked.png
- ✅ OBJ_Sarcophagus_Closed.png
- ⏳ OBJ_Crown_Glow1.png（发光效果，需特殊处理）
- ⏳ OBJ_Crown_Glow2.png
- ⏳ OBJ_Crown_Glow3.png
- ⏳ OBJ_Necklace_Glow1.png
- ⏳ OBJ_Necklace_Glow2.png
- ⏳ OBJ_Necklace_Glow3.png
- ⏳ OBJ_Ring_Base.png（需重试）
- ⏳ OBJ_Ring_Glow1.png
- ⏳ OBJ_Ring_Glow2.png
- ⏳ OBJ_Ring_Glow3.png
- ⏳ OBJ_Mummy_Anim_01.png 至 08.png（8个动画帧）
- ⏳ OBJ_Mummy_Revived.png
- ⏳ OBJ_Cartier_Case_Closed.png
- ⏳ OBJ_Cartier_Case_Open.png
- ⏳ OBJ_Diary_Alchemist.png
- ⏳ OBJ_Door_Open.png
- ⏳ OBJ_MusicBox_Closed.png
- ⏳ OBJ_MusicBox_Open.png
- ⏳ OBJ_Poster_Blurry.png
- ⏳ OBJ_Poster_Clear.png
- ⏳ OBJ_Sarcophagus_Open.png
- ⏳ OBJ_Scale.png
- ⏳ OBJ_Stele_Hieroglyph.png

### 第二章 (2/41)
- ✅ CHAR_BluePuppet_Idle.png
- ✅ ITEM_C2_001_Sunflower_Seed.png
- ⏳ CHAR_PurplePuppet_Merged.png
- ⏳ CHAR_RedPuppet_Idle.png
- ⏳ ITEM_C2_002_WaterLily_Seed.png
- ⏳ ITEM_C2_003_Moonflower_Seed.png
- ⏳ ITEM_C2_004_Rose_Seed.png
- ⏳ ITEM_C2_005_Red_Flower.png
- ⏳ ITEM_C2_006_Blue_Flower.png
- ⏳ ITEM_C2_007_Purple_Flower.png
- ⏳ ITEM_C2_008_White_Flower.png
- ⏳ ITEM_C2_009_Watering_Can.png
- ⏳ 28 个 Props（齿轮、植物等）

### 第三章 (0/29)
- ⏳ 11 个 Items
- ⏳ 4 个 Paintings
- ⏳ 14 个 Props

### 其他场景 (0/20)
- ⏳ Intro 场景资产
- ⏳ Memories 场景资产
- ⏳ Climax 场景资产
- ⏳ Ending 场景资产

---

## 处理策略

### 优先级 P0（关键游戏资产）
1. 第一章剩余图标（4个）
2. 第一章木乃伊动画（9个）
3. 第二章角色（2个）
4. 第二章种子和花朵（8个）

### 优先级 P1（重要道具）
1. 第一章珠宝发光效果（9个）
2. 第一章其他道具（13个）
3. 第二章齿轮和植物（28个）

### 优先级 P2（补充资产）
1. 第三章所有资产（29个）
2. 其他场景资产（20个）

---

## 技术笔记

### 成功率统计
- **总尝试**: 21 个文件
- **高质量**: 14 个 (66.7%)
- **中等质量**: 5 个 (23.8%)
- **低质量**: 2 个 (9.5%)

### 常见问题

#### 1. 绿色棋盘格而非纯色
**原因**: NanoBanana Pro 有时会生成棋盘格图案
**解决**: 重试或调整提示词

#### 2. 物体内部变绿
**原因**: AI 难以区分透明元素（如玻璃、宝石）和背景
**解决**: 在提示词中明确说明哪些部分应保持原样

#### 3. 发光效果失败
**原因**: 发光效果与背景混合，AI 难以分离
**解决**: 可能需要手动处理或使用其他工具

### 最佳实践

#### 提示词模板
```
Remove all background and replace with flat solid bright green color (#00FF00, chroma key green). 
The background must be completely uniform pure green with no gradients, no patterns, no checkerboard. 
Keep the [OBJECT_NAME] exactly the same.
```

#### 特殊情况提示词
对于有透明元素的物体（如放大镜）：
```
Replace the checkered background with solid bright green (#00FF00). 
Keep the [OBJECT_FRAME] exactly as it is. 
The [TRANSPARENT_PART] should remain clear/white/slightly tinted, not green. 
Only the outer background should be pure green.
```

---

## 下一步行动

1. **继续使用 NanoBanana Pro 处理 P0 优先级资产**
   - 预计需要 20-30 个文件
   - 成功率约 65-70%

2. **对失败的文件重试**
   - 调整提示词
   - 尝试不同的描述方式

3. **记录每批处理的成功率和问题**
   - 优化提示词模板
   - 识别难以处理的资产类型

4. **考虑替代方案**
   - 对于发光效果，可能需要在 UE4 中通过材质实现
   - 对于复杂透明元素，可能需要手动处理

---

## 工具和资源

### 已生成的绿幕文件位置
- `/home/ubuntu/greenscreen_batch/`（临时目录）

### 质量检查报告
- `/home/ubuntu/greenscreen_quality_check.md`

### 原始假透明文件列表
- `/tmp/fake_transparent_list.txt`

---

## 提交记录

### 2024-12-08
- **提交**: 6dea624
- **内容**: 转换14个资产为纯绿幕背景
- **文件数**: 14
- **成功率**: 66.7%
