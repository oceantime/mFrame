//
// Created by yuantong on 2017/8/1.
//

#ifndef WM_CANVAS_WMMANAGER_H
#define WM_CANVAS_WMMANAGER_H

#include "view/WmRenderer.h"
#include "util/auto_ptr.h"
#include <map>
#include <string>
#include <memory/AutoPtr.h>

using namespace WmCanvas;

class WmManager{
public:
    WmRenderer* findRenderer(const std::string& key);
    void removeRenderer(const std::string& key);
    WmRenderer* newRenderer(const std::string& key);
    WmManager();
    virtual ~WmManager();

private:
    static AutoPtr<WmManager> theManager;
    std::map<std::string, WmRenderer*> m_renderMap;

public:
    static WmManager* getSingleton();
    static void release();
};

#endif //WM_CANVAS_WMMANAGER_H
