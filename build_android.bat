@echo off
REM GCanvas Android 编译脚本
REM 用于编译生成 Android 所需的组件包

echo ====================================
echo GCanvas Android 编译工具
echo ====================================
echo.

REM 检查是否存在 gradlew
if not exist "gradlew.bat" (
    echo 错误: gradlew.bat 未找到！
    echo 请在项目根目录运行此脚本。
    pause
    exit /b 1
)

echo [1/5] 清理构建目录...
call gradlew.bat clean
if errorlevel 1 (
    echo 清理失败！
    pause
    exit /b 1
)
echo 清理完成。
echo.

echo [2/5] 编译 gcanvas_library (核心库)...
call gradlew.bat :android:gcanvas_library:assembleRelease
if errorlevel 1 (
    echo gcanvas_library 编译失败！
    pause
    exit /b 1
)
echo gcanvas_library 编译完成。
echo.

echo [3/5] 编译 bridge_spec...
call gradlew.bat :android:bridge_spec:assembleRelease
if errorlevel 1 (
    echo bridge_spec 编译失败！
    pause
    exit /b 1
)
echo bridge_spec 编译完成。
echo.

echo [4/5] 编译 gcanvas_imageloader_fresco...
call gradlew.bat :android:adapters:gcanvas_imageloader_fresco:assembleRelease
if errorlevel 1 (
    echo gcanvas_imageloader_fresco 编译失败！
    pause
    exit /b 1
)
echo gcanvas_imageloader_fresco 编译完成。
echo.

echo [5/5] 编译完成！
echo.
echo ====================================
echo 生成的 AAR 文件位置:
echo ====================================
echo 1. android\gcanvas_library\build\outputs\aar\
echo 2. android\bridge_spec\build\outputs\aar\
echo 3. android\adapters\gcanvas_imageloader_fresco\build\outputs\aar\
echo.

REM 列出生成的文件
echo 详细列表:
echo.
if exist "android\gcanvas_library\build\outputs\aar\" (
    echo [gcanvas_library]:
    dir /b "android\gcanvas_library\build\outputs\aar\*.aar"
    echo.
)
if exist "android\bridge_spec\build\outputs\aar\" (
    echo [bridge_spec]:
    dir /b "android\bridge_spec\build\outputs\aar\*.aar"
    echo.
)
if exist "android\adapters\gcanvas_imageloader_fresco\build\outputs\aar\" (
    echo [gcanvas_imageloader_fresco]:
    dir /b "android\adapters\gcanvas_imageloader_fresco\build\outputs\aar\*.aar"
    echo.
)

echo ====================================
echo 是否要发布到本地 Maven 仓库？(Y/N)
set /p choice=请选择: 

if /i "%choice%"=="Y" (
    echo.
    echo 正在发布到本地 Maven 仓库...
    echo.
    
    echo [1/3] 发布 gcanvas_library...
    call gradlew.bat :android:gcanvas_library:publishToMavenLocal
    
    echo [2/3] 发布 bridge_spec...
    call gradlew.bat :android:bridge_spec:publishToMavenLocal
    
    echo [3/3] 发布 gcanvas_imageloader_fresco...
    call gradlew.bat :android:adapters:gcanvas_imageloader_fresco:publishToMavenLocal
    
    echo.
    echo 发布完成！Maven 本地仓库位置:
    echo %USERPROFILE%\.m2\repository\com\taobao\gcanvas\
    echo.
)

echo ====================================
echo 构建过程全部完成！
echo ====================================
pause
