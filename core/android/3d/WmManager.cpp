
#include "WmManager.h"


using namespace std;
using namespace wmcanvas;

AutoPtr<WmManager> WmManager::theManager;

WmManager *WmManager::getSingleton() {
    if (theManager.IsNULL()) {
        theManager = new WmManager();
    }
    return theManager.RawData();
}


void WmManager::release() {
    if (!theManager.IsNULL()) {
        theManager = nullptr;
    }
}

WmManager::WmManager() {
}

WmManager::~WmManager() {
    map<string, WmRenderer*>::iterator it = m_renderMap.begin();
    for (; it != m_renderMap.end(); ++it) {
        if (it->second) {
            delete it->second;
            it->second = 0;
        }
    }
    m_renderMap.clear();
}


WmRenderer* WmManager::findRenderer(const std::string& key) {
    map<string, WmRenderer*>::iterator it = m_renderMap.find(key);
    if(it != m_renderMap.end()){
        return it->second;
    }

    return NULL;
}

void WmManager::removeRenderer(const std::string& key) {
    map<string, WmRenderer*>::iterator it = m_renderMap.find(key);
    if(it != m_renderMap.end()){
        it->second->stop();
        delete it->second;
        it->second = 0;
        m_renderMap.erase(key);
    }
}


WmRenderer* WmManager::newRenderer(const std::string& key) {
    WmRenderer* renderer = findRenderer(key);
    if(!renderer){
        renderer = new WmRenderer(key);
        m_renderMap[key] = renderer;
    }

    return renderer;
}
