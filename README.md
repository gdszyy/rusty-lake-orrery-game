# 锈湖：炼金术士的天体仪 (Rusty Lake: The Alchemist's Orrery)

![Project Status](https://img.shields.io/badge/status-in%20development-yellow)
![Engine](https://img.shields.io/badge/engine-Unreal%20Engine%204.27-blue)
![Platform](https://img.shields.io/badge/platform-Android-green)
![License](https://img.shields.io/badge/license-Private-red)

一款融合锈湖系列超现实风格的混合现实解谜游戏，结合电子游戏与3D打印实体物品，为一周年纪念与2026年跨年创造独特体验。

---

## 📖 项目概述

### 核心概念

本项目是一个**混合现实解谜体验**，由两个部分组成：

1. **电子游戏**：基于Unreal Engine 4开发的Android点击式解谜游戏《锈湖：观星者》
2. **实体物品**：一个由3D打印制作、通过磁力机制解锁的"记忆天体仪"

玩家在游戏中探索炼金术士的天文台，完成三个章节的谜题，获得三个神秘符号。然后在现实世界中，使用这些符号解锁实体天体仪，获得最终的周年纪念礼物。

### 故事背景

在2025年的最后一天，玩家收到一个3D打印的"记忆天体仪"和一张带有QR码的卡片。扫描后进入游戏，扮演"继承者"，探索一位失踪炼金术士的记忆世界。炼金术士相信每段重要记忆都对应独特的星辰排列，他建造了天体仪试图在跨年时捕获"永恒的完美瞬间"。玩家需要修正混乱的星辰轨迹，解锁现实中的天体仪。

### 游戏特色

- **纯正锈湖风格**：超现实、怪诞的视觉与叙事风格
- **碎片化叙事**：通过日记、笔记、符号等零散线索拼凑真相
- **个人化彩蛋**：融入六个共同回忆（卡地亚珠宝展、埃及展、双人成形、安吉百草园、列宾艺术特展、小刚演唱会）
- **混合现实交互**：虚拟解谜与物理解锁的完美结合

---

## 🎮 游戏架构

### 三章节结构

| 章节 | 主题 | 彩蛋 | 产出 |
|------|------|------|------|
| **第一章：乌鸦与钟楼** | 时间的循环与停滞 | 小刚演唱会 + 安吉百草园 | 外层环符号 |
| **第二章：蝴蝶与画廊** | 空间的折叠与美学 | 卡地亚珠宝展 + 列宾艺术特展 | 中层环符号 |
| **第三章：双子与舞台** | 灵魂的共鸣与镜像 | 埃及展 + 双人成形 | 内层环符号 |

### 技术栈

- **游戏引擎**：Unreal Engine 4.27
- **开发语言**：C++ (底层模块) + Blueprint (游戏逻辑)
- **目标平台**：Android
- **美术生成**：NanoBananaPro (AI图像生成)
- **动画生成**：Sola (AI视频生成)

---

## 🏗️ 项目结构

```
rusty-lake-orrery-game/
├── Source/                      # C++源代码
│   └── RustyLakeOrrery/
│       ├── Public/              # 公共头文件
│       └── Private/             # 私有实现文件
├── Content/                     # 游戏资源
│   ├── Blueprints/              # 蓝图资源
│   ├── Materials/               # 材质
│   ├── Textures/                # 纹理贴图
│   ├── Sounds/                  # 音效和音乐
│   ├── UI/                      # 用户界面
│   ├── Animations/              # 角色动画
│   └── Maps/                    # 关卡地图
├── Docs/                        # 项目文档
│   ├── ARCHITECTURE.md          # 架构设计
│   ├── CODING_STANDARDS.md      # 编码规范
│   ├── MODULES.md               # C++模块说明
│   ├── INTERACTION_DESIGN.md    # 交互设计
│   └── WORKFLOW/                # 各角色工作流程
│       ├── ART_WORKFLOW.md      # 美术工作流程
│       ├── ANIMATION_WORKFLOW.md # 动画工作流程
│       ├── TECH_WORKFLOW.md     # 技术规划流程
│       ├── LEVEL_DESIGN_WORKFLOW.md # 关卡设计流程
│       └── INTEGRATION_WORKFLOW.md # 集成指导流程
└── Config/                      # 配置文件
    └── DefaultEngine.ini        # 引擎配置
```

---

## 👥 团队分工

本项目采用多个Manus对话协作的方式进行开发：

| 角色 | 职责 | 工作内容 |
|------|------|----------|
| **主架构师** | 项目框架与规范 | 建立项目结构、编写技术文档、定义C++模块 |
| **美术对话** | 视觉资源制作 | 使用NanoBananaPro生成UI、环境、道具等美术资源 |
| **动画对话** | 角色动画制作 | 使用Sola生成角色动画视频并转换为游戏动画 |
| **技术规划对话** | 技术架构设计 | 分析交互功能、规划C++模块和蓝图结构 |
| **关卡设计对话** | 关卡与解谜设计 | 设计谜题逻辑链、场景布局、填充数据表、编写交互细节 |
| **集成指导对话** | 项目整合与完成 | 整合所有模块、指导最终项目完成 |

---

## 📚 文档导航

### 核心文档

- [架构设计](Docs/ARCHITECTURE.md) - 游戏整体架构和技术选型
- [编码规范](Docs/CODING_STANDARDS.md) - C++和蓝图开发规范
- [C++模块说明](Docs/MODULES.md) - 所有C++模块的详细说明
- [交互设计](Docs/INTERACTION_DESIGN.md) - 游戏交互功能详细设计

### 工作流程文档

- [美术工作流程](Docs/WORKFLOW/ART_WORKFLOW.md) - 美术资源制作指南
- [动画工作流程](Docs/WORKFLOW/ANIMATION_WORKFLOW.md) - 角色动画制作指南
- [技术规划流程](Docs/WORKFLOW/TECH_WORKFLOW.md) - 技术架构规划指南
- [关卡设计流程](Docs/WORKFLOW/LEVEL_DESIGN_WORKFLOW.md) - 关卡与解谜设计指南
- [集成指导流程](Docs/WORKFLOW/INTEGRATION_WORKFLOW.md) - 项目整合指南

---

## 🚀 快速开始

### 环境要求

- Unreal Engine 4.27
- Visual Studio 2019 或更高版本
- Android SDK (API Level 29+)
- Git

### 克隆仓库

```bash
git clone https://github.com/gdszyy/rusty-lake-orrery-game.git
cd rusty-lake-orrery-game
```

### 生成项目文件

```bash
# 右键点击 RustyLakeOrrery.uproject
# 选择 "Generate Visual Studio project files"
```

### 打开项目

```bash
# 双击 RustyLakeOrrery.uproject 打开UE4编辑器
```

---

## 📋 开发路线图

### Phase 1: 项目框架搭建 ✅
- [x] 创建GitHub仓库
- [x] 建立项目目录结构
- [x] 编写核心文档和规范

### Phase 2: C++模块开发 🔄
- [ ] 实现核心游戏框架模块
- [ ] 实现交互系统模块
- [ ] 实现UI系统模块
- [ ] 实现音效管理模块

### Phase 3: 美术资源制作 ⏳
- [ ] 生成UI界面元素
- [ ] 生成环境场景资源
- [ ] 生成道具和物品资源

### Phase 4: 动画制作 ⏳
- [ ] 生成角色动画
- [ ] 生成交互动画
- [ ] 生成过场动画

### Phase 5: 蓝图逻辑实现 ⏳
- [ ] 实现三个章节的游戏逻辑
- [ ] 实现谜题系统
- [ ] 实现存档系统

### Phase 6: 测试与优化 ⏳
- [ ] 功能测试
- [ ] 性能优化
- [ ] Android平台适配

### Phase 7: 打包发布 ⏳
- [ ] Android APK打包
- [ ] 最终测试
- [ ] 发布准备

---

## 📝 开发规范

### 命名规范

- **C++类**：使用 `U` 前缀（UObject派生）或 `A` 前缀（AActor派生），如 `UInventoryComponent`
- **蓝图**：使用 `BP_` 前缀，如 `BP_PlayerCharacter`
- **材质**：使用 `M_` 前缀，如 `M_RustyMetal`
- **纹理**：使用 `T_` 前缀，如 `T_WallDiffuse`
- **音效**：使用 `SFX_` 前缀，如 `SFX_DoorOpen`

### 代码风格

- 遵循Unreal Engine C++编码标准
- 使用4空格缩进
- 所有公共接口必须添加注释
- 变量命名使用驼峰命名法

详见 [编码规范文档](Docs/CODING_STANDARDS.md)

---

## 🤝 贡献指南

本项目采用多Manus对话协作模式，每个对话负责特定的开发任务。请参考对应的工作流程文档进行开发。

---

## 📄 许可证

本项目为私有项目，仅用于个人纪念目的。

---

## 📧 联系方式

项目负责人：gdszyy
GitHub：https://github.com/gdszyy

---

**注意**：本项目是一个充满爱意的周年纪念礼物项目。请保持代码质量和文档完整性，为最终的惊喜体验做好准备。
