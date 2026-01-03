#include <errno.h>
#include <sstream>

#include "wmcanvas/WmPoint.h"
#include "WmCanvas.hpp"
#include "WmCanvas2dContext.h"

#ifdef ANDROID

#include "platform/Android/WmFontCache.h"
#include "platform/Android/WmCanvas2DContextAndroid.h"
#include "support/CharacterSet.h"

#endif

namespace wmcanvas {

WmCanvas::WmCanvas(std::string canvasId, const WmCanvasConfig &config, WmCanvasHooks *hooks) :
        mContextId(canvasId),
        mConfig(config)
{
    mHooks = hooks;
    LOG_D("Create Canvas");
}

void WmCanvas::CreateContext() {
    mCanvasContext = new WmCanvasContext(0, 0, mConfig, mHooks);
    mCanvasContext->mContextId = this->mContextId;
}

void WmCanvas::Clear() {
    LOG_D("Canvas::DoContextLost start.");
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    LOG_D("Canvas::DoContextLost end.");
}


WmCanvas::~WmCanvas() {
    if (mCanvasContext != NULL) {
        delete mCanvasContext;
        mCanvasContext = NULL;
    }
    // Clear();
}


/**
 * View大小变化回调(GCanvas创建时会主动调一次)
 */
void WmCanvas::OnSurfaceChanged(int x, int y, int width, int height) {
    GLint maxRenderbufferSize;
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);
    //                       "<function:%s, maxSize:%d, width:%d, height:%d>", __FUNCTION__,
    //                       maxRenderbufferSize, width, height);
    //     return;
    // }

    if (width == 0 || height == 0) {
        return;
    }

    if (mCanvasContext->mWidth != width || mCanvasContext->mHeight != height) {
        mCanvasContext->mX = x;
        mCanvasContext->mY = y;
        mCanvasContext->mWidth = width;
        mCanvasContext->mHeight = height;
        mCanvasContext->InitializeGLEnvironment();
    }
    mCanvasContext->SetContextLost(false);
}

bool WmCanvas::IsGCanvasReady()
{
    return mCanvasContext->mIsContextReady;
}

void WmCanvas::drawFrame() {
    mCanvasContext->SendVertexBufferToGPU();
}

}
