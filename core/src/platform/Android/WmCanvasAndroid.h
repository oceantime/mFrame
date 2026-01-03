/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef WMCANVAS_WMCANVASANDROID_H
#define WMCANVAS_WMCANVASANDROID_H


#include "WmCanvas.hpp"
#include "WmCanvas2DContextAndroid.h"


class WmCanvasAndroid : public wmcanvas::WmCanvas {


public:


    API_EXPORT WmCanvasAndroid(std::string contextId, const WmCanvasConfig& config, WmCanvasHooks *hooks = nullptr);


    void CreateContext() override;


    API_EXPORT WmCanvas2DContextAndroid* GetCanvasContextAndroid();


};


#endif //WMCANVAS_WMCANVASANDROID_H
