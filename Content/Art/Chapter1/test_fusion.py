#!/usr/bin/env python3
"""
视觉融合测试脚本
将生成的道具叠加到场景底图上，验证分层生成工作流程的效果
"""

from PIL import Image
import os

# 定义路径
base_dir = "/home/ubuntu/rusty-lake-orrery-game/Content/Art/Chapter1"
scene_path = os.path.join(base_dir, "Scenes/BG_Chapter1_Museum_Base.png")
output_path = os.path.join(base_dir, "Scenes/BG_Chapter1_Museum_Composed.png")

# 加载场景底图
scene = Image.open(scene_path)
scene_width, scene_height = scene.size

print(f"场景尺寸: {scene_width}x{scene_height}")

# 定义道具放置位置（根据场景中的虚线框位置估算）
props_config = [
    {
        "name": "Prop_Ch1_CartierDisplay.png",
        "position": (int(scene_width * 0.5), int(scene_height * 0.4)),  # 中央展柜
        "scale": 0.35
    },
    {
        "name": "Prop_Ch1_Sarcophagus_Closed.png",
        "position": (int(scene_width * 0.2), int(scene_height * 0.45)),  # 左侧石棺
        "scale": 0.4
    },
    {
        "name": "Prop_Ch1_MusicBox_Closed.png",
        "position": (int(scene_width * 0.8), int(scene_height * 0.45)),  # 右侧音乐盒
        "scale": 0.3
    }
]

# 叠加道具
for prop_config in props_config:
    prop_path = os.path.join(base_dir, "Props", prop_config["name"])
    
    if not os.path.exists(prop_path):
        print(f"警告: 道具文件不存在 - {prop_config['name']}")
        continue
    
    # 加载道具
    prop = Image.open(prop_path)
    
    # 缩放道具
    scale = prop_config["scale"]
    new_size = (int(prop.width * scale), int(prop.height * scale))
    prop_resized = prop.resize(new_size, Image.Resampling.LANCZOS)
    
    # 计算粘贴位置（居中对齐）
    x = prop_config["position"][0] - new_size[0] // 2
    y = prop_config["position"][1] - new_size[1] // 2
    
    # 叠加到场景上（使用alpha通道作为蒙版）
    if prop_resized.mode == 'RGBA':
        scene.paste(prop_resized, (x, y), prop_resized)
    else:
        scene.paste(prop_resized, (x, y))
    
    print(f"已叠加: {prop_config['name']} at ({x}, {y})")

# 保存合成结果
scene.save(output_path)
print(f"\n合成场景已保存: {output_path}")
