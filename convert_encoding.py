#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
将core目录下的GB2312/GBK编码文件转换为UTF-8无BOM格式
"""

import os
import sys

# 需要转换的文件列表（相对于项目根目录）
files_to_convert = [
    'core/src/WmCanvas.cpp',
    'core/src/commandbuffer/WmCommandBuffer.cpp',
    'core/src/commandbuffer/WmCommandBuffer.hpp',
    'core/src/platform/Android/egl/WmEGLContext.h',
    'core/src/platform/Android/egl/WmEGLPbufferContext.cpp',
    'core/src/platform/Android/egl/WmEGLWindowContext.cpp',
    'core/src/platform/Android/WmCanvas2DContextAndroid.cpp',
    'core/src/platform/Android/WmCanvas2DContextImpl.cpp',
    'core/src/platform/Android/WmFont.cpp',
    'core/src/platform/Android/WmFontFamily.cpp',
    'core/src/platform/Android/WmFontFamily.h',
    'core/src/platform/Android/WmFontManagerAndroid.cpp',
    'core/src/platform/Android/WmFrameBufferObjectImpl.cpp',
    'core/src/platform/Android/WmFreeTypeWrap.cpp',
    'core/src/platform/Linux/WmFontLinux.cpp',
    'core/src/platform/Linux/WmFontManagerLinux.cpp',
    'core/src/platform/Linux/WmFontManagerLinux.hpp',
    'core/src/platform/Linux/WmSystemFontInformation.cpp',
    'core/src/webgl/WmCommandDecoderWebGL.cpp',
    'core/src/webgl/WmCommandDecoderWebGL.hpp',
    'core/src/wmcanvas/GL/WmFrameBufferObject.cpp',
    'core/src/wmcanvas/GL/WmTexture.cpp',
    'core/src/wmcanvas/WmPoint.h',
]

def convert_file(filepath):
    """将单个文件从GB2312/UTF-16转换为UTF-8"""
    try:
        # 读取原始文件
        with open(filepath, 'rb') as f:
            raw_bytes = f.read()
        
        # 检查BOM并确定编码
        encoding = None
        start_pos = 0
        
        if len(raw_bytes) >= 3 and raw_bytes[0:3] == b'\xef\xbb\xbf':
            # UTF-8 BOM - 移除BOM并保持UTF-8
            encoding = 'utf-8'
            start_pos = 3
            print(f"⚠ {filepath}: 移除UTF-8 BOM")
        elif len(raw_bytes) >= 2 and raw_bytes[0:2] == b'\xff\xfe':
            # UTF-16 LE BOM
            encoding = 'utf-16-le'
            start_pos = 2
            print(f"⚠ {filepath}: 检测到UTF-16 LE编码")
        elif len(raw_bytes) >= 2 and raw_bytes[0:2] == b'\xfe\xff':
            # UTF-16 BE BOM
            encoding = 'utf-16-be'
            start_pos = 2
            print(f"⚠ {filepath}: 检测到UTF-16 BE编码")
        
        if start_pos > 0:
            raw_bytes = raw_bytes[start_pos:]
        
        # 尝试解码
        content = None
        if encoding:
            try:
                content = raw_bytes.decode(encoding)
            except UnicodeDecodeError:
                print(f"❌ {filepath}: 无法以{encoding}解码")
                return False
        else:
            # 没有BOM，尝试GB2312/GBK
            try:
                content = raw_bytes.decode('gb2312')
            except UnicodeDecodeError:
                try:
                    content = raw_bytes.decode('gbk')
                except UnicodeDecodeError:
                    try:
                        content = raw_bytes.decode('gbk', errors='replace')
                        if content.count('�') > len(content) * 0.01:
                            print(f"⚠ {filepath}: 可能已经是UTF-8或其他编码")
                            return False
                    except:
                        print(f"❌ {filepath}: 无法解码")
                        return False
        
        # 写入UTF-8格式（无BOM），保持原有换行符
        with open(filepath, 'wb') as f:
            f.write(content.encode('utf-8'))
        
        print(f"✓ {filepath}")
        return True
        
    except Exception as e:
        print(f"❌ {filepath}: {str(e)}")
        return False

def main():
    print("=" * 60)
    print("开始转换文件编码：GB2312/GBK -> UTF-8 (无BOM)")
    print("=" * 60)
    print()
    
    base_dir = os.path.dirname(os.path.abspath(__file__))
    
    success_count = 0
    fail_count = 0
    
    for rel_path in files_to_convert:
        filepath = os.path.join(base_dir, rel_path.replace('/', os.sep))
        
        if not os.path.exists(filepath):
            print(f"⚠ {rel_path}: 文件不存在")
            fail_count += 1
            continue
        
        if convert_file(filepath):
            success_count += 1
        else:
            fail_count += 1
    
    print()
    print("=" * 60)
    print(f"转换完成！成功: {success_count}, 失败: {fail_count}")
    print("=" * 60)
    
    return 0 if fail_count == 0 else 1

if __name__ == '__main__':
    sys.exit(main())
