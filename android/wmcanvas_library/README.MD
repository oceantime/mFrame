# 模块说明
此模块为最终输出到端内的 WmCanvas 模块，产物为 wmcanvas_library aar 文件，aar 内包含 C++ 层实现和 Java 层实现，
其中 C++ 层文件包含 WmCanvas.so、wmcanvas_runtime.so、freetype.so，而 Java 层主要是 WmCanvas View 上层类接口。


# Build 说明

### AAR
./gradlew :android:wmcanvas_library:assembleDebug (debug包)
./gradlew :android:wmcanvas_library:assembleRelease (release包)

### Standalone Flavor
./gradlew :android:wmcanvas_library:assembleStandaloneDebug
./gradlew :android:wmcanvas_library:assembleStandaloneRelease


# 库输出说明

- **Standalone Flavor**: 生成 libwmcanvas_runtime.so (支持独立运行)
- 所有 Flavor 都包含 libfreetype.so 和 libc++_shared.so







