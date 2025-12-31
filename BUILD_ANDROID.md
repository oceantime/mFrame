# WmCanvas Android 构建指南

本指南将帮助你编译 WmCanvas 项目并生成 Android 所需的组件包。

## 前提条件

- **JDK**: OpenJDK 8 (1.8.0_472) 或更高版本
- **Android SDK**: API Level 28
- **Android NDK**: 21.1.6352462
- **Gradle**: 8.12.0（通过 wrapper 自动下载）
- **Android Gradle Plugin**: 8.5.0

## 项目结构说明

WmCanvas Android 包含以下主要模块：

1. **gcanvas_library** (corelib) - 核心库
   - Group ID: `com.taobao.gcanvas`
   - Artifact ID: `gcanvas_library`
   - 版本: 1.2.0

2. **img_fresco** - Fresco 图片加载适配器
   - Group ID: `com.taobao.gcanvas.adapters`
   - Artifact ID: `img_fresco`
   - 版本: 1.1.0

3. **spec** - Bridge 规范接口
   - Group ID: `com.taobao.gcanvas.bridges`
   - Artifact ID: `spec`
   - 版本: 1.1.0

4. **weex** - Weex Bridge (需要单独配置)
   - Group ID: `com.taobao.gcanvas.bridges`
   - Artifact ID: `weex`

## 编译步骤

### 方法一：发布到本地 Maven 仓库（推荐）

这种方法会将编译后的 AAR 文件发布到本地 Maven 仓库，便于其他项目引用。

1. **配置 SDK 路径**

在项目根目录创建或编辑 `local.properties` 文件，添加 Android SDK 路径：

```properties
sdk.dir=C\:\\Users\\YourUsername\\AppData\\Local\\Android\\sdk
```

**注意**: 不要在 `local.properties` 中设置 `ndk.dir`（已废弃）。NDK 版本通过 `build.gradle` 中的 `android.ndkVersion` 配置。

2. **编译并发布所有模块到本地 Maven**

在项目根目录下运行：

```bash
# Windows PowerShell
.\gradlew clean
.\gradlew :android:gcanvas_library:assembleRelease
.\gradlew :android:bridge_spec:assembleRelease
.\gradlew :android:adapters:gcanvas_imageloader_fresco:assembleRelease

# 发布到本地 Maven 仓库 (需要配置 maven-publish)
.\gradlew :android:gcanvas_library:publishToMavenLocal
.\gradlew :android:bridge_spec:publishToMavenLocal
.\gradlew :android:adapters:gcanvas_imageloader_fresco:publishToMavenLocal
```

### 方法二：直接编译 AAR 文件

如果只需要 AAR 文件，不需要发布到 Maven：

```bash
# Windows PowerShell
.\gradlew clean

# 编译所有 Release 版本
.\gradlew assembleRelease

# 或者单独编译每个模块
.\gradlew :android:gcanvas_library:assembleRelease
.\gradlew :android:bridge_spec:assembleRelease
.\gradlew :android:adapters:gcanvas_imageloader_fresco:assembleRelease
```

生成的 AAR 文件位置及大小：

| 模块 | 文件名 | 大小 | 路径 |
|------|--------|------|------|
| gcanvas_library (ali) | gcanvas_library-ali-release.aar | 924.04 KB | `android/gcanvas_library/build/outputs/aar/` |
| gcanvas_library (standalone) | gcanvas_library-standalone-release.aar | 776.41 KB | `android/gcanvas_library/build/outputs/aar/` |
| bridge_spec (ali) | bridge_spec-ali-release.aar | 15.57 KB | `android/bridge_spec/build/outputs/aar/` |
| bridge_spec (standalone) | bridge_spec-standalone-release.aar | 15.57 KB | `android/bridge_spec/build/outputs/aar/` |
| gcanvas_imageloader_fresco | gcanvas_imageloader_fresco-release.aar | 5.53 KB | `android/adapters/gcanvas_imageloader_fresco/build/outputs/aar/` |

**注意事项：**
- gcanvas_library 和 bridge_spec 有两个 flavor：`ali`（含 Weex）和 `standalone`（不含 Weex）
- 推荐使用 `ali` 版本以获得完整功能
- gcanvas_imageloader_fresco 没有 flavor，只有一个 release 版本

## 使用生成的组件

### 使用本地 Maven 仓库

在你的项目 `build.gradle` 中添加：

```gradle
repositories {
    mavenLocal()
    maven { url 'https://maven.aliyun.com/repository/google' }
    maven { url 'https://maven.aliyun.com/repository/jcenter' }
}

dependencies {
    implementation 'com.taobao.gcanvas:gcanvas_library:1.2.0'
    implementation 'com.taobao.gcanvas.adapters:img_fresco:1.1.0'
    implementation 'com.taobao.gcanvas.bridges:spec:1.1.0'
}
```

### 使用本地 AAR 文件

1. 将 AAR 文件复制到你的项目 `app/libs` 目录
2. 在 `build.gradle` 中添加：

```gradle
repositories {
    flatDir {
        dirs 'libs'
    }
}

dependencies {
    implementation(name:'gcanvas_library-release', ext:'aar')
    implementation(name:'bridge_spec-release', ext:'aar')
    implementation(name:'gcanvas_imageloader_fresco-release', ext:'aar')
    
    // 需要的依赖
    implementation 'com.alibaba:fastjson:1.1.54.android'
    implementation 'com.facebook.fresco:fresco:1.8.0'
}
```

## 常见问题

## 常见问题

### 1. ~~SAXParseException XML 解析警告~~（已解决）

**状态**: ✅ 已通过升级到 Gradle 8.12.0 + AGP 8.5.0 解决

旧版本 AGP 3.3.0 在使用新版 Android SDK 工具时会出现 XML 解析警告。升级到 AGP 8.5.0 后已完全消除。

### 2. NDK 编译失败

确保安装了正确版本的 NDK，并在 `local.properties` 中正确配置了路径。

### 3. CMake 错误

确保安装了 CMake 3.10 或更高版本。可以通过 Android SDK Manager 安装。

### 4. 缺少 Weex Bridge

当前项目配置中 `weex_bridge` 模块路径不存在。如果需要 Weex 支持，需要：
- 检查 `bridges/weex-gcanvas/android/weex_bridge` 目录是否存在
- 或者从 settings.gradle 中移除该模块的引用

### 5. ~~发布到 Bintray~~（已废弃）

**状态**: ❌ JFrog Bintray 已于 2021 年 2 月关闭服务

如需发布到远程仓库，建议使用：
- **Maven Central**: 通过 Sonatype OSSRH
- **GitHub Packages**: GitHub 原生 Maven 仓库
- **JitPack**: 直接从 GitHub 仓库构建

当前项目已移除 Bintray 插件配置。

## 验证编译结果

编译成功后，可以检查以下位置：

```bash
# 查看生成的 AAR 文件
dir android\gcanvas_library\build\outputs\aar\
dir android\bridge_spec\build\outputs\aar\
dir android\adapters\gcanvas_imageloader_fresco\build\outputs\aar\

# 查看本地 Maven 仓库（Windows）
dir %USERPROFILE%\.m2\repository\com\taobao\gcanvas\
```

## 参考资料

- [WmCanvas 官方文档](https://alibaba.github.io/WmCanvas/)
- [Android 开发指南](docs/Guide_Android_Setup_Weex.md)
- [CSDN 参考文章](https://blog.csdn.net/qq_33718648/article/details/82286559)
