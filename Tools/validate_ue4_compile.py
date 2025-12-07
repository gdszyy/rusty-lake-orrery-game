#!/usr/bin/env python3
"""
UE4 编译验证脚本
模拟 UE4 编译器和 UnrealHeaderTool 的检查
"""

import re
import sys
from pathlib import Path
from typing import Set, List, Dict, Tuple

class UE4CompileValidator:
    def __init__(self, header_path: Path, cpp_path: Path):
        self.header_path = header_path
        self.cpp_path = cpp_path
        self.errors = []
        self.warnings = []
        
    def validate_all(self) -> bool:
        """执行所有验证"""
        print(f"🔍 验证 UE4 组件编译")
        print(f"   头文件: {self.header_path.name}")
        print(f"   实现文件: {self.cpp_path.name}")
        print("=" * 70)
        
        # 1. 验证头文件基本语法
        print("\n📋 [1/5] 检查头文件基本语法...")
        if self.validate_header_syntax():
            print("   ✅ 通过")
        else:
            print("   ❌ 失败")
        
        # 2. 验证委托声明顺序
        print("\n📋 [2/5] 检查委托声明顺序...")
        if self.validate_delegate_order():
            print("   ✅ 通过")
        else:
            print("   ❌ 失败")
        
        # 3. 验证成员声明完整性
        print("\n📋 [3/5] 检查成员声明完整性...")
        if self.validate_member_declarations():
            print("   ✅ 通过")
        else:
            print("   ❌ 失败")
        
        # 4. 验证 Broadcast 调用
        print("\n📋 [4/5] 检查委托 Broadcast 调用...")
        if self.validate_broadcast_calls():
            print("   ✅ 通过")
        else:
            print("   ❌ 失败")
        
        # 5. 验证包含文件
        print("\n📋 [5/5] 检查必要的包含文件...")
        if self.validate_includes():
            print("   ✅ 通过")
        else:
            print("   ❌ 失败")
        
        # 输出结果
        print("\n" + "=" * 70)
        if self.errors:
            print(f"❌ 发现 {len(self.errors)} 个错误:\n")
            for i, error in enumerate(self.errors, 1):
                print(f"  {i}. {error}")
        
        if self.warnings:
            print(f"\n⚠️  发现 {len(self.warnings)} 个警告:\n")
            for i, warning in enumerate(self.warnings, 1):
                print(f"  {i}. {warning}")
        
        if not self.errors and not self.warnings:
            print("✅ 所有检查通过! 代码应该可以成功编译。")
            return True
        elif not self.errors:
            print("\n✅ 没有错误,但有警告。代码应该可以编译。")
            return True
        else:
            print("\n❌ 发现错误,代码无法编译。请修复后重试。")
            return False
    
    def validate_header_syntax(self) -> bool:
        """验证头文件基本语法"""
        content = self.header_path.read_text(encoding='utf-8')
        passed = True
        
        if '#pragma once' not in content:
            self.errors.append("头文件缺少 '#pragma once'")
            passed = False
        
        if '.generated.h' not in content:
            self.errors.append("头文件缺少 '.generated.h' 包含")
            passed = False
        
        if 'UCLASS' in content:
            if 'GENERATED_BODY()' not in content and 'GENERATED_UCLASS_BODY()' not in content:
                self.errors.append("UCLASS 缺少 GENERATED_BODY() 或 GENERATED_UCLASS_BODY()")
                passed = False
        
        return passed
    
    def validate_delegate_order(self) -> bool:
        """验证委托声明顺序"""
        content = self.header_path.read_text(encoding='utf-8')
        
        # 提取委托声明
        declarations = {}
        pattern = r'DECLARE_DYNAMIC(?:_MULTICAST)?_DELEGATE(?:_\w+)?\s*\(\s*(\w+)'
        for i, line in enumerate(content.split('\n'), 1):
            match = re.search(pattern, line)
            if match:
                delegate_name = match.group(1)
                declarations[delegate_name] = i
        
        # 提取委托使用
        usages = {}
        lines = content.split('\n')
        for i, line in enumerate(lines, 1):
            if 'DECLARE_DYNAMIC' in line:
                continue
            if 'UPROPERTY' in line:
                for j in range(i, min(i + 5, len(lines) + 1)):
                    next_line = lines[j - 1]
                    match = re.search(r'\b(F\w+Delegate)\s+\w+\s*;', next_line)
                    if match:
                        delegate_name = match.group(1)
                        if delegate_name not in usages:
                            usages[delegate_name] = []
                        usages[delegate_name].append(j)
                        break
        
        passed = True
        for delegate_name, usage_lines in usages.items():
            if delegate_name not in declarations:
                self.errors.append(
                    f"委托 '{delegate_name}' 在第 {usage_lines[0]} 行使用但未声明"
                )
                passed = False
            else:
                declaration_line = declarations[delegate_name]
                for usage_line in usage_lines:
                    if usage_line < declaration_line:
                        self.errors.append(
                            f"委托 '{delegate_name}' 在第 {usage_line} 行使用，"
                            f"但在第 {declaration_line} 行才声明 (声明必须在使用之前)"
                        )
                        passed = False
        
        return passed
    
    def validate_member_declarations(self) -> bool:
        """验证成员声明完整性"""
        # 从头文件提取所有成员
        header_content = self.header_path.read_text(encoding='utf-8')
        declared_members = set()
        
        # 提取 UPROPERTY 成员
        for match in re.finditer(r'UPROPERTY[^;]+;\s*\w+[\w<>:,\s*&]*\s+(\w+)\s*[;=]', header_content):
            declared_members.add(match.group(1))
        
        # 提取委托成员
        for match in re.finditer(r'(F\w+Delegate)\s+(\w+)\s*;', header_content):
            declared_members.add(match.group(2))
        
        # 从 cpp 提取 Broadcast 调用
        cpp_content = self.cpp_path.read_text(encoding='utf-8')
        broadcast_calls = []
        
        for i, line in enumerate(cpp_content.split('\n'), 1):
            if '//' in line:
                line = line[:line.index('//')]
            match = re.search(r'(\w+)\.Broadcast\(', line)
            if match:
                member_name = match.group(1)
                broadcast_calls.append((member_name, i))
        
        passed = True
        for member_name, line_num in broadcast_calls:
            if member_name not in declared_members:
                self.errors.append(
                    f"'{member_name}' 在 {self.cpp_path.name}:{line_num} 行调用 Broadcast，"
                    f"但未在头文件中声明"
                )
                passed = False
        
        return passed
    
    def validate_broadcast_calls(self) -> bool:
        """验证 Broadcast 调用的参数"""
        header_content = self.header_path.read_text(encoding='utf-8')
        cpp_content = self.cpp_path.read_text(encoding='utf-8')
        
        # 提取委托声明及其参数
        delegate_params = {}
        pattern = r'DECLARE_DYNAMIC(?:_MULTICAST)?_DELEGATE(?:_(\w+))?\s*\(\s*(\w+)(?:,\s*(\w+),\s*(\w+))?\s*\)'
        
        for match in re.finditer(pattern, header_content):
            param_type = match.group(1)  # OneParam, TwoParams, etc.
            delegate_name = match.group(2)
            
            if param_type is None:
                delegate_params[delegate_name] = 0
            elif 'OneParam' in param_type:
                delegate_params[delegate_name] = 1
            elif 'TwoParams' in param_type:
                delegate_params[delegate_name] = 2
            else:
                delegate_params[delegate_name] = 0
        
        # 查找委托成员名称
        delegate_members = {}
        for match in re.finditer(r'(F\w+Delegate)\s+(\w+)\s*;', header_content):
            delegate_type = match.group(1)
            member_name = match.group(2)
            if delegate_type in delegate_params:
                delegate_members[member_name] = delegate_params[delegate_type]
        
        # 验证 Broadcast 调用
        passed = True
        for i, line in enumerate(cpp_content.split('\n'), 1):
            if '//' in line:
                line = line[:line.index('//')]
            match = re.search(r'(\w+)\.Broadcast\(([^)]*)\)', line)
            if match:
                member_name = match.group(1)
                args = match.group(2)
                
                if member_name in delegate_members:
                    expected_params = delegate_members[member_name]
                    actual_params = len([a.strip() for a in args.split(',') if a.strip()])
                    
                    if actual_params != expected_params:
                        self.errors.append(
                            f"委托 '{member_name}' 在 {self.cpp_path.name}:{i} 行的 Broadcast 调用参数数量不匹配 "
                            f"(期望 {expected_params} 个参数，实际 {actual_params} 个)"
                        )
                        passed = False
        
        return passed
    
    def validate_includes(self) -> bool:
        """验证必要的包含文件"""
        header_content = self.header_path.read_text(encoding='utf-8')
        cpp_content = self.cpp_path.read_text(encoding='utf-8')
        
        passed = True
        
        # 检查头文件包含
        if 'UCLASS' in header_content and 'CoreMinimal.h' not in header_content:
            self.warnings.append("头文件建议包含 'CoreMinimal.h'")
        
        # 检查 cpp 包含头文件
        header_name = self.header_path.name
        if f'#include "{header_name}"' not in cpp_content:
            self.errors.append(f"实现文件未包含对应的头文件 '{header_name}'")
            passed = False
        
        return passed

def main():
    if len(sys.argv) < 3:
        print("用法: python3 validate_ue4_compile.py <header_file> <cpp_file>")
        sys.exit(1)
    
    header_path = Path(sys.argv[1])
    cpp_path = Path(sys.argv[2])
    
    if not header_path.exists():
        print(f"❌ 头文件不存在: {header_path}")
        sys.exit(1)
    
    if not cpp_path.exists():
        print(f"❌ 实现文件不存在: {cpp_path}")
        sys.exit(1)
    
    validator = UE4CompileValidator(header_path, cpp_path)
    success = validator.validate_all()
    
    sys.exit(0 if success else 1)

if __name__ == '__main__':
    main()
