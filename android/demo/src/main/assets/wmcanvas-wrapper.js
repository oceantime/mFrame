/**
 * WMCanvas Wrapper - JavaScript 端统一封装
 * 
 * 功能说明：
 * - WMCanvasCamera API 由 Java 端直接注入到 window 对象
 * - 该文件仅用于检测和日志记录
 * 
 * 可用API：
 * - window.WMCanvasCamera.start(width, height)
 * - window.WMCanvasCamera.stop()
 * - window.WMCanvasCamera.setFrameEnabled(enabled)
 * - window.WMCanvasCamera.takePicture()
 * - window.WMCanvasCamera.getCapabilities()
 */

(function() {
    'use strict';
    
    console.log('✅ WMCanvas wrapper loaded');
    
    // 检测相机API是否可用
    if (typeof WMCanvasCamera !== 'undefined') {
        console.log('✅ WMCanvasCamera API available');
        console.log('📹 Available methods:', Object.keys(WMCanvasCamera));
    } else {
        console.warn('⚠️ WMCanvasCamera API not available');
        console.warn('💡 Make sure Java side has injected the bridge:');
        console.warn('   webView.addJavascriptInterface(new WMCanvasCameraBridge(), \"WMCanvasCamera\")');
    }
})();
