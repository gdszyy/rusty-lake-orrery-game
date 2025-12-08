#!/usr/bin/env python3
"""
资源预览图册生成工具 (Asset Catalog Generator)

用途：自动生成美术资源的HTML预览图册，方便团队快速浏览和选择资源
作者：Manus AI
版本：1.0.0
日期：2025-12-08
"""

import base64
import sys
from pathlib import Path
from typing import Dict, List
from collections import defaultdict


class AssetCatalogGenerator:
    """资源预览图册生成器"""
    
    def __init__(self, art_root: Path, output_path: Path):
        """
        初始化生成器
        
        Args:
            art_root: 美术资源根目录路径
            output_path: 输出HTML文件路径
        """
        self.art_root = Path(art_root)
        self.output_path = Path(output_path)
        self.assets: Dict[str, Dict[str, List[Path]]] = defaultdict(lambda: defaultdict(list))
        
    def scan_assets(self) -> None:
        """扫描所有美术资源"""
        for png_file in sorted(self.art_root.rglob("*.png")):
            relative_path = png_file.relative_to(self.art_root)
            parts = relative_path.parts
            
            if len(parts) >= 2:
                chapter = parts[0]  # e.g., "Chapter1"
                category = parts[1]  # e.g., "Icons", "Props", "Scenes"
                self.assets[chapter][category].append(png_file)
    
    def _image_to_base64(self, image_path: Path) -> str:
        """
        将图片转换为base64编码
        
        Args:
            image_path: 图片文件路径
            
        Returns:
            base64编码的图片数据URI
        """
        with open(image_path, "rb") as f:
            image_data = f.read()
            base64_data = base64.b64encode(image_data).decode("utf-8")
            return f"data:image/png;base64,{base64_data}"
    
    def _get_file_size(self, file_path: Path) -> str:
        """
        获取文件大小的可读格式
        
        Args:
            file_path: 文件路径
            
        Returns:
            格式化的文件大小字符串
        """
        size_bytes = file_path.stat().st_size
        
        if size_bytes < 1024:
            return f"{size_bytes} B"
        elif size_bytes < 1024 * 1024:
            return f"{size_bytes / 1024:.1f} KB"
        else:
            return f"{size_bytes / (1024 * 1024):.2f} MB"
    
    def generate_html(self) -> None:
        """生成HTML预览图册"""
        html_parts = []
        
        # HTML头部
        html_parts.append("""<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>《锈湖：炼金术士的天体仪》美术资源预览图册</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
            background: linear-gradient(135deg, #1a1a1a 0%, #2d2d2d 100%);
            color: #e0e0e0;
            padding: 20px;
            line-height: 1.6;
        }
        
        .container {
            max-width: 1400px;
            margin: 0 auto;
            background: rgba(30, 30, 30, 0.95);
            border-radius: 12px;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.5);
            padding: 40px;
        }
        
        h1 {
            text-align: center;
            color: #d4af37;
            font-size: 2.5em;
            margin-bottom: 10px;
            text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.5);
        }
        
        .subtitle {
            text-align: center;
            color: #a0a0a0;
            margin-bottom: 40px;
            font-size: 1.1em;
        }
        
        .stats {
            display: flex;
            justify-content: center;
            gap: 30px;
            margin-bottom: 40px;
            padding: 20px;
            background: rgba(212, 175, 55, 0.1);
            border-radius: 8px;
            border: 1px solid rgba(212, 175, 55, 0.3);
        }
        
        .stat-item {
            text-align: center;
        }
        
        .stat-number {
            font-size: 2em;
            font-weight: bold;
            color: #d4af37;
        }
        
        .stat-label {
            color: #a0a0a0;
            font-size: 0.9em;
            margin-top: 5px;
        }
        
        .chapter {
            margin-bottom: 50px;
        }
        
        .chapter-title {
            font-size: 2em;
            color: #d4af37;
            margin-bottom: 20px;
            padding-bottom: 10px;
            border-bottom: 2px solid rgba(212, 175, 55, 0.3);
        }
        
        .category {
            margin-bottom: 40px;
        }
        
        .category-title {
            font-size: 1.5em;
            color: #b8956a;
            margin-bottom: 20px;
            padding-left: 15px;
            border-left: 4px solid #d4af37;
        }
        
        .asset-grid {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(280px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }
        
        .asset-card {
            background: rgba(40, 40, 40, 0.8);
            border-radius: 8px;
            padding: 15px;
            transition: all 0.3s ease;
            border: 1px solid rgba(212, 175, 55, 0.2);
        }
        
        .asset-card:hover {
            transform: translateY(-5px);
            box-shadow: 0 8px 20px rgba(212, 175, 55, 0.3);
            border-color: rgba(212, 175, 55, 0.5);
        }
        
        .asset-image-container {
            width: 100%;
            height: 200px;
            display: flex;
            align-items: center;
            justify-content: center;
            background: repeating-conic-gradient(#2a2a2a 0% 25%, #333333 0% 50%) 50% / 20px 20px;
            border-radius: 6px;
            margin-bottom: 12px;
            overflow: hidden;
        }
        
        .asset-image {
            max-width: 100%;
            max-height: 100%;
            object-fit: contain;
            cursor: pointer;
        }
        
        .asset-name {
            font-weight: bold;
            color: #e0e0e0;
            margin-bottom: 8px;
            word-break: break-word;
            font-size: 0.95em;
        }
        
        .asset-info {
            display: flex;
            justify-content: space-between;
            color: #a0a0a0;
            font-size: 0.85em;
        }
        
        .modal {
            display: none;
            position: fixed;
            z-index: 1000;
            left: 0;
            top: 0;
            width: 100%;
            height: 100%;
            background-color: rgba(0, 0, 0, 0.9);
            align-items: center;
            justify-content: center;
        }
        
        .modal-content {
            max-width: 90%;
            max-height: 90%;
            object-fit: contain;
        }
        
        .close {
            position: absolute;
            top: 30px;
            right: 50px;
            color: #d4af37;
            font-size: 50px;
            font-weight: bold;
            cursor: pointer;
            transition: color 0.3s;
        }
        
        .close:hover {
            color: #fff;
        }
        
        .filter-bar {
            margin-bottom: 30px;
            padding: 20px;
            background: rgba(40, 40, 40, 0.8);
            border-radius: 8px;
            display: flex;
            gap: 15px;
            flex-wrap: wrap;
            align-items: center;
        }
        
        .filter-bar label {
            color: #d4af37;
            font-weight: bold;
        }
        
        .filter-bar input {
            padding: 8px 15px;
            border: 1px solid rgba(212, 175, 55, 0.3);
            background: rgba(30, 30, 30, 0.8);
            color: #e0e0e0;
            border-radius: 4px;
            font-size: 0.95em;
        }
        
        .filter-bar input:focus {
            outline: none;
            border-color: #d4af37;
        }
        
        footer {
            text-align: center;
            margin-top: 50px;
            padding-top: 30px;
            border-top: 1px solid rgba(212, 175, 55, 0.2);
            color: #808080;
            font-size: 0.9em;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🎨 美术资源预览图册</h1>
        <div class="subtitle">《锈湖：炼金术士的天体仪》Art Asset Catalog</div>
""")
        
        # 统计信息
        total_assets = sum(len(assets) for chapter_data in self.assets.values() for assets in chapter_data.values())
        chapter_count = len(self.assets)
        category_count = sum(len(categories) for categories in self.assets.values())
        
        html_parts.append(f"""
        <div class="stats">
            <div class="stat-item">
                <div class="stat-number">{total_assets}</div>
                <div class="stat-label">总资源数</div>
            </div>
            <div class="stat-item">
                <div class="stat-number">{chapter_count}</div>
                <div class="stat-label">章节数</div>
            </div>
            <div class="stat-item">
                <div class="stat-number">{category_count}</div>
                <div class="stat-label">资源类别</div>
            </div>
        </div>
        
        <div class="filter-bar">
            <label for="searchInput">🔍 搜索资源：</label>
            <input type="text" id="searchInput" placeholder="输入资源名称进行搜索..." style="flex: 1; min-width: 300px;">
        </div>
""")
        
        # 章节和资源
        for chapter in sorted(self.assets.keys()):
            html_parts.append(f'<div class="chapter" data-chapter="{chapter}">')
            html_parts.append(f'<h2 class="chapter-title">📖 {chapter}</h2>')
            
            for category in sorted(self.assets[chapter].keys()):
                assets = self.assets[chapter][category]
                html_parts.append(f'<div class="category">')
                html_parts.append(f'<h3 class="category-title">📁 {category} ({len(assets)})</h3>')
                html_parts.append('<div class="asset-grid">')
                
                for asset_path in sorted(assets):
                    asset_name = asset_path.name
                    asset_size = self._get_file_size(asset_path)
                    
                    # 将图片转换为base64
                    try:
                        image_data = self._image_to_base64(asset_path)
                        
                        html_parts.append(f'''
                        <div class="asset-card" data-name="{asset_name.lower()}">
                            <div class="asset-image-container">
                                <img src="{image_data}" alt="{asset_name}" class="asset-image" onclick="openModal(this.src)">
                            </div>
                            <div class="asset-name">{asset_name}</div>
                            <div class="asset-info">
                                <span>{asset_size}</span>
                            </div>
                        </div>
                        ''')
                    except Exception as e:
                        print(f"警告: 无法处理图片 {asset_path}: {e}", file=sys.stderr)
                
                html_parts.append('</div>')  # asset-grid
                html_parts.append('</div>')  # category
            
            html_parts.append('</div>')  # chapter
        
        # 模态框和JavaScript
        html_parts.append("""
        <footer>
            <p>生成时间: <span id="generateTime"></span></p>
            <p>Powered by Manus AI | 《锈湖：炼金术士的天体仪》项目</p>
        </footer>
    </div>
    
    <div id="imageModal" class="modal" onclick="closeModal()">
        <span class="close">&times;</span>
        <img class="modal-content" id="modalImage">
    </div>
    
    <script>
        // 设置生成时间
        document.getElementById('generateTime').textContent = new Date().toLocaleString('zh-CN');
        
        // 图片模态框
        function openModal(src) {
            const modal = document.getElementById('imageModal');
            const modalImg = document.getElementById('modalImage');
            modal.style.display = 'flex';
            modalImg.src = src;
        }
        
        function closeModal() {
            document.getElementById('imageModal').style.display = 'none';
        }
        
        // 搜索功能
        const searchInput = document.getElementById('searchInput');
        searchInput.addEventListener('input', function() {
            const searchTerm = this.value.toLowerCase();
            const assetCards = document.querySelectorAll('.asset-card');
            
            assetCards.forEach(card => {
                const assetName = card.getAttribute('data-name');
                if (assetName.includes(searchTerm)) {
                    card.style.display = 'block';
                } else {
                    card.style.display = 'none';
                }
            });
        });
        
        // ESC键关闭模态框
        document.addEventListener('keydown', function(event) {
            if (event.key === 'Escape') {
                closeModal();
            }
        });
    </script>
</body>
</html>
""")
        
        # 写入文件
        html_content = "".join(html_parts)
        self.output_path.parent.mkdir(parents=True, exist_ok=True)
        self.output_path.write_text(html_content, encoding="utf-8")
        
        print(f"✅ 预览图册生成成功: {self.output_path}")
        print(f"📊 总计: {total_assets} 个资源")
        print(f"📖 章节: {chapter_count} 个")
        print(f"📁 类别: {category_count} 个")


def main():
    """主函数"""
    # 检查参数
    if len(sys.argv) > 2:
        art_root = Path(sys.argv[1])
        output_path = Path(sys.argv[2])
    elif len(sys.argv) > 1:
        art_root = Path(sys.argv[1])
        output_path = Path(__file__).parent.parent / "Docs" / "CATALOG" / "asset_catalog.html"
    else:
        # 默认路径
        art_root = Path(__file__).parent.parent / "Content" / "Art"
        output_path = Path(__file__).parent.parent / "Docs" / "CATALOG" / "asset_catalog.html"
    
    if not art_root.exists():
        print(f"❌ 错误: 美术资源目录不存在: {art_root}", file=sys.stderr)
        sys.exit(1)
    
    print(f"🔍 正在扫描美术资源目录: {art_root}")
    print(f"📝 输出路径: {output_path}")
    print()
    
    # 创建生成器并执行
    generator = AssetCatalogGenerator(art_root, output_path)
    generator.scan_assets()
    generator.generate_html()
    
    print()
    print(f"💡 提示: 在浏览器中打开 {output_path} 即可查看预览图册")


if __name__ == "__main__":
    main()
