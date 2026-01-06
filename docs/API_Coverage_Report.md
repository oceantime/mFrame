# WmCanvas API覆盖度报告

生成时间: 2026-01-06
基于: core/src/wmcanvas/WmCanvas2dContext.h 和 core/src/webgl/WmWebGLRenderContext.h

## 一、Canvas 2D API 实现状态

### 1.1 已实现的核心API (✅)

#### 绘图API
- ✅ `fillRect(x, y, width, height)` - 填充矩形
- ✅ `strokeRect(x, y, width, height)` - 描边矩形
- ✅ `clearRect(x, y, width, height)` - 清空矩形区域
- ✅ `fill()` - 填充路径
- ✅ `stroke()` - 描边路径

#### 路径API
- ✅ `beginPath()` - 开始新路径
- ✅ `closePath()` - 闭合路径
- ✅ `moveTo(x, y)` - 移动到点
- ✅ `lineTo(x, y)` - 直线到点
- ✅ `arc(x, y, radius, startAngle, endAngle, counterclockwise)` - 绘制弧线
- ✅ `arcTo(x1, y1, x2, y2, radius)` - 绘制圆弧到指定点
- ✅ `quadraticCurveTo(cpx, cpy, x, y)` - 二次贝塞尔曲线
- ✅ `bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y)` - 三次贝塞尔曲线
- ✅ `rect(x, y, width, height)` - 矩形路径

#### 文本API
- ✅ `fillText(text, x, y, maxWidth)` - 填充文本
- ✅ `strokeText(text, x, y, maxWidth)` - 描边文本
- ⚠️ `measureText(text)` - 测量文本 (空实现，需补充)

#### 图像API
- ✅ `drawImage(image, dx, dy)` - 绘制图像
- ✅ `drawImage(image, dx, dy, dWidth, dHeight)` - 绘制缩放图像
- ✅ `drawImage(image, sx, sy, sWidth, sHeight, dx, dy, dWidth, dHeight)` - 绘制裁剪图像
- ✅ `createImageData(width, height)` - 创建图像数据
- ✅ `getImageData(x, y, width, height)` - 获取图像数据
- ✅ `putImageData(imageData, dx, dy)` - 写入图像数据

#### 变换API
- ✅ `translate(x, y)` - 平移
- ✅ `rotate(angle)` - 旋转
- ✅ `scale(x, y)` - 缩放
- ✅ `transform(a, b, c, d, e, f)` - 变换矩阵
- ✅ `setTransform(a, b, c, d, e, f)` - 设置变换矩阵
- ✅ `resetTransform()` - 重置变换

#### 状态API
- ✅ `save()` - 保存状态
- ✅ `restore()` - 恢复状态

#### 渐变和样式API
- ✅ `createLinearGradient(x0, y0, x1, y1)` - 创建线性渐变
- ✅ `createRadialGradient(x0, y0, r0, x1, y1, r1)` - 创建径向渐变
- ✅ `createPattern(image, repetition)` - 创建图案
- ✅ `addColorStop(offset, color)` - 添加渐变色标

#### 裁剪API
- ✅ `clip()` - 裁剪路径
- ✅ `resetClip()` - 重置裁剪 (WmCanvas扩展)

#### 属性
- ✅ `fillStyle` - 填充样式
- ✅ `strokeStyle` - 描边样式
- ✅ `lineWidth` - 线宽
- ✅ `lineCap` - 线端样式
- ✅ `lineJoin` - 线连接样式
- ✅ `miterLimit` - 斜接限制
- ✅ `globalAlpha` - 全局透明度
- ✅ `globalCompositeOperation` - 合成操作
- ✅ `font` - 字体样式
- ✅ `textAlign` - 文本对齐
- ✅ `textBaseline` - 文本基线

### 1.2 空实现的API (⚠️需补充)

- ⚠️ `measureText(text)` - 返回空对象，需要实现返回文本宽度
- ⚠️ `isPointInPath(x, y)` - 定义了但未实现
- ⚠️ `capture()` - 截图功能，定义了但未实现

### 1.3 未定义的API (❌标准API但未实现)

#### 阴影API
- ❌ `shadowColor` - 阴影颜色
- ❌ `shadowBlur` - 阴影模糊
- ❌ `shadowOffsetX` - 阴影X偏移
- ❌ `shadowOffsetY` - 阴影Y偏移

