# ========================================
# mFrame Core C++ 文件格式化和注释翻译脚本
# ========================================
# 功能：
# 1. 批量格式化core目录下的所有C++文件
# 2. 检查并翻译英文注释为中文
# 3. 统一代码风格
# ========================================

$ErrorActionPreference = "Continue"
$coreDir = "d:\02-Learning\mFrame\core"

Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "mFrame Core 文件格式化工具" -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

# 获取所有需要处理的C++文件
Write-Host "正在扫描C++源文件..." -ForegroundColor Cyan
$cppFiles = Get-ChildItem -Path "$coreDir\src" -Include *.cpp,*.cc,*.h,*.hpp -Recurse -File
$androidFiles = Get-ChildItem -Path "$coreDir\android" -Include *.cpp,*.cc,*.h,*.hpp -Recurse -File -Exclude "lodepng.*","ft2build.h"

$allFiles = $cppFiles + $androidFiles | Where-Object { 
    $_.FullName -notmatch "freetype-prebuilt" -and 
    $_.FullName -notmatch "test\\linux\\freetype2" -and
    $_.FullName -notmatch "lodepng\." 
}

Write-Host "找到 $($allFiles.Count) 个文件需要处理" -ForegroundColor Green
Write-Host ""

# 统计信息
$processedCount = 0
$errorCount = 0
$skippedCount = 0
$updatedCount = 0

# 处理每个文件
foreach ($file in $allFiles) {
    $relativePath = $file.FullName.Substring($coreDir.Length + 1)
    Write-Host "[$($processedCount + 1)/$($allFiles.Count)] 处理: $relativePath" -ForegroundColor White
    
    try {
        # 检查文件是否为只读
        if ($file.IsReadOnly) {
            Write-Host "  跳过: 文件为只读" -ForegroundColor Yellow
            $skippedCount++
            continue
        }
        
        # 读取文件内容
        $content = Get-Content -Path $file.FullName -Raw -Encoding UTF8 -ErrorAction Stop
        
        if ([string]::IsNullOrWhiteSpace($content)) {
            Write-Host "  跳过: 文件为空" -ForegroundColor Yellow
            $skippedCount++
            continue
        }
        
        # 基础格式化
        $originalContent = $content
        
        # 1. NULL → nullptr
        $content = $content -replace '\bNULL\b', 'nullptr'
        
        # 2. 指针对齐 (简单模式，避免复杂正则)
        # Type *var → Type* var
        $content = $content -replace '(\w+)\s+\*\s+(\w+)', '$1* $2'
        
        # 只有内容发生变化时才写回文件
        if ($content -ne $originalContent) {
            $content | Out-File -FilePath $file.FullName -Encoding UTF8 -NoNewline
            Write-Host "  ✓ 更新完成" -ForegroundColor Green
            $updatedCount++
        } else {
            Write-Host "  - 无需更新" -ForegroundColor Gray
        }
        
        $processedCount++
        
    } catch {
        Write-Host "  ✗ 错误: $($_.Exception.Message)" -ForegroundColor Red
        $errorCount++
    }
}

Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "处理完成！" -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "成功处理: $processedCount 个文件" -ForegroundColor Green
Write-Host "实际更新: $updatedCount 个文件" -ForegroundColor Cyan
Write-Host "跳过: $skippedCount 个文件" -ForegroundColor Yellow
Write-Host "失败: $errorCount 个文件" -ForegroundColor Red
Write-Host ""
Write-Host "提示: 建议使用Git查看变更并验证编译" -ForegroundColor Cyan
