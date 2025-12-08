# 美术资源工具集 (Art Asset Tools)

**作者**: Manus AI  
**版本**: 1.0.0  
**日期**: 2025-12-08

---

本目录包含一系列用于辅助《锈湖：炼金术士的天体仪》项目美术资源管理的命令行工具。

## 1. 动画序列验证工具 (Animation Sequence Validator)

**脚本**: `validate_animation_sequences.py`

**用途**: 自动扫描 `Content/Art` 目录下的所有美术资源，检查动画序列的完整性和连续性，并生成详细的验证报告。

### 功能特性

- **自动识别**: 能够自动识别以 `_` 加数字（如 `_01`, `_1`）结尾的PNG文件，并将其归类为动画序列。
- **完整性检查**: 验证一个序列的帧数是否满足最小数量要求（默认为3帧）。
- **连续性检查**: 确保序列的帧序号是连续的（如 1, 2, 3, 4），没有跳帧或缺失。
- **多变体识别**: 能够智能识别并区分合理的多变体设计（如同一帧号但后缀不同，`_Blank`, `_Starry`），避免误报为错误。
- **报告生成**: 生成清晰的文本报告，汇总统计信息（完整、警告、错误），并列出每个序列的详细检查结果。

### 使用方法

在项目根目录下执行以下命令：

```bash
python3 Tools/validate_animation_sequences.py
```

脚本将自动扫描默认的资源目录 (`Content/Art`)，并在控制台打印报告。同时，一份详细的报告将保存至 `Docs/REPORTS/animation_validation_report.txt`。

你也可以指定一个不同的美术资源目录作为参数：

```bash
python3 Tools/validate_animation_sequences.py /path/to/your/art/directory
```

## 2. 资源预览图册生成工具 (Asset Catalog Generator)

**脚本**: `generate_asset_catalog.py`

**用途**: 自动扫描 `Content/Art` 目录下的所有美术资源，并生成一个独立的、可交互的HTML预览图册，方便团队成员快速浏览、搜索和评估所有资源。

### 功能特性

- **自动扫描与分类**: 自动遍历所有资源，并按“章节”和“类别”（如Icons, Props, Scenes）进行组织。
- **Base64内联**: 将所有图片资源通过Base64编码直接嵌入HTML文件中，生成一个完全独立的单文件图册，无需Web服务器即可本地打开。
- **统计信息**: 在页面顶部显示总资源数、章节数、类别数等关键统计数据。
- **交互式预览**: 
    - **网格布局**: 以卡片形式清晰展示每个资源。
    - **详细信息**: 每个卡片显示资源名称和文件大小。
    - **点击放大**: 点击任意图片可全屏模态框放大预览。
- **实时搜索**: 提供一个搜索框，可以根据资源名称动态过滤和查找特定资源。
- **美观的UI**: 采用暗色主题，设计风格与“锈湖”系列保持一致，提供良好的视觉体验。

### 使用方法

在项目根目录下执行以下命令：

```bash
python3 Tools/generate_asset_catalog.py
```

脚本将自动扫描默认的资源目录 (`Content/Art`)，并生成预览图册。完成后，最终的HTML文件将保存至 `Docs/CATALOG/asset_catalog.html`。

**注意**: 由于所有图片都被内联到HTML文件中，生成的文件可能会非常大（例如，对于139个资源，文件大小可能超过250MB）。请耐心等待脚本执行完毕。

生成后，直接在浏览器中打开该HTML文件即可查看。

```bash
# 例如，在macOS上
open Docs/CATALOG/asset_catalog.html

# 在Windows上
start Docs\CATALOG\asset_catalog.html

# 在Linux上
xdg-open Docs/CATALOG/asset_catalog.html
```
