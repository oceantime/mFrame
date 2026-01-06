<template>
    <div class="list-view">
        <div class="header">
            <button class="nav-btn" @click="goToCamera">📷 相机演示</button>
            <button class="nav-btn tests" @click="goToTests">🧪 API测试</button>
            
            <!-- 功能按钮 -->
            <div class="controls">
                <button class="control-btn" @click="scrollToTop" title="滚动到顶部">
                    ⬆️
                </button>
                <button class="control-btn" @click="scrollToBottom" title="滚动到底部">
                    ⬇️
                </button>
                <button 
                    class="control-btn" 
                    :class="{ active: useGPU }" 
                    @click="toggleGPU"
                    title="GPU加速开关"
                >
                    ⚡
                </button>
            </div>
        </div>
        
        <list-scroll
            ref="scroll"
            :list-data="listData"
            :item-height="120"
            :loadmore-offset="3000"
            :use-g-p-u="useGPU"
            @loadmore="handleLoadMore"
            @scroll="handleScroll"
            @performance="handlePerformance"
            @itemClick="handleItemClick"
        />
        
        <!-- 统计信息 -->
        <div class="stats" v-if="listData.length > 0">
            <div class="stats-title">📊 列表统计</div>
            <div>总数据: {{ listData.length }} 条</div>
            <div>总高度: {{ totalHeight }}px</div>
            <div>当前位置: {{ Math.round(scrollY) }}px</div>
            <div>GPU加速: {{ useGPU ? '✅ 开启' : '❌ 关闭' }}</div>
            <div>当前页: {{ currentPage }}</div>
            <div class="perf-divider">⚡ 性能指标</div>
            <div>FPS: {{ performanceData.fps }} / 平均: {{ performanceData.avgFPS }}</div>
            <div>缓存命中率: {{ performanceData.cacheHitRate }}%</div>
            <div>缓存池: {{ performanceData.cacheSize }} 项</div>
        </div>
        
        <!-- 加载中 -->
        <div class="message" v-if="isLoading && listData.length === 0">
            <div class="spinner"></div>
            <div>加载中...</div>
        </div>
        
        <!-- 空状态 -->
        <div class="message" v-if="!isLoading && listData.length === 0">
            <div>😔 暂无数据</div>
        </div>
    </div>
</template>

<script>
import ListScroll from '@/share/components/scroll/listScroll.vue'

/**
 * 生成Mock数据（用于测试Canvas池缓存和Tween滚动效果）
 */
function generateMockData(page = 1, pageSize = 20) {
    const startIndex = (page - 1) * pageSize;
    const data = [];
    
    const titles = [
        '🚀 Canvas长列表性能优化实践',
        '💡 前端虚拟滚动技术深度解析',
        '⚡ WebGL渲染引擎性能对比',
        '🎨 PixiJS在大型项目中的应用',
        '📱 移动端H5性能优化指南',
        '🔥 Tween.js动画库最佳实践',
        '✨ 离屏Canvas缓存技术详解',
        '🎯 LRU缓存算法在前端的应用',
        '🌟 React虚拟DOM与Canvas对比',
        '💻 高性能列表渲染方案选型'
    ];
    
    const contents = [
        '通过Canvas池缓存技术，FPS提升40-60%',
        '虚拟滚动是处理大量数据的最佳方案',
        'WebGL硬件加速能显著提升渲染性能',
        'PixiJS提供了完整的2D渲染解决方案',
        '移动端需要特别关注内存和电量消耗',
        '平滑动画能大幅提升用户体验',
        '离屏Canvas可以避免重复绘制',
        'LRU算法能有效控制内存占用',
        '不同场景需要选择合适的渲染方案',
        '性能优化需要在功能和体验间平衡'
    ];
    
    const authors = ['张三', '李四', '王五', '赵六', '钱七', '孙八', '周九', '吴十'];
    
    for (let i = 0; i < pageSize; i++) {
        const index = startIndex + i;
        data.push({
            postId: `mock_${index}`,
            title: `${titles[index % titles.length]} #${index + 1}`,
            subject: [contents[index % contents.length]],
            author: {
                maskName: authors[index % authors.length],
                authorName: authors[index % authors.length]
            },
            creationDate: Date.now() - Math.random() * 86400000 * 30,
            commentCount: Math.floor(Math.random() * 500),
            viewCount: Math.floor(Math.random() * 10000),
            images: index % 3 === 0 ? [{
                url: `https://picsum.photos/200/200?random=${index}`
            }] : []
        });
    }
    
    return { data };
}

