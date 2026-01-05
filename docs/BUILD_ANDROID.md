# WmCanvas Android 构建指南

本指南详细说明如何编译 WmCanvas Android 项目并生成可用的库文件。

## 前提条件

### 必需工具
- **JDK**: OpenJDK 8 (1.8.0_472) 或更高版本
- **Android SDK**: API Level 28
- **Android NDK**: 21.1.6352462
- **CMake**: 3.10+ (通过 Android SDK Manager 安装)
- **Gradle**: 8.12.0 (通过 wrapper 自动下载)
- **Android Gradle Plugin**: 8.5.0

### 环境配置

1. **配置 Android SDK 路径**

在项目根目录创建或编辑 `local.properties` 文件：

```properties
sdk.dir=C\:\\Users\\YourUsername\\AppData\\Local\\Android\\sdk
```

**注意**: 
- 路径中的反斜杠需要转义 (`\\`)
- 不要设置 `ndk.dir`（已废弃），NDK 版本由 `build.gradle` 中的 `android.ndkVersion` 控制

2. **验证环境**

```bash
# 检查 Java 版本
java -version

# 检查 Gradle
.\gradlew --version

# 列出可用任务
.\gradlew tasks
```

## 项目结构

WmCanvas Android 包含以下主要模块：

### 1. wmcanvas_library（核心库）
- **Group ID**: `com.honghu.canvas`
- **Artifact ID**: `wmcanvas_library`
- **版本**: 2.0.0
- **功能**: Canvas 2D/WebGL 渲染引擎、相机集成
- **输出**: AAR 文件

### 2. demo（演示应用）
- **Package**: `com.honghu.wmcanvas.demo`
- **功能**: Vue 3 单页应用演示
- **包含**: 列表优化、相机预览、Canvas 绘图

### 构建配置

- **Build Flavors**: `standalone`（精简版，不含 Weex）
- **Build Types**: `debug`, `release`
- **目标架构**: `armeabi-v7a`, `arm64-v8a`, `x86`, `x86_64`

## 构建步骤

### 方法一：构建 Library AAR（推荐）

生成可复用的 AAR 库文件。

```bash
# 清理旧的构建产物
.\gradlew clean

# 构建 Release 版本 AAR
.\gradlew :android:wmcanvas_library:assembleStandaloneRelease

# 构建 Debug 版本 AAR
.\gradlew :android:wmcanvas_library:assembleStandaloneDebug
```

**输出位置**：
```
android/wmcanvas_library/build/outputs/aar/
├── wmcanvas_library-standalone-release.aar  (~800 KB)
└── wmcanvas_library-standalone-debug.aar
```

### 方法二：构建 Demo APK

生成可安装的演示应用。

```bash
# 构建 Demo Release APK
.\gradlew :android:demo:assembleStandaloneRelease

# 构建 Demo Debug APK
.\gradlew :android:demo:assembleStandaloneDebug
```

**输出位置**：
```
android/demo/build/outputs/apk/standalone/release/
└── demo-standalone-release-unsigned.apk
```

**注意**: Release APK 需要签名才能安装。

### 方法三：发布到本地 Maven 仓库

便于在其他项目中通过 Maven 依赖引用。

```bash
# 发布到本地 Maven (~/.m2/repository)
.\gradlew :android:wmcanvas_library:publishToMavenLocal
```

**Maven 坐标**：
```gradle
implementation 'com.honghu.canvas:wmcanvas_library:2.0.0'
```

**本地仓库位置** (Windows):
```
%USERPROFILE%\.m2\repository\com\honghu\canvas\wmcanvas_library\2.0.0\
```

### 方法四：构建所有模块

一次性构建所有模块的所有变体。

```bash
# 构建所有 Release 版本
.\gradlew assembleStandaloneRelease

# 构建所有 Debug 版本
.\gradlew assembleStandaloneDebug

# 构建所有变体
.\gradlew assemble
```

## 使用构建产物

### 使用 AAR 文件

