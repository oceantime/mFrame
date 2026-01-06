# WMCanvas 演示应用 - 使用说明

## 📁 项目结构

```
android/demo/src/main/assets/
├── index.html              # 应用入口页面
├── wmcanvas-wrapper.js     # WMCanvas API 检测脚本
├── package.json            # Vue项目依赖配置
├── vite.config.js          # Vite构建配置
├── dist/                   # Vue构建输出目录
│   ├── app.js             # 编译后的主应用JS
│   └── assets/
│       └── main.css       # 样式文件
└── src/                    # Vue应用源码
    ├── main.js            # Vue应用入口
    ├── App.vue            # 根组件（导航栏）
    ├── router/            # 路由配置
    │   └── index.js
    ├── views/             # 页面视图
    │   ├── list/          # 列表演示页
    │   ├── picture/       # 相机演示页
    │   └── gcanvas/       # Canvas演示页
    └── share/             # 共享组件
        └── components/
            ├── scroll/    # 虚拟滚动组件
            └── camera/    # 相机渲染组件
```

## 🎯 功能模块

### 1. 单页应用架构
**入口**: `index.html`

基于Vue 3 + Vue Router的单页应用，提供：
- 顶部导航栏快速切换
- 三个独立功能视图
- 前端路由无刷新切换

### 2. 列表视图 (`/#/list`)

**Canvas长列表优化演示**

核心功能：
- ✅ 虚拟滚动渲染（仅渲染可见区域）
- ✅ Canvas对象池复用
- ✅ GPU硬件加速切换
- ✅ Tween.js平滑滚动动画
- ✅ 实时FPS性能监控
- ✅ 缓存命中率统计

性能指标：
- 列表容量：10,000项
- 可见项数：动态计算
- 目标帧率：60 FPS
- 缓存策略：LRU

### 3. 相机视图 (`/#/picture`)

**实时相机预览与拍照**

核心功能：
- ✅ Camera2 API集成
- ✅ 640x480分辨率捕获
- ✅ Canvas渲染预览
- ✅ 实时帧率显示
- ✅ 拍照功能（闪白效果）
- ✅ 画布尺寸自适应

技术细节：
- 图像格式：YUV_420_888 → JPEG → Base64
- 传输方式：JavaScript回调
- 渲染引擎：Canvas 2D
- 帧率统计：每秒更新

### 4. Canvas演示 (`/#/gcanvas`)

**Canvas 2D API 绘图示例**

演示内容：
- 渐变背景绘制
- 旋转矩形动画
- 多个圆形动画
- 文字渲染
- 清空画布操作

## 🚀 使用指南

### 在WebView中加载

**方式一：加载主页（推荐）**
```java
WebView webView = findViewById(R.id.webview);
webView.loadUrl("file:///android_asset/index.html");
```

**方式二：直接加载特定页面**
```java
// 列表视图
webView.loadUrl("file:///android_asset/index.html#/list");

// 相机视图
webView.loadUrl("file:///android_asset/index.html#/picture");

// Canvas演示
webView.loadUrl("file:///android_asset/index.html#/gcanvas");
```

### WebView配置要求

```java
WebSettings settings = webView.getSettings();

// 必需配置
settings.setJavaScriptEnabled(true);
settings.setDomStorageEnabled(true);
settings.setAllowFileAccess(true);

// 推荐配置
settings.setAllowFileAccessFromFileURLs(true);
settings.setAllowUniversalAccessFromFileURLs(true);
settings.setCacheMode(WebSettings.LOAD_DEFAULT);

// 性能优化
settings.setRenderPriority(WebSettings.RenderPriority.HIGH);
```

### 注入JavaScript接口

```java
// 相机功能API
webView.addJavascriptInterface(
    new WMCanvasCameraBridge(), 
    "WMCanvasCamera"
);
```

## 📡 WMCanvasCamera API

### Java端实现

