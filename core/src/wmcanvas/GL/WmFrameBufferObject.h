#ifndef WMCANVAS_WMFRAMEBUFFEROBJECT_H
#define WMCANVAS_WMFRAMEBUFFEROBJECT_H

#include "WmTexture.h"
#include "WmPoint.h"
#include "WmTransform.h"

#include <map>
#include <string>
#include <vector>
#include <map>
#include <set>

struct WmCanvasLog;

class WmFrameBufferObject
{
public:
    WmFrameBufferObject();

    ~WmFrameBufferObject();


    WmFrameBufferObject(WmFrameBufferObject&& src);


    bool InitFBO(int width, int height, WmColorRGBA color, std::vector<WmCanvasLog> *errVec = nullptr);

    bool InitFBO(int width, int height, WmColorRGBA color, bool enableMsaa, std::vector<WmCanvasLog> *errVec = nullptr);

    void BindFBO();

    void UnbindFBO();

    void DeleteFBO();

    int DetachTexture();

    void GLClearScreen(WmColorRGBA color);


    int Width()
    {
        return mFboTexture.GetWidth();
    }

    int Height()
    {
        return mFboTexture.GetHeight();

    }

    void SetSize(int w, int h)
    {
        mWidth=w;
        mHeight=h;
    }
    int ExpectedWidth()
    {
        return mWidth;
    }

    int ExpectedHeight()
    {
        return mHeight;
    }

    bool mIsFboSupported = true;

    WmTexture mFboTexture;
    GLuint mFboFrame = 0;
    GLuint mFboStencil = 0;
    GLint mSaveFboFrame = 0;
    WmTransform mSavedTransform;

    int mWidth;
    int mHeight;
};

using WmFrameBufferObjectPtr = std::shared_ptr<WmFrameBufferObject>;

class WmFrameBufferObjectPool
{
public:
    typedef std::pair<int, int> Key;
    typedef std::multimap<Key, WmFrameBufferObject*> Map;

public:
    WmFrameBufferObjectPool() = default;

    ~WmFrameBufferObjectPool()
    {
        for(auto& i : mPool)
        {
            delete i.second;
        }
    }

    WmFrameBufferObjectPtr GetFrameBuffer(int width, int height);

private:

    Map mPool;
};

#endif /* WMCANVAS_WMFRAMEBUFFEROBJECT_H */