#### 方式 1: 本地 AAR 文件

1. 将 AAR 文件复制到项目的 `app/libs` 目录

2. 在 `build.gradle` 中配置：

```gradle
repositories {
    flatDir {
        dirs 'libs'
    }
}

dependencies {
    implementation(name: 'wmcanvas_library-standalone-release', ext: 'aar')
}
```

#### 方式 2: Maven Local

在项目 `settings.gradle` 或 `build.gradle` 中添加：

```gradle
repositories {
    mavenLocal()
    google()
    mavenCentral()
}

dependencies {
    implementation 'com.honghu.canvas:wmcanvas_library:2.0.0'
}
```

### 集成到应用

1. **添加权限** (`AndroidManifest.xml`)

```xml
<uses-permission android:name="android.permission.INTERNET" />
<uses-permission android:name="android.permission.CAMERA" />
```

2. **配置 WebView**

```java
WebView webView = findViewById(R.id.webview);
WebSettings settings = webView.getSettings();

// 启用 JavaScript
settings.setJavaScriptEnabled(true);
settings.setDomStorageEnabled(true);
settings.setAllowFileAccess(true);
settings.setAllowFileAccessFromFileURLs(true);
settings.setAllowUniversalAccessFromFileURLs(true);

// 启用 WebView 调试
if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
    WebView.setWebContentsDebuggingEnabled(true);
}

// 注入相机 API
webView.addJavascriptInterface(
    new WMCanvasCameraBridge(), 
    "WMCanvasCamera"
);

// 加载页面
webView.loadUrl("file:///android_asset/index.html");
```

3. **实现 Camera Bridge** (可选)

参考 [WebViewActivity.java](../android/demo/src/main/java/com/honghu/wmcanvas/demo/WebViewActivity.java)

## 构建优化

### 减小 APK/AAR 体积

1. **启用代码混淆** (Release 构建)

```gradle
buildTypes {
    release {
        minifyEnabled true
        proguardFiles getDefaultProguardFile('proguard-android-optimize.txt'), 'proguard-rules.pro'
    }
}
```

2. **筛选 ABI**

只打包需要的架构：

```gradle
android {
    defaultConfig {
        ndk {
            abiFilters 'armeabi-v7a', 'arm64-v8a'
        }
    }
}
```

3. **拆分 APK**

为不同架构生成单独的 APK：

```gradle
android {
    splits {
        abi {
            enable true
            reset()
            include 'armeabi-v7a', 'arm64-v8a', 'x86', 'x86_64'
            universalApk false
        }
    }
}
```

### 加速构建

1. **启用并行编译**

在 `gradle.properties` 中：

```properties
org.gradle.parallel=true
org.gradle.caching=true
org.gradle.configureondemand=true
```

2. **增加 Gradle 内存**

```properties
org.gradle.jvmargs=-Xmx4096m -XX:MaxMetaspaceSize=512m
```

## 调试与验证

### 验证 AAR 内容

```bash
# 解压 AAR 文件 (AAR 本质是 ZIP)
Expand-Archive android\wmcanvas_library\build\outputs\aar\wmcanvas_library-standalone-release.aar -DestinationPath .\temp

# 查看结构
tree .\temp /F

# 检查 .so 文件
dir .\temp\jni\*\*.so
```

**预期结构**：
```
temp/
├── AndroidManifest.xml
├── classes.jar
├── R.txt
├── res/
├── assets/
└── jni/
    ├── armeabi-v7a/
    │   └── libWmCanvas.so
    ├── arm64-v8a/
    │   └── libWmCanvas.so
    ├── x86/
    │   └── libWmCanvas.so
    └── x86_64/
        └── libWmCanvas.so
```

### 运行 Demo

```bash
# 安装到设备
.\gradlew :android:demo:installStandaloneDebug

# 或使用 adb
adb install android\demo\build\outputs\apk\standalone\debug\demo-standalone-debug.apk

# 启动应用
adb shell am start -n com.honghu.wmcanvas.demo/.WebViewActivity

# 查看日志
adb logcat -s WMCanvasCamera:* chromium:*
```