```java
public class WMCanvasCameraBridge {
    @JavascriptInterface
    public String start(int width, int height) {
        // 启动相机
        return "{\"success\":true,\"message\":\"Camera starting\"}";
    }

    @JavascriptInterface
    public String stop() {
        // 停止相机
        return "{\"success\":true,\"message\":\"Camera stopped\"}";
    }

    @JavascriptInterface
    public void setFrameEnabled(boolean enabled) {
        // 启用/禁用帧传输
    }

    @JavascriptInterface
    public String takePicture() {
        // 拍照
        return "{\"success\":true,\"message\":\"Picture taken\"}";
    }

    @JavascriptInterface
    public String getCapabilities() {
        // 获取功能支持
        return "{\"camera\":true,\"canvas2d\":true}";
    }
}
```

### JavaScript端调用

```javascript
// 启动相机
const result = window.WMCanvasCamera.start(640, 480);
console.log(JSON.parse(result));

// 启用帧传输
window.WMCanvasCamera.setFrameEnabled(true);

// 拍照
const photo = window.WMCanvasCamera.takePicture();
console.log(JSON.parse(photo));

// 停止相机
window.WMCanvasCamera.stop();
```

### 相机帧回调

```javascript
// 全局回调函数接收Base64图像
window.updateCameraFrame = function(base64ImageData) {
    // base64ImageData: "data:image/jpeg;base64,..."
    const img = new Image();
    img.onload = () => {
        ctx.drawImage(img, 0, 0, canvas.width, canvas.height);
    };
    img.src = base64ImageData;
};
```

## 🔧 开发与构建

### 安装依赖
```bash
cd android/demo/src/main/assets
npm install -p pnpm@lastest
pnpm up rollup@lastest vite@latest
pnpm add -D @rollup/rollup-win32-x64-msvc
pnpm build
```

### 开发模式
```bash
pnpm dev
# 访问 http://localhost:5173
```

### 生产构建
```bash
pnpm build
# 输出到 dist/ 目录
```

### 构建配置
详见 `vite.config.js`：
- 单文件输出：`dist/app.js`
- CSS提取：`dist/assets/main.css`
- Hash禁用：便于WebView加载

## 📊 性能优化

### 列表视图优化
1. **虚拟滚动**：只渲染可见区域（~10项）
2. **Canvas池**：复用Canvas对象，避免频繁创建
3. **LRU缓存**：最多缓存50个Canvas
4. **GPU加速**：可选的`will-change: transform`
5. **平滑动画**：Tween.js缓动函数

### 相机视图优化
1. **图像压缩**：JPEG质量70%
2. **分辨率控制**：640x480平衡性能与质量
3. **帧率控制**：requestAnimationFrame节流
4. **异步处理**：Camera2 Handler线程

## 🐛 调试技巧

### Chrome DevTools调试
```java
// 启用WebView调试
if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
    WebView.setWebContentsDebuggingEnabled(true);
}
```

然后访问：`chrome://inspect`

### 日志输出
- Java端：`android.util.Log.d("WMCanvasCamera", message)`
- JavaScript端：`console.log()` → Logcat

### 常见问题

**相机无法启动**
- 检查相机权限：`Manifest.permission.CAMERA`
- 确认Bridge注入：`typeof WMCanvasCamera !== 'undefined'`

**帧率过低**
- 关闭GPU加速尝试
- 减少Canvas缓存数量
- 降低相机分辨率

**页面白屏**
- 检查`file:///`协议支持
- 确认`setAllowFileAccessFromFileURLs(true)`
- 查看Logcat错误日志

## 📝 更新日志

### v2.0.0 (2026-01-05)
- ✅ 重命名GCanvas → WMCanvas
- ✅ 简化wmcanvas-wrapper.js（仅检测）
- ✅ WMCanvasCamera直接由Java注入
- ✅ 移除无用的Canvas拦截代码
- ✅ 更新文档和API说明

### v1.0.0
- 初始版本
- Vue 3单页应用
- 三个演示视图
- Camera2集成

## 📚 相关文档

- [WebViewActivity.java](../../../java/com/honghu/wmcanvas/demo/WebViewActivity.java) - Android端实现
- [camera.vue](src/share/components/camera/camera.vue) - 相机组件
- [wmcanvas-wrapper.js](wmcanvas-wrapper.js) - API检测脚本
- [项目根目录README](../../../../../../README.MD) - 完整架构文档

## 📄 许可证

详见项目根目录 LICENSE 文件
