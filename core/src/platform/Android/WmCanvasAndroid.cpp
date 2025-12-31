/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

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



