# 绿幕转换工作交接文档

## 📋 任务概述

将 feature/art-ui-v2 分支中的 **121 个假透明美术资产**转换为**纯绿幕背景**，以支持 Unreal Engine 4 的 Chroma Key 抠图功能。

---

## ✅ 已完成工作

### 第一批转换（2024-12-08）

- **已完成**: 15 个文件
- **成功率**: 71.4% (15/21 尝试)
- **Git 提交**: 6dea624, bb3aa97
- **状态**: 已推送到远程仓库

#### 已转换文件清单

**第一章图标** (4/7):
- ✅ ITEM_Crown_Icon.png
- ✅ ITEM_Symbol_Past.png  
- ✅ ITEM_Truth_Feather_Icon.png
- ✅ ITEM_Note_Feather_Icon.png (重试成功)

**第一章道具** (9/24):
- ✅ ITEM_Feather_Note.png
- ✅ ITEM_Feather_Truth.png
- ✅ ITEM_Magnifier_Prop.png
- ✅ OBJ_Crown_Base.png
- ✅ OBJ_Necklace_Base.png
- ✅ OBJ_Mummy_Lying.png
- ✅ OBJ_Crow_Idle.png
- ✅ OBJ_Door_Locked.png
- ✅ OBJ_Sarcophagus_Closed.png

**第二章** (2/41):
- ✅ CHAR_BluePuppet_Idle.png
- ✅ ITEM_C2_001_Sunflower_Seed.png

---

## 🔄 待处理工作

### 剩余文件统计

- **总数**: 106 个文件
- **分布**:
  - 第一章: 24 个 (图标 3 + 道具 21)
  - 第二章: 39 个 (角色 2 + 物品 9 + 道具 28)
  - 第三章: 29 个 (物品 11 + 绘画 4 + 道具 14)
  - 其他场景: 14 个

### 优先级计划

#### P0 - 关键游戏资产 (22个)

**第一章剩余图标** (3个):
- ITEM_Magnifier_Icon.png (已尝试2次，仍为绿色棋盘格)
- ITEM_Necklace_Icon.png (已尝试2次，仍为绿色棋盘格)
- ITEM_Ring_Icon.png (宝石变绿问题)

**第一章木乃伊动画** (9个):
- OBJ_Mummy_Anim_01.png 至 08.png (8个动画帧)
- OBJ_Mummy_Revived.png

**第二章角色** (2个):
- CHAR_PurplePuppet_Merged.png
- CHAR_RedPuppet_Idle.png

**第二章种子和花朵** (8个):
- ITEM_C2_002_WaterLily_Seed.png
- ITEM_C2_003_Moonflower_Seed.png
- ITEM_C2_004_Rose_Seed.png
- ITEM_C2_005_Red_Flower.png
- ITEM_C2_006_Blue_Flower.png
- ITEM_C2_007_Purple_Flower.png
- ITEM_C2_008_White_Flower.png
- ITEM_C2_009_Watering_Can.png

#### P1 - 重要道具 (50个)

**第一章珠宝发光效果** (9个):
- OBJ_Crown_Glow1.png, Glow2.png, Glow3.png
- OBJ_Necklace_Glow1.png, Glow2.png, Glow3.png
- OBJ_Ring_Base.png (已尝试，绿色棋盘格)
- OBJ_Ring_Glow1.png, Glow2.png, Glow3.png

**第一章其他道具** (12个):
- OBJ_Cartier_Case_Closed.png
- OBJ_Cartier_Case_Open.png
- OBJ_Diary_Alchemist.png
- OBJ_Door_Open.png
- OBJ_MusicBox_Closed.png
- OBJ_MusicBox_Open.png
- OBJ_Poster_Blurry.png
- OBJ_Poster_Clear.png
- OBJ_Sarcophagus_Open.png
- OBJ_Scale.png
- OBJ_Stele_Hieroglyph.png

**第二章齿轮和植物** (29个):
- 4 个齿轮 (Gold, Silver, Bronze, Copper)
- 25 个植物生长阶段

#### P2 - 补充资产 (34个)

**第三章** (29个):
- 11 个 Items
- 4 个 Paintings
- 14 个 Props

**其他场景** (5个):
- Intro, Memories, Climax, Ending 场景资产

---

## 🛠️ 技术方案

### 使用工具

**NanoBanana Pro** 的图像编辑功能：
- 工具名称: `generate_image_variation`
- 功能: 基于参考图像进行 AI 编辑
- 输出格式: PNG (RGBA)

### 标准提示词模板

#### 基础模板（适用于大多数物体）