#### 其他
- ❌ `toDataURL(type, quality)` - 导出为Data URL
- ❌ `createEvent()` - 创建事件
- ❌ `setLineDash(segments)` - 设置虚线模式
- ❌ `getLineDash()` - 获取虚线模式
- ❌ `lineDashOffset` - 虚线偏移

### 1.4 WmCanvas扩展API (🌟非标准)

- 🌟 `loadTexture(src, id)` - 加载纹理
- 🌟 `unloadTexture(id)` - 卸载纹理
- 🌟 `render()` - 强制渲染
- 🌟 `resetClip()` - 重置裁剪区域
- 🌟 `BindImage(rgbaData, format, width, height)` - 绑定图像数据 (Android)
- 🌟 `SetDevicePixelRatio(ratio)` - 设置设备像素比
- 🌟 `SetCanvasDimension(w, h)` - 设置画布尺寸

---

## 二、WebGL API 实现状态

### 2.1 已实现的核心API (✅)

#### 上下文管理
- ✅ `viewport(x, y, width, height)` - 设置视口
- ✅ `clear(mask)` - 清空缓冲区
- ✅ `clearColor(r, g, b, a)` - 设置清空颜色
- ✅ `clearDepth(depth)` - 设置深度清空值
- ✅ `clearStencil(s)` - 设置模板清空值
- ✅ `flush()` - 刷新命令
- ⚠️ `finish()` - 等待完成 (未定义)

#### 缓冲区
- ✅ `createBuffer()` - 创建缓冲区
- ✅ `deleteBuffer(buffer)` - 删除缓冲区
- ✅ `bindBuffer(target, buffer)` - 绑定缓冲区
- ✅ `bufferData(target, data, usage)` - 缓冲区数据
- ✅ `bufferSubData(target, offset, data)` - 缓冲区子数据
- ✅ `getBufferParameter(target, pname)` - 获取缓冲区参数
- ✅ `isBuffer(buffer)` - 检查是否为缓冲区

#### 着色器程序
- ✅ `createShader(type)` - 创建着色器
- ✅ `deleteShader(shader)` - 删除着色器
- ✅ `shaderSource(shader, source)` - 设置着色器源码
- ✅ `compileShader(shader)` - 编译着色器
- ✅ `getShaderParameter(shader, pname)` - 获取着色器参数
- ✅ `getShaderInfoLog(shader)` - 获取着色器日志
- ✅ `getShaderSource(shader)` - 获取着色器源码
- ✅ `getShaderPrecisionFormat(shaderType, precisionType)` - 获取精度格式
- ✅ `isShader(shader)` - 检查是否为着色器

#### 程序对象
- ✅ `createProgram()` - 创建程序
- ✅ `deleteProgram(program)` - 删除程序
- ✅ `attachShader(program, shader)` - 附加着色器
- ✅ `detachShader(program, shader)` - 分离着色器
- ✅ `linkProgram(program)` - 链接程序
- ✅ `useProgram(program)` - 使用程序
- ✅ `validateProgram(program)` - 验证程序
- ✅ `getProgramParameter(program, pname)` - 获取程序参数
- ✅ `getProgramInfoLog(program)` - 获取程序日志
- ✅ `getAttachedShaders(program)` - 获取附加的着色器
- ✅ `isProgram(program)` - 检查是否为程序

#### 顶点属性
- ✅ `getAttribLocation(program, name)` - 获取属性位置
- ✅ `bindAttribLocation(program, index, name)` - 绑定属性位置
- ✅ `enableVertexAttribArray(index)` - 启用顶点属性数组
- ✅ `disableVertexAttribArray(index)` - 禁用顶点属性数组
- ✅ `vertexAttribPointer(index, size, type, normalized, stride, offset)` - 顶点属性指针
- ✅ `vertexAttrib2fv(index, values)` - 顶点属性值
- ✅ `getVertexAttrib(index, pname)` - 获取顶点属性
- ✅ `getVertexAttribOffset(index, pname)` - 获取顶点属性偏移
- ✅ `getActiveAttrib(program, index)` - 获取活动属性