### Chrome DevTools 调试

1. 在代码中启用 WebView 调试
2. Chrome 浏览器访问 `chrome://inspect`
3. 在 "Remote Target" 中选择设备
4. 点击 "inspect" 打开 DevTools

## 常见问题

### 1. NDK 找不到

**错误**：
```
NDK is not installed
```

**解决**：
- 通过 Android Studio SDK Manager 安装 NDK 21.1.6352462
- 或在 `local.properties` 中指定 NDK 路径（不推荐）

### 2. CMake 版本不兼容

**错误**：
```
CMake Error: CMake was unable to find a build program
```

**解决**：
```bash
# 通过 Android SDK Manager 安装 CMake 3.10+
```

### 3. 编译超时

**错误**：
```
Read timed out
```

**解决**：
- 配置国内镜像源（阿里云 Maven）
- 增加 Gradle 超时时间

### 4. 内存不足

**错误**：
```
OutOfMemoryError: Java heap space
```

**解决**：
在 `gradle.properties` 中增加内存：
```properties
org.gradle.jvmargs=-Xmx4096m
```

### 5. ~~Bintray 发布失败~~（已废弃）

**状态**: ❌ JFrog Bintray 已于 2021 年关闭

**替代方案**：
- **Maven Central** (通过 Sonatype OSSRH)
- **GitHub Packages**
- **JitPack** (直接从 GitHub 构建)

### 6. ~~XML 解析警告~~（已解决）

**状态**: ✅ 升级到 Gradle 8.12.0 + AGP 8.5.0 后已解决

旧版本 AGP 3.3.0 与新版 Android SDK 工具不兼容导致的警告。

## 持续集成 (CI/CD)

### GitHub Actions 示例

```yaml
name: Build Android

on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]

jobs:
  build:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Set up JDK 8
      uses: actions/setup-java@v3
      with:
        java-version: '8'
        distribution: 'temurin'
    
    - name: Cache Gradle
      uses: actions/cache@v3
      with:
        path: |
          ~/.gradle/caches
          ~/.gradle/wrapper
        key: ${{ runner.os }}-gradle-${{ hashFiles('**/*.gradle*', '**/gradle-wrapper.properties') }}
    
    - name: Grant execute permission for gradlew
      run: chmod +x gradlew
    
    - name: Build with Gradle
      run: ./gradlew :android:wmcanvas_library:assembleStandaloneRelease
    
    - name: Upload AAR
      uses: actions/upload-artifact@v3
      with:
        name: wmcanvas-library
        path: android/wmcanvas_library/build/outputs/aar/*.aar
```

## 版本历史

### v2.0.0 (2026-01-05)
- ✅ 重命名 GCanvas → WmCanvas
- ✅ 更新包名 `com.taobao.gcanvas` → `com.honghu.canvas`
- ✅ 升级 Gradle 8.12.0 + AGP 8.5.0
- ✅ 移除 Weex 依赖
- ✅ 简化构建配置
- ✅ 更新 Demo 为 Vue 3 单页应用

### v1.2.0
- GCanvas 原始版本
- 支持 Weex、ReactNative
- Bintray 发布支持

## 参考文档

### 项目文档
- [项目架构文档](../README.MD)
- [Demo 使用说明](../android/demo/src/main/assets/README.md)
- [WebViewActivity 实现](../android/demo/src/main/java/com/honghu/wmcanvas/demo/WebViewActivity.java)

### 外部资源
- [Android NDK 指南](https://developer.android.com/ndk/guides)
- [CMake 文档](https://cmake.org/documentation/)
- [Gradle 构建指南](https://docs.gradle.org/)

## 技术支持

如遇问题，请：
1. 检查本文档的"常见问题"章节
2. 查看 GitHub Issues
3. 提交新的 Issue 并附上完整的错误日志

---

**最后更新**: 2026年1月5日  
**维护者**: WmCanvas Team
