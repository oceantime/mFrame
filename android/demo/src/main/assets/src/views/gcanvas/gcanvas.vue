<template>
    <div class="gcanvas-container">
        <h1>🎨 GCanvas WebView Demo</h1>
        
        <canvas ref="canvas" id="myCanvas" width="400" height="400"></canvas>
        
        <div class="controls">
            <button @click="drawRect">绘制矩形</button>
            <button @click="drawCircle">绘制圆形</button>
            <button @click="drawAnimation">{{ isAnimating ? '停止动画' : '动画效果' }}</button>
            <button @click="clearCanvas">清空画布</button>
            <button @click="testBridge">测试Bridge</button>
        </div>
        
        <div class="info">
            {{ infoMessage }}
        </div>
    </div>
</template>

<script>
export default {
    name: 'GCanvasView',
    data() {
        return {
            ctx: null,
            animationId: null,
            isAnimating: false,
            infoMessage: '点击上方按钮测试Canvas绘制功能'
        }
    },
    mounted() {
        this.init();
    },
    beforeUnmount() {
        if (this.animationId) {
            cancelAnimationFrame(this.animationId);
        }
    },
    methods: {
        init() {
            const canvas = this.$refs.canvas;
            this.ctx = canvas.getContext('2d');
            this.updateInfo(`Canvas已初始化，尺寸: ${canvas.width}x${canvas.height}`);
            
            // 测试Android Bridge
            if (typeof AndroidBridge !== 'undefined') {
                try {
                    const deviceInfo = AndroidBridge.getDeviceInfo();
                    this.updateInfo('设备信息: ' + deviceInfo);
                } catch (e) {
                    console.error('AndroidBridge error:', e);
                }
            }
        },

        drawRect() {
            this.clearCanvas();
            
            // 绘制渐变矩形
            const gradient = this.ctx.createLinearGradient(0, 0, 400, 0);
            gradient.addColorStop(0, '#FF6B6B');
            gradient.addColorStop(1, '#4ECDC4');
            
            this.ctx.fillStyle = gradient;
            this.ctx.fillRect(50, 50, 300, 200);
            
            // 绘制边框
            this.ctx.strokeStyle = '#333';
            this.ctx.lineWidth = 3;
            this.ctx.strokeRect(50, 50, 300, 200);
            
            this.updateInfo('已绘制渐变矩形');
        },

        drawCircle() {
            this.clearCanvas();
            
            // 绘制多个圆
            const circles = [
                {x: 100, y: 100, r: 60, color: '#FF6B6B'},
                {x: 200, y: 200, r: 80, color: '#4ECDC4'},
                {x: 300, y: 100, r: 50, color: '#FFE66D'}
            ];
            
            circles.forEach(circle => {
                this.ctx.beginPath();
                this.ctx.arc(circle.x, circle.y, circle.r, 0, Math.PI * 2);
                this.ctx.fillStyle = circle.color;
                this.ctx.fill();
                this.ctx.strokeStyle = '#333';
                this.ctx.lineWidth = 2;
                this.ctx.stroke();
            });
            
            this.updateInfo('已绘制多个圆形');
        },

        drawAnimation() {
            if (this.animationId) {
                cancelAnimationFrame(this.animationId);
                this.animationId = null;
                this.isAnimating = false;
                this.updateInfo('动画已停止');
                return;
            }
            
            let angle = 0;
            const centerX = 200;
            const centerY = 200;
            const radius = 100;
            
            const animate = () => {
                // 清空画布
                this.ctx.clearRect(0, 0, 400, 400);
                
                // 绘制背景圆
                this.ctx.beginPath();
                this.ctx.arc(centerX, centerY, radius, 0, Math.PI * 2);
                this.ctx.strokeStyle = '#ddd';
                this.ctx.lineWidth = 2;
                this.ctx.stroke();
                
                // 计算小球位置
                const x = centerX + Math.cos(angle) * radius;
                const y = centerY + Math.sin(angle) * radius;
                
                // 绘制旋转的小球
                this.ctx.beginPath();
                this.ctx.arc(x, y, 15, 0, Math.PI * 2);
                const gradient = this.ctx.createRadialGradient(x, y, 0, x, y, 15);
                gradient.addColorStop(0, '#FFE66D');
                gradient.addColorStop(1, '#FF6B6B');
                this.ctx.fillStyle = gradient;
                this.ctx.fill();
                
                // 绘制中心点
                this.ctx.beginPath();
                this.ctx.arc(centerX, centerY, 5, 0, Math.PI * 2);
                this.ctx.fillStyle = '#333';
                this.ctx.fill();
                
                angle += 0.05;
                this.animationId = requestAnimationFrame(animate);
            };
            
            this.isAnimating = true;
            animate();
            this.updateInfo('动画已启动，再次点击停止');
        },

        clearCanvas() {
            if (this.animationId) {
                cancelAnimationFrame(this.animationId);
                this.animationId = null;
                this.isAnimating = false;
            }
            this.ctx.clearRect(0, 0, 400, 400);
            this.updateInfo('画布已清空');
        },

        testBridge() {
            if (typeof AndroidBridge !== 'undefined') {
                try {
                    AndroidBridge.showToast('Hello from JavaScript!');
                    this.updateInfo('已调用Android Bridge显示Toast');
                } catch (e) {
                    this.updateInfo('Android Bridge调用失败: ' + e.message);
                }
            } else {
                this.updateInfo('Android Bridge不可用（在浏览器中运行）');
            }
        },

        updateInfo(message) {
            const time = new Date().toLocaleTimeString();
            this.infoMessage = `[${time}] ${message}`;
        }
    }
}
</script>

<style scoped>
.gcanvas-container {
    max-width: 800px;
    margin: 0 auto;
    background: white;
    padding: 20px;
    border-radius: 8px;
    box-shadow: 0 2px 4px rgba(0,0,0,0.1);
}

h1 {
    color: #333;
    margin-bottom: 20px;
    font-size: 24px;
}

canvas {
    border: 1px solid #ddd;
    display: block;
    margin: 20px auto;
    background: white;
}

.controls {
    margin: 20px 0;
    text-align: center;
}

button {
    background: #4CAF50;
    color: white;
    border: none;
    padding: 10px 20px;
    margin: 5px;
    border-radius: 4px;
    cursor: pointer;
    font-size: 14px;
    transition: all 0.2s ease;
}

button:hover {
    background: #45a049;
}

button:active {
    transform: scale(0.98);
}

.info {
    margin-top: 20px;
    padding: 10px;
    background: #e7f3ff;
    border-left: 4px solid #2196F3;
    color: #0d47a1;
    word-break: break-all;
}
</style>
