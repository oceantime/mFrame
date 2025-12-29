# [GCanvas](https://alibaba.github.io/GCanvas/)

GCanvas is a cross-platform rendering engine for mobile devices developed by Alibaba. It is written with C++ based on OpenGL ES, so it can provide high performance 2D/WebGL rendering capabilities for JavaScript runtime. It also  has browser-like canvas APIs, so it's very convenient and flexiable for use, especially for web developers.

Supported operating systems are Android 4.0+ (API 14) and iOS 8.0+.

- [Getting Started](#getting-started)
- [Documentation](#documentation)
- [Examples](#examples)
- [Built With](#build-with)
- [Opening Issues](#opening-issues)
- [Contributing](#contributing)
- [License](#license)

## Distribution
###  GCanvas

* iOS  [GCanvas](https://cocoapods.org/pods/GCanvas)   [![CocoaPods Version](https://badge.fury.io/co/GCanvas.svg)](https://badge.fury.io/co/GCanvas)

* Android Maven Artifacts:
  ```groovy
  // Core library
  implementation "com.taobao.gcanvas:corelib:1.2.0"
  
  // Image adapter
  implementation "com.taobao.gcanvas.adapters:img_fresco:1.1.0"
  
  // Bridge specification
  implementation "com.taobao.gcanvas.bridges:spec:1.1.0"
  
  // Weex bridge (if needed)
  implementation "com.taobao.gcanvas.bridges:weex:1.1.0"
  ```

### GCanvas NPM Package
* [@gcanvas/core](https://www.npmjs.com/package/@gcanvas/core)   [![npm version](https://badge.fury.io/js/%40gcanvas%2Fcore.svg)](https://www.npmjs.com/package/@gcanvas/core)

## Features
- Cross-platform, support popular iOS and Android.
- High performance, accelerate graphic draw by OpenGL ES.
- Provide JavaScript runtime,  such as [Weex](https://github.com/apache/incubator-weex) and [ReactNative](https://github.com/facebook/react-native/).  convenient to use JavaScript API like HTML canvas.
- Scalable Architecture, easy to implement a GCanvas bridge by yourself following the guide [Custom Native Bridge](https://alibaba.github.io/GCanvas/docs/Custom%20Bridge.html) .
- Small size.

## Introduction

See the  [Introduction to GCanvas](https://alibaba.github.io/GCanvas/docs/Developer's%20Guide.html) for a detailed  introduction to GCanvas.

## Getting Started

We will continue to maintain the C++ `core` engine. And will no further update and support `Weex`、 `ReactNative` and `JS bridge`.

### Android Build

#### Build Commands
```bash
# Build Ali flavor (includes all modules with Weex support)
.\gradlew.bat assembleAliRelease

# Build Standalone flavor (without Weex support)
.\gradlew.bat assembleStandaloneRelease
```

#### Output Artifacts
After successful compilation, the following AAR packages will be generated:

| Package Name | Maven Coordinates | Output Path | Description |
|-------------|-------------------|-------------|-------------|
| corelib | `com.taobao.gcanvas:corelib:1.2.0` | `android/gcanvas_library/build/outputs/aar/gcanvas_library-ali-release.aar` | Core GCanvas library with native rendering engine |
| img_fresco | `com.taobao.gcanvas.adapters:img_fresco:1.1.0` | `android/adapters/bridge_adapter/build/outputs/aar/bridge_adapter-release.aar` | Image loading adapter interface |
| spec | `com.taobao.gcanvas.bridges:spec:1.1.0` | `android/bridge_spec/build/outputs/aar/bridge_spec-release.aar` | Bridge specification for custom implementations |

**Note:** The AAR files contain native libraries for both `armeabi-v7a` and `arm64-v8a` architectures.

### Node
Follow [Node Guide](./node/README.md),  use GCanvas in Node.js.  

### JavaScript
Try our [Playground](https://alibaba.github.io/GCanvas/playground.html).
GCanvas has browser-like canvas APIs, so almost all of the APIs are exactly same as HTML5 canvas. At this moment, we have already supported 90% of 2D APIs and 99% of WebGL APIs. You can find out those informations in [2D  APIs](https://alibaba.github.io/GCanvas/docs/Graphics%202D.html) and [WebGL APIs](https://alibaba.github.io/GCanvas/docs/WebGL.html).

## Documentation
Check [Documentation](https://alibaba.github.io/GCanvas/docs/Introduction.html) for more information.

## Examples
We take [Weex](https://github.com/apache/incubator-weex) as example, code snippet of context 2d using `GCanvas`.
```javascript
import { enable, WeexBridge, Image as GImage } from "@gcanvas/core";

var gcanvas = enable(this.$refs.canvas_holder, {bridge: WeexBridge});
var ctx = gcanvas.getContext("2d");
//rect
ctx.fillStyle = 'red';
ctx.fillRect(0, 0, 100, 100);

//rect
ctx.fillStyle = 'black';
ctx.fillRect(100, 100, 100, 100);
ctx.fillRect(25, 210, 700, 5);

//circle
ctx.arc(450, 200, 100, 0, Math.PI * 2, true);
ctx.fill();

var image = new GImage();
image.src = 'https://gw.alicdn.com/tfs/TB1KwRTlh6I8KJjy0FgXXXXzVXa-225-75.png';
image.onload = function(){
  ctx.drawImage(image, 100, 300);
};
```

## Built With

* [Freetype](https://www.freetype.org/) - Used for font rendering on Android


## Changelog

New Changelog record in [CHANGELOG](./docs/Changelog.md) for details.

## Open Issues
If you encounter a bug with GCanvas we would like to hear about it. Search the [existing issues]() and try to make sure your problem doesn’t already exist before opening a new issue. It’s helpful if you include the version of GCanvas  and OS you’re using. Please include a stack trace and reduced repro case when appropriate, too.

## Contributing

Please read [CONTRIBUTING](./docs/Contributing.md) for details on our code of conduct, and the process for submitting pull requests to us.

## Authors

GCanvas Open Source Team

## License

This project is licensed under the Apache [LICENSE](./LICENSE)
