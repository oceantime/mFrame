//
// Created by 胡帅 on 8/9/17.
//

#ifndef ANDROID_WmCanvas_PROXY_H
#define ANDROID_WmCanvas_PROXY_H

#include <WmCanvasWeex.hpp>

#include "Wmrenderer.h"

using namespace WmCanvas;

class GRenderer;

class WmCanvasWeexAndroid : public WmCanvasWeex {
public:
    WmCanvasWeexAndroid(std::string componentid, GRenderer *context);

    ~WmCanvasWeexAndroid();

    void signalUpGLthread();

    void setRefreshFlag(bool refresh);


public:
    GRenderer *mRender;
};


#endif //ANDROID_WmCanvas_PROXY_H