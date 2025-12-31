# Canvas 演示应用 - 使用说明

## 📁 目录结构

```
third_party/GCanvasMaster/android/demo/src/main/assets/
├── index.html              # 唯一入口 - 加载Vue单页应用
├── wmcanvas-wrapper.js     # 统一封装（Canvas拦截 + 相机API）
├── package.json            # Vue项目依赖
├── vite.config.js          # Vite构建配置
├── CODE_CLEANUP_REPORT.md  # 代码整合报告
├── dist/                   # Vue应用构建输出
│   ├── app.js             # 主应用JS
│   └── assets/
│       └── main.css       # 样式文件
└── src/                    # Vue应用源码
    ├── main.js            # Vue应用入口
    ├── App.vue            # 根组件（含导航栏）
    ├── router/            # 路由配置
    │   └── index.js
    ├── views/             # 页面组件
    │   ├── list/          # 列表演示
    │   ├── picture/       # 相机演示
    │   └── gcanvas/       # GCanvas演示
    └── share/             # 共享组件
        └── components/
            ├── scroll/    # 滚动组件
            └── camera/    # 相机组件
```

## 🎯 功能说明

### 1. 单页应用架构
**入口**: `index.html`

Vue单页应用，包含顶部导航栏和三个主要视图：

- **📋 列表视图** (`/list`): Canvas长列表优化演示
- **📷 相机视图** (`/picture`): 实时相机预览和拍照
- **🎯 GCanvas视图** (`/gcanvas`): Canvas绘图功能演示

### 2. 列表视图功能
**路由**: `/#/list`

功能特性：
- ✅ Canvas长列表虚拟滚动
- ✅ GPU加速开关
- ✅ Canvas池缓存优化
- ✅ Tween平滑滚动动画
- ✅ FPS性能监控
- ✅ 缓存命中率统计

### 3. 相机视图功能
**路由**: `/#/picture`

功能特性：
- ✅ 实时相机预览（Camera2 API）
- ✅ 640x480分辨率捕获
- ✅ 拍照功能
- ✅ 帧率显示
- ✅ Canvas渲染预览

### 4. GCanvas视图功能
**路由**: `/#/gcanvas`

Canvas绘图API演示：
- 绘制渐变矩形
- 绘制多个圆形
- 旋转动画效果
- 清空画布
- Android Bridge测试

## 🚀 使用方式

### 在Android应用中加载

**唯一入口（推荐）**
```java
webView.loadUrl("file:///android_asset/index.html");
```
这将加载Vue单页应用，用户可通过导航栏切换不同视图。

### 直接访问特定路由
```java
// 列表视图
webView.loadUrl("file:///android_asset/index.html#/list");

// 相机视图
webView.loadUrl("file:///android_asset/index.html#/picture");

// GCanvas演示
webView.loadUrl("file:///android_asset/index.html#/gcanvas");
```

### WMCanvas API Bridge

确保在Android端实现以下Bridge：

```java
webView.addJavascriptInterface(new WMCanvasBridge(), "WMCanvasAndroid");
webView.addJavascriptInterface(new AndroidBridge(), "AndroidBridge");
```

## 🔧 WMCanvas统一封装工作原理

**文件**: `wmcanvas-wrapper.js`

### 第一部分：Canvas 2D API拦截器

自动拦截Canvas 2D API调用（预留架构）：

1. **方法拦截**: 
   - 绘图方法：`fillRect`, `strokeRect`, `arc`, `fill`, `stroke` 等
   - 变换方法：`translate`, `rotate`, `scale` 等
   - 状态方法：`save`, `restore` 等

2. **属性拦截**:
   - 样式属性：`fillStyle`, `strokeStyle`, `lineWidth` 等
   - 文本属性：`font`, `textAlign`, `textBaseline` 等
   - 合成属性：`globalAlpha`, `globalCompositeOperation` 等

3. **命令缓冲**:
   所有Canvas操作会被记录到 `window.WMCanvasCommands` 数组中，
   供Native层批量处理（当前预留功能）。

4. **双模式运行**:
   - **有WMCanvas**: 同时调用原生Canvas API和记录命令
   - **无WMCanvas**: 只调用原生Canvas API（兼容模式）

### 第二部分：相机API封装器

将WMCanvasAndroid Bridge同步方法包装为Promise API：

```javascript
// 启动相机
await window.WMCanvasCamera.start(width, height);

// 设置帧传输
window.WMCanvasCamera.setFrameEnabled(true);

// 拍照
const result = await window.WMCanvasCamera.takePicture();

// 停止相机
await window.WMCanvasCamera.stop();

// 获取功能支持
const caps = window.WMCanvasCamera.getCapabilities();
```

## 📊 性能优化特性

### Canvas池缓存
- 复用离屏Canvas，避免重复创建
- LRU策略控制缓存数量
- 缓存命中率实时监控

### 虚拟滚动
- 只渲染可见区域
- 动态计算渲染范围
- 减少DOM操作

### GPU加速
- 可选的GPU硬件加速
- 适合大量Canvas元素
- 可能增加内存消耗

### 平滑动画
- Tween.js动画库
- 缓动函数优化体验
- 60fps流畅滚动

## 🐛 调试信息

所有模式都会在控制台输出详细日志：

```javascript
// Vue应用初始化
console.log('=== 开始初始化Vue应用 ===')

// WMCanvas检测
console.log('✅ WMCanvas Android Bridge detected')

// Android Bridge检测
console.log('✅ Android Bridge detected')
console.log('📱 Device Info:', deviceInfo)
```

## 📝 开发与构建

### 修改Vue应用

1. 编辑 `src/` 目录下的源文件
2. 重新构建：
   ```bash
   cd third_party/GCanvasMaster/android/demo/src/main/assets
   npm install  # 首次需要安装依赖
   npm run build
   ```
3. 构建输出会更新到 `dist/` 目录

### Vite配置

**文件**: `vite.config.js`
- Vue 3 + Vue Router
- 相对路径构建（适配Android assets）
- 开发服务器配置

## ⚠️ 注意事项

1. **文件访问权限**
   - 确保WebView启用文件访问：`setAllowFileAccess(true)`
   - 确保允许访问文件URL：`setAllowFileAccessFromFileURLs(true)`

2. **JavaScript启用**
   - 必须启用JavaScript：`setJavaScriptEnabled(true)`
   - 必须启用DOM Storage：`setDomStorageEnabled(true)`

3. **网络请求**
   - Vue应用包含API请求功能
   - 失败时会自动降级到Mock数据
   - 需要网络权限和CORS配置

4. **内存管理**
   - Canvas池有上限（默认50个）
   - GPU模式可能增加内存
   - 监控性能指标调整参数

## 🔗 相关文件

- 原始app资源: `app/src/main/assets/`
- 构建配置: `app/src/main/assets/package.json`
- Vue源码: `app/src/main/assets/src/`
- 列表组件: `app/src/main/assets/src/views/list/list.vue`
- 优化文档: `app/src/main/assets/src/views/list/POC_优化方案.md`

## 📄 许可证

遵循 WmCanvas 项目的原始许可证。
