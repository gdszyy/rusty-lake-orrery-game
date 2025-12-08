# 绿幕转换 - 快速启动指南

> 这是一份精简的快速启动指南。完整文档请参考 [GREENSCREEN_HANDOVER.md](./GREENSCREEN_HANDOVER.md)

---

## 🚀 5分钟快速开始

### 1. 准备工作

```bash
# 克隆仓库（如果还没有）
cd /home/ubuntu
gh repo clone gdszyy/rusty-lake-orrery-game
cd rusty-lake-orrery-game
git checkout feature/art-ui-v2

# 创建工作目录
mkdir -p /home/ubuntu/greenscreen_batch
```

### 2. 查看待办清单

```bash
cat GREENSCREEN_TODO.md
```

**当前进度**: 15/121 (12.4%)

### 3. 处理第一个文件

使用 `generate_image_variation` 工具：

```python
generate_image_variation(
    brief="处理 [文件名]",
    path="/home/ubuntu/greenscreen_batch/[文件名].png",
    prompt="Remove all background and replace with flat solid bright green color (#00FF00, chroma key green). The background must be completely uniform pure green with no gradients, no patterns, no checkerboard. Keep the [物体描述] exactly the same.",
    references=["/home/ubuntu/rusty-lake-orrery-game/Content/Art/[路径]/[文件名].png"]
)
```

### 4. 查看结果

```python
file(
    action="view",
    path="/home/ubuntu/greenscreen_batch/[文件名].png"
)
```

### 5. 如果成功，复制到原目录

```bash
cp /home/ubuntu/greenscreen_batch/[文件名].png \
   /home/ubuntu/rusty-lake-orrery-game/Content/Art/[路径]/[文件名].png
```

### 6. 提交到 Git

```bash
cd /home/ubuntu/rusty-lake-orrery-game
git add Content/Art
git commit -m "feat: 转换第X批资产为纯绿幕背景"
git push origin feature/art-ui-v2
```

---

## 📋 下一步处理的文件

### 优先级 P0（建议先处理）

**第一章木乃伊动画** (9个文件):
```
/home/ubuntu/rusty-lake-orrery-game/Content/Art/Chapter1/Props/Mummy/
├── OBJ_Mummy_Anim_01.png
├── OBJ_Mummy_Anim_02.png
├── OBJ_Mummy_Anim_03.png
├── OBJ_Mummy_Anim_04.png
├── OBJ_Mummy_Anim_05.png
├── OBJ_Mummy_Anim_06.png
├── OBJ_Mummy_Anim_07.png
├── OBJ_Mummy_Anim_08.png
└── OBJ_Mummy_Revived.png
```

**提示词**:
```
Remove all background and replace with flat solid bright green color (#00FF00, chroma key green). The background must be completely uniform pure green with no gradients, no patterns, no checkerboard. Keep the mummy character exactly the same.
```

---

## 📝 提示词速查表

### 基础物体
```
Remove all background and replace with flat solid bright green color (#00FF00, chroma key green). 
The background must be completely uniform pure green with no gradients, no patterns, no checkerboard. 
Keep the [OBJECT] exactly the same.
```

### 透明元素（玻璃、宝石）
```
Replace the checkered background with solid bright green (#00FF00). 
Keep the [FRAME] exactly as it is. 
The [TRANSPARENT_PART] should remain clear/white/slightly tinted, not green. 
Only the outer background should be pure solid green.
```

### 角色
```
Remove all background and replace with flat solid bright green color (#00FF00, chroma key green). 
The background must be completely uniform pure green with no gradients, no patterns, no checkerboard. 
Keep the character exactly the same with all details preserved.
```

### 植物
```
Remove all background and replace with flat solid bright green color (#00FF00, chroma key green). 
The background must be completely uniform pure green with no gradients, no patterns, no checkerboard. 
Keep the plant exactly the same.
```

---

## ✅ 质量检查清单

生成后检查：

- [ ] 背景是纯绿色或均匀绿色？
- [ ] 物体保持原样？
- [ ] 边缘清晰？
- [ ] 没有明显瑕疵？

**如果全部是 ✓** → 复制到原目录  
**如果有 ✗** → 考虑重试或调整提示词

---

## 🔄 批量处理建议

**每批 10-15 个文件**，然后：

1. ✅ 统计成功率
2. 📁 复制成功的文件
3. 💾 提交到 Git  
4. 📊 更新进度文档

---

## 📞 需要帮助？

查看完整文档：
- [GREENSCREEN_HANDOVER.md](./GREENSCREEN_HANDOVER.md) - 详细交接文档
- [GREENSCREEN_TODO.md](./GREENSCREEN_TODO.md) - 待办清单

---

**祝工作顺利！** 🚀
