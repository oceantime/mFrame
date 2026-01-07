#ifndef WMCANVAS_WMCANVASMANAGER_H
#define WMCANVAS_WMCANVASMANAGER_H

#include "WmCanvas.hpp"

#include <map>
#include <queue>

namespace wmcanvas {

/**
 * @brief WmCanvasManager类：Canvas管理器
 * 
 * 该类负责管理所有Canvas实例的创建、删除和查询
 * 使用单例模式确保全局唯一的管理器实例
 */
class WmCanvasManager {
   public:
    /**
     * @brief 构造函数
     */
    WmCanvasManager();
    
    /**
     * @brief 虚析构函数
     */
    virtual ~WmCanvasManager();

    /**
     * @brief 创建新的Canvas实例
     * @param canvasId Canvas的唯一标识符
     * @param config Canvas配置参数
     * @param hooks Canvas钩子回调函数指针（可选）
     * @return Canvas指针（如果已存在则返回已有实例）
     */
    WmCanvas* NewCanvas(const std::string canvasId, WmCanvasConfig config, WmCanvasHooks* hooks = nullptr);
    
    /**
     * @brief 移除指定Canvas实例
     * @param canvasId Canvas的唯一标识符
     */
    void RemoveCanvas(const std::string canvasId);
    
    /**
     * @brief 根据ID获取Canvas实例
     * @param canvasId Canvas的唯一标识符
     * @return Canvas指针（如果不存在则返回nullptr）
     */
    WmCanvas* GetCanvas(const std::string canvasId);
    
    /**
     * @brief 添加Canvas实例到管理器
     * @param p Canvas指针
     */
    void AddCanvas(WmCanvas* p);
    
    /**
     * @brief 获取当前Canvas实例数量
     * @return Canvas实例数量
     */
    int CanvasCount();
    
    /**
     * @brief 清除所有Canvas实例和命令队列
     */
    void Clear();

#ifdef ANDROID
    /**
     * @brief 添加命令到队列（Android平台专用）
     * @param contextId 上下文标识符
     * @param cmd 命令指针
     */
    void AddtoQueue(const std::string contextId, struct WmCanvasCmd*);
    
    /**
     * @brief 根据上下文ID获取命令队列（Android平台专用）
     * @param contextId 上下文标识符
     * @return 命令队列指针
     */
    std::queue<struct WmCanvasCmd*>* getQueueByContextId(const std::string contextId);
    
    /**
     * @brief 根据上下文ID清除命令队列（Android平台专用）
     * @param contextId 上下文标识符
     */
    void clearQueueByContextId(const std::string contextId);
    
    /**
     * @brief 清除指定命令队列（Android平台专用）
     * @param queue 命令队列指针
     */
    void clearQueue(std::queue<struct WmCanvasCmd*>* queue);
#endif

    /**
     * @brief 获取CanvasManager单例
     * @return CanvasManager指针
     */
    static WmCanvasManager* GetManager();
    
    /**
     * @brief 释放CanvasManager单例
     */
    static void Release();

   protected:
    std::map<std::string, WmCanvas*> mCanvases; // Canvas实例映射表
#ifdef ANDROID
    std::map<std::string, std::queue<struct WmCanvasCmd*>*> mCmdQueue; // 命令队列映射表（Android平台）
#endif
};

}  // namespace wmcanvas

#endif /* WMCANVAS_WMCANVASMANAGER_H */
