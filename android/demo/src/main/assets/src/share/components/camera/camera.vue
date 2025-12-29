<template>
    <div class="camera-container">
        <canvas 
            ref="canvas" 
            :width="canvasWidth" 
            :height="canvasHeight"
            class="camera-canvas"
        ></canvas>
    </div>
</template>

<script>
export default {
    name: 'CameraRenderer',
    props: {
        cameraActive: {
            type: Boolean,
            default: false
        }
    },
    data() {
        return {
            canvasWidth: 400,
            canvasHeight: 400,
            ctx: null,
            animationId: null,
            fps: 0,
            lastFrameTime: Date.now(),
            frameCount: 0,
            cameraFrameImage: null
        };
    },
    watch: {
        cameraActive(newVal) {
            if (newVal) {
                this.startCameraRenderLoop();
            } else {
                this.stopRenderLoop();
                this.startAnimationLoop();
            }
        }
    },
    mounted() {
        this.initCanvas();
        this.setupCameraCallback();
        this.startAnimationLoop();
    },
    beforeUnmount() {
        this.stopRenderLoop();
        if (window.WMCanvasCamera) {
            window.WMCanvasCamera.setFrameEnabled(false);
        }
    },
    methods: {
        initCanvas() {
            const canvas = this.$refs.canvas;
            this.ctx = canvas.getContext('2d');
            
            this.resizeCanvas();
            window.addEventListener('resize', this.resizeCanvas);
            
            this.$emit('camera-status-change', {
                size: `${this.canvasWidth} x ${this.canvasHeight}`
            });
        },

        resizeCanvas() {
            const container = this.$el;
            if (container) {
                this.canvasWidth = container.clientWidth;
                this.canvasHeight = container.clientHeight;
                
                this.$emit('camera-status-change', {
                    size: `${this.canvasWidth} x ${this.canvasHeight}`
                });
                
                console.log('Canvas resized:', this.canvasWidth, 'x', this.canvasHeight);
            }
        },

        setupCameraCallback() {
            // 设置全局回调函数用于接收相机帧
            window.updateCameraFrame = (base64ImageData) => {
                if (!this.cameraActive) {
                    console.log('updateCameraFrame called but camera not active');
                    return;
                }
                
                const img = new Image();
                img.onload = () => {
                    this.cameraFrameImage = img;
                    console.log('Camera frame image loaded:', img.width, 'x', img.height);
                };
                img.onerror = () => {
                    console.error('Failed to load camera frame image');
                };
                img.src = base64ImageData;
            };
        },

        async startCamera() {
            if (!window.WMCanvasCamera) {
                throw new Error('WMCanvasCamera API not available');
            }
            
            await window.WMCanvasCamera.start(this.canvasWidth, this.canvasHeight);
            window.WMCanvasCamera.setFrameEnabled(true);
            
            console.log('Camera API started');
        },

        async stopCamera() {
            if (!window.WMCanvasCamera) {
                return;
            }
            
            await window.WMCanvasCamera.stop();
            window.WMCanvasCamera.setFrameEnabled(false);
            this.cameraFrameImage = null;
            this.ctx.clearRect(0, 0, this.canvasWidth, this.canvasHeight);
            
            console.log('Camera API stopped');
        },

        async takePicture() {
            if (!window.WMCanvasCamera) {
                throw new Error('WMCanvasCamera API not available');
            }
            
            const result = await window.WMCanvasCamera.takePicture();
            console.log('Picture taken successfully:', result);
            
            // 闪白效果
            const prevAlpha = this.ctx.globalAlpha;
            this.ctx.globalAlpha = 0.8;
            this.ctx.fillStyle = 'white';
            this.ctx.fillRect(0, 0, this.canvasWidth, this.canvasHeight);
            this.ctx.globalAlpha = prevAlpha;
            
            return result;
        },

        stopRenderLoop() {
            if (this.animationId) {
                cancelAnimationFrame(this.animationId);
                this.animationId = null;
                console.log('Render loop stopped');
            }
        },

        startCameraRenderLoop() {
            this.stopRenderLoop();
            
            let frameRenderCount = 0;
            
            const render = () => {
                if (!this.cameraActive) {
                    this.stopRenderLoop();
                    return;
                }
                
                this.frameCount++;
                const now = Date.now();
                
                if (now - this.lastFrameTime >= 1000) {
                    this.fps = this.frameCount;
                    this.frameCount = 0;
                    this.lastFrameTime = now;
                    
                    this.$emit('fps-update', this.fps);
                    
                    console.log('Render FPS:', this.fps, ', frames with image:', frameRenderCount, ', hasImage:', this.cameraFrameImage !== null);
                    frameRenderCount = 0;
                }
                
                try {
                    this.ctx.clearRect(0, 0, this.canvasWidth, this.canvasHeight);
                    
                    if (this.cameraFrameImage) {
                        frameRenderCount++;
                        this.ctx.drawImage(this.cameraFrameImage, 0, 0, this.canvasWidth, this.canvasHeight);
                    } else {
                        // 等待相机帧
                        this.ctx.fillStyle = 'red';
                        this.ctx.fillRect(0, 0, 100, 100);
                        this.ctx.fillStyle = 'white';
                        this.ctx.font = '20px Arial';
                        this.ctx.fillText('Waiting for camera...', 10, 50);
                    }
                } catch (err) {
                    console.error('Render error:', err);
                }
                
                this.animationId = requestAnimationFrame(render);
            };
            
            console.log('Starting camera render loop');
            render();
        },

        startAnimationLoop() {
            this.stopRenderLoop();
            
            const render = () => {
                if (this.cameraActive) {
                    this.stopRenderLoop();
                    return;
                }
                
                this.frameCount++;
                const now = Date.now();
                
                if (now - this.lastFrameTime >= 1000) {
                    this.fps = this.frameCount;
                    this.frameCount = 0;
                    this.lastFrameTime = now;
                    this.$emit('fps-update', this.fps);
                }
                
                this.ctx.clearRect(0, 0, this.canvasWidth, this.canvasHeight);
                this.drawShapes();
                
                this.animationId = requestAnimationFrame(render);
            };
            
            render();
        },

        drawOverlay() {
            const time = Date.now() / 1000;
            const centerX = this.canvasWidth / 2;
            const centerY = this.canvasHeight / 2;
            
            // 半透明矩形
            this.ctx.fillStyle = 'rgba(139, 92, 246, 0.3)';
            this.ctx.fillRect(50, 50, 100, 100);
            
            // 圆形
            this.ctx.beginPath();
            this.ctx.arc(this.canvasWidth - 100, 100, 50, 0, Math.PI * 2);
            this.ctx.fillStyle = 'rgba(236, 72, 153, 0.4)';
            this.ctx.fill();
            
            // 旋转线条
            this.ctx.strokeStyle = 'rgba(34, 197, 94, 0.6)';
            this.ctx.lineWidth = 3;
            this.ctx.beginPath();
            this.ctx.moveTo(centerX, centerY);
            this.ctx.lineTo(centerX + Math.cos(time) * 100, centerY + Math.sin(time) * 100);
            this.ctx.stroke();
            
            // 文字
            this.ctx.fillStyle = 'rgba(255, 255, 255, 0.8)';
            this.ctx.font = 'bold 24px Arial';
            this.ctx.fillText('WMCanvas Camera', 10, this.canvasHeight - 20);
        },

        drawShapes() {
            const time = Date.now() / 1000;
            const centerX = this.canvasWidth / 2;
            const centerY = this.canvasHeight / 2;
            
            // 渐变背景
            const gradient = this.ctx.createLinearGradient(0, 0, this.canvasWidth, this.canvasHeight);
            gradient.addColorStop(0, 'rgba(139, 92, 246, 0.5)');
            gradient.addColorStop(1, 'rgba(236, 72, 153, 0.5)');
            this.ctx.fillStyle = gradient;
            this.ctx.fillRect(0, 0, this.canvasWidth, this.canvasHeight);
            
            // 旋转矩形
            this.ctx.save();
            this.ctx.translate(centerX, centerY);
            this.ctx.rotate(time);
            this.ctx.fillStyle = '#8b5cf6';
            this.ctx.fillRect(-100, -100, 200, 200);
            this.ctx.restore();
            
            // 圆形动画
            for (let i = 0; i < 5; i++) {
                this.ctx.beginPath();
                this.ctx.arc(
                    centerX + Math.cos(time + i) * 150,
                    centerY + Math.sin(time + i) * 150,
                    30,
                    0,
                    Math.PI * 2
                );
                this.ctx.fillStyle = `rgba(${100 + i * 30}, ${200 - i * 30}, 255, 0.6)`;
                this.ctx.fill();
            }
        }
    }
};
</script>

<style scoped>
.camera-container {
    flex: 1;
    position: relative;
    overflow: hidden;
    background: #000;
    display: flex;
    align-items: center;
    justify-content: center;
}

.camera-canvas {
    display: block;
    width: 100%;
    height: 100%;
    touch-action: none;
}
</style>
