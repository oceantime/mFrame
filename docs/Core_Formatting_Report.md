# Core目录C++文件格式化处理报告

## 处理日期
2026年1月7日

## 处理范围
core目录下所有C++源文件和头文件（276个文件）

## 已完成的工作

### 1. 创建格式化配置
- ✅ 创建了`.clang-format`配置文件
- ✅ 配置基于Google C++风格，适配项目需求
- ✅ 设置缩进为4空格，列宽120字符
- ✅ 统一指针和引用左对齐风格

### 2. 核心文件手动处理（已完成）

#### WmCanvas模块
- ✅ `core/src/WmCanvas.cpp` - 格式化并添加详细中文注释
- ✅ `core/src/WmCanvas.hpp` - 格式化并添加详细中文注释
- 主要改进：
  * 将所有英文注释翻译为中文
  * 添加详细的函数和参数说明
  * 修正代码格式，统一缩进和对齐
  * 将NULL替换为nullptr
  * 优化include头文件顺序

#### WmCanvasManager模块
- ✅ `core/src/WmCanvasManager.cpp` - 格式化并添加详细中文注释
- ✅ `core/src/WmCanvasManager.h` - 格式化并添加详细中文注释
- 主要改进：
  * 完善单例模式实现的注释
  * 添加所有公共方法的详细说明
  * 优化指针类型格式（从`Type *var`改为`Type* var`）
  * 将NULL替换为nullptr
  * 添加Android平台特定功能的注释

#### CommandBuffer模块
- ✅ `core/src/commandbuffer/WmCommandBuffer.hpp` - 格式化并添加详细中文注释
- ✅ `core/src/commandbuffer/WmCommandBuffer.cpp` - 格式化并添加详细中文注释
- 主要改进：
  * 详细说明命令缓冲区编码格式
  * 添加所有模板函数的说明
  * 优化代码格式和缩进
  * 添加参数和返回值的详细注释
  * 修正循环变量类型（int改为size_t）

### 3. 自动化工具
- ✅ 创建了`format-core-files.ps1` PowerShell脚本
- 功能：
  * 自动扫描core目录下所有C++文件
  * 支持clang-format自动格式化
  * 基础格式化（NULL→nullptr，指针对齐）
  * 跳过第三方库文件（freetype、lodepng等）
  * 生成详细的处理报告

## 格式化规范

### 代码风格
1. **缩进**：4空格，禁止使用Tab
2. **列宽**：最大120字符
3. **括号风格**：K&R风格（花括号不换行）
4. **指针/引用**：左对齐（`Type* var`而不是`Type *var`）
5. **空格**：
   - 控制语句后加空格：`if (condition)`
   - 函数参数括号内不加空格：`func(arg)`

### 注释规范
1. **文件头注释**：说明文件用途
2. **类注释**：使用Doxygen格式`@brief`
3. **函数注释**：
   - `@brief`：简要说明
   - `@param`：参数说明
   - `@return`：返回值说明
4. **行内注释**：使用中文，简洁明了
5. **注释语言**：统一使用中文

### 代码规范
1. **NULL指针**：统一使用`nullptr`（C++11）
2. **类型安全**：使用size_t而不是int表示大小和索引
3. **命名空间**：统一使用`wmcanvas`命名空间
4. **头文件顺序**：
   - 标准库头文件
   - 第三方库头文件  
   - 项目内头文件

## 下一步工作

### 批量处理（需执行）
```powershell
# 运行格式化脚本
.\format-core-files.ps1
```

### 验证编译
```powershell
# 清理并重新编译
.\gradlew clean
.\gradlew :android:wmcanvas_library:assembleStandaloneDebug
```

### 需要特别关注的文件类别

#### 1. Platform相关文件（约60个文件）
- `core/src/platform/Android/*` - Android平台实现
- `core/src/platform/Linux/*` - Linux平台实现  
- `core/src/platform/iOS/*` - iOS平台实现

#### 2. WmCanvas 2D上下文（约30个文件）
- `core/src/wmcanvas/*` - 2D绘图上下文实现

#### 3. WebGL相关（约15个文件）
- `core/src/webgl/*` - WebGL渲染上下文

#### 4. JNI绑定（约10个文件）
- `core/android/*.cpp` - JNI接口实现

#### 5. 支持库（约10个文件）
- `core/src/support/*` - 工具函数和辅助类

## 已知问题和注意事项

### 跳过的文件
以下文件不需要处理：
1. 第三方库：
   - `core/android/freetype-prebuilt/**/*`
   - `core/test/linux/freetype2/**/*`
   - `core/android/png/thirdparty/lodepng.*`

2. 测试文件中的第三方代码：
   - `core/test/linux/util/lodepng.*`

### 特殊处理
1. **JNI函数签名**：保持Java规范的命名风格
2. **宏定义**：保持全大写命名
3. **外部C接口**：保持`extern "C"`块的格式
4. **模板实例化**：显式实例化语句保持独立一行

## 验证清单
- [ ] 运行格式化脚本完成
- [ ] 检查Git diff确认变更合理
- [ ] 编译通过（Debug和Release）
- [ ] 单元测试通过（如有）
- [ ] Demo应用运行正常
- [ ] 代码审查

## 相关文档
- [C++编码规范](https://google.github.io/styleguide/cppguide.html)
- [Doxygen注释指南](https://www.doxygen.nl/manual/docblocks.html)
- [Clang-Format配置](https://clang.llvm.org/docs/ClangFormatStyleOptions.html)

## 总结
已完成核心模块的格式化和注释翻译工作，建立了统一的代码规范和自动化处理工具。
剩余文件可通过批量脚本处理，预计可覆盖90%以上的文件。
特殊文件需要人工review确保格式化不影响功能。
