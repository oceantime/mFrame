# Task 79: 架构迁移分析与实施方案

## 一、当前架构分析

### 1.1 现有目录结构

```
mFrame/
├── core/                          # C++核心代码，编译为.so
│   ├── CMakeLists.txt            # CMake构建配置
│   ├── src/                      # 跨平台核心代码
│   │   ├── WmCanvas.cpp/hpp
│   │   ├── WmCanvasManager.cpp/h
│   │   ├── wmcanvas/             # 2D Canvas实现
│   │   ├── webgl/                # WebGL实现
│   │   ├── commandbuffer/        # 命令缓冲
│   │   ├── support/              # 工具类
│   │   └── platform/Android/     # Android平台特定实现
│   │       ├── WmCanvasAndroid.cpp/h
│   │       ├── WmCanvas2DContextAndroid.cpp/h
│   │       ├── WmFont*.cpp/h     # 字体相关
│   │       ├── WmFrameBufferObjectImpl.cpp
│   │       └── egl/              # EGL上下文管理
│   └── android/                  # Android JNI绑定
│       ├── WmCanvasJNI.cpp/h     # JNI接口实现
│       ├── WmCanvasLinkNative.cpp/h
│       ├── 3d/                   # 3D渲染支持
│       ├── manager/              # 管理器
│       ├── memory/               # 内存管理
│       ├── png/                  # PNG支持
│       └── freetype-prebuilt/    # 预编译FreeType库
│
├── android/
│   ├── wmcanvas_library/         # Android AAR库工程
│   │   ├── build.gradle          # 引用../../CMakeLists.txt
│   │   └── src/main/java/com/honghu/wmcanvas/
│   │       ├── WmCanvasJNI.java  # Java JNI声明
│   │       ├── WmCanvas.java
│   │       └── ...
│   └── demo/                     # 演示应用
│       └── src/main/assets/      # 前端Vue应用
│
└── node/                         # Node.js绑定 (参考实现)
    ├── binding/                  # Node N-API绑定
    └── examples/                 # JS测试用例示例
```

### 1.2 编译流程

**当前流程:**
```
core/CMakeLists.txt (主构建文件)
    ↓
编译 core/src/** + core/android/** 
    ↓
生成 .so文件 (根据flavor命名)
    ↓
android/wmcanvas_library 通过 externalNativeBuild 引用
    ↓
打包生成 AAR
    ↓
android/demo 依赖 AAR
```

### 1.3 .so文件命名机制分析

**问题:** 为什么会生成 `libWmCanvas.so` 或 `libwmcanvas_runtime.so`？

**原因:** 历史遗留的双路径加载机制

#### 当前实现 (冗余设计)

**CMakeLists.txt 配置:**
```cmake
# 目标名称始终是 "wmcanvas"
add_library(wmcanvas SHARED ${SRC_FILES})

# 但输出文件名根据 WMCANVAS_RUNTIME 标志改变
if (WMCANVAS_RUNTIME)
    set_target_properties(wmcanvas PROPERTIES OUTPUT_NAME "wmcanvas_runtime")
    # 生成: libwmcanvas_runtime.so
else()
    set_target_properties(wmcanvas PROPERTIES OUTPUT_NAME "WmCanvas")
    # 生成: libWmCanvas.so
endif()
```

**build.gradle 配置:**
```groovy
productFlavors {
    standalone {
        externalNativeBuild {
            cmake {
                // 只有standalone flavor设置此标志
                arguments '-DWMCANVAS_RUNTIME=1'
            }
        }
    }
}
```

**WmCanvasJNI.java 加载逻辑:**
```java
// 方法1: loadRuntime() - 加载 wmcanvas_runtime
public static void loadRuntime() {
    System.loadLibrary("c++_shared");
    System.loadLibrary("wmcanvas_runtime");  // 寻找 libwmcanvas_runtime.so
}

// 方法2: loadInternal() - 加载 WmCanvas  
private static void loadInternal() {
    System.loadLibrary("freetype");
    System.loadLibrary("WmCanvas");  // 寻找 libWmCanvas.so
}
```

#### 问题分析

