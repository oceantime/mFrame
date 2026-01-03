/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef WMCANVAS_WMCANVASMANAGER_H
#define WMCANVAS_WMCANVASMANAGER_H

#include "WmCanvas.hpp"

#include <map>
#include <queue>

namespace wmcanvas
{
class WmCanvasManager
{
public:
    WmCanvasManager();
    virtual ~WmCanvasManager();

    WmCanvas* NewCanvas(const std::string canvasId, WmCanvasConfig config, WmCanvasHooks *hooks = nullptr);
    void RemoveCanvas(const std::string canvasId);
    WmCanvas *GetCanvas(const std::string canvasId);
    void AddCanvas(WmCanvas *p);
    int CanvasCount();
    void Clear();
#ifdef ANDROID
    void AddtoQueue(const std::string contextId,struct GCanvasCmd *);
    std::queue<struct GCanvasCmd *> * getQueueByContextId(const std::string contextId);
    void clearQueueByContextId(const std::string contextId);
    void clearQueue(std::queue<struct GCanvasCmd *> *queue);
#endif
    static WmCanvasManager *GetManager();
    static void Release();

protected:
    std::map< std::string, WmCanvas * > mCanvases;
#ifdef ANDROID
    std::map<std::string,std::queue<struct GCanvasCmd *> *> mCmdQueue;
#endif
    
};

using GCanvasManager = WmCanvasManager;

}

#endif /* WMCANVAS_WMCANVASMANAGER_H */
