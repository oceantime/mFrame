//
// Created by yuantong on 2018/4/16.
//

#ifndef WMCANVAS_GRENDER_H
#define WMCANVAS_GRENDER_H

#include <EGL/egl.h>
#include <android/native_window.h>
#include <stdint.h>
#include <string>
#include <support/Log.h>



#define EGL_LOGI(T, ...)  LOG_I("EGL(%s):" T, (this->traceId.data()), ##__VA_ARGS__)

#define EGL_LOGE(T, ...)  LOG_E("EGL(%s):" T, (this->traceId.data()), ##__VA_ARGS__)


// FIXME
API_EXPORT extern EGLContext currentEGLContext;


/**
 * A helper class for EGL resource management.
 */
class WmEGLContext {

public:

    WmEGLContext(EGLDisplay sharedDisplay, EGLContext  sharedContext):eglDisplay(sharedDisplay),
        eglSharedContext(sharedContext), isEGLInit(false), width(0), height(0) {
    }

    WmEGLContext() {}

    virtual ~WmEGLContext() {};

    virtual bool Init(ANativeWindow *window) { return true; };

    inline bool HasInit() {
        return isEGLInit;
    }

    virtual ANativeWindow *GetWindow() const { return nullptr; }

    virtual EGLint Resume(ANativeWindow *window) { return EGL_SUCCESS; };


    virtual bool Resize(int32_t width, int32_t height) { return true; };


    virtual void Pause() {}


    virtual EGLint SwapBuffer() = 0;


    virtual bool MakeCurrent() = 0;


    virtual bool IsPreserveBackBuffer() = 0;


    inline bool IsContextValid() {
        return eglDisplay != EGL_NO_DISPLAY && eglContext != EGL_NO_CONTEXT;
    }


    inline bool IsValid() {
        return EGL_NO_CONTEXT != eglContext && EGL_NO_DISPLAY != eglDisplay && eglSurface != EGL_NO_SURFACE;
    }


    inline EGLDisplay GetEGLDisplay() {
        return eglDisplay;
    }


    inline EGLContext GetEGLContext() {
        return eglContext;
    }


    inline EGLSurface GetEGLSurface() const { return eglSurface; }


    inline bool IsSharingContext() { return eglSharedContext != EGL_NO_CONTEXT;}


    inline int32_t GetWidth() const { return width; }


    inline int32_t GetHeight() const { return height; }


    inline float GetGLESVersion() {
        return glVersion;
    }


    inline void SetUsePreserveBackBuffer(bool v) {
        usePreserveBackBuffer = v;
    }


    inline int32_t GetColorSize() const { return colorSize; }


    inline int32_t GetDepthSize() const { return depthSize; }


    inline void SetTraceId(std::string traceId) {
        this->traceId = traceId;
    }


    inline bool ClearCurrent() {
        EGLBoolean r = eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (r == EGL_TRUE) {
            currentEGLContext = nullptr;
        }
        return r == EGL_TRUE;
    }



protected:

    EGLDisplay eglDisplay = EGL_NO_DISPLAY;

    EGLContext eglContext = EGL_NO_CONTEXT;

    EGLContext eglSharedContext = EGL_NO_CONTEXT;

    EGLSurface eglSurface = EGL_NO_SURFACE;

    EGLConfig eglConfig;

    bool isEGLInit = false;

    float glVersion;

    bool usePreserveBackBuffer;


    std::string traceId;


    int32_t width;
    int32_t height;

    int32_t colorSize;
    int32_t depthSize;

};





/**
 * A helper class for EGL resource management.
 */
class WmEGLWindowContext : public WmEGLContext
{
protected:
    ANativeWindow *nWindow;
    bool isGLESInit;
    bool isES3Supported;
    bool isSwapRequest;


    bool InitEGLSurface();

    bool InitEGLContext();

    void Terminate();

public:

    API_EXPORT WmEGLWindowContext(EGLDisplay sharedDisplay, EGLContext sharedContext);

    API_EXPORT WmEGLWindowContext();

    ~WmEGLWindowContext();


    API_EXPORT EGLint SwapBuffer() override ;

    bool Init(ANativeWindow *window) override;

    bool Invalidate();


    API_EXPORT EGLint Resume(ANativeWindow *window) override;


    void Pause() override;


    ANativeWindow *GetWindow() const override  { return nWindow; }


    bool Resize(int32_t newWidth, int32_t newHeight) override;

    bool MakeCurrent() override;


    EGLint MakeCurrentWithScene(const char* scene, bool check = true);


    EGLSurface CreateWindowSurface(std::string& scene);


    bool IsPreserveBackBuffer() override;


private:

    void DestroyEGLSurfaceIf();

};



/**
 * ¿Î∆¡Context
 */
class WmEGLPbufferContext : public WmEGLContext
{
protected:

    bool isGLESInit;
    bool isES3Supported;
    bool isSwapRequest;

    std::string traceId;

    void InitGLES();

    bool InitEGLSurface();

    bool InitEGLContext();

    void Terminate();

public:

    API_EXPORT WmEGLPbufferContext(EGLDisplay sharedDisplay, EGLContext sharedContext, int w, int h);

    API_EXPORT WmEGLPbufferContext(int w, int h);

    ~WmEGLPbufferContext();


    EGLint SwapBuffer() override ;

    bool Init(ANativeWindow *window) override;

    bool Invalidate();

    void Pause() override ;


    bool MakeCurrent() override ;



    bool IsPreserveBackBuffer() override;


};



class WmSharedEGLContext : public WmEGLContext {

public:

    API_EXPORT WmSharedEGLContext();


    ~WmSharedEGLContext();


    bool MakeCurrent() override;


    EGLint SwapBuffer() override;


    bool IsPreserveBackBuffer() override;


};


#endif //WMCANVAS_GRENDER_H