```
Remove all background and replace with flat solid bright green color (#00FF00, chroma key green). 
The background must be completely uniform pure green with no gradients, no patterns, no checkerboard. 
Keep the [OBJECT_NAME] exactly the same.
```

**示例**:
```
Remove all background and replace with flat solid bright green color (#00FF00, chroma key green). 
The background must be completely uniform pure green with no gradients, no patterns, no checkerboard. 
Keep the crown illustration exactly the same.
```

#### 透明元素模板（适用于玻璃、宝石等）

```
Replace the checkered background with solid bright green (#00FF00). 
Keep the [OBJECT_FRAME] exactly as it is. 
The [TRANSPARENT_PART] should remain clear/white/slightly tinted, not green. 
Only the outer background should be pure solid green.
```

**示例**:
```
Replace the checkered background with solid bright green (#00FF00). 
Keep the magnifying glass frame and handle exactly as they are. 
The glass lens should remain clear and transparent, not filled with green. 
Only the outer background should be pure solid green.
```

#### 发光效果模板（实验性）

```
Remove all background and replace with flat solid bright green color (#00FF00, chroma key green). 
The background must be completely uniform pure green. 
Keep the [OBJECT] with its glowing effect intact. 
The glow should remain visible and not be absorbed into the green background.
```

### 处理流程

#### 单个文件处理步骤

1. **调用工具**:
   ```
   generate_image_variation(
       path="/home/ubuntu/greenscreen_batch/[FILENAME].png",
       prompt="[使用上述模板]",
       references=["[原文件路径]"]
   )
   ```

2. **查看结果**: 检查生成的图片

3. **质量评估**:
   - ✅ 高质量: 纯绿色背景，物体完好
   - ⚠️ 中等质量: 绿色棋盘格或部分绿色
   - ❌ 低质量: 背景错误或物体变形

4. **决策**:
   - 高质量 → 复制到原目录
   - 中等质量 → 考虑重试或接受
   - 低质量 → 重试或跳过

#### 批量处理建议

**每批处理 10-15 个文件**，然后：
1. 统计成功率
2. 复制高质量文件到原目录
3. 提交到 Git
4. 更新进度文档

---

## 📁 文件和目录结构

### 工作目录

```
/home/ubuntu/
├── rusty-lake-orrery-game/          # Git 仓库
│   ├── Content/Art/                  # 原始资产目录
│   │   ├── Chapter1/
│   │   │   ├── Icons/               # 图标
│   │   │   └── Props/               # 道具
│   │   ├── Chapter2/
│   │   │   ├── Characters/          # 角色
│   │   │   ├── Items/               # 物品
│   │   │   └── Props/               # 道具
│   │   └── Chapter3/
│   ├── GREENSCREEN_TODO.md          # 待办清单
│   ├── GREENSCREEN_HANDOVER.md      # 本交接文档
│   └── .git/
├── greenscreen_batch/                # 临时工作目录
│   └── [生成的绿幕文件]
└── [其他临时文件]
```

### 关键文件

| 文件 | 路径 | 说明 |
|------|------|------|
| 待办清单 | `/home/ubuntu/rusty-lake-orrery-game/GREENSCREEN_TODO.md` | 详细的待处理文件列表 |
| 交接文档 | `/home/ubuntu/rusty-lake-orrery-game/GREENSCREEN_HANDOVER.md` | 本文档 |
| 质量报告 | `/home/ubuntu/greenscreen_quality_check.md` | 第一批处理的质量分析 |
| 完成报告 | `/home/ubuntu/greenscreen_conversion_final_report.md` | 第一批处理的详细报告 |
| 文件列表 | `/tmp/fake_transparent_list.txt` | 所有需要转换的文件列表 |

---

## 🔧 操作指南

### 开始工作

#### 1. 克隆仓库（如果尚未克隆）

```bash
cd /home/ubuntu
gh repo clone gdszyy/rusty-lake-orrery-game
cd rusty-lake-orrery-game
git checkout feature/art-ui-v2
```

#### 2. 创建工作目录

```bash
mkdir -p /home/ubuntu/greenscreen_batch
```

#### 3. 查看待办清单

```bash
cat /home/ubuntu/rusty-lake-orrery-game/GREENSCREEN_TODO.md
```

### 处理文件

#### 示例：处理一个图标

```python
# 使用 generate_image_variation 工具
generate_image_variation(
    brief="处理 ITEM_Magnifier_Icon",
    path="/home/ubuntu/greenscreen_batch/ITEM_Magnifier_Icon.png",
    prompt="Remove all background and replace with flat solid bright green color (#00FF00, chroma key green). The background must be completely uniform pure green with no gradients, no patterns, no checkerboard. Keep the magnifying glass exactly the same.",
    references=["/home/ubuntu/rusty-lake-orrery-game/Content/Art/Chapter1/Icons/ITEM_Magnifier_Icon.png"]
)
```