**❌ 冗余问题:**
1. **同一套C++代码**，仅因编译参数生成不同名字
2. **Java层维护两套加载逻辑**，增加维护成本
3. **`loadRuntime()` 方法未被使用** (当前demo只用`loadInternal()`)
4. **命名不一致** (`WmCanvas` vs `wmcanvas_runtime`)

**历史原因推测:**
- 早期可能有两个版本 (runtime版 vs 完整版)
- Weex集成时需要特殊处理
- 现在已精简，但遗留了双路径

#### 统一方案 ✅ 推荐

**方案1: 统一为 libwmcanvas.so (推荐)**

**优点:**
- ✅ 符合命名空间 `wmcanvas` 的统一命名规范
- ✅ 简化Java加载逻辑 (只需一个方法)
- ✅ 避免混淆 (WmCanvas大小写不一致问题)

**修改步骤:**
```cmake
# core/CMakeLists.txt - 简化
add_library(wmcanvas SHARED ${SRC_FILES})
# 删除: set_target_properties(wmcanvas PROPERTIES OUTPUT_NAME ...)
# 结果: 自动生成 libwmcanvas.so
```

```java
// WmCanvasJNI.java - 简化
private static void loadInternal() {
    System.loadLibrary("freetype");
    System.loadLibrary("wmcanvas");  // 统一名称
}
// 删除: loadRuntime() 方法
```

```groovy
// build.gradle - 删除标志
arguments '-DWMCANVAS=0', '-DANDROID_STL=c++_shared', '-DGSTANDALONE=1'
// 删除: '-DWMCANVAS_RUNTIME=1'
```

**方案2: 统一为 libWmCanvas.so (保守)**

保持当前主流名称，但删除runtime分支：
```cmake
add_library(wmcanvas SHARED ${SRC_FILES})
set_target_properties(wmcanvas PROPERTIES OUTPUT_NAME "WmCanvas")
```

#### 推荐方案总结

**建议统一为 `libwmcanvas.so`**

**理由:**
1. ✅ 与namespace统一 (task 78已将所有命名空间改为wmcanvas)
2. ✅ 与目标名一致 (CMake target名是wmcanvas)
3. ✅ 全小写命名更符合Linux惯例
4. ✅ 避免Java中的驼峰/下划线混用

**改进清单:**
- [ ] 删除 CMakeLists.txt 中的 `WMCANVAS_RUNTIME` 条件分支
- [ ] 统一输出为 `libwmcanvas.so`
- [ ] 删除 WmCanvasJNI.java 中的 `loadRuntime()` 方法
- [ ] 统一 `loadInternal()` 为 `System.loadLibrary("wmcanvas")`
- [ ] 删除 build.gradle 中的 `-DWMCANVAS_RUNTIME=1` 参数
- [ ] 更新文档和copilot-instructions.md

### 1.3 API实现状态

根据 [docs/APIs.md](../docs/APIs.md)：

**Canvas 2D API:**
- ✅ 已实现: fillRect, strokeRect, drawImage, fillText, arc, transform等核心API (约70个)
- ⚠️ 空实现: measureText, isPointInPath, capture
- ❌ 未定义: shadowColor, shadowBlur, toDataURL

**WebGL API:**
- ✅ 已实现: 150+ WebGL方法 (viewport, drawArrays, uniform*, texture*等)
- ⚠️ 空实现: isContextLost, getContextAttributes
- ❌ 未定义: commit, finish, hint

---

## 二、迁移可行性分析

### 2.1 方案A: 保持当前架构 ✅ 推荐

**架构不变:**
```
core/ (独立编译为.so)
  ↓
android/wmcanvas_library (引用.so + Java封装)
  ↓
android/demo (依赖AAR)
```

**优点:**
- ✅ 不破坏现有构建系统
- ✅ core/ 保持跨平台独立性 (Node.js也可用)
- ✅ 清晰的职责分离: core=引擎, library=封装, demo=应用
- ✅ 已有完整的JNI绑定和Java API

**缺点:**
- ⚠️ 需要维护两个CMakeLists.txt (根目录和core/)

