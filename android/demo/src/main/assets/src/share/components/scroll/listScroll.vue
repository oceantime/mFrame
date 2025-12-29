<template>
    <div class="canvas-container" ref="container">
        <canvas ref="canvas"></canvas>
    </div>
</template>

<script>
import * as TWEEN from '@tweenjs/tween.js';

export default {
    name: 'ListScroll',
    props: {
        listData: {
            type: Array,
            required: true
        },
        itemHeight: {
            type: Number,
            default: 120
        },
        loadmoreOffset: {
            type: Number,
            default: 3000
        },
        useGPU: {
            type: Boolean,
            default: true
        }
    },
    emits: ['loadmore', 'scroll', 'performance', 'itemClick'],
    data() {
        return {
            scrollY: 0,
            startY: 0,
            lastY: 0,
            velocity: 0,
            isScrolling: false,
            animationId: null,
            canvas: null,
            ctx: null,
            canvasHeight: 0,
            canvasWidth: 0,
            imageCache: {},
            
            // === Canvas池缓存相关（AlloyTeam方案） ===
            canvasPool: new Map(),           // Canvas缓存池
            maxCacheSize: 50,                // 最大缓存数量（防止内存泄漏）
            cacheHitCount: 0,                // 命中次数统计
            cacheMissCount: 0,               // 未命中次数统计
            
            // === Tween平滑滚动相关（Gent方案） ===
            scrollTween: null,               // 当前滚动动画
            tweenAnimationFrame: null,       // 动画帧ID
            
            // === economy渲染节流相关（Gent方案） ===
            visibleIndexMap: new Set(),      // 当前可见项的索引集合
            lastRenderTime: 0,               // 上次渲染时间
            renderThrottle: 8,               // 渲染节流间隔（120fps，优化响应速度）
            pendingRender: null,             // 待处理的渲染请求
            
            // === 性能监控数据 ===
            currentFPS: 0,                   // 当前FPS
            averageFPS: 0,                   // 平均FPS
            
            // === 事件系统相关 ===
            touchStartTime: 0,               // 触摸开始时间
            touchStartPos: { x: 0, y: 0 },   // 触摸开始位置
            touchMoved: false,               // 是否发生了移动
            clickThreshold: 10               // 点击判定阈值（像素）
        };
    },
    computed: {
        totalHeight() {
            return this.listData.length * this.itemHeight;
        },
        visibleStart() {
            return Math.floor(this.scrollY / this.itemHeight);
        },
        visibleEnd() {
            return Math.ceil((this.scrollY + this.canvasHeight) / this.itemHeight);
        },
        visibleItems() {
            return this.listData.slice(
                Math.max(0, this.visibleStart - 2),
                Math.min(this.listData.length, this.visibleEnd + 2)
            );
        },
        // 可见区域索引范围（economy优化）
        visibleIndexRange() {
            const start = Math.max(0, this.visibleStart - 2);
            const end = Math.min(this.listData.length, this.visibleEnd + 2);
            return { start, end };
        }
    },
    watch: {
        listData: {
            handler(newData, oldData) {
                // 数据变化时清除Canvas缓存
                if (this.canvasPool && this.canvasPool.size > 0) {
                    this.canvasPool.clear();
                    console.log('🧹 数据更新，清除Canvas池（' + this.canvasPool.size + '项）');
                    // 重置统计
                    this.cacheHitCount = 0;
                    this.cacheMissCount = 0;
                }
                
                this.render();
            },
            deep: true
        },
        useGPU(newVal) {
            this.toggleGPU(newVal);
        }
    },
    mounted() {
        this.initCanvas();
        this.initTouchEvents();
        this.render();
        window.addEventListener('resize', this.handleResize);
        
        // === 性能监控 ===
        this.startPerformanceMonitor();
    },
    beforeUnmount() {
        window.removeEventListener('resize', this.handleResize);
        if (this.animationId) {
            cancelAnimationFrame(this.animationId);
        }
    },
    methods: {
        initCanvas() {
            this.canvas = this.$refs.canvas;
            const container = this.$refs.container;
            
            // 获取容器尺寸
            this.canvasWidth = container.clientWidth;
            this.canvasHeight = container.clientHeight;
            
            // 处理高DPI屏幕（修复模糊问题）
            const dpr = window.devicePixelRatio || 1;
            
            // 设置Canvas实际像素尺寸（乘以dpr）
            this.canvas.width = this.canvasWidth * dpr;
            this.canvas.height = this.canvasHeight * dpr;
            
            // 设置Canvas显示尺寸（CSS）
            this.canvas.style.width = this.canvasWidth + 'px';
            this.canvas.style.height = this.canvasHeight + 'px';
            
            this.ctx = this.canvas.getContext('2d');
            
            // 缩放绘图上下文以匹配像素密度
            this.ctx.scale(dpr, dpr);
            
            this.toggleGPU(this.useGPU);
            
            console.log('📐 Canvas初始化:', this.canvasWidth, 'x', this.canvasHeight, '| DPR:', dpr);
        },

        toggleGPU(enabled) {
            if (!this.canvas) return;
            
            if (enabled && typeof window.WMCanvas !== 'undefined') {
                this.ctx = window.WMCanvas.enable(this.canvas);
                console.log('✨ GPU加速已启用');
            } else {
                this.ctx = this.canvas.getContext('2d');
                console.log('💻 使用CPU渲染');
            }
        },

        initTouchEvents() {
            const canvas = this.canvas;
            
            canvas.addEventListener('touchstart', (e) => {
                e.preventDefault();
                this.isScrolling = true;
                this.startY = e.touches[0].clientY;
                this.lastY = this.startY;
                this.velocity = 0;
                
                // 记录点击开始信息
                this.touchStartTime = Date.now();
                this.touchStartPos = {
                    x: e.touches[0].clientX,
                    y: e.touches[0].clientY
                };
                this.touchMoved = false;
                
                // 停止所有现有动画
                if (this.animationId) {
                    cancelAnimationFrame(this.animationId);
                    this.animationId = null;
                }
                
                if (this.scrollTween) {
                    this.scrollTween.stop();
                    this.scrollTween = null;
                }
                
                if (this.tweenAnimationFrame) {
                    cancelAnimationFrame(this.tweenAnimationFrame);
                    this.tweenAnimationFrame = null;
                }
            });

            canvas.addEventListener('touchmove', (e) => {
                e.preventDefault();
                if (!this.isScrolling) return;
                
                this.touchMoved = true;  // 标记发生了移动
                
                const currentY = e.touches[0].clientY;
                const deltaY = currentY - this.lastY;
                this.velocity = deltaY;
                
                this.scrollY = Math.max(
                    0,
                    Math.min(
                        this.totalHeight - this.canvasHeight,
                        this.scrollY - deltaY
                    )
                );
                
                this.lastY = currentY;
                
                // touchmove中直接渲染，不使用节流，提高响应速度
                this.doRender();
                this.$emit('scroll', this.scrollY);
                
                if (this.totalHeight - (this.scrollY + this.canvasHeight) < this.loadmoreOffset) {
                    this.$emit('loadmore');
                }
            });

            canvas.addEventListener('touchend', (e) => {
                e.preventDefault();
                
                // 检测是否为点击事件（而非滚动）
                const touchEndTime = Date.now();
                const touchDuration = touchEndTime - this.touchStartTime;
                const touchEndPos = {
                    x: e.changedTouches[0].clientX,
                    y: e.changedTouches[0].clientY
                };
                
                // 计算移动距离
                const moveDistance = Math.sqrt(
                    Math.pow(touchEndPos.x - this.touchStartPos.x, 2) +
                    Math.pow(touchEndPos.y - this.touchStartPos.y, 2)
                );
                
                // 判定为点击：时间短(<300ms) 且 移动距离小(<10px)
                const isClick = touchDuration < 300 && moveDistance < this.clickThreshold;
                
                if (isClick) {
                    this.handleClick(touchEndPos.x, touchEndPos.y);
                }
                
                this.isScrolling = false;
                this.startInertiaScroll();
            });
        },

        startInertiaScroll() {
            // 速度阈值：超过此值使用Tween平滑滚动
            const velocityThreshold = 5;
            
            if (Math.abs(this.velocity) > velocityThreshold) {
                // 快速滑动：使用Tween实现惯性效果
                const distance = this.velocity * 30; // 根据速度计算惯性距离
                const targetY = this.scrollY - distance;
                const duration = 400; // 惯性动画时长
                
                // 停止现有动画
                if (this.scrollTween) {
                    this.scrollTween.stop();
                }
                
                const startY = this.scrollY;
                const coords = { y: startY };
                
                this.scrollTween = new TWEEN.Tween(coords)
                    .to({ y: targetY }, duration)
                    .easing(TWEEN.Easing.Quadratic.Out)  // 二次缓出，模拟惯性减速
                    .onUpdate(() => {
                        // 确保不超出边界
                        const clampedY = Math.max(
                            0,
                            Math.min(this.totalHeight - this.canvasHeight, coords.y)
                        );
                        this.scrollY = clampedY;
                        this.render();
                        this.$emit('scroll', this.scrollY);
                        
                        // 检查是否需要加载更多
                        if (this.totalHeight - (this.scrollY + this.canvasHeight) < this.loadmoreOffset) {
                            this.$emit('loadmore');
                        }
                    })
                    .onComplete(() => {
                        this.scrollTween = null;
                    })
                    .start();
                
                this.animateTween();
                
            } else {
                // 慢速滑动：使用原有的简单惯性动画
                const friction = 0.95;
                const minVelocity = 0.5;
                
                const animate = () => {
                    if (Math.abs(this.velocity) < minVelocity) {
                        this.velocity = 0;
                        return;
                    }
                    
                    this.velocity *= friction;
                    this.scrollY = Math.max(
                        0,
                        Math.min(
                            this.totalHeight - this.canvasHeight,
                            this.scrollY - this.velocity
                        )
                    );
                    
                    this.render();
                    this.$emit('scroll', this.scrollY);
                    
                    if (this.totalHeight - (this.scrollY + this.canvasHeight) < this.loadmoreOffset) {
                        this.$emit('loadmore');
                    }
                    
                    this.animationId = requestAnimationFrame(animate);
                };
                
                this.animationId = requestAnimationFrame(animate);
            }
        },

        render() {
            // 渲染节流控制（防止过度渲染）
            const now = Date.now();
            if (now - this.lastRenderTime < this.renderThrottle) {
                // 使用 requestAnimationFrame 延迟渲染
                if (!this.pendingRender) {
                    this.pendingRender = requestAnimationFrame(() => {
                        this.pendingRender = null;
                        this.doRender();
                    });
                }
                return;
            }
            
            this.doRender();
        },
        
        // 实际渲染方法
        doRender() {
            if (!this.ctx) return;
            
            this.lastRenderTime = Date.now();
            
            // 清空并绘制背景
            this.ctx.clearRect(0, 0, this.canvasWidth, this.canvasHeight);
            this.ctx.fillStyle = '#ffffff';
            this.ctx.fillRect(0, 0, this.canvasWidth, this.canvasHeight);
            
            const { start, end } = this.visibleIndexRange;
            
            // 仅渲染可见区域的列表项
            for (let i = start; i < end; i++) {
                const item = this.listData[i];
                const y = i * this.itemHeight - this.scrollY;
                this.renderItem(item, y);
            }
            
            // 图片按需加载优化
            this.optimizedImageLoading();
            
            // WMCanvas GPU加速刷新
            if (typeof window.WMCanvas !== 'undefined' && this.ctx.flush) {
                this.ctx.flush();
            }
        },
        
        // 图片按需加载优化（economy机制）
        optimizedImageLoading() {
            const { start, end } = this.visibleIndexRange;
            const newVisibleSet = new Set();
            
            // 仅加载可见区域的图片
            for (let i = start; i < end; i++) {
                newVisibleSet.add(i);
                const item = this.listData[i];
                
                // 如果是新进入可见区域的item，加载图片
                if (!this.visibleIndexMap.has(i) && item.imageUrl) {
                    this.loadImage(item.imageUrl);
                }
            }
            
            // 可选：清理不再可见的图片缓存（防止内存泄漏）
            if (this.imageCache && Object.keys(this.imageCache).length > 100) {
                this.visibleIndexMap.forEach(i => {
                    if (!newVisibleSet.has(i)) {
                        const item = this.listData[i];
                        if (item && item.imageUrl && this.imageCache[item.imageUrl]) {
                            // console.log('🗑️ 清理不可见图片缓存');
                            delete this.imageCache[item.imageUrl];
                        }
                    }
                });
            }
            
            this.visibleIndexMap = newVisibleSet;
        },

        renderItem(item, y) {
            // 生成缓存键（包含item唯一ID和尺寸）
            const cacheKey = `${item.id}_${this.itemHeight}_${this.canvasWidth}`;
            
            // 检查缓存
            if (this.canvasPool.has(cacheKey)) {
                const cachedCanvas = this.canvasPool.get(cacheKey);
                this.ctx.drawImage(cachedCanvas, 0, y);
                this.cacheHitCount++;
                return; // 命中缓存，直接返回
            }
            
            // 未命中，记录统计
            this.cacheMissCount++;
            
            // 创建离屏Canvas
            const offscreenCanvas = document.createElement('canvas');
            offscreenCanvas.width = this.canvasWidth;
            offscreenCanvas.height = this.itemHeight;
            const offCtx = offscreenCanvas.getContext('2d');
            
            // 在离屏Canvas上绘制（y坐标为0）
            this.drawItemContent(offCtx, item, 0);
            
            // LRU缓存管理（超出限制删除最早的）
            if (this.canvasPool.size >= this.maxCacheSize) {
                const firstKey = this.canvasPool.keys().next().value;
                this.canvasPool.delete(firstKey);
            }
            
            // 存入缓存池
            this.canvasPool.set(cacheKey, offscreenCanvas);
            
            // 绘制到主Canvas
            this.ctx.drawImage(offscreenCanvas, 0, y);
        },
        
        // 抽取绘制内容为独立方法（用于离屏Canvas绘制）
        drawItemContent(ctx, item, baseY) {
            const padding = 15;
            const imageSize = 70;
            const rightMargin = 15;
            
            // 分割线
            ctx.fillStyle = '#e5e5e5';
            ctx.fillRect(0, baseY + this.itemHeight - 1, this.canvasWidth, 1);
            
            // 图片绘制
            let textStartX = padding;
            if (item.imageUrl && item.imageUrl !== 'null' && item.imageUrl !== null) {
                const imageY = baseY + (this.itemHeight - imageSize) / 2;
                
                if (this.imageCache[item.imageUrl]) {
                    const img = this.imageCache[item.imageUrl];
                    if (img && img.complete && img.naturalWidth > 0) {
                        try {
                            ctx.fillStyle = '#f0f0f0';
                            ctx.fillRect(padding, imageY, imageSize, imageSize);
                            
                            ctx.drawImage(img, padding, imageY, imageSize, imageSize);
                            textStartX = padding * 2 + imageSize;
                        } catch (e) {
                            ctx.fillStyle = '#cccccc';
                            ctx.fillRect(padding, imageY, imageSize, imageSize);
                            textStartX = padding * 2 + imageSize;
                        }
                    }
                } else if (item.imageUrl) {
                    this.loadImage(item.imageUrl);
                }
            }
            
            const maxTextWidth = this.canvasWidth - textStartX - rightMargin;
            
            // 标题
            ctx.fillStyle = '#333';
            ctx.font = 'bold 16px sans-serif';
            const title = this.truncateText(item.title, maxTextWidth);
            ctx.fillText(title, textStartX, baseY + 30);
            
            // 内容
            ctx.fillStyle = '#666';
            ctx.font = '14px sans-serif';
            const content = this.truncateText(item.content || '', maxTextWidth);
            ctx.fillText(content, textStartX, baseY + 55);
            
            // 作者信息
            ctx.fillStyle = '#999';
            ctx.font = '12px sans-serif';
            const author = `👤 ${item.author || '未知'} · ${item.time || ''}`;
            ctx.fillText(author, textStartX, baseY + 80);
            
            // 统计信息
            const stats = `💬 ${item.replies || 0} · 👁 ${item.views || 0}`;
            ctx.fillText(stats, textStartX, baseY + 100);
        },

        truncateText(text, maxWidth) {
            if (!text) return '';
            
            let width = this.ctx.measureText(text).width;
            if (width <= maxWidth) return text;
            
            let ellipsis = '...';
            let ellipsisWidth = this.ctx.measureText(ellipsis).width;
            let len = text.length;
            
            while (width + ellipsisWidth > maxWidth && len-- > 0) {
                text = text.substring(0, len);
                width = this.ctx.measureText(text).width;
            }
            
            return text + ellipsis;
        },

        async loadImage(url) {
            if (this.imageCache[url]) return;
            
            const img = new Image();
            img.crossOrigin = 'anonymous';
            this.imageCache[url] = img;
            
            const blobUrl = await this.loadImageWithReferer(url);
            img.onload = () => {
                this.render();
            };
            img.onerror = () => {
                console.warn('⚠️ 图片加载失败:', url);
                this.imageCache[url] = null;
            };
            img.src = blobUrl;
        },

        async loadImageWithReferer(url) {
            if (!url) {
                return 'https://gw.alicdn.com/imgextra/i2/O1CN01NdbMqL1aKvZZYKC6S_!!6000000003312-0-tps-300-300.jpg';
            }

            try {
                const response = await fetch(url, {
                    headers: { 'Referer': 'https://w3m-beta.huawei.com/' }
                });
                
                if (!response.ok) throw new Error('Image load failed');
                
                const blob = await response.blob();
                return URL.createObjectURL(blob);
            } catch (error) {
                console.log('⚠️ 图片加载失败，使用占位图');
                return 'https://gw.alicdn.com/imgextra/i2/O1CN01NdbMqL1aKvZZYKC6S_!!6000000003312-0-tps-300-300.jpg';
            }
        },

        handleResize() {
            this.initCanvas();
            this.render();
        },
        
        // 性能监控系统
        startPerformanceMonitor() {
            // 1. FPS监控
            let lastTime = performance.now();
            let frames = 0;
            let fpsHistory = [];
            
            const checkFPS = () => {
                frames++;
                const now = performance.now();
                
                if (now >= lastTime + 1000) {
                    const fps = Math.round((frames * 1000) / (now - lastTime));
                    fpsHistory.push(fps);
                    
                    // 保留最近10秒的数据
                    if (fpsHistory.length > 10) {
                        fpsHistory.shift();
                    }
                    
                    const avgFPS = Math.round(
                        fpsHistory.reduce((a, b) => a + b, 0) / fpsHistory.length
                    );
                    
                    // 更新响应式数据
                    this.currentFPS = fps;
                    this.averageFPS = avgFPS;
                    
                    // 计算缓存命中率
                    const totalCache = this.cacheHitCount + this.cacheMissCount;
                    const hitRate = totalCache > 0 
                        ? Math.round((this.cacheHitCount / totalCache) * 100) 
                        : 0;
                    
                    // emit性能数据给父组件
                    this.$emit('performance', {
                        fps: fps,
                        avgFPS: avgFPS,
                        cacheHitRate: hitRate,
                        cacheSize: this.canvasPool.size,
                        visibleItems: this.visibleIndexMap.size
                    });

                    console.log('📊 实时FPS:', fps, '| 平均FPS:', avgFPS, '| 缓存命中率:', hitRate + '%');
                    
                    frames = 0;
                    lastTime = now;
                }
                requestAnimationFrame(checkFPS);
            };
            checkFPS();
            
            // 2. Canvas池统计
            setInterval(() => {
                if (this.cacheHitCount + this.cacheMissCount > 0) {
                    const hitRate = (this.cacheHitCount / (this.cacheHitCount + this.cacheMissCount) * 100).toFixed(2);
                    console.log('🎨 Canvas缓存统计:', {
                        命中率: hitRate + '%',
                        缓存池大小: this.canvasPool.size + '/' + this.maxCacheSize,
                        命中次数: this.cacheHitCount,
                        未命中次数: this.cacheMissCount
                    });
                }
            }, 5000);
            
            // 3. 内存使用监控（如果支持）
            if (performance.memory) {
                setInterval(() => {
                    const used = (performance.memory.usedJSHeapSize / 1048576).toFixed(2);
                    const total = (performance.memory.totalJSHeapSize / 1048576).toFixed(2);
                    console.log('💾 内存使用:', used, 'MB /', total, 'MB');
                }, 10000);
            }
        },

        scrollTo(targetY, duration = 600) {
            // 确保目标位置在有效范围内
            const clampedY = Math.max(
                0, 
                Math.min(this.totalHeight - this.canvasHeight, targetY)
            );
            
            // 停止现有动画
            if (this.scrollTween) {
                this.scrollTween.stop();
                this.scrollTween = null;
            }
            
            if (this.tweenAnimationFrame) {
                cancelAnimationFrame(this.tweenAnimationFrame);
                this.tweenAnimationFrame = null;
            }
            
            const startY = this.scrollY;
            const coords = { y: startY };
            
            // 创建Tween动画
            this.scrollTween = new TWEEN.Tween(coords)
                .to({ y: clampedY }, duration)
                .easing(TWEEN.Easing.Cubic.InOut)  // 三次缓动，最平滑
                .onUpdate(() => {
                    this.scrollY = coords.y;
                    this.render();
                    this.$emit('scroll', this.scrollY);
                })
                .onComplete(() => {
                    this.scrollTween = null;
                    console.log('✅ 滚动动画完成');
                })
                .start();
            
            // 启动动画循环
            this.animateTween();
        },
        
        // Tween动画循环方法
        animateTween() {
            if (!this.scrollTween) return;
            
            const animate = (time) => {
                if (this.scrollTween) {
                    TWEEN.update(time);
                    this.tweenAnimationFrame = requestAnimationFrame(animate);
                }
            };
            
            this.tweenAnimationFrame = requestAnimationFrame(animate);
        },

        // === 事件系统：点击检测和区域判定 ===
        
        /**
         * 处理点击事件
         * @param {number} x - 点击位置X坐标（相对于Canvas）
         * @param {number} y - 点击位置Y坐标（相对于Canvas）
         */
        handleClick(x, y) {
            // 获取Canvas相对坐标
            const rect = this.canvas.getBoundingClientRect();
            const canvasX = x - rect.left;
            const canvasY = y - rect.top;
            
            // 执行hitTest
            const item = this.hitTest(canvasX, canvasY);
            
            if (item) {
                console.log('🖱️ 点击了列表项:', item.index, item.data);
                
                // emit点击事件给父组件
                this.$emit('itemClick', {
                    index: item.index,
                    data: item.data,
                    position: { x: canvasX, y: canvasY }
                });
            } else {
                console.log('🖱️ 点击了空白区域');
            }
        },
        
        /**
         * hitTest：检测点击位置是否在某个列表项上
         * @param {number} x - Canvas坐标X
         * @param {number} y - Canvas坐标Y
         * @returns {Object|null} - 返回被点击的项信息，或null
         */
        hitTest(x, y) {
            // 将Canvas坐标转换为列表逻辑坐标（考虑滚动偏移）
            const logicalY = y + this.scrollY;
            
            // 计算点击位置对应的列表项索引
            const index = Math.floor(logicalY / this.itemHeight);
            
            // 检查索引是否有效
            if (index < 0 || index >= this.listData.length) {
                return null;
            }
            
            // 计算该项的区域边界
            const itemTop = index * this.itemHeight;
            const itemBottom = itemTop + this.itemHeight;
            
            // 检查点击位置是否在项的Y范围内
            if (logicalY < itemTop || logicalY > itemBottom) {
                return null;
            }
            
            // 检查点击位置是否在项的X范围内
            if (x < 0 || x > this.canvasWidth) {
                return null;
            }
            
            // 返回命中的项信息
            return {
                index: index,
                data: this.listData[index],
                bounds: {
                    top: itemTop,
                    bottom: itemBottom,
                    left: 0,
                    right: this.canvasWidth
                }
            };
        },
        
        /**
         * 获取指定位置的列表项（别名方法）
         * @param {number} x - Canvas坐标X
         * @param {number} y - Canvas坐标Y
         * @returns {Object|null}
         */
        getItemAtPosition(x, y) {
            return this.hitTest(x, y);
        },
        
        /**
         * 检测一个矩形区域与列表项的交集
         * @param {Object} rect - 矩形区域 {x, y, width, height}
         * @returns {Array} - 返回与区域相交的所有列表项
         */
        getItemsInRect(rect) {
            const { x, y, width, height } = rect;
            const items = [];
            
            // 转换为逻辑坐标
            const logicalTop = y + this.scrollY;
            const logicalBottom = logicalTop + height;
            
            // 计算可能相交的索引范围
            const startIndex = Math.max(0, Math.floor(logicalTop / this.itemHeight));
            const endIndex = Math.min(
                this.listData.length - 1,
                Math.ceil(logicalBottom / this.itemHeight)
            );
            
            // 遍历范围内的所有项
            for (let i = startIndex; i <= endIndex; i++) {
                const itemTop = i * this.itemHeight;
                const itemBottom = itemTop + this.itemHeight;
                
                // 检查矩形是否与项相交
                if (this.rectIntersects(
                    { x: 0, y: itemTop, width: this.canvasWidth, height: this.itemHeight },
                    { x, y: logicalTop, width, height }
                )) {
                    items.push({
                        index: i,
                        data: this.listData[i],
                        bounds: {
                            top: itemTop,
                            bottom: itemBottom,
                            left: 0,
                            right: this.canvasWidth
                        }
                    });
                }
            }
            
            return items;
        },
        
        /**
         * 检测两个矩形是否相交
         * @param {Object} rect1 - 矩形1 {x, y, width, height}
         * @param {Object} rect2 - 矩形2 {x, y, width, height}
         * @returns {boolean}
         */
        rectIntersects(rect1, rect2) {
            return !(
                rect1.x + rect1.width < rect2.x ||
                rect2.x + rect2.width < rect1.x ||
                rect1.y + rect1.height < rect2.y ||
                rect2.y + rect2.height < rect1.y
            );
        }
    }
};
</script>

<style scoped>
.canvas-container {
    flex: 1;
    position: relative;
    overflow: hidden;
    background: white;
}

.canvas-container canvas {
    display: block;
    width: 100%;
    height: 100%;
    touch-action: none;
}
</style>
