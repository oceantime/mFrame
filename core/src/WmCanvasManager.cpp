#include "WmCanvasManager.h"
#ifdef IOS
#include "WmShaderManager.h"
#endif

namespace wmcanvas {

/**
 * @brief CanvasManager包装结构体
 * 用于实现单例模式的智能指针包装
 */
typedef struct {
    std::shared_ptr<WmCanvasManager> sCanvasMgr;
} WmCanvasManagerWrapStruct;

/**
 * @brief 获取静态Manager实例
 * @return CanvasManager包装结构体
 */
WmCanvasManagerWrapStruct StaticManager() {
    static WmCanvasManagerWrapStruct s;
    if (s.sCanvasMgr.get() == nullptr) {
        s.sCanvasMgr = std::shared_ptr<WmCanvasManager>(new WmCanvasManager());
    }
    return s;
}

/**
 * @brief 获取CanvasManager单例
 * @return CanvasManager指针
 */
WmCanvasManager* WmCanvasManager::GetManager() {
    WmCanvasManagerWrapStruct s = StaticManager();
    return s.sCanvasMgr.get();
}

/**
 * @brief 释放CanvasManager单例
 */
void WmCanvasManager::Release() {
    WmCanvasManagerWrapStruct s = StaticManager();
    if (s.sCanvasMgr.get() != nullptr) {
        s.sCanvasMgr.reset();
    }
}

/**
 * @brief 构造函数
 */
WmCanvasManager::WmCanvasManager() {}

/**
 * @brief 析构函数：清理所有Canvas资源
 */
WmCanvasManager::~WmCanvasManager() {
    Clear();
}

/**
 * @brief 创建新的Canvas实例
 * @param canvasId Canvas的唯一标识符
 * @param config Canvas配置参数
 * @param hooks Canvas钩子回调函数指针
 * @return Canvas指针（如果已存在则返回已有实例）
 */
WmCanvas* WmCanvasManager::NewCanvas(const std::string canvasId, WmCanvasConfig config, WmCanvasHooks* hooks) {
    WmCanvas* c = GetCanvas(canvasId);
    if (!c) {
        c = new WmCanvas(canvasId, config, hooks);
        c->CreateContext();
        mCanvases[canvasId] = c;
    }
    return c;
}

/**
 * @brief 移除指定Canvas实例
 * @param canvasId Canvas的唯一标识符
 */
void WmCanvasManager::RemoveCanvas(const std::string canvasId) {
    std::map<std::string, WmCanvas*>::iterator it = mCanvases.find(canvasId);
    if (it != mCanvases.end()) {
        delete it->second;
        it->second = nullptr;
        mCanvases.erase(it);
    }

#ifdef IOS
    // iOS平台：移除最后一个canvas时释放manager
    if (mCanvases.size() == 0) {
        WmShaderManager::release();
        Release();
    }
#endif
}

/**
 * @brief 根据ID获取Canvas实例
 * @param canvasId Canvas的唯一标识符
 * @return Canvas指针（如果不存在则返回nullptr）
 */
WmCanvas* WmCanvasManager::GetCanvas(const std::string canvasId) {
    std::map<std::string, WmCanvas*>::iterator it = mCanvases.find(canvasId);
    if (it != mCanvases.end()) {
        return it->second;
    }
    return nullptr;
}

/**
 * @brief 清除所有Canvas实例和命令队列
 */
void WmCanvasManager::Clear() {
    // 清理所有Canvas实例
    std::map<std::string, WmCanvas*>::iterator it = mCanvases.begin();
    for (; it != mCanvases.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = nullptr;
        }
    }
    mCanvases.clear();

#ifdef WMCANVAS
#ifdef ANDROID
    // Android平台：清理命令队列
    std::map<std::string, std::queue<struct WmCanvasCmd*>*>::iterator it1 = mCmdQueue.begin();
    for (; it1 != mCmdQueue.end(); ++it1) {
        if (it1->second) {
            clearQueue(it1->second);
            delete it1->second;
            it1->second = nullptr;
        }
    }
    mCmdQueue.clear();
#endif
#endif
}

/**
 * @brief 添加Canvas实例到管理器
 * @param p Canvas指针
 */
void WmCanvasManager::AddCanvas(WmCanvas* p) {
    mCanvases[p->mContextId] = p;
}

/**
 * @brief 获取当前Canvas实例数量
 * @return Canvas实例数量
 */
int WmCanvasManager::CanvasCount() {
    return (int)mCanvases.size();
}

}  // namespace wmcanvas