#### 查看结果

```python
# 使用 file 工具查看生成的图片
file(
    action="view",
    path="/home/ubuntu/greenscreen_batch/ITEM_Magnifier_Icon.png"
)
```

### 批量复制成功的文件

#### 创建复制脚本

```bash
cat > /tmp/copy_batch.sh << 'EOF'
#!/bin/bash

# 定义源和目标目录
BATCH="/home/ubuntu/greenscreen_batch"
BASE="/home/ubuntu/rusty-lake-orrery-game/Content/Art"

# 复制文件（根据实际情况修改）
cp $BATCH/ITEM_Example.png $BASE/Chapter1/Icons/

echo "复制完成"
EOF

bash /tmp/copy_batch.sh
```

### 提交到 Git

```bash
cd /home/ubuntu/rusty-lake-orrery-game

# 查看修改
git status

# 添加文件
git add Content/Art

# 提交
git commit -m "feat: 转换第X批资产为纯绿幕背景

转换文件列表：
- [列出文件]

技术参数：
- 背景颜色: #00FF00
- 成功率: XX%
- 工具: NanoBanana Pro"

# 推送
git push origin feature/art-ui-v2
```

---

## 📊 质量标准

### 高质量 ✅

**标准**:
- 背景为纯绿色 (#00FF00) 或均匀绿色
- 物体本身保持原样，颜色和细节完整
- 边缘清晰，无明显瑕疵
- 无渐变、无图案、无棋盘格

**处理**: 直接复制到原目录并提交

### 中等质量 ⚠️

**标准**:
- 背景为绿色棋盘格（可接受）
- 物体内部有少量绿色（如羽毛纹理、玻璃反光）
- 边缘略有瑕疵但不影响使用

**处理**: 
- 可以接受，在 UE4 中通过 Chroma Key 参数调整
- 或者尝试重新生成

### 低质量 ❌

**标准**:
- 背景不是绿色（灰色、棕色、其他颜色）
- 物体严重变形或颜色错误
- 无法通过 Chroma Key 处理

**处理**: 必须重新生成或跳过

---

## ⚠️ 已知问题和解决方案

### 问题 1: 绿色棋盘格

**现象**: 生成绿色棋盘格而非纯色

**频率**: 约 20-30%

**解决方案**:
1. 在提示词中强调 "no checkerboard"
2. 重试 1-2 次
3. 如果仍失败，接受棋盘格（在 UE4 中效果相同）

### 问题 2: 透明元素变绿

**现象**: 玻璃、宝石等透明部分被填充绿色

**受影响文件**:
- ITEM_Magnifier_Icon.png
- ITEM_Ring_Icon.png
- OBJ_Ring_Base.png

**解决方案**:
1. 使用"透明元素模板"提示词
2. 明确说明哪些部分应保持透明
3. 如果仍失败，考虑在 UE4 中通过材质处理

### 问题 3: 发光效果失败

**现象**: 发光效果资产背景变为其他颜色

**受影响文件**:
- OBJ_Crown_Glow1.png
- 其他 Glow 系列文件

**解决方案**:
1. 使用"发光效果模板"
2. 如果多次失败，考虑跳过
3. 在 UE4 中通过材质和粒子系统实现发光效果

### 问题 4: 上传 API 失败

**现象**: "Sandbox Upload API Failed" 错误

**解决方案**:
1. 等待几分钟后重试
2. 检查网络连接
3. 如果持续失败，跳过该文件，稍后处理

---

## 📈 进度跟踪

### 更新进度

每完成一批（10-15 个文件），更新以下文档：

#### 1. 更新 GREENSCREEN_TODO.md

将 ⏳ 改为 ✅ 或 ❌：

```markdown
- ✅ ITEM_Example.png（已完成）
- ❌ ITEM_Failed.png（失败，需重试）
```

#### 2. 创建批次报告

```markdown
### 第X批处理（日期）

- **处理数量**: XX 个
- **成功**: XX 个
- **失败**: XX 个
- **成功率**: XX%
- **提交 SHA**: xxxxxxx
```

#### 3. 更新统计

```markdown
## 总体进度

- **已完成**: XX/121 (XX%)
- **剩余**: XX 个
- **预计完成时间**: XX 小时
```

---

## 🎯 建议的工作计划

### 第一天（2-3小时）

**目标**: 完成 P0 优先级的 22 个文件

1. **第一章剩余图标** (3个) - 30分钟
   - 重点处理透明元素问题
   - 可接受中等质量

2. **第一章木乃伊动画** (9个) - 1小时
   - 动画帧通常较简单
   - 预期成功率 70%+

3. **第二章角色** (2个) - 20分钟
   - 参考已成功的 BluePuppet

4. **第二章种子和花朵** (8个) - 1小时
   - 参考已成功的 Sunflower_Seed

### 第二天（3-4小时）

**目标**: 完成 P1 优先级的 50 个文件

1. **第一章珠宝发光效果** (9个) - 1.5小时
   - 预期困难，可能需要多次重试
   - 如果失败率高，考虑跳过

2. **第一章其他道具** (12个) - 1小时
   - 参考已成功的道具

3. **第二章齿轮和植物** (29个) - 2小时
   - 分批处理
   - 植物生长阶段可能较复杂

### 第三天（2-3小时）

**目标**: 完成 P2 优先级的 34 个文件

1. **第三章资产** (29个) - 2小时
   - 新类型资产，需要探索最佳提示词

2. **其他场景资产** (5个) - 30分钟

3. **重试失败文件** - 30分钟
   - 调整提示词重试

---

## 📝 提交规范

### Git Commit 消息格式

```
feat: 转换第X批资产为纯绿幕背景（XX个文件）

转换文件列表：
- ChapterX/Type: File1, File2, File3
- ChapterY/Type: File4, File5

技术参数：
- 背景颜色: #00FF00 (纯绿色)
- 成功率: XX% (XX/XX)
- 工具: NanoBanana Pro
- 处理时间: XX分钟

备注：
- [特殊说明，如果有]
```

### 示例

```
feat: 转换第2批资产为纯绿幕背景（9个文件）

转换文件列表：
- Chapter1/Props/Mummy: OBJ_Mummy_Anim_01~08, OBJ_Mummy_Revived

技术参数：
- 背景颜色: #00FF00 (纯绿色)
- 成功率: 77.8% (7/9)
- 工具: NanoBanana Pro
- 处理时间: 45分钟

备注：
- OBJ_Mummy_Anim_03 和 05 需要重试
```

---

## 🔗 相关资源

### 文档

- [GREENSCREEN_TODO.md](./GREENSCREEN_TODO.md) - 待办清单
- [greenscreen_conversion_final_report.md](/home/ubuntu/greenscreen_conversion_final_report.md) - 第一批完成报告
- [greenscreen_quality_check.md](/home/ubuntu/greenscreen_quality_check.md) - 质量检查报告

### GitHub

- **仓库**: https://github.com/gdszyy/rusty-lake-orrery-game
- **分支**: feature/art-ui-v2
- **最新提交**: bb3aa97

### UE4 集成

参考 `greenscreen_conversion_final_report.md` 中的 "UE4 集成建议" 章节。

---

## ❓ 常见问题

### Q1: 如何判断是否需要重试？

**A**: 
- 如果背景是绿色棋盘格 → 可以接受或重试1次
- 如果物体有少量绿色 → 可以接受
- 如果背景不是绿色 → 必须重试
- 如果物体严重变形 → 必须重试

### Q2: 重试多少次后应该放弃？

**A**: 
- 普通物体：重试 2-3 次
- 透明元素：重试 3-4 次
- 发光效果：重试 2 次后考虑跳过

### Q3: 如何处理批量相似的文件？

**A**: 
- 先处理 1 个，找到最佳提示词
- 然后使用相同提示词处理其他文件
- 例如：木乃伊动画帧、植物生长阶段

### Q4: 如果遇到新问题怎么办？

**A**: 
1. 记录问题和受影响的文件
2. 尝试调整提示词
3. 在交接文档中添加新的"已知问题"
4. 如果无法解决，标记为"需要特殊处理"

### Q5: 完成后需要做什么？

**A**: 
1. 更新所有进度文档
2. 创建最终完成报告
3. 提交所有更改到 Git
4. 在 GitHub 上创建 Pull Request（如果需要）

---

## 📞 联系和支持

如果遇到无法解决的问题：

1. **查看已有文档**: 先查看所有相关文档
2. **记录问题**: 在交接文档中添加新的问题记录
3. **标记文件**: 将无法处理的文件标记为"需要人工处理"

---

## ✨ 最后的话

这个任务需要耐心和细心。NanoBanana Pro 的 AI 有时会出现不可预测的结果，这是正常的。重要的是：

1. **保持一致性**: 使用标准化的提示词和流程
2. **记录进度**: 及时更新文档
3. **灵活应对**: 根据结果调整策略
4. **质量优先**: 不要为了速度牺牲质量

祝工作顺利！🚀

---

**文档版本**: v1.0  
**创建日期**: 2024-12-08  
**最后更新**: 2024-12-08  
**创建者**: Manus AI Agent
