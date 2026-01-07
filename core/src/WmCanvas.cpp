#include <errno.h>
#include <sstream>

#include "WmCanvas.hpp"
#include "WmCanvas2dContext.h"
#include "support/CharacterSet.h"
#include "wmcanvas/WmPoint.h"

#ifdef ANDROID

#include "platform/Android/WmCanvas2DContextAndroid.h"
#include "platform/Android/WmFontCache.h"

#endif

namespace wmcanvas {

/**
 * @brief 构造函数：创建Canvas实例
 * @param canvasId Canvas的唯一标识符
 * @param config Canvas配置参数
 * @param hooks Canvas钩子回调函数指针
 */
WmCanvas::WmCanvas(std::string canvasId, const WmCanvasConfig& config, WmCanvasHooks* hooks)
    : mContextId(canvasId), mConfig(config) {
    mHooks = hooks;
    LOG_D("创建Canvas实例");
}

/**
 * @brief 创建Canvas上下文
 */
void WmCanvas::CreateContext() {
    mCanvasContext = new WmCanvasContext(0, 0, mConfig, mHooks);
    mCanvasContext->mContextId = this->mContextId;
}

/**
 * @brief 清除Canvas内容
 */
void WmCanvas::Clear() {
    LOG_D("Canvas::DoContextLost 开始");
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    LOG_D("Canvas::DoContextLost 结束");
}

/**
 * @brief 析构函数：释放Canvas资源
 */
WmCanvas::~WmCanvas() {
    if (mCanvasContext != nullptr) {
        delete mCanvasContext;
        mCanvasContext = nullptr;
    }
}

/**
 * @brief Surface尺寸变化回调函数（Canvas创建时会主动调用一次）
 * @param x Surface的X坐标
 * @param y Surface的Y坐标
 * @param width Surface的宽度
 * @param height Surface的高度
 */
void WmCanvas::OnSurfaceChanged(int x, int y, int width, int height) {
    GLint maxRenderbufferSize;
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);

    // 如果宽度或高度为0，直接返回
    if (width == 0 || height == 0) {
        return;
    }

    // 如果尺寸发生变化，更新上下文尺寸并重新初始化GL环境
    if (mCanvasContext->mWidth != width || mCanvasContext->mHeight != height) {
        mCanvasContext->mX = x;
        mCanvasContext->mY = y;
        mCanvasContext->mWidth = width;
        mCanvasContext->mHeight = height;
        mCanvasContext->InitializeGLEnvironment();
    }
    mCanvasContext->SetContextLost(false);
}

/**
 * @brief 检查Canvas是否准备就绪
 * @return 如果Canvas已准备就绪返回true，否则返回false
 */
bool WmCanvas::IsGCanvasReady() {
    return mCanvasContext->mIsContextReady;
}

/**
 * @brief 绘制当前帧
 */
void WmCanvas::drawFrame() {
    mCanvasContext->SendVertexBufferToGPU();
}

}  // namespace wmcanvas
