# 第一章复用资产清单

**来源分支**: `feature/art-ui`  
**目标分支**: `feature/art-ui-v2`  
**复用日期**: 2025-12-07

---

## 复用资产列表

### 场景资产 (Scenes)

| 原文件名 | 新文件名 | 资产编号 | 状态 |
|---------|---------|---------|------|
| BG_Chapter1_Museum_Base.png | BG_Museum_Hall1.png | BG_Ch1_001 | ✅ 已复用 |
| BG_Chapter1_Museum_Composed.png | BG_Museum_Hall1_Composed.png | - | ✅ 已复用（合成参考） |

### 可交互道具 (Props)

| 原文件名 | 新文件名 | 资产编号 | 状态 |
|---------|---------|---------|------|
| Prop_Ch1_CartierDisplay.png | OBJ_Cartier_Case_Closed.png | OBJ_Ch1_001 | ✅ 已复用（关闭状态） |
| Prop_Ch1_Sarcophagus_Closed.png | OBJ_Sarcophagus_Closed.png | OBJ_Ch1_006 | ✅ 已复用（关闭状态） |
| Prop_Ch1_MusicBox_Closed.png | OBJ_MusicBox_Closed.png | OBJ_Ch1_015 | ✅ 已复用（关闭状态） |
| Prop_Ch1_BalanceScale.png | OBJ_Scale.png | OBJ_Ch1_012 | ✅ 已复用 |
| Prop_Ch1_Crow.png | OBJ_Crow_Idle.png | OBJ_Ch1_018 | ✅ 已复用（idle基准帧） |
| Prop_Ch1_Magnifier.png | ITEM_Magnifier_Prop.png | ITEM_Ch1_001 | ✅ 已复用 |
| Prop_Ch1_AlchemistDiary.png | OBJ_Diary_Alchemist.png | ITEM_Ch1_009 | ✅ 已复用 |

### 物品栏图标 (Icons)

| 原文件名 | 新文件名 | 资产编号 | 状态 |
|---------|---------|---------|------|
| Icon_Ch1_Magnifier.png | ITEM_Magnifier_Icon.png | ITEM_Ch1_002 | ✅ 已复用 |
| Icon_Ch1_Jewel_Ring.png | ITEM_Ring_Icon.png | ITEM_Ch1_003 | ✅ 已复用 |
| Icon_Ch1_Jewel_Necklace.png | ITEM_Necklace_Icon.png | ITEM_Ch1_004 | ✅ 已复用 |
| Icon_Ch1_Jewel_Crown.png | ITEM_Crown_Icon.png | ITEM_Ch1_005 | ✅ 已复用 |
| Icon_Ch1_NoteFeather.png | ITEM_Note_Feather_Icon.png | ITEM_Ch1_006 | ✅ 已复用 |
| Icon_Ch1_TruthFeather.png | ITEM_Truth_Feather_Icon.png | ITEM_Ch1_007 | ✅ 已复用 |
| Icon_Ch1_PastSymbol.png | ITEM_Symbol_Past.png | ITEM_Ch1_008 | ✅ 已复用 |

---

## 复用统计

- **总复用资产数**: 16个（包含1个合成参考）
- **场景资产**: 2个
- **可交互道具**: 7个
- **物品栏图标**: 7个

---

## 质量保证

所有复用资产均来自 `feature/art-ui` 分支，已通过质量评估：

- ✅ 100%符合锈湖风格指南
- ✅ 透明背景处理优秀，无白边问题
- ✅ 光照方向统一（从上方照射）
- ✅ 所有资产均达到可用标准

---

## 后续工作

### 需要补充的多状态资产

以下资产已有基准状态，需使用"参考图生成法"补充其他状态：

1. **OBJ_Cartier_Case** - 需生成打开状态
2. **OBJ_Sarcophagus** - 需生成打开状态 + 5帧开合动画
3. **OBJ_MusicBox** - 需生成打开状态 + 10帧开盖+旋转动画
4. **OBJ_Crow** - 需生成4帧啄食动画

### 待制作的新资产

请参考 `Docs/ART_ASSETS_LIST.md` 中标记为 ❌ 的资产。
