#!/usr/bin/env python3
"""
验证 Unreal Engine 4 头文件中的委托声明顺序
检查所有 DECLARE_DYNAMIC_*_DELEGATE 宏是否在使用之前声明
"""

import re
import sys
from pathlib import Path
from typing import List, Tuple, Dict

def extract_delegate_declarations(content: str) -> Dict[str, int]:
    """提取所有委托声明及其行号"""
    declarations = {}
    pattern = r'DECLARE_DYNAMIC(?:_MULTICAST)?_DELEGATE(?:_\w+)?\s*\(\s*(\w+)'
    
    for i, line in enumerate(content.split('\n'), 1):
        match = re.search(pattern, line)
        if match:
            delegate_name = match.group(1)
            declarations[delegate_name] = i
    
    return declarations

def extract_delegate_usages(content: str) -> Dict[str, List[int]]:
    """提取所有委托使用及其行号"""
    usages = {}
    lines = content.split('\n')
    
    for i, line in enumerate(lines, 1):
        # 跳过声明行本身
        if 'DECLARE_DYNAMIC' in line:
            continue
        
        # 查找 UPROPERTY 后面的委托类型使用
        if 'UPROPERTY' in line:
            # 查找接下来几行中的委托类型
            for j in range(i, min(i + 5, len(lines) + 1)):
                next_line = lines[j - 1]
                # 匹配类似 "FDelegateName DelegateVariable;" 的模式
                match = re.search(r'\b(F\w+Delegate)\s+\w+\s*;', next_line)
                if match:
                    delegate_name = match.group(1)
                    if delegate_name not in usages:
                        usages[delegate_name] = []
                    usages[delegate_name].append(j)
                    break
    
    return usages

def validate_delegate_order(file_path: Path) -> Tuple[bool, List[str]]:
    """验证委托声明顺序"""
    content = file_path.read_text(encoding='utf-8')
    
    declarations = extract_delegate_declarations(content)
    usages = extract_delegate_usages(content)
    
    errors = []
    
    for delegate_name, usage_lines in usages.items():
        if delegate_name not in declarations:
            errors.append(f"❌ 委托 '{delegate_name}' 被使用但未声明")
            continue
        
        declaration_line = declarations[delegate_name]
        
        for usage_line in usage_lines:
            if usage_line < declaration_line:
                errors.append(
                    f"❌ 委托 '{delegate_name}' 在第 {usage_line} 行使用，"
                    f"但在第 {declaration_line} 行才声明"
                )
    
    return len(errors) == 0, errors

def check_basic_syntax(file_path: Path) -> Tuple[bool, List[str]]:
    """检查基本的 UE4 头文件语法"""
    content = file_path.read_text(encoding='utf-8')
    errors = []
    
    # 检查是否有 #pragma once
    if '#pragma once' not in content:
        errors.append("❌ 缺少 '#pragma once'")
    
    # 检查是否包含 .generated.h
    if '.generated.h' not in content:
        errors.append("❌ 缺少 '.generated.h' 包含")
    
    # 检查 UCLASS 是否有 GENERATED_BODY
    if 'UCLASS' in content:
        if 'GENERATED_BODY()' not in content and 'GENERATED_UCLASS_BODY()' not in content:
            errors.append("❌ UCLASS 缺少 GENERATED_BODY() 或 GENERATED_UCLASS_BODY()")
    
    # 检查委托声明是否在类体内
    lines = content.split('\n')
    in_class = False
    class_depth = 0
    
    for i, line in enumerate(lines, 1):
        if 'UCLASS' in line:
            in_class = True
        
        if in_class:
            if '{' in line:
                class_depth += line.count('{')
            if '}' in line:
                class_depth -= line.count('}')
                if class_depth == 0:
                    in_class = False
        
        if 'DECLARE_DYNAMIC' in line and not in_class:
            errors.append(f"❌ 第 {i} 行: 委托声明必须在 UCLASS 类体内")
    
    return len(errors) == 0, errors

def main():
    if len(sys.argv) < 2:
        print("用法: python3 validate_ue4_header.py <header_file>")
        sys.exit(1)
    
    file_path = Path(sys.argv[1])
    
    if not file_path.exists():
        print(f"❌ 文件不存在: {file_path}")
        sys.exit(1)
    
    print(f"🔍 验证 UE4 头文件: {file_path.name}")
    print("=" * 60)
    
    # 检查基本语法
    print("\n📋 检查基本语法...")
    syntax_ok, syntax_errors = check_basic_syntax(file_path)
    
    if syntax_ok:
        print("✅ 基本语法检查通过")
    else:
        print("❌ 基本语法检查失败:")
        for error in syntax_errors:
            print(f"  {error}")
    
    # 检查委托声明顺序
    print("\n📋 检查委托声明顺序...")
    order_ok, order_errors = validate_delegate_order(file_path)
    
    if order_ok:
        print("✅ 委托声明顺序正确")
    else:
        print("❌ 委托声明顺序错误:")
        for error in order_errors:
            print(f"  {error}")
    
    # 总结
    print("\n" + "=" * 60)
    if syntax_ok and order_ok:
        print("✅ 所有检查通过!")
        sys.exit(0)
    else:
        print("❌ 发现错误，请修复后重新验证")
        sys.exit(1)

if __name__ == '__main__':
    main()
