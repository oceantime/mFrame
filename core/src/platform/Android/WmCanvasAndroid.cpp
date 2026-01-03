#include "WmCanvasAndroid.h"
#include "WmCanvas2DContextAndroid.h"
#include "WmCanvas.hpp"


WmCanvasAndroid::WmCanvasAndroid(std::string contextId, const WmCanvasConfig& config, WmCanvasHooks *hooks) :
    WmCanvas(contextId, config, hooks) {
    CreateContext();
}


void WmCanvasAndroid::CreateContext() {
    mCanvasContext = new WmCanvas2DContextAndroid(0, 0, mConfig, mHooks);
    mCanvasContext->mContextId = this->mContextId;
}


WmCanvas2DContextAndroid* WmCanvasAndroid::GetCanvasContextAndroid() {
    return (WmCanvas2DContextAndroid*)mCanvasContext;
}