/**
 * API数据获取
 */
async function fetchListData(page = 1) {
    const url = 'https://w3m-beta.huawei.com/mcloud/umag/FreeProxyForText/xsgw_beta/app/user/v1/sections/posts/list/get';
    const params = {
        sectionId: "713534611705233414",
        limit: 20,
        orderBy: "0",
        page: page.toString(),
        categoryId: "",
        lastPostTime: "",
        lastReplyTime: "",
        lastPostId: "",
        isSinking: "",
        expireStatus: 0
    };

    try {
        console.log('🌐 API请求，页码:', page);
        const response = await fetch(url, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
                'Device': '5',
                'Referer': 'https://w3m-beta.huawei.com/',
                'Sec-Fetch-Dest': 'empty',
                'Sec-Fetch-Mode': 'cors',
                'Sec-Fetch-Site': 'same-origin',
                'Secure': 'true'
            },
            body: JSON.stringify(params)
        });

        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }

        const data = await response.json();
        console.log('✅ API响应，页码:', page);
        console.log('📊 数据量:', Array.isArray(data.data) ? data.data.length : 0);
        return data;
    } catch (error) {
        console.error('❌ API请求失败，使用Mock数据:', error);
        // API失败时使用Mock数据
        console.log('📦 生成Mock数据，页码:', page);
        return generateMockData(page);
    }
}

/**
 * 图片加载（带Referer）
 */
async function loadImageWithReferer(url) {
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
}

