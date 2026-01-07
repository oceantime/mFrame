# Batch convert file encoding from GB2312/GBK to UTF-8 without BOM
# Generate detailed report

$files = @(
    "core/src/WmCanvas.cpp",
    "core/src/commandbuffer/WmCommandBuffer.cpp",
    "core/src/commandbuffer/WmCommandBuffer.hpp",
    "core/src/platform/Android/egl/WmEGLContext.h",
    "core/src/platform/Android/egl/WmEGLPbufferContext.cpp",
    "core/src/platform/Android/egl/WmEGLWindowContext.cpp",
    "core/src/platform/Android/WmCanvas2DContextAndroid.cpp",
    "core/src/platform/Android/WmCanvas2DContextImpl.cpp",
    "core/src/platform/Android/WmFont.cpp",
    "core/src/platform/Android/WmFontFamily.cpp",
    "core/src/platform/Android/WmFontFamily.h",
    "core/src/platform/Android/WmFontManagerAndroid.cpp",
    "core/src/platform/Android/WmFrameBufferObjectImpl.cpp",
    "core/src/platform/Android/WmFreeTypeWrap.cpp",
    "core/src/platform/Linux/WmFontLinux.cpp",
    "core/src/platform/Linux/WmFontManagerLinux.cpp",
    "core/src/platform/Linux/WmFontManagerLinux.hpp",
    "core/src/platform/Linux/WmSystemFontInformation.cpp",
    "core/src/webgl/WmCommandDecoderWebGL.cpp",
    "core/src/webgl/WmCommandDecoderWebGL.hpp",
    "core/src/wmcanvas/GL/WmFrameBufferObject.cpp",
    "core/src/wmcanvas/GL/WmTexture.cpp",
    "core/src/wmcanvas/WmPoint.h"
)

$reportPath = "encoding-conversion-report.txt"
$convertedCount = 0
$failedCount = 0
$skippedCount = 0
$report = @()

$report += "================================================================"
$report += "File Encoding Conversion Report - GB2312/GBK to UTF-8 (No BOM)"
$report += "Conversion Time: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')"
$report += "================================================================"
$report += ""

foreach ($file in $files) {
    $fullPath = Join-Path $PSScriptRoot $file
    
    $report += "----------------------------------------------------------------"
    $report += "File: $file"
    
    if (-not (Test-Path $fullPath)) {
        $report += "Status: FILE NOT FOUND"
        $failedCount++
        $report += ""
        continue
    }
    
    try {
        # Read original bytes
        $bytes = [System.IO.File]::ReadAllBytes($fullPath)
        
        # Detect original encoding
        $originalEncoding = "Unknown"
        $hasBOM = $false
        
        if ($bytes.Length -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF) {
            $originalEncoding = "UTF-8 with BOM"
            $hasBOM = $true
        }
        elseif ($bytes.Length -ge 2 -and $bytes[0] -eq 0xFF -and $bytes[1] -eq 0xFE) {
            $originalEncoding = "UTF-16 LE"
        }
        elseif ($bytes.Length -ge 2 -and $bytes[0] -eq 0xFE -and $bytes[1] -eq 0xFF) {
            $originalEncoding = "UTF-16 BE"
        }
        else {
            # Check for GB2312/GBK characters
            $hasGBKChars = $false
            for ($i = 0; $i -lt $bytes.Length - 1; $i++) {
                if ($bytes[$i] -ge 0x81 -and $bytes[$i] -le 0xFE -and 
                    $bytes[$i+1] -ge 0x40 -and $bytes[$i+1] -le 0xFE) {
                    $hasGBKChars = $true
                    break
                }
            }
            if ($hasGBKChars) {
                $originalEncoding = "GB2312/GBK"
            }
            else {
                # Check if already valid UTF-8
                try {
                    $utf8Test = [System.Text.Encoding]::UTF8.GetString($bytes)
                    if ($utf8Test -match '[\u4e00-\u9fa5]') {
                        $originalEncoding = "UTF-8 (no BOM, already correct)"
                        $report += "Original Encoding: $originalEncoding"
                        $report += "Status: SKIPPED (already UTF-8 no BOM)"
                        $skippedCount++
                        $report += ""
                        continue
                    }
                    else {
                        $originalEncoding = "ASCII/UTF-8 compatible"
                    }
                }
                catch {
                    $originalEncoding = "ASCII/Unknown"
                }
            }
        }
        
        $report += "Original Encoding: $originalEncoding"
        
        # Decode using GB2312
        $gb2312 = [System.Text.Encoding]::GetEncoding('GB2312')
        $content = $gb2312.GetString($bytes)
        
        # Extract Chinese comment examples
        $chineseComments = @()
        $lines = $content -split "`n"
        $lineNum = 0
        foreach ($line in $lines) {
            $lineNum++
            if ($line -match '//.*[\u4e00-\u9fa5]') {
                $chineseComments += "Line $lineNum : $($line.Trim())"
                if ($chineseComments.Count -ge 3) { break }
            }
            elseif ($line -match '/\*.*[\u4e00-\u9fa5]') {
                $chineseComments += "Line $lineNum : $($line.Trim())"
                if ($chineseComments.Count -ge 3) { break }
            }
        }
        
        if ($chineseComments.Count -gt 0) {
            $report += "Chinese Comments Found: $($chineseComments.Count) locations"
            $report += "Examples:"
            foreach ($comment in $chineseComments) {
                $report += "  $comment"
            }
        }
        else {
            $report += "Chinese Comments: None detected"
        }
        
        # Write as UTF-8 without BOM
        $utf8NoBOM = New-Object System.Text.UTF8Encoding $false
        $utf8Bytes = $utf8NoBOM.GetBytes($content)
        [System.IO.File]::WriteAllBytes($fullPath, $utf8Bytes)
        
        # Verify conversion
        $verifyBytes = [System.IO.File]::ReadAllBytes($fullPath)
        $isUTF8NoBOM = $true
        if ($verifyBytes.Length -ge 3 -and $verifyBytes[0] -eq 0xEF -and $verifyBytes[1] -eq 0xBB -and $verifyBytes[2] -eq 0xBF) {
            $isUTF8NoBOM = $false
        }
        
        if ($isUTF8NoBOM) {
            $report += "After Conversion: UTF-8 without BOM"
            $report += "Status: SUCCESS"
            $convertedCount++
        }
        else {
            $report += "Status: FAILED (BOM still present)"
            $failedCount++
        }
    }
    catch {
        $report += "Status: FAILED"
        $report += "Error: $($_.Exception.Message)"
        $failedCount++
    }
    
    $report += ""
}

$report += "================================================================"
$report += "Conversion Summary"
$report += "================================================================"
$report += "Total Files: $($files.Count)"
$report += "Successfully Converted: $convertedCount"
$report += "Skipped (already correct): $skippedCount"
$report += "Failed: $failedCount"
$report += "================================================================"

# Output to console
$report | ForEach-Object { Write-Host $_ }

# Save to file
$utf8NoBOM = New-Object System.Text.UTF8Encoding $false
[System.IO.File]::WriteAllLines($reportPath, $report, $utf8NoBOM)

Write-Host ""
Write-Host "Detailed report saved to: $reportPath" -ForegroundColor Green
