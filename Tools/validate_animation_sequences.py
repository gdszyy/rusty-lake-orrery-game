#!/usr/bin/env python3
"""
动画序列验证工具 (Animation Sequence Validator)

用途：自动检查美术资源中动画序列的完整性和连续性
作者：Manus AI
版本：1.0.0
日期：2025-12-08
"""

import re
import sys
from pathlib import Path
from collections import defaultdict
from typing import Dict, List, Tuple


class AnimationSequenceValidator:
    """动画序列验证器"""
    
    def __init__(self, art_root: Path):
        """
        初始化验证器
        
        Args:
            art_root: 美术资源根目录路径
        """
        self.art_root = Path(art_root)
        self.animation_pattern = re.compile(r'(.+?)_(\d{2}|\d{1})(?:_(.+?))?\.png$')
        self.animation_groups: Dict[str, List[Tuple[int, str]]] = defaultdict(list)
        self.issues: List[Dict] = []
        
    def scan_animations(self) -> None:
        """扫描所有PNG文件，识别动画序列"""
        for png_file in self.art_root.rglob("*.png"):
            match = self.animation_pattern.search(png_file.name)
            if match:
                base_name = match.group(1)
                frame_num = match.group(2)
                suffix = match.group(3) or ""
                
                # 构建唯一键：父目录 + 基础名称
                key = f"{png_file.parent.relative_to(self.art_root)}/{base_name}"
                self.animation_groups[key].append((int(frame_num), png_file.name, suffix))
    
    def validate_sequences(self) -> None:
        """验证动画序列的完整性和连续性"""
        # 过滤出真正的动画序列（至少3帧）
        animation_sequences = {
            k: v for k, v in self.animation_groups.items() 
            if len(v) >= 3
        }
        
        for key, frames in animation_sequences.items():
            frames.sort()
            frame_numbers = [f[0] for f in frames]
            frame_count = len(frames)
            
            # 检查序列连续性
            expected = list(range(1, frame_count + 1))
            
            # 检查是否有重复帧号
            if len(frame_numbers) != len(set(frame_numbers)):
                # 统计重复帧
                from collections import Counter
                duplicates = [num for num, count in Counter(frame_numbers).items() if count > 1]
                
                # 检查是否为合理的多变体设计
                is_valid_variant = self._check_variant_design(frames, duplicates)
                
                if is_valid_variant:
                    self.issues.append({
                        "type": "info",
                        "sequence": key,
                        "message": f"检测到多变体设计（帧 {duplicates} 存在多个版本），这是合理的设计模式",
                        "frames": frame_numbers,
                        "details": [f[1] for f in frames]
                    })
                else:
                    self.issues.append({
                        "type": "warning",
                        "sequence": key,
                        "message": f"序列存在重复帧号：{duplicates}",
                        "frames": frame_numbers,
                        "expected": expected,
                        "details": [f[1] for f in frames]
                    })
            
            # 检查序列是否连续
            elif frame_numbers != expected:
                self.issues.append({
                    "type": "error",
                    "sequence": key,
                    "message": "序列不连续或起始帧号不为1",
                    "frames": frame_numbers,
                    "expected": expected,
                    "details": [f[1] for f in frames]
                })
            else:
                # 序列完整
                self.issues.append({
                    "type": "success",
                    "sequence": key,
                    "message": f"序列完整（共{frame_count}帧）",
                    "frames": frame_numbers,
                    "details": [f[1] for f in frames]
                })
    
    def _check_variant_design(self, frames: List[Tuple[int, str, str]], duplicates: List[int]) -> bool:
        """
        检查重复帧号是否为合理的多变体设计
        
        Args:
            frames: 帧列表
            duplicates: 重复的帧号列表
            
        Returns:
            是否为合理的多变体设计
        """
        # 检查重复帧是否都有不同的后缀
        for dup_num in duplicates:
            dup_frames = [f for f in frames if f[0] == dup_num]
            suffixes = [f[2] for f in dup_frames]
            
            # 如果所有重复帧都有非空且不同的后缀，则认为是合理的多变体设计
            if all(suffixes) and len(suffixes) == len(set(suffixes)):
                continue
            else:
                return False
        
        return True
    
    def generate_report(self) -> str:
        """
        生成验证报告
        
        Returns:
            格式化的报告文本
        """
        report_lines = []
        report_lines.append("=" * 80)
        report_lines.append("动画序列验证报告")
        report_lines.append("=" * 80)
        report_lines.append("")
        
        # 统计
        total_sequences = len([i for i in self.issues if i["type"] in ["success", "error", "warning"]])
        success_count = len([i for i in self.issues if i["type"] == "success"])
        error_count = len([i for i in self.issues if i["type"] == "error"])
        warning_count = len([i for i in self.issues if i["type"] == "warning"])
        info_count = len([i for i in self.issues if i["type"] == "info"])
        
        report_lines.append(f"【总体统计】")
        report_lines.append(f"  发现动画序列: {total_sequences} 个")
        report_lines.append(f"  ✅ 完整序列: {success_count} 个")
        report_lines.append(f"  ⚠️  警告: {warning_count} 个")
        report_lines.append(f"  ❌ 错误: {error_count} 个")
        report_lines.append(f"  ℹ️  信息: {info_count} 个")
        report_lines.append("")
        
        # 详细信息
        if self.issues:
            report_lines.append("【详细检查结果】")
            report_lines.append("")
            
            for issue in sorted(self.issues, key=lambda x: (x["type"] != "error", x["type"] != "warning", x["sequence"])):
                icon_map = {
                    "success": "✅",
                    "error": "❌",
                    "warning": "⚠️",
                    "info": "ℹ️"
                }
                icon = icon_map.get(issue["type"], "•")
                
                report_lines.append(f"{icon} {issue['sequence']}")
                report_lines.append(f"   {issue['message']}")
                report_lines.append(f"   帧序号: {issue['frames']}")
                
                if "expected" in issue:
                    report_lines.append(f"   预期序号: {issue['expected']}")
                
                if "details" in issue and len(issue["details"]) <= 10:
                    report_lines.append(f"   文件列表:")
                    for detail in issue["details"]:
                        report_lines.append(f"     - {detail}")
                
                report_lines.append("")
        else:
            report_lines.append("未发现动画序列（3帧以上）")
        
        report_lines.append("=" * 80)
        
        return "\n".join(report_lines)
    
    def get_exit_code(self) -> int:
        """
        获取退出码
        
        Returns:
            0: 无错误, 1: 有错误
        """
        error_count = len([i for i in self.issues if i["type"] == "error"])
        return 1 if error_count > 0 else 0


def main():
    """主函数"""
    # 检查参数
    if len(sys.argv) > 1:
        art_root = Path(sys.argv[1])
    else:
        # 默认路径
        art_root = Path(__file__).parent.parent / "Content" / "Art"
    
    if not art_root.exists():
        print(f"错误: 美术资源目录不存在: {art_root}", file=sys.stderr)
        sys.exit(1)
    
    print(f"正在扫描美术资源目录: {art_root}")
    print()
    
    # 创建验证器并执行验证
    validator = AnimationSequenceValidator(art_root)
    validator.scan_animations()
    validator.validate_sequences()
    
    # 生成并打印报告
    report = validator.generate_report()
    print(report)
    
    # 保存报告到文件
    report_file = Path(__file__).parent.parent / "Docs" / "REPORTS" / "animation_validation_report.txt"
    report_file.parent.mkdir(parents=True, exist_ok=True)
    report_file.write_text(report, encoding="utf-8")
    print(f"\n报告已保存至: {report_file}")
    
    # 返回退出码
    sys.exit(validator.get_exit_code())


if __name__ == "__main__":
    main()
