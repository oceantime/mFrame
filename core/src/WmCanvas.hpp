#ifndef WMCANVAS_WMCANVAS_H
#define WMCANVAS_WMCANVAS_H

#include "export.h"
#include "support/Log.h"
#include "wmcanvas/WmCanvas2dContext.h"

namespace wmcanvas {

/**
 * @brief WmCanvas类：Canvas画布的主要管理类
 * 
 * 该类负责管理Canvas的生命周期、上下文创建和Surface变化处理
 */
class WmCanvas {
   public:
    /**
     * @brief 构造函数
     * @param canvasId Canvas的唯一标识符
     * @param config Canvas配置参数
     * @param hooks Canvas钩子回调函数指针（可选）
     */
    API_EXPORT WmCanvas(std::string canvasId, const WmCanvasConfig& config, WmCanvasHooks* hooks = nullptr);
    
    /**
     * @brief 虚析构函数
     */
    API_EXPORT virtual ~WmCanvas();
    
    /**
     * @brief 创建Canvas上下文
     */
    virtual void CreateContext();
    
    /**
     * @brief 清除Canvas内容
     */
    virtual void Clear();

    /**
     * @brief Surface尺寸变化回调
     * @param x Surface的X坐标
     * @param y Surface的Y坐标
     * @param width Surface的宽度
     * @param height Surface的高度
     */
    API_EXPORT void OnSurfaceChanged(int x, int y, int width, int height);

    /**
     * @brief 获取Canvas上下文对象
     * @return Canvas上下文指针
     */
    WmCanvasContext* GetGCanvasContext() { return mCanvasContext; }
    
    /**
     * @brief 获取设备像素比
     * @return 设备像素比值
     */
    float GetDevicePixelRatio() { return mCanvasContext->GetDevicePixelRatio(); }
    
    /**
     * @brief 检查Canvas是否准备就绪
     * @return 如果Canvas已准备就绪返回true，否则返回false
     */
    API_EXPORT bool IsGCanvasReady();

    /**
     * @brief 绘制当前帧
     */
    API_EXPORT void drawFrame();

   public:
    std::string mContextId;          // Canvas的唯一标识符
    WmCanvasConfig mConfig;          // Canvas配置参数
    WmCanvasHooks* mHooks;           // Canvas钩子回调函数指针
    WmCanvasContext* mCanvasContext; // Canvas上下文对象指针
};

}  // namespace wmcanvas

#endif /* WMCANVAS_WMCANVAS_H */
