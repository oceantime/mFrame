/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#ifndef WMCANVAS_WMCANVAS_H
#define WMCANVAS_WMCANVAS_H

#include "wmcanvas/WmCanvas2dContext.h"
#include "support/Log.h"
#include "export.h"

namespace wmcanvas
{

class WmCanvas
{
public:
    API_EXPORT WmCanvas(std::string canvasId, const WmCanvasConfig& config, WmCanvasHooks *hooks = nullptr);
    API_EXPORT virtual ~WmCanvas();
    virtual void CreateContext();
    virtual void Clear();
    
    API_EXPORT void OnSurfaceChanged(int x, int y, int width, int height);
    
    WmCanvasContext* GetGCanvasContext() { return mCanvasContext; }
    float GetDevicePixelRatio() { return mCanvasContext->GetDevicePixelRatio();}
    API_EXPORT bool IsGCanvasReady();
    
    API_EXPORT void drawFrame();

public:
    std::string mContextId;
    WmCanvasConfig mConfig;
    WmCanvasHooks *mHooks;
    
    WmCanvasContext *mCanvasContext;
};
}

#endif /* GCANVAS_GCANVAS_H */
