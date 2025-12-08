#!/usr/bin/env python3
import os
import sys
from PIL import Image
from pathlib import Path

def should_be_transparent(filepath):
    """判断文件是否应该是透明底的"""
    filename = os.path.basename(filepath)
    
    # 场景背景不应该透明
    if filename.startswith('BG_'):
        return False
    
    # 以下类型应该透明
    transparent_prefixes = ['ITEM_', 'OBJ_', 'CHAR_', 'PLANT_', 'PAINT_']
    
    for prefix in transparent_prefixes:
        if filename.startswith(prefix):
            return True
    
    return False

def check_image_mode(filepath):
    """检查图片模式"""
    try:
        img = Image.open(filepath)
        return img.mode, img.size
    except Exception as e:
        return None, str(e)

def scan_directory(root_dir):
    """扫描目录中的所有 PNG 文件"""
    results = {
        'rgb_should_transparent': [],  # RGB 模式但应该透明
        'rgba_real_transparent': [],   # RGBA 模式且真透明
        'rgba_fake_transparent': [],   # RGBA 模式但假透明
        'bg_images': []                # 背景图片
    }
    
    for png_file in Path(root_dir).rglob('*.png'):
        filepath = str(png_file)
        should_trans = should_be_transparent(filepath)
        mode, size = check_image_mode(filepath)
        
        if mode is None:
            continue
        
        if not should_trans:
            results['bg_images'].append(filepath)
            continue
        
        if mode == 'RGB':
            results['rgb_should_transparent'].append(filepath)
        elif mode == 'RGBA':
            # 检查是否真透明
            img = Image.open(filepath)
            alpha = img.split()[3]
            alpha_range = alpha.getextrema()
            
            if alpha_range[0] == 255 and alpha_range[1] == 255:
                results['rgba_fake_transparent'].append(filepath)
            else:
                results['rgba_real_transparent'].append(filepath)
    
    return results

if __name__ == "__main__":
    root_dir = sys.argv[1] if len(sys.argv) > 1 else "."
    
    print(f"Scanning directory: {root_dir}")
    print("=" * 80)
    
    results = scan_directory(root_dir)
    
    print(f"\n📊 统计结果:")
    print(f"  - RGB 模式（应该透明）: {len(results['rgb_should_transparent'])} 个")
    print(f"  - RGBA 模式（假透明）: {len(results['rgba_fake_transparent'])} 个")
    print(f"  - RGBA 模式（真透明）: {len(results['rgba_real_transparent'])} 个")
    print(f"  - 背景图片（不需要透明）: {len(results['bg_images'])} 个")
    
    print(f"\n🚨 需要转换为绿幕的文件 ({len(results['rgb_should_transparent']) + len(results['rgba_fake_transparent'])} 个):")
    print("=" * 80)
    
    all_fake = results['rgb_should_transparent'] + results['rgba_fake_transparent']
    for filepath in sorted(all_fake):
        rel_path = os.path.relpath(filepath, root_dir)
        print(f"  {rel_path}")
    
    # 保存列表到文件
    with open('/tmp/fake_transparent_list.txt', 'w') as f:
        for filepath in sorted(all_fake):
            f.write(filepath + '\n')
    
    print(f"\n✅ 完整列表已保存到: /tmp/fake_transparent_list.txt")
