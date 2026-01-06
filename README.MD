# WmCanvas 项目架构文档

---

## 目录

1. [核心逻辑目录和类清单](#一核心逻辑目录和类清单)
2. [平台实现目录和类](#二平台实现目录和类)
3. [Core中的各种模式分析](#三core中的各种模式分析)

---

## 一、核心逻辑目录和类清单

### 1.1 核心渲染引擎 (`core/src/`)

#### 根目录
| 文件 | 说明 |
|------|------|
| `WmCanvas.cpp/hpp` | 画布基类，提供基础画布功能 |
| `WmCanvasManager.cpp/h` | 画布管理器，负责多画布实例管理 |
| `WmCanvasWeex.cpp/hpp` | Weex特定实现（可裁剪） |
| `export.h` | 导出符号定义，控制API可见性 |

#### 2D渲染引擎 (`wmcanvas/`)
| 文件 | 说明 |
|------|------|
| `WmCanvas2dContext.cpp/h` | 2D上下文核心，实现Canvas 2D API |
| `WmCanvasState.cpp/h` | 状态管理（save/restore） |
| `WmPath.cpp/h` | 路径处理 |
| `WmPath2D.cpp/h` | Path2D对象实现 |
| `WmPathStroker.cpp/h` | 路径描边算法 |
| `WmFontManager.cpp/h` | 字体管理器 |
| `WmFontStyle.cpp/h` | 字体样式解析 |
| `WmGlyphCache.cpp/h` | 字形缓存优化 |
| `WmTreemap.cpp/h` | 纹理图集管理 |
| `WmConvert.cpp/h` | 类型转换工具 |

#### WebGL渲染 (`webgl/`)
| 文件 | 说明 |
|------|------|
| `WmWebglContext.cpp/h` | WebGL上下文 |
| `WmWebGLRenderContext.cpp` | WebGL渲染上下文 |
| `WmWebGLRenderContextInner.cpp` | 内部实现细节 |
| `WmCommandDecoderWebGL.cpp` | WebGL命令解码器 |

#### 命令缓冲 (`commandbuffer/`)
| 文件 | 说明 |
|------|------|
| `WmCommandBuffer.cpp` | 命令缓冲管理 |
| `WmCommandDecoder.cpp` | 命令解码器基类 |

#### 工具支持 (`support/`)
| 文件 | 说明 |
|------|------|
| `CharacterSet.cpp` | 字符集处理 |
| `Encode.cpp` | 编码转换 |
| `FileUtils.cpp` | 文件操作工具 |
| `Util.cpp` | 通用工具函数 |
| `Log.cpp` | 日志系统 |
| `Value.cpp` | 值处理 |

#### OpenGL封装 (`wmcanvas/GL/`)
| 文件 | 说明 |
|------|------|
| `WmFrameBufferObject.cpp` | FBO（帧缓冲对象）管理 |
| `WmShader.cpp` | 着色器封装 |
| `WmShaderManager.cpp` | 着色器管理器 |
| `WmTexture.cpp` | 纹理管理 |
| `GLUtil.cpp` | OpenGL工具函数 |

---

## 二、平台实现目录和类

### 2.1 Android平台

#### 核心实现 (`core/src/platform/Android/`)
| 文件 | 说明 |
|------|------|
| `WmCanvasAndroid.cpp/h` | Android平台画布实现 |
| `WmCanvas2DContextAndroid.cpp/h` | Android 2D上下文 |
| `WmCanvas2DContextImpl.cpp` | 2D上下文具体实现 |
| `WmFontManagerAndroid.cpp/h` | Android字体管理器 |
| `WmFrameBufferObjectImpl.cpp` | FBO平台实现 |
| `WmPreCompiledShaders.cpp/h` | 预编译着色器（性能优化） |

#### 字体系统
| 文件 | 说明 |
|------|------|
| `WmFont.cpp/h` | 字体类 |
| `WmFontCache.cpp/h` | 字体缓存 |
| `WmFontFamily.cpp/h` | 字体族管理 |
| `WmFreeTypeWrap.cpp/h` | FreeType库封装 |
| `WmSystemFontInformation.cpp/h` | 系统字体信息获取 |

#### EGL封装 (`egl/`)
| 文件 | 说明 |
|------|------|
| `WmEGLPbufferContext.cpp` | 离屏渲染上下文 |
| `WmEGLWindowContext.cpp` | 窗口渲染上下文 |
| `WmSharedEGLContext.cpp` | 共享EGL上下文 |

#### JNI层 (`core/android/`)
| 文件 | 说明 |
|------|------|
| `WmCanvasJNI.cpp/h` | JNI接口定义和实现 |
| `WmCanvasLinkNative.cpp/h` | Native层链接（Weex相关） |
| `WmCanvasManagerAndroid.cpp` | Android管理器扩展 |

#### 辅助模块 (`core/android/`)
| 目录/文件 | 说明 |
|----------|------|
| `png/` | PNG编解码（lodepng） |
| `memory/` | 内存管理 |
| `3d/` | 3D相关模块 |
| `3d/jsc/` | JavaScript Core绑定 |
| `3d/util/` | 3D工具 |
| `3d/view/` | 3D视图 |
| `freetype-prebuilt/` | 预编译FreeType库 |

#### Java层 (`android/wmcanvas_library/src/main/java/com/honghu/wmcanvas/`)
| 文件/目录 | 说明 |
|----------|------|
| `WmCanvas.java` | Java API主类，模式定义 |
| `WmCanvasJNI.java` | JNI调用封装 |
| `WmCanvasResult.java` | 结果封装 |
| `WmCanvasWebView.java` | WebView集成 |
| `surface/` | Surface管理（WmTextureView等） |
| `util/` | 工具类（WmCanvasHelper等） |
| `audio/` | 音频支持 |

### 2.2 iOS平台 (`ios/BridgeModule/`)

| 文件 | 说明 |
|------|------|
| `WmCanvasModule.h/m` | iOS模块主类 |
| `WmCanvasObject.h/m` | 画布对象 |
| `WmCanvasPlugin.h/mm` | 插件接口 |
| `WmCanvasModuleProtocol.h` | 模块协议定义 |
| `WmCanvasViewProtocol.h` | 视图协议定义 |
| `WmCanvasSDK.h` | SDK头文件 |
| `WCVCommon.h/m` | 通用工具 |
| `WCVLog.h/m` | 日志系统 |

### 2.3 鸿蒙OS扩展建议 ⭐

建议新增以下目录结构：

```
core/src/platform/HarmonyOS/
├── WmCanvasHarmony.cpp/h              # 鸿蒙画布实现
├── WmCanvas2DContextHarmony.cpp/h     # 鸿蒙2D上下文
├── WmFontManagerHarmony.cpp/h        # 鸿蒙字体管理
├── WmFrameBufferObjectImpl.cpp       # FBO实现
├── egl/
│   ├── WmEGLHarmonyContext.cpp       # 鸿蒙EGL上下文
│   ├── WmEGLPbufferContext.cpp       # 离屏渲染
│   └── WmEGLWindowContext.cpp        # 窗口渲染

harmony/
├── napi/                              # N-API接口层
│   ├── WmCanvasNAPI.cpp/h             # NAPI绑定
│   └── WmCanvasModule.cpp             # 模块导出
├── wmcanvas_library/                   # 鸿蒙Library
│   └── src/main/ets/                 # ArkTS源码
│       └── com/honghu/wmcanvas/
│           ├── WmCanvas.ets           # 主类
│           ├── WmCanvasJNI.ets        # NAPI调用
│           └── surface/              # Surface管理
└── demo/                              # 鸿蒙Demo
```

**关键适配点**：
- 使用N-API替代JNI
- 适配ArkUI组件系统
- 使用鸿蒙图形栈（EGL/OpenGL ES）
- 适配鸿蒙字体系统

---

## 三、Core中的各种模式分析

### 3.1 已有的Canvas模式

#### 源码位置 (`android/wmcanvas_library/src/main/java/com/honghu/wmcanvas/WmCanvas.java`)
```java
public enum ViewMode {
    NONE_MODE,           // 无模式
    SINGLE_CANVAS_MODE,  // 单画布模式
    SWITCH_MODE,         // 切换模式
    HYBRID_MODE,         // 混合模式（默认）
    FLOAT_HYBRID_MODE,   // 浮动混合模式
    WEEX_MODE            // Weex模式
}

public static final ViewMode DEFAULT_VIEW_MODE = ViewMode.HYBRID_MODE;
```

### 3.2 各模式详细说明

#### 1. NONE_MODE（无模式）

**功能描述**：
- 未初始化状态或禁用状态
- 不进行任何渲染
- 用于占位或延迟初始化

**相关代码文件**：
- `android/wmcanvas_library/src/main/java/com/honghu/wmcanvas/WmCanvas.java`

**使用场景**：
- Canvas未创建时的初始状态
- 禁用Canvas功能时

---

#### 2. SINGLE_CANVAS_MODE（单画布模式）

**功能描述**：
- 一个GLSurfaceView/TextureView对应一个Canvas
- Canvas独占整个View
- 性能最优，适合全屏Canvas应用

**相关代码文件**：
- `android/wmcanvas_library/src/main/java/com/honghu/wmcanvas/WmCanvas.java`
- `android/wmcanvas_library/src/main/java/com/honghu/wmcanvas/util/WmCanvasHelper.java`
- `android/wmcanvas_library/src/main/java/com/honghu/wmcanvas/surface/WmTextureView.java`
- `android/wmcanvas_library/src/main/java/com/honghu/wmcanvas/surface/WmTextureViewCallback.java`

**使用场景**：
- 游戏应用（全屏渲染）
- 图表应用
- Canvas为主要内容的页面

**代码示例**：
```java
WmCanvasHelper.parseViewModeString("canvas"); // 返回 SINGLE_CANVAS_MODE
```

---

#### 3. SWITCH_MODE（切换模式）

**功能描述**：
- 多个Canvas实例可以切换显示
- 同一时刻只有一个Canvas激活
- 节省GPU资源

**相关代码文件**：
- `android/wmcanvas_library/src/main/java/com/honghu/wmcanvas/WmCanvas.java`
- `android/wmcanvas_library/src/main/java/com/honghu/wmcanvas/util/WmCanvasHelper.java`

**使用场景**：
- Tab页切换
- 页面轮播
- 多Canvas场景但不需要同时显示

**代码示例**：
```java
WmCanvasHelper.parseViewModeString("switch"); // 返回 SWITCH_MODE
```

---

#### 4. HYBRID_MODE（混合模式，默认）

**功能描述**：
- Canvas与Native View混合渲染
- Canvas可以与其他UI组件共存
- 平衡性能和灵活性
- **项目默认模式**

**相关代码文件**：
- `android/wmcanvas_library/src/main/java/com/honghu/wmcanvas/WmCanvas.java`（默认值）
- `android/wmcanvas_library/src/main/java/com/honghu/wmcanvas/util/WmCanvasHelper.java`

**使用场景**：
- 大多数应用场景
- Canvas作为页面的一部分
- 需要Canvas与Native控件交互

**代码示例**：
```java
WmCanvasHelper.parseViewModeString("hybrid"); // 返回 HYBRID_MODE
WmCanvasHelper.parseViewModeString("default"); // 返回默认模式 HYBRID_MODE
```

---

#### 5. FLOAT_HYBRID_MODE（浮动混合模式）

**功能描述**：
- Canvas浮动在Native View之上
- Canvas作为覆盖层（Overlay）
- 类似Dialog或悬浮窗效果

**相关代码文件**：
- `android/wmcanvas_library/src/main/java/com/honghu/wmcanvas/WmCanvas.java`
- `android/wmcanvas_library/src/main/java/com/honghu/wmcanvas/util/WmCanvasHelper.java`

**使用场景**：
- 悬浮绘图工具
- 水印效果
- 需要Canvas覆盖在其他内容之上

**代码示例**：
```java
WmCanvasHelper.parseViewModeString("float"); // 返回 FLOAT_HYBRID_MODE
```

---

### 3.3 模式选择建议

| 场景 | 推荐模式 | 理由 |
|------|---------|------|
| 游戏/全屏Canvas | SINGLE_CANVAS_MODE | 性能最优 |
| 普通Web页面 | HYBRID_MODE | 灵活性好 |
| Tab切换 | SWITCH_MODE | 节省资源 |
| 悬浮效果 | FLOAT_HYBRID_MODE | 层级控制 |

---