#### Uniform变量
- ✅ `getUniformLocation(program, name)` - 获取uniform位置
- ✅ `getActiveUniform(program, index)` - 获取活动uniform
- ✅ `getUniform(program, location)` - 获取uniform值
- ✅ `uniform1f/uniform2f/uniform3f/uniform4f` - 设置float uniform
- ✅ `uniform1i/uniform2i/uniform3i/uniform4i` - 设置int uniform
- ✅ `uniform1fv/uniform2fv/uniform3fv/uniform4fv` - 设置float数组uniform
- ✅ `uniform1iv/uniform2iv/uniform3iv/uniform4iv` - 设置int数组uniform
- ✅ `uniformMatrix2fv/uniformMatrix3fv/uniformMatrix4fv` - 设置矩阵uniform

#### 纹理
- ✅ `createTexture()` - 创建纹理
- ✅ `deleteTexture(texture)` - 删除纹理
- ✅ `bindTexture(target, texture)` - 绑定纹理
- ✅ `activeTexture(texture)` - 激活纹理单元
- ✅ `texImage2D(...)` - 设置纹理图像
- ✅ `texSubImage2D(...)` - 更新纹理图像
- ✅ `texParameteri(target, pname, param)` - 设置纹理参数(整数)
- ✅ `texParameterf(target, pname, param)` - 设置纹理参数(浮点)
- ✅ `generateMipmap(target)` - 生成Mipmap
- ✅ `copyTexImage2D(...)` - 从帧缓冲复制到纹理
- ✅ `copyTexSubImage2D(...)` - 从帧缓冲复制到纹理子区域
- ✅ `compressedTexImage2D(...)` - 压缩纹理图像
- ✅ `compressedTexSubImage2D(...)` - 压缩纹理子图像
- ✅ `getTexParameter(target, pname)` - 获取纹理参数
- ✅ `isTexture(texture)` - 检查是否为纹理
- ✅ `pixelStorei(pname, param)` - 像素存储参数

#### 帧缓冲
- ✅ `createFramebuffer()` - 创建帧缓冲
- ✅ `deleteFramebuffer(framebuffer)` - 删除帧缓冲
- ✅ `bindFramebuffer(target, framebuffer)` - 绑定帧缓冲
- ✅ `framebufferTexture2D(...)` - 附加纹理到帧缓冲
- ✅ `framebufferRenderbuffer(...)` - 附加渲染缓冲到帧缓冲
- ✅ `checkFramebufferStatus(target)` - 检查帧缓冲状态
- ✅ `getFramebufferAttachmentParameter(...)` - 获取帧缓冲附件参数
- ✅ `isFramebuffer(framebuffer)` - 检查是否为帧缓冲

#### 渲染缓冲
- ✅ `createRenderbuffer()` - 创建渲染缓冲
- ✅ `deleteRenderbuffer(renderbuffer)` - 删除渲染缓冲
- ✅ `bindRenderbuffer(target, renderbuffer)` - 绑定渲染缓冲
- ✅ `renderbufferStorage(target, internalformat, width, height)` - 渲染缓冲存储
- ✅ `getRenderbufferParameter(target, pname)` - 获取渲染缓冲参数
- ✅ `isRenderbuffer(renderbuffer)` - 检查是否为渲染缓冲

#### 绘制命令
- ✅ `drawArrays(mode, first, count)` - 绘制数组
- ✅ `drawArraysInstancedANGLE(mode, first, count, primcount)` - 实例化绘制数组
- ✅ `drawElements(mode, count, type, offset)` - 绘制元素

#### 状态管理
- ✅ `enable(cap)` - 启用功能
- ✅ `disable(cap)` - 禁用功能
- ✅ `isEnabled(cap)` - 检查是否启用
- ✅ `depthFunc(func)` - 深度测试函数
- ✅ `depthMask(flag)` - 深度写入掩码
- ✅ `depthRange(zNear, zFar)` - 深度范围
- ✅ `cullFace(mode)` - 剔除面
- ✅ `frontFace(mode)` - 正面方向
- ✅ `lineWidth(width)` - 线宽
- ✅ `polygonOffset(factor, units)` - 多边形偏移
- ✅ `scissor(x, y, width, height)` - 裁剪矩形
- ✅ `colorMask(red, green, blue, alpha)` - 颜色写入掩码

