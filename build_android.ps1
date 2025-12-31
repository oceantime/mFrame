# WmCanvas Android Build Script for PowerShell
# 用于编译生成 Android 所需的组件包

Write-Host "====================================" -ForegroundColor Cyan
Write-Host "WmCanvas Android 编译工具" -ForegroundColor Cyan
Write-Host "====================================" -ForegroundColor Cyan
Write-Host ""

# 检查是否存在 gradlew
if (-not (Test-Path "gradlew.bat")) {
    Write-Host "错误: gradlew.bat 未找到！" -ForegroundColor Red
    Write-Host "请在项目根目录运行此脚本。" -ForegroundColor Red
    Read-Host "按任意键退出"
    exit 1
}

try {
    Write-Host "[1/5] 清理构建目录..." -ForegroundColor Yellow
    .\gradlew.bat clean
    if ($LASTEXITCODE -ne 0) { throw "清理失败" }
    Write-Host "清理完成。" -ForegroundColor Green
    Write-Host ""

    Write-Host "[2/5] 编译 gcanvas_library (核心库)..." -ForegroundColor Yellow
    .\gradlew.bat :android:gcanvas_library:assembleRelease
    if ($LASTEXITCODE -ne 0) { throw "gcanvas_library 编译失败" }
    Write-Host "gcanvas_library 编译完成。" -ForegroundColor Green
    Write-Host ""

    Write-Host "[3/5] 编译 bridge_spec..." -ForegroundColor Yellow
    .\gradlew.bat :android:bridge_spec:assembleRelease
    if ($LASTEXITCODE -ne 0) { throw "bridge_spec 编译失败" }
    Write-Host "bridge_spec 编译完成。" -ForegroundColor Green
    Write-Host ""

    Write-Host "[4/5] 编译 gcanvas_imageloader_fresco..." -ForegroundColor Yellow
    .\gradlew.bat :android:adapters:gcanvas_imageloader_fresco:assembleRelease
    if ($LASTEXITCODE -ne 0) { throw "gcanvas_imageloader_fresco 编译失败" }
    Write-Host "gcanvas_imageloader_fresco 编译完成。" -ForegroundColor Green
    Write-Host ""

    Write-Host "[5/5] 编译完成！" -ForegroundColor Green
    Write-Host ""
    Write-Host "====================================" -ForegroundColor Cyan
    Write-Host "生成的 AAR 文件位置:" -ForegroundColor Cyan
    Write-Host "====================================" -ForegroundColor Cyan
    Write-Host "1. android\gcanvas_library\build\outputs\aar\"
    Write-Host "2. android\bridge_spec\build\outputs\aar\"
    Write-Host "3. android\adapters\gcanvas_imageloader_fresco\build\outputs\aar\"
    Write-Host ""

    # 列出生成的文件
    Write-Host "详细列表:" -ForegroundColor Cyan
    Write-Host ""
    
    if (Test-Path "android\gcanvas_library\build\outputs\aar\") {
        Write-Host "[gcanvas_library]:" -ForegroundColor Yellow
        Get-ChildItem "android\gcanvas_library\build\outputs\aar\*.aar" | ForEach-Object { Write-Host "  - $($_.Name)" -ForegroundColor White }
        Write-Host ""
    }
    
    if (Test-Path "android\bridge_spec\build\outputs\aar\") {
        Write-Host "[bridge_spec]:" -ForegroundColor Yellow
        Get-ChildItem "android\bridge_spec\build\outputs\aar\*.aar" | ForEach-Object { Write-Host "  - $($_.Name)" -ForegroundColor White }
        Write-Host ""
    }
    
    if (Test-Path "android\adapters\gcanvas_imageloader_fresco\build\outputs\aar\") {
        Write-Host "[gcanvas_imageloader_fresco]:" -ForegroundColor Yellow
        Get-ChildItem "android\adapters\gcanvas_imageloader_fresco\build\outputs\aar\*.aar" | ForEach-Object { Write-Host "  - $($_.Name)" -ForegroundColor White }
        Write-Host ""
    }

    Write-Host "====================================" -ForegroundColor Cyan
    $publish = Read-Host "是否要发布到本地 Maven 仓库？(Y/N)"
    
    if ($publish -eq "Y" -or $publish -eq "y") {
        Write-Host ""
        Write-Host "正在发布到本地 Maven 仓库..." -ForegroundColor Yellow
        Write-Host ""
        
        Write-Host "[1/3] 发布 gcanvas_library..." -ForegroundColor Yellow
        .\gradlew.bat :android:gcanvas_library:publishToMavenLocal
        
        Write-Host "[2/3] 发布 bridge_spec..." -ForegroundColor Yellow
        .\gradlew.bat :android:bridge_spec:publishToMavenLocal
        
        Write-Host "[3/3] 发布 gcanvas_imageloader_fresco..." -ForegroundColor Yellow
        .\gradlew.bat :android:adapters:gcanvas_imageloader_fresco:publishToMavenLocal
        
        Write-Host ""
        Write-Host "发布完成！Maven 本地仓库位置:" -ForegroundColor Green
        Write-Host "$env:USERPROFILE\.m2\repository\com\taobao\gcanvas\" -ForegroundColor White
        Write-Host ""
    }

    Write-Host "====================================" -ForegroundColor Cyan
    Write-Host "构建过程全部完成！" -ForegroundColor Green
    Write-Host "====================================" -ForegroundColor Cyan
}
catch {
    Write-Host ""
    Write-Host "错误: $_" -ForegroundColor Red
    Write-Host "构建失败！" -ForegroundColor Red
    Read-Host "按任意键退出"
    exit 1
}

Read-Host "按任意键退出"
