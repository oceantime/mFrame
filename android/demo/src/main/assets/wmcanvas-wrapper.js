/**
 * WMCanvas Wrapper - JavaScript 端统一封装
 * 
 * 功能：
 * 1. Canvas 2D API 拦截器 - 捕获Canvas调用并记录到命令缓冲区
 * 2. 相机 API 封装器 - 将WMCanvasAndroid Bridge包装为Promise API
 * 
 * 当前状态：
 * - Canvas拦截：预留架构，用于未来的Canvas原生加速实现
 * - 相机API：正在使用，提供window.WMCanvasCamera接口
 * 
 * 未来可能的用途：
 * - 将Canvas 2D绘制命令发送到原生层进行硬件加速
 * - 实现离屏渲染和批量绘制优化
 */

// 全局命令缓冲区
window.WMCanvasCommands = [];

// 保存原始的 Canvas 方法
const OriginalHTMLCanvasElement = HTMLCanvasElement;
const originalGetContext = HTMLCanvasElement.prototype.getContext;

// 重写 getContext 方法
HTMLCanvasElement.prototype.getContext = function(contextType, contextAttributes) {
    const context = originalGetContext.call(this, contextType, contextAttributes);
    
    // 如果 Android Bridge 可用且是 2D context，包装它以启用 WMCanvas 加速
    if (contextType === '2d' && typeof WMCanvasAndroid !== 'undefined') {
        console.log('Wrapping Canvas context with WMCanvas acceleration');
        return wrapCanvas2DContext(context, this);
    }
    
    return context;
};

/**
 * 包装 Canvas 2D Context
 */
function wrapCanvas2DContext(ctx, canvas) {
    const canvasId = canvas.id || 'canvas_' + Date.now();
    
    // 需要拦截的绘图方法
    const drawingMethods = [
        'fillRect', 'strokeRect', 'clearRect',
        'fillText', 'strokeText',
        'beginPath', 'closePath', 'moveTo', 'lineTo',
        'bezierCurveTo', 'quadraticCurveTo',
        'arc', 'arcTo', 'ellipse', 'rect',
        'fill', 'stroke',
        'save', 'restore',
        'scale', 'rotate', 'translate', 'transform', 'setTransform',
        'clip', 'drawImage'
    ];
    
    // 需要拦截的属性
    const properties = [
        'fillStyle', 'strokeStyle', 'lineWidth', 'lineCap', 'lineJoin',
        'miterLimit', 'shadowBlur', 'shadowColor', 'shadowOffsetX', 'shadowOffsetY',
        'font', 'textAlign', 'textBaseline', 'globalAlpha', 'globalCompositeOperation'
    ];
    
    // 包装绘图方法
    drawingMethods.forEach(method => {
        const original = ctx[method];
        if (typeof original === 'function') {
            ctx[method] = function(...args) {
                // 记录命令
                window.WMCanvasCommands.push({
                    method: method,
                    args: args
                });
                
                // 调用原始方法
                return original.apply(ctx, args);
            };
        }
    });
    
    // 包装属性
    properties.forEach(prop => {
        // 保存原始属性描述符
        const descriptor = Object.getOwnPropertyDescriptor(Object.getPrototypeOf(ctx), prop) ||
                          Object.getOwnPropertyDescriptor(ctx, prop);
        
        if (!descriptor) return;
        
        // 保存原始的 getter 和 setter
        const originalGetter = descriptor.get;
        const originalSetter = descriptor.set;
        
        Object.defineProperty(ctx, prop, {
            get: function() {
                if (originalGetter) {
                    return originalGetter.call(this);
                }
                return this['_WMCanvas_' + prop];
            },
            set: function(newValue) {
                // 调用原始 setter 来更新实际的 Canvas 状态
                if (originalSetter) {
                    originalSetter.call(this, newValue);
                } else {
                    this['_WMCanvas_' + prop] = newValue;
                }
                
                // 记录属性变化到 WMCanvas
                window.WMCanvasCommands.push({
                    method: 'setProperty',
                    property: prop,
                    value: newValue
                });
            },
            configurable: true
        });
    });
    
    return ctx;
}

/**
 * WMCanvas Context 类 (备用实现)
 */
class WMCanvasContext {
    constructor(canvasId) {
        this.canvasId = canvasId;
        this.commands = [];
        
        // 绘图状态
        this.fillStyle = '#000000';
        this.strokeStyle = '#000000';
        this.lineWidth = 1;
        this.font = '10px sans-serif';
        this.textAlign = 'start';
        this.textBaseline = 'alphabetic';
    }
    
    // 矩形绘制
    fillRect(x, y, width, height) {
        this.commands.push(['fillRect', x, y, width, height]);
        this.flush();
    }
    
    strokeRect(x, y, width, height) {
        this.commands.push(['strokeRect', x, y, width, height]);
        this.flush();
    }
    
    clearRect(x, y, width, height) {
        this.commands.push(['clearRect', x, y, width, height]);
        this.flush();
    }
    
    // 路径
    beginPath() {
        this.commands.push(['beginPath']);
    }
    
    closePath() {
        this.commands.push(['closePath']);
    }
    
    moveTo(x, y) {
        this.commands.push(['moveTo', x, y]);
    }
    
    lineTo(x, y) {
        this.commands.push(['lineTo', x, y]);
    }
    