export default {
    name: 'ListView',
    components: {
        ListScroll
    },
    data() {
        return {
            listData: [],
            currentPage: 1,
            isLoading: false,
            useGPU: true,
            scrollY: 0,
            lastDataLength: 0,
            isLoadingMore: false,
            // 性能监控数据
            performanceData: {
                fps: 0,
                avgFPS: 0,
                cacheHitRate: 0,
                cacheSize: 0,
                visibleItems: 0
            }
        };
    },
    computed: {
        totalHeight() {
            return this.listData.length * 120;
        }
    },
    watch: {
        listData: {
            handler(newData) {
                const newLength = newData.length;
                console.log('=== WATCH listData ===', {
                    newLength,
                    lastLength: this.lastDataLength,
                    isAppending: newLength > this.lastDataLength && this.lastDataLength > 0
                });
                
                if (newLength > this.lastDataLength && this.lastDataLength > 0) {
                    console.log('✅ 检测到追加操作，跳过setListData');
                } else {
                    console.log('🔄 初始加载或替换，调用setListData');
                }
                
                this.lastDataLength = newLength;
            },
            deep: true
        }
    },
    async mounted() {
        console.log('=== ListView mounted ===');
        await this.loadInitialData();
    },
    methods: {
        async loadInitialData() {
            this.isLoading = true;
            try {
                const result = await fetchListData(1);
                console.log('🔍 检查API结果:', result);
                
                if (result && Array.isArray(result.data)) {
                    this.listData = await this.formatPosts(result.data);
                    this.currentPage = 1;
                    console.log('✅ 初始数据加载完成:', this.listData.length);
                } else {
                    console.error('❌ API返回数据结构不正确:', result);
                }
            } catch (error) {
                console.error('❌ 初始数据加载失败:', error);
            } finally {
                this.isLoading = false;
            }
        },

        async handleLoadMore() {
            if (this.isLoadingMore) {
                console.log('⏸️ 正在加载，跳过');
                return;
            }
            
            this.isLoadingMore = true;
            console.log('=== handleLoadMore ===', '当前页:', this.currentPage);
            
            try {
                const nextPage = this.currentPage + 1;
                const result = await fetchListData(nextPage);
                
                if (result && Array.isArray(result.data) && result.data.length > 0) {
                    const newPosts = await this.formatPosts(result.data);
                    console.log('📥 准备追加数据:', newPosts.length, '条');
                    
                    this.listData.push(...newPosts);
                    this.currentPage = nextPage;
                    
                    console.log('✅ 数据追加完成，总数:', this.listData.length);
                } else {
                    console.log('⚠️ 没有更多数据了');
                }
            } catch (error) {
                console.error('❌ 加载更多失败:', error);
            } finally {
                setTimeout(() => {
                    this.isLoadingMore = false;
                }, 500);
            }
        },

        async formatPosts(posts) {
            return Promise.all(posts.map(async (post) => {
                let imageUrl = '';
                if (post.images && post.images.length > 0) {
                    imageUrl = post.images[0].url || '';
                }
                
                return {
                    id: post.postId || '',
                    title: post.title || '无标题',
                    content: post.subject && post.subject[0] ? post.subject[0] : '',
                    author: post.author?.maskName || post.author?.authorName || '匿名',
                    time: post.creationDate ? new Date(parseInt(post.creationDate)).toLocaleString('zh-CN') : '',
                    replies: post.commentCount || 0,
                    views: post.viewCount || 0,
                    imageUrl: imageUrl
                };
            }));
        },

        handleScroll(scrollY) {
            this.scrollY = scrollY;
        },

        handlePerformance(perfData) {
            this.performanceData = perfData;
        },

        handleItemClick(clickInfo) {
            console.log('📌 列表项被点击:', clickInfo);
            
            // 可以在这里添加点击后的行为，比如：
            // - 显示详情弹窗
            // - 路由跳转到详情页
            // - 高亮选中项
            // - 执行其他业务逻辑
            
            const { index, data } = clickInfo;
            alert(`点击了第 ${index + 1} 项\n标题: ${data.title}\n作者: ${data.author}`);
        },

        scrollToTop() {
            this.$refs.scroll.scrollTo(0);
        },

        scrollToBottom() {
            this.$refs.scroll.scrollTo(this.totalHeight);
        },

        toggleGPU() {
            this.useGPU = !this.useGPU;
            console.log('🔧 GPU加速:', this.useGPU ? '开启' : '关闭');
        },

        goToCamera() {
            this.$router.push('/picture');
        },

        goToTests() {
            this.$router.push('/tests');
        }
    }
};

// 导出工具函数供子组件使用
export { loadImageWithReferer };
</script>

<style scoped>
.list-view {
    width: 100%;
    height: 100%;
    display: flex;
    flex-direction: column;
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
    margin-right: 10px;
}

.nav-btn.tests {
    background: rgba(46, 213, 115, 0.3);
    border-color: #2ed573;
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

.control-btn.active {
    background: rgba(16, 185, 129, 0.6);
    border-color: #10b981;
}

/* ========== Stats Display ========== */
.stats {
    position: fixed;
    top: 80px;
    right: 20px;
    background: rgba(0, 0, 0, 0.7);
    color: white;
    padding: 15px;
    border-radius: 10px;
    font-size: 12px;
    line-height: 1.6;
    z-index: 100;
    max-width: 200px;
}

.stats-title {
    font-weight: bold;
    margin-bottom: 5px;
    font-size: 14px;
    color: #10b981;
}

/* ========== Loading/Empty State ========== */
.message {
    position: absolute;
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);
    text-align: center;
    color: #666;
    font-size: 16px;
}

.spinner {
    width: 40px;
    height: 40px;
    border: 4px solid #f3f3f3;
    border-top: 4px solid #667eea;
    border-radius: 50%;
    animation: spin 1s linear infinite;
    margin: 0 auto 10px;
}

@keyframes spin {
    0% { transform: rotate(0deg); }
    100% { transform: rotate(360deg); }
}
</style>
