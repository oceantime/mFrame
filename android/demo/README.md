# WmCanvas Android Demo

这是一个展示如何在Android WebView中使用GCanvas的示例项目。

## ⚠️ 构建说明

由于项目使用的Android Gradle Plugin 3.3.0与较新版本的工具链存在兼容性问题，demo模块可能无法直接编译。但所有示例代码都已完整提供，可以作为参考集成到您自己的项目中。

## 项目结构

```
android/demo/
├── src/main/
│   ├── java/com/taobao/gcanvas/demo/
│   │   ├── MainActivity.java          # 主界面Activity
│   │   └── WebViewActivity.java       # WebView界面，加载HTML5页面
│   ├── res/
│   │   ├── layout/
│   │   │   ├── activity_main.xml      # 主界面布局
│   │   │   └── activity_webview.xml   # WebView界面布局
│   │   └── values/
│   │       ├── strings.xml
│   │       └── styles.xml
│   ├── assets/
│   │   └── gcanvas_demo.html          # Canvas演示HTML页面
│   └── AndroidManifest.xml
└── build.gradle
```

## 功能特性

### 1. WebView配置
- ✅ 启用JavaScript支持
- ✅ 支持缩放和手势操作
- ✅ 自适应屏幕显示
- ✅ 支持本地文件访问
- ✅ 启用DOM存储
- ✅ 硬件加速渲染

### 2. JavaScript Bridge
提供Android与JavaScript的双向通信：

**JavaScript调用Android：**
```javascript
// 显示Toast消息
AndroidBridge.showToast("Hello from JS!");

// 获取设备信息
const deviceInfo = AndroidBridge.getDeviceInfo();
```

**Android调用JavaScript：**
```java
mWebView.evaluateJavascript("yourJsFunction()", null);
```

### 3. Canvas演示功能
HTML5页面提供以下Canvas绘制示例：
- 🎨 绘制渐变矩形
- ⭕ 绘制多个圆形
- 🔄 旋转动画效果
- 🧹 清空画布
- 🌉 测试Bridge通信

## 集成到您的项目

### 步骤1：添加依赖

在您的app模块的`build.gradle`中添加：

```groovy
android {
    // ... 其他配置
    
    compileOptions {
        sourceCompatibility JavaVersion.VERSION_1_8
        targetCompatibility JavaVersion.VERSION_1_8
    }
    
    // 如果使用了gcanvas_library的flavor，需要匹配
    flavorDimensions "target_app"
    productFlavors {
        ali {
            dimension "target_app"
        }
    }
}

dependencies {
    implementation project(':android:gcanvas_library')
    implementation 'com.android.support:appcompat-v7:28.0.0'
}
```

### 步骤2：复制源代码

1. 复制 `WebViewActivity.java` 到您的项目
2. 复制 `gcanvas_demo.html` 到 `assets` 目录
3. 复制布局文件 `activity_webview.xml`
4. 在 `AndroidManifest.xml` 中添加权限和Activity声明

### 步骤3：配置AndroidManifest.xml

```xml
<uses-permission android:name="android.permission.INTERNET" />
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />

<application>
    <activity 
        android:name=".WebViewActivity"
        android:label="WmCanvas WebView Demo" />
</application>
```

##使用说明

### 加载本地HTML
```java
mWebView.loadUrl("file:///android_asset/gcanvas_demo.html");
```

### 加载网络URL
```java
mWebView.loadUrl("https://your-domain.com/page.html");
```

### 自定义JavaScript接口
在 `WebViewActivity.java` 中添加更多接口方法：

```java
public class JsInterface {
    @JavascriptInterface
    public void yourMethod(String param) {
        // 处理JavaScript调用
    }
}
```

在HTML中调用：
```javascript
AndroidBridge.yourMethod("参数");
```

## 技术要点

### WebView安全配置
```java
// 启用文件访问（开发阶段）
webSettings.setAllowFileAccess(true);
webSettings.setAllowFileAccessFromFileURLs(true);
webSettings.setAllowUniversalAccessFromFileURLs(true);
```

**注意：** 生产环境中应该谨慎使用文件访问权限。

### Canvas性能优化
- 使用硬件加速：`setRenderPriority(RenderPriority.HIGH)`
- 启用缓存：`setCacheMode(WebSettings.LOAD_DEFAULT)`
- 合理使用`requestAnimationFrame`而非`setInterval`

### 内存管理
在Activity销毁时正确清理WebView：
```java
@Override
protected void onDestroy() {
    if (mWebView != null) {
        mWebView.loadDataWithBaseURL(null, "", "text/html", "utf-8", null);
        mWebView.clearHistory();
        mWebView.destroy();
        mWebView = null;
    }
    super.onDestroy();
}
```

## HTML5 Canvas示例

`gcanvas_demo.html` 提供了完整的Canvas使用示例，包括：

1. **基础绘制**：矩形、圆形、渐变
2. **动画效果**：使用 requestAnimationFrame 实现平滑动画
3. **Bridge通信**：JavaScript与Android的双向调用示例

核心代码片段：

```javascript
const canvas = document.getElementById('myCanvas');
const ctx = canvas.getContext('2d');

// 绘制渐变矩形
const gradient = ctx.createLinearGradient(0, 0, 400, 0);
gradient.addColorStop(0, '#FF6B6B');
gradient.addColorStop(1, '#4ECDC4');
ctx.fillStyle = gradient;
ctx.fillRect(50, 50, 300, 200);

// 动画
function animate() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    // ... 绘制代码
    requestAnimationFrame(animate);
}
```

## 常见问题

### Q: WebView显示空白页面
**A:** 检查以下几点：
1. 是否启用了JavaScript：`setJavaScriptEnabled(true)`
2. 网络权限是否配置：`INTERNET`权限
3. 文件路径是否正确
4. 查看Logcat日志排查错误

### Q: JavaScript调用Android方法无响应
**A:** 确认：
1. 是否添加了`@JavascriptInterface`注解
2. 是否调用了`addJavascriptInterface()`
3. JavaScript中对象名称是否匹配

### Q: Canvas动画卡顿
**A:** 优化建议：
1. 使用硬件加速
2. 减少绘制复杂度
3. 使用`requestAnimationFrame`控制帧率
4. 避免在动画中进行大量计算

## 参考资料

- [Android WebView官方文档](https://developer.android.com/reference/android/webkit/WebView)
- [HTML5 Canvas API](https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API)
- [GCanvas项目文档](https://alibaba.github.io/WmCanvas/)
- [参考博客](https://blog.csdn.net/qq_33718648/article/details/82286559)

## 许可证

遵循GCanvas项目的Apache License 2.0许可证。
