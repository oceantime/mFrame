#include "WmCanvasManager.h"
#ifdef IOS
#include "WmShaderManager.h"
#endif

namespace wmcanvas
{
typedef struct {
    std::shared_ptr<WmCanvasManager> sCanvasMgr;
} WmCanvasManagerWrapStruct;


WmCanvasManagerWrapStruct StaticManager()
{
    static WmCanvasManagerWrapStruct s;
    if( s.sCanvasMgr.get() == nullptr )
    {
        s.sCanvasMgr = std::shared_ptr<WmCanvasManager>(new WmCanvasManager());
    }
    return s;
}

/*static*/
WmCanvasManager *WmCanvasManager::GetManager()
{
    WmCanvasManagerWrapStruct s = StaticManager();
    return s.sCanvasMgr.get();
}

/*static*/
void WmCanvasManager::Release()
{
    WmCanvasManagerWrapStruct s = StaticManager();

    if ( s.sCanvasMgr.get() != nullptr)
    {
        s.sCanvasMgr.reset();
    }
}

WmCanvasManager::WmCanvasManager()
{
}

WmCanvasManager::~WmCanvasManager()
{
    Clear();
}

WmCanvas* WmCanvasManager::NewCanvas(const std::string canvasId, WmCanvasConfig config, WmCanvasHooks *hooks)
{
    WmCanvas *c = GetCanvas(canvasId);
    if (!c)
    {
        c = new WmCanvas(canvasId, config, hooks);
        c->CreateContext();
        mCanvases[canvasId] = c;
    }
    
    return c;
}

void WmCanvasManager::RemoveCanvas(const std::string canvasId)
{
    std::map< std::string, WmCanvas * >::iterator it = mCanvases.find(canvasId);
    if (it != mCanvases.end())
    {
        delete it->second;
        it->second = 0;
        mCanvases.erase(it);
    }
    
#ifdef IOS
    //release manager while remove last canvas
    if( mCanvases.size() == 0 )
    {
        WmShaderManager::release();
        Release();
    }
#endif

}
WmCanvas *WmCanvasManager::GetCanvas(const std::string canvasId)
{
    std::map< std::string, WmCanvas * >::iterator it = mCanvases.find(canvasId);
    if (it != mCanvases.end())
    {
        return it->second;
    }
    return 0;
}

void WmCanvasManager::Clear()
{
    std::map< std::string, WmCanvas * >::iterator it = mCanvases.begin();
    for (; it != mCanvases.end(); ++it)
    {
        if (it->second)
        {
            delete it->second;
            it->second = 0;
        }
    }
    mCanvases.clear();
#ifdef WMCANVAS
#ifdef ANDROID
    std::map<std::string, std::queue<struct GCanvasCmd *> *>::iterator it1 = mCmdQueue.begin();
    for (; it1 != mCmdQueue.end(); ++it1)
    {
        if (it1->second)
        {
            clearQueue(it1->second);
            delete it1->second;
            it1->second = 0;
        }
    }
    mCmdQueue.clear();
#endif
#endif
}


void WmCanvasManager::AddCanvas(WmCanvas *p)
{
    mCanvases[p->mContextId] = p;
}

int WmCanvasManager::CanvasCount()
{
    return (int)mCanvases.size();
}

}
