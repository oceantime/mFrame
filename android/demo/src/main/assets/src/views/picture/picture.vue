<template>
    <div class="picture-view">
        <div class="header">
            <button class="nav-btn" @click="goToList">📋 列表演示</button>
            
            <!-- 功能按钮 -->
            <div class="controls">
                <button 
                    class="control-btn" 
                    @click="startCamera" 
                    :disabled="cameraActive"
                    title="启动相机"
                >
                    📷
                </button>
                <button 
                    class="control-btn" 
                    @click="stopCamera"
                    :disabled="!cameraActive"
                    title="停止相机"
                >
                    ⏹️
                </button>
                <button 
                    class="control-btn" 
                    @click="takePicture"
                    :disabled="!cameraActive"
                    title="拍照"
                >
                    📸
                </button>
            </div>
        </div>
        
        <camera-renderer
            ref="camera"
            :camera-active="cameraActive"
            @camera-status-change="handleCameraStatusChange"
            @fps-update="handleFpsUpdate"
        />
        
        <!-- 信息显示 -->
        <div class="info">
            <div class="fps">FPS: {{ fps }}</div>
            <div class="status">相机状态: <span :style="{ color: statusColor }">{{ statusText }}</span></div>
            <div class="status">画布: <span>{{ canvasSize }}</span></div>
        </div>
    </div>
</template>

<script>
import CameraRenderer from '@/share/components/camera/camera.vue'

export default {
    name: 'PictureView',
    components: {
        CameraRenderer
    },
    data() {
        return {
            cameraActive: false,
            fps: 0,
            canvasSize: '--',
            statusText: '未启动',
            statusColor: '#666'
        };
    },
    methods: {
        async startCamera() {
            try {
                console.log('Starting camera...');
                await this.$refs.camera.startCamera();
                this.cameraActive = true;
                this.statusText = '运行中';
                this.statusColor = '#4ade80';
                console.log('Camera started successfully');
            } catch (error) {
                console.error('Failed to start camera:', error);
                alert('启动相机失败: ' + error.message);
            }
        },

        async stopCamera() {
            try {
                console.log('Stopping camera...');
                await this.$refs.camera.stopCamera();
                this.cameraActive = false;
                this.statusText = '已停止';
                this.statusColor = '#ef4444';
                console.log('Camera stopped');
            } catch (error) {
                console.error('Failed to stop camera:', error);
            }
        },

        async takePicture() {
            try {
                console.log('Taking picture...');
                const result = await this.$refs.camera.takePicture();
                
                if (result && result.path) {
                    setTimeout(() => {
                        alert('照片已保存到:\n' + result.path);
                    }, 100);
                }
            } catch (error) {
                console.error('Failed to take picture:', error);
                alert('拍照失败: ' + error.message);
            }
        },

        handleCameraStatusChange({ size }) {
            this.canvasSize = size;
        },

        handleFpsUpdate(fps) {
            this.fps = fps;
        },

        goToList() {
            this.$router.push('/list');
        }
    }
};
</script>

<style scoped>
.picture-view {
    width: 100%;
    height: 100%;
    display: flex;
    flex-direction: column;
    background: #000;
}

/* ========== Header ========== */
.header {
    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
    padding: 15px 20px;
    display: flex;
    align-items: center;
    justify-content: space-between;
    box-shadow: 0 2px 10px rgba(0,0,0,0.1);
    flex-shrink: 0;
    z-index: 100;
}

.nav-btn {
    padding: 8px 16px;
    font-size: 14px;
    border: 2px solid white;
    border-radius: 8px;
    background: rgba(255, 255, 255, 0.2);
    color: white;
    cursor: pointer;
    font-weight: 600;
    transition: all 0.3s ease;
}

.nav-btn:active {
    transform: scale(0.95);
    background: rgba(255, 255, 255, 0.3);
}

/* ========== Control Buttons ========== */
.controls {
    display: flex;
    gap: 10px;
    align-items: center;
}

.control-btn {
    width: 44px;
    height: 44px;
    border-radius: 50%;
    border: 2px solid white;
    background: rgba(255, 255, 255, 0.2);
    color: white;
    font-size: 20px;
    cursor: pointer;
    transition: all 0.3s ease;
    display: flex;
    align-items: center;
    justify-content: center;
}

.control-btn:active {
    transform: scale(0.95);
    background: rgba(255, 255, 255, 0.35);
}

.control-btn:disabled {
    opacity: 0.4;
    cursor: not-allowed;
}

/* ========== Info Display ========== */
.info {
    position: fixed;
    top: 80px;
    right: 20px;
    background: rgba(0, 0, 0, 0.7);
    color: white;
    padding: 15px;
    border-radius: 8px;
    font-size: 14px;
    max-width: 300px;
    z-index: 1000;
}

.fps {
    color: #4ade80;
    font-weight: bold;
    margin-bottom: 5px;
}

.status {
    margin-top: 5px;
    color: #a855f7;
}

.draw-mode {
    margin-top: 10px;
    padding: 10px;
    background: rgba(139, 92, 246, 0.3);
    border-radius: 4px;
    white-space: pre-line;
    font-size: 12px;
}
</style>
