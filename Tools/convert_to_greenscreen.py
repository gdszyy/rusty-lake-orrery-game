#!/usr/bin/env python3
import os
import sys
from PIL import Image
import numpy as np

def convert_fake_transparent_to_greenscreen(input_path, output_path=None):
    """
    将假透明图片转换为绿幕背景
    假透明的特征：RGB 模式，背景是灰白色棋盘格或纯色
    """
    if output_path is None:
        output_path = input_path
    
    try:
        img = Image.open(input_path)
        
        # 如果已经是 RGBA，检查是否需要转换
        if img.mode == 'RGBA':
            alpha = img.split()[3]
            alpha_range = alpha.getextrema()
            if alpha_range[0] < 255:  # 有真透明
                print(f"  ✓ 已经是真透明，跳过: {os.path.basename(input_path)}")
                return False
        
        # 转换为 RGBA（如果需要）
        if img.mode != 'RGBA':
            img = img.convert('RGBA')
        
        # 获取图片数据
        data = np.array(img)
        
        # 检测背景色（通常是灰白色棋盘格或接近白色）
        # 假设背景是浅色（RGB 值都 > 200）
        r, g, b, a = data[:,:,0], data[:,:,1], data[:,:,2], data[:,:,3]
        
        # 识别可能的背景像素：
        # 1. 浅灰色到白色 (R,G,B 都 > 200)
        # 2. 或者是棋盘格的灰色 (R,G,B 相近且 > 180)
        light_pixels = (r > 200) & (g > 200) & (b > 200)
        gray_pixels = (r > 180) & (g > 180) & (b > 180) & (np.abs(r - g) < 10) & (np.abs(g - b) < 10)
        
        background_mask = light_pixels | gray_pixels
        
        # 将背景替换为绿幕 (0, 255, 0)
        data[background_mask] = [0, 255, 0, 255]
        
        # 创建新图片
        result_img = Image.fromarray(data, 'RGBA')
        
        # 保存
        result_img.save(output_path, 'PNG')
        
        # 统计转换的像素数
        converted_pixels = np.sum(background_mask)
        total_pixels = data.shape[0] * data.shape[1]
        percentage = (converted_pixels / total_pixels) * 100
        
        print(f"  ✓ 转换完成: {os.path.basename(input_path)} ({percentage:.1f}% 背景)")
        return True
        
    except Exception as e:
        print(f"  ✗ 转换失败: {os.path.basename(input_path)} - {str(e)}")
        return False

def batch_convert(file_list_path):
    """批量转换文件列表中的图片"""
    with open(file_list_path, 'r') as f:
        files = [line.strip() for line in f if line.strip()]
    
    print(f"准备转换 {len(files)} 个文件...")
    print("=" * 80)
    
    success_count = 0
    skip_count = 0
    fail_count = 0
    
    for filepath in files:
        if not os.path.exists(filepath):
            print(f"  ✗ 文件不存在: {filepath}")
            fail_count += 1
            continue
        
        result = convert_fake_transparent_to_greenscreen(filepath)
        if result:
            success_count += 1
        elif result is False:
            skip_count += 1
        else:
            fail_count += 1
    
    print("=" * 80)
    print(f"\n📊 转换统计:")
    print(f"  ✓ 成功: {success_count} 个")
    print(f"  ⊘ 跳过: {skip_count} 个")
    print(f"  ✗ 失败: {fail_count} 个")
    print(f"  总计: {len(files)} 个")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: convert_to_greenscreen.py <file_list.txt>")
        sys.exit(1)
    
    batch_convert(sys.argv[1])