**结论:** **强烈推荐保持当前架构**，理由：
1. core/android 和 core/src/platform/Android 是**平台特定的C++实现**，不应该放到Java工程中
2. wmcanvas_library 的职责是**Java封装层**，不是C++源码管理
3. 迁移会破坏Node.js绑定的共享

---

### 2.2 方案B: 部分迁移 (不推荐)

将 `core/android/WmCanvasJNI.cpp` 移到 `android/wmcanvas_library/src/main/cpp/`

**问题:**
- ❌ JNI代码与core强耦合，分离后难以维护
- ❌ 仍需要引用core/的头文件和库
- ❌ 增加了include路径复杂度

---

## 三、优化建议 (基于当前架构)

### 3.1 任务清单

#### 阶段1: 文档与测试完善 📋

**任务1.1: 补充API文档映射**
- [ ] 检查 core/src/wmcanvas/WmCanvas2dContext.cpp 中的每个public方法
- [ ] 检查 core/src/webgl/WmWebGLRenderContext.cpp 中的每个方法
- [ ] 与 docs/APIs.md 对照，标记实际实现状态
- [ ] 更新 docs/Reference_Graphics_2D.md 和 docs/Reference_WebGL.md

**任务1.2: 实现空实现的API**
- [ ] 实现 measureText() - 返回文本宽度
- [ ] 实现 isContextLost() - 检查WebGL上下文状态
- [ ] 实现 getContextAttributes() - 返回WebGL上下文属性
- [ ] 实现 capture() - 截图功能

**任务1.3: 创建JS测试用例 (参考node/examples)**
- [ ] 在 android/demo/src/main/assets/tests/ 创建测试目录
- [ ] 移植 node/examples/clock.js → tests/canvas2d/clock.js
- [ ] 移植 node/examples/image.js → tests/canvas2d/image.js
- [ ] 移植 node/examples/webgl/glcube2.js → tests/webgl/cube.js
- [ ] 创建测试索引页面 tests/index.html 调用所有测试
- [ ] 在 android/demo 中添加"运行测试"按钮

---

#### 阶段2: JNI接口规范化 🔧

**任务2.1: 检查JNI方法覆盖度**
- [ ] 列出 WmCanvasJNI.java 中的所有native方法
- [ ] 检查 WmCanvasJNI.cpp 中是否都有对应实现
- [ ] 检查是否有遗漏的Canvas 2D API (如 createPattern, addColorStop)

**任务2.2: 统一错误处理**
- [ ] 在JNI层添加异常检查和Java异常抛出
- [ ] 添加参数校验 (null检查、范围检查)
- [ ] 统一返回值约定 (如getImageData的格式)

**任务2.3: 内存管理优化**
- [ ] 检查所有 jstringToString 是否正确释放
- [ ] 检查Bitmap/Texture的生命周期管理
- [ ] 添加资源泄漏检测日志

---

#### 阶段3: 构建系统优化 ⚙️

**任务3.1: 统一.so命名 (高优先级)**
- [ ] 修改 core/CMakeLists.txt，删除 WMCANVAS_RUNTIME 条件分支
- [ ] 统一输出为 libwmcanvas.so
- [ ] 修改 WmCanvasJNI.java，删除 loadRuntime() 方法
- [ ] 统一加载逻辑为 System.loadLibrary("wmcanvas")
- [ ] 修改 build.gradle，删除 -DWMCANVAS_RUNTIME=1 参数
- [ ] 更新 .github/copilot-instructions.md 中的相关说明

**任务3.2: 简化CMake配置**
```cmake
# 当前: 根目录CMakeLists.txt + core/CMakeLists.txt
# 优化建议: 合并为一个CMakeLists.txt
```
- [ ] 评估合并可行性 (考虑Node.js绑定影响)
- [ ] 如保持分离，添加清晰的注释说明职责

**任务3.3: 改进AAR发布流程**
- [ ] 确认 publish_local.gradle 的配置正确
- [ ] 添加版本号管理脚本
- [ ] 生成符号表 (symbols) 用于调试