#### 混合
- ✅ `blendColor(red, green, blue, alpha)` - 混合颜色
- ✅ `blendEquation(mode)` - 混合方程
- ✅ `blendEquationSeparate(modeRGB, modeAlpha)` - 分离混合方程
- ✅ `blendFunc(sfactor, dfactor)` - 混合函数
- ✅ `blendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha)` - 分离混合函数

#### 模板
- ✅ `stencilFunc(func, ref, mask)` - 模板测试函数
- ✅ `stencilFuncSeparate(face, func, ref, mask)` - 分离模板测试函数
- ✅ `stencilMask(mask)` - 模板写入掩码
- ✅ `stencilMaskSeparate(face, mask)` - 分离模板写入掩码
- ✅ `stencilOp(fail, zfail, zpass)` - 模板操作
- ✅ `stencilOpSeparate(face, fail, zfail, zpass)` - 分离模板操作

#### 查询和读取
- ✅ `getParameter(pname)` - 获取参数
- ✅ `getError()` - 获取错误
- ✅ `readPixels(x, y, width, height, format, type, pixels)` - 读取像素
- ✅ `sampleCoverage(value, invert)` - 采样覆盖

#### 扩展
- ✅ `getSupportedExtensions()` - 获取支持的扩展
- ✅ `getExtension(name)` - 获取扩展

### 2.2 空实现的API (⚠️需补充)

- ⚠️ `isContextLost()` - 检查上下文是否丢失 (定义了但为空实现)
- ⚠️ `getContextAttributes()` - 获取上下文属性 (定义了但为空实现)

### 2.3 未定义的API (❌标准API但未实现)

- ❌ `commit()` - 提交渲染 (OffscreenCanvas API)
- ❌ `finish()` - 等待GPU完成
- ❌ `hint(target, mode)` - 性能提示

---

## 三、API覆盖率统计

### Canvas 2D API
- **已实现**: 70个API ✅
- **空实现**: 3个API ⚠️ (measureText, isPointInPath, capture)
- **未定义**: 10个API ❌ (shadow系列, toDataURL等)
- **WmCanvas扩展**: 7个API 🌟
- **覆盖率**: 约87.5% (70/80)

### WebGL API
- **已实现**: 150+个API ✅
- **空实现**: 2个API ⚠️ (isContextLost, getContextAttributes)
- **未定义**: 3个API ❌ (commit, finish, hint)
- **覆盖率**: 约98.7% (150/152)

---

## 四、优先级建议

### 高优先级 (影响基本功能)
1. ⚠️ **measureText()** - 文本测量，布局必需
2. ⚠️ **isContextLost()** - WebGL上下文恢复必需
3. ❌ **shadowColor/shadowBlur/shadowOffsetX/Y** - 阴影效果常用

### 中优先级 (增强功能)
4. ⚠️ **isPointInPath()** - 点击检测
5. ⚠️ **getContextAttributes()** - 调试和诊断
6. ❌ **setLineDash/getLineDash** - 虚线绘制
7. ⚠️ **capture()** - 截图功能

### 低优先级 (可选功能)
8. ❌ **toDataURL()** - 导出图像
9. ❌ **finish()** - GPU同步
10. ❌ **hint()** - 性能优化提示

---

## 五、测试覆盖建议

### 已创建的测试
- ✅ fillRect.js
- ✅ strokeRect.js
- ✅ arc.js
- ✅ text.js (fillText/strokeText)
- ✅ transforms.js (translate/rotate/scale)

### 待创建的测试
- [ ] gradients.js - 渐变测试 (线性/径向)
- [ ] patterns.js - 图案测试
- [ ] paths.js - 复杂路径测试 (bezierCurveTo/quadraticCurveTo)
- [ ] images.js - 图像绘制测试 (drawImage各种重载)
- [ ] imageData.js - 像素操作测试
- [ ] compositing.js - 合成模式测试
- [ ] clipping.js - 裁剪测试
- [ ] state.js - save/restore状态测试
- [ ] webgl-triangle.js - WebGL三角形
- [ ] webgl-cube.js - WebGL立方体
- [ ] webgl-texture.js - WebGL纹理

---

生成时间: 2026-01-06
作者: WmCanvas Team
版本: v1.0
