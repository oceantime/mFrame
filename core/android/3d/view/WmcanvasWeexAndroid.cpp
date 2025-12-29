//
// Created by 胡帅 on 8/9/17.
//

#include "WmCanvasWeexAndroid.h"
#include "Wmrenderer.h"

using namespace WmCanvas;

WmCanvasWeexAndroid::WmCanvasWeexAndroid(std::string componentid, GRenderer *context) :
        WmCanvasWeex(componentid, WmCanvasConfig({false, true})), mRender(context) {

}

WmCanvasWeexAndroid::~WmCanvasWeexAndroid() {
    mRender = nullptr;
}

/**
 * WmCanvasWeexAndroid和GRenderer形成环状依赖,
 */
void WmCanvasWeexAndroid::signalUpGLthread() {
    if (mRender != nullptr) {
        mRender->signalUpGLthread();
    }
}

void WmCanvasWeexAndroid::setRefreshFlag(bool refresh) {
    if (mRender != nullptr) {
        mRender->setRefreshFlag(refresh);
    }
}