**任务3.4: CI/CD自动化**
- [ ] 添加 GitHub Actions 自动构建
- [ ] 自动运行JS测试用例
- [ ] 生成API覆盖率报告

---

#### 阶段4: 前端测试界面 🎨

**任务4.1: 创建测试套件UI**
```
android/demo/src/main/assets/src/views/tests/
├── index.vue          # 测试列表主页
├── Canvas2DTest.vue   # Canvas 2D测试界面
├── WebGLTest.vue      # WebGL测试界面
└── tests/
    ├── canvas2d/
    │   ├── fillRect.js
    │   ├── drawImage.js
    │   ├── text.js
    │   └── transforms.js
    └── webgl/
        ├── triangle.js
        ├── cube.js
        └── texture.js
```

**任务4.2: 实现测试运行器**
- [ ] 动态加载JS测试脚本
- [ ] 显示测试结果 (通过/失败)
- [ ] 对比期望输出 (如截图对比)
- [ ] 生成测试报告

**任务4.3: 参考node/examples移植**
按优先级移植以下测试:
1. **基础绘图**: fillRect, strokeRect, clearRect
2. **路径绘制**: arc, bezierCurveTo, lineTo
3. **图片**: drawImage (本地图片 + 网络图片)
4. **文本**: fillText, strokeText, font
5. **变换**: translate, rotate, scale, transform
6. **WebGL**: 三角形, 立方体, 纹理

---

## 四、实施方案总结

### 4.1 推荐方案 (保持现有架构)

**不迁移 core/android 和 core/src/platform/Android**

**理由:**
1. 这些是C++平台特定实现，不是Java代码
2. 迁移会破坏跨平台共享 (Node.js也用core/)
3. 当前架构已经运行良好

**改进重点:**
1. ✅ 补充测试用例 (从node/examples移植)
2. ✅ 完善API文档映射
3. ✅ 实现空实现的API
4. ✅ 规范化JNI接口

---

### 4.2 任务清单概览