    arc(x, y, radius, startAngle, endAngle, anticlockwise) {
        this.commands.push(['arc', x, y, radius, startAngle, endAngle, anticlockwise || false]);
    }
    
    fill() {
        this.commands.push(['fill']);
        this.flush();
    }
    
    stroke() {
        this.commands.push(['stroke']);
        this.flush();
    }
    
    // 文字
    fillText(text, x, y, maxWidth) {
        this.commands.push(['fillText', text, x, y, maxWidth]);
        this.flush();
    }
    
    strokeText(text, x, y, maxWidth) {
        this.commands.push(['strokeText', text, x, y, maxWidth]);
        this.flush();
    }
    
    // 变换
    save() {
        this.commands.push(['save']);
    }
    
    restore() {
        this.commands.push(['restore']);
    }
    
    scale(x, y) {
        this.commands.push(['scale', x, y]);
    }
    
    rotate(angle) {
        this.commands.push(['rotate', angle]);
    }
    
    translate(x, y) {
        this.commands.push(['translate', x, y]);
    }
    
    // 刷新命令到 Native
    flush() {
        if (typeof WMCanvasAndroid !== 'undefined' && this.commands.length > 0) {
            const commandsJson = JSON.stringify(this.commands);
            WMCanvasAndroid.render(this.canvasId, commandsJson);
            this.commands = [];
        }
    }
}

// 导出模块
if (typeof module !== 'undefined' && module.exports) {
    module.exports = { WMCanvasContext };
}

console.log('✅ WMCanvas wrapper loaded - Canvas 2D API interception enabled');

// ==================== 相机 API 封装 ====================

/**
 * WMCanvas Camera Wrapper
 * 将Android同步Bridge API包装为异步Promise API
 */
(function() {
    'use strict';
    
    // 检查是否有Android Bridge
    if (typeof WMCanvasAndroid === 'undefined') {
        console.log('⚠️ WMCanvasAndroid Bridge not available, camera features disabled');
        return;
    }
    
    console.log('✅ WMCanvasAndroid Bridge detected, wrapping camera APIs');
    
    // 创建Camera API包装器
    window.WMCanvasCamera = {
        /**
         * 启动相机
         * @param {number} width - 宽度
         * @param {number} height - 高度
         * @returns {Promise<Object>}
         */
        start: function(width, height) {
            return new Promise((resolve, reject) => {
                try {
                    const result = WMCanvasAndroid.start(width, height);
                    console.log('📹 Camera start result:', result);
                    
                    // 尝试解析JSON响应
                    try {
                        const response = JSON.parse(result);
                        if (response.success) {
                            resolve(response);
                        } else {
                            reject(new Error(response.message || 'Camera start failed'));
                        }
                    } catch (e) {
                        // 如果不是JSON，假设成功
                        console.log('⚠️ Camera start returned non-JSON, assuming success');
                        resolve({ success: true, message: result });
                    }
                } catch (error) {
                    console.error('❌ Camera start error:', error);
                    reject(error);
                }
            });
        },
        
        /**
         * 停止相机
         * @returns {Promise<Object>}
         */
        stop: function() {
            return new Promise((resolve, reject) => {
                try {
                    const result = WMCanvasAndroid.stop();
                    console.log('🛑 Camera stop result:', result);
                    
                    try {
                        const response = JSON.parse(result);
                        if (response.success) {
                            resolve(response);
                        } else {
                            reject(new Error(response.message || 'Camera stop failed'));
                        }
                    } catch (e) {
                        console.log('⚠️ Camera stop returned non-JSON, assuming success');
                        resolve({ success: true, message: result });
                    }
                } catch (error) {
                    console.error('❌ Camera stop error:', error);
                    reject(error);
                }
            });
        },
        
        /**
         * 设置帧传输开关
         * @param {boolean} enabled
         */
        setFrameEnabled: function(enabled) {
            try {
                WMCanvasAndroid.setFrameEnabled(enabled);
                console.log('🎬 Frame enabled:', enabled);
            } catch (error) {
                console.error('❌ setFrameEnabled error:', error);
            }
        },
        
        /**
         * 拍照
         * @returns {Promise<Object>}
         */
        takePicture: function() {
            return new Promise((resolve, reject) => {
                try {
                    const result = WMCanvasAndroid.takePicture();
                    console.log('📸 Take picture result:', result);
                    
                    try {
                        const response = JSON.parse(result);
                        if (response.success) {
                            resolve(response);
                        } else {
                            reject(new Error(response.message || 'Take picture failed'));
                        }
                    } catch (e) {
                        console.log('⚠️ Take picture returned non-JSON, assuming success');
                        resolve({ success: true, message: result });
                    }
                } catch (error) {
                    console.error('❌ Take picture error:', error);
                    reject(error);
                }
            });
        },
        
        /**
         * 获取功能支持情况
         * @returns {Object}
         */
        getCapabilities: function() {
            try {
                const result = WMCanvasAndroid.getCapabilities();
                return JSON.parse(result);
            } catch (error) {
                console.error('❌ getCapabilities error:', error);
                return { camera: false, canvas2d: false, webgl: false };
            }
        }
    };
    
    console.log('✅ WMCanvasCamera API ready');
    console.log('📋 Capabilities:', window.WMCanvasCamera.getCapabilities());
    
})();