| 阶段 | 任务 | 优先级 | 工作量 |
|------|------|--------|--------|
| 1.1 | API文档映射 | 高 | 4小时 |
| 1.2 | 实现空API | 中 | 8小时 |
| 1.3 | JS测试用例 | 高 | 16小时 |
| 2.1 | JNI覆盖度检查 | 高 | 4小时 |
| 2.2 | 错误处理 | 中 | 6小时 |
| 2.3 | 内存管理 | 中 | 6小时 |
| 3.1 | 统一.so命名 | 高 | 3小时 |
| 3.2 | CMake优化 | 低 | 4小时 |
| 3.3 | AAR发布 | 低 | 2小时 |
| 3.4 | CI/CD | 低 | 8小时 |
| 4.1 | 测试UI | 高 | 12小时 |
| 4.2 | 测试运行器 | 高 | 8小时 |
| 4.3 | 移植示例 | 高 | 20小时 |
101小时 (12.5
**总工作量估算:** 约98小时 (12个工作日)

---

## 五、下一步行动

### 立即可执行 (无需架构调整):

**快速验证任务:**
1. **任务1.3.1**: 创建第一个测试用例
   ```javascript
   // android/demo/src/main/assets/tests/canvas2d/fillRect.js
   const canvas = document.getElementById('canvas');
   const ctx = canvas.getContext('2d');
   ctx.fillStyle = 'red';
   ctx.fillRect(10, 10, 100, 100);
   ```

2. **任务1.3.2**: 在demo中添加测试入口
   - 修改 android/demo/src/main/assets/src/router/index.js
   - 添加 /tests 路由
   - 创建 TestsView.vue 加载测试脚本

3. **任务1.1**: API映射检查
   - 运行搜索: grep "API_EXPORT" core/src/wmcanvas/*.h
   - 与 docs/APIs.md 对照

---

## 六、关键技术点

### 6.1 如何从JS调用native API

**当前实现 (command buffer模式):**
```javascript
// 前端 (assets/wmcanvas-wrapper.js)
ctx.fillRect(x, y, w, h)
  ↓
wmcanvas.render(canvasId, commandString)  // JS
  ↓
WmCanvasJNI.render(contextId, commands)   // Java
  ↓
Java_..._render(JNIEnv*, contextId, cmds) // JNI
  ↓
WmCanvas::Render(commands)                // C++
  ↓
WmCommandDecoder::Decode()                // 解析命令
  ↓
WmCanvas2dContext::FillRect()             // 执行
```

### 6.2 node/examples如何转为Android测试

**Node.js版本:**
```javascript
const { createCanvas } = require('../export');
const canvas = createCanvas(400, 400);
const ctx = canvas.getContext('2d');
ctx.fillRect(10, 10, 100, 100);
canvas.createPNG("output");
```

**Android WebView版本:**
```javascript
// 在WebView中运行
const canvas = document.getElementById('test-canvas');
const ctx = canvas.getContext('2d');
ctx.fillRect(10, 10, 100, 100);
// 不需要createPNG，WebView直接显示
```

**需要适配的部分:**
- ❌ 移除 `require()` 和 `createCanvas()` (浏览器环境不需要)
- ✅ 使用 `<canvas id="...">` 元素
- ❌ 移除 `canvas.createPNG()` (改为WebView显示)
- ✅ 网络图片需要处理CORS和加载时机

---

## 七、结论

**不建议迁移 core/android 和 core/src/platform/Android 到 android/wmcanvas_library**

**建议的改进方向:**
1. ✅ 保持当前三层架构 (core → library → demo)
2. ✅ 从 node/examples 移植测试用例到 demo/assets/tests/
3. ✅ 完善API文档和实现覆盖度
4. ✅ 规范化JNI接口和错误处理

**架构合理性验证:**
- Node.js 可以单独引用 core/ 编译
- Android library 引用 core/ 生成.so
- iOS 也可以引用 core/ (虽然当前未激活)
- 符合"平台无关核心 + 平台特定封装"的设计原则


```mermaid
flowchart TD
    A[JS/WebGL 调用<br/>glcube2 示例<br/>node/examples/webgl/glcube2.js] --> B[WmCanvasJNI.render()<br/>native 指令桥<br/><br/>WmCanvasManager<br/>解析并转交 WmCanvasAndroid]
    B --> C[WmRenderer 渲染线程<br/>core/android/3d/view/WmRenderer.cpp#L32-L360<br/>· 初始化 EGL / ANativeWindow<br/>· requestCreateCanvas / requestViewportChanged<br/>· renderLoop -> drawFrame -> eglSwapBuffers]
    subgraph TextureView 层
        D[WmTextureView<br/>android/wmcanvas_library/.../WmTextureView.java#L11-L102] --> E[WmTextureViewCallback<br/>android/wmcanvas_library/.../WmTextureViewCallback.java#L29-L152]
        E -->|SurfaceTexture 事件| F[JNI onSurfaceChanged<br/>core/android/3d/Wm3d_jni.cpp#L34-L146<br/>· 绑定 Surface<br/>· 启动 WmRenderer]
    end
    C --> G[WmCanvasAndroid / WmCanvas2DContextAndroid<br/>core/src/platform/Android/WmCanvasAndroid.cpp#L5-L23<br/>core/src/platform/Android/WmCanvas2DContextAndroid.cpp#L144-L220<br/>· ResizeCanvas<br/>· FBO/Shader 管线<br/>· Clear/Draw 批次]
    G --> H[TextureView SurfaceTexture<br/>最终帧显示在 UI]

    E -.->|sendEvent 回调| B
    C -->|bindTexture / texSubImage2D （可选）| G
```



---

## 八、确认问题

**请确认以下事项后再开始执行:**

1. **是否同意保持当前架构?** (不迁移C++代码到library)
2. **优先级排序?** (测试用例 > API完善 > 构建优化)
3. **测试用例范围?** (先做Canvas 2D还是同时做WebGL?)
4. **是否需要CI/CD?** (GitHub Actions自动构建和测试)

**如果确认，我将按以下顺序执行:**
1. 创建测试目录结构
2. 移植第一个简单测试 (fillRect)
3. 添加测试路由和界面
4. 逐步移植更多测试用例

---

生成时间: 2026-01-06
版本: v1.0
