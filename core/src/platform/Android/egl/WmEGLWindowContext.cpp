
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <string>
#include <support/Log.h>

#include "WmEGLContext.h"





EGLint WmEGLWindowContext::SwapBuffer() {
    if (!IsValid()) {
        // EGL_LOGI("swapBuffer:fail:EGL invalid");
        return false;
    }
    // EGL_LOGI("swapBuffer:start");
    EGLBoolean b = eglSwapBuffers(eglDisplay, eglSurface);
    if (!b) {
        EGLint err = eglGetError();
        // mymock
        // 锟斤拷锟叫碉拷swapBuffer fail锟侥讹拷trace锟斤拷锟斤拷 (锟叫匡拷锟杰碉拷锟斤拷锟斤拷锟襟，碉拷锟斤拷锟斤拷锟捷憋拷锟斤拷?)
        // TRACE_EXCEPTION("eglSwapBuffers_fail", "eglGetError=%x", err);
        if (err == EGL_BAD_SURFACE) { // 锟斤拷要锟斤拷锟皆恢革拷锟斤拷锟斤拷锟戒报锟斤拷
            // DestroyEGLSurfaceIf();
            // InitEGLSurface();
            return err;
        } else if (err == EGL_CONTEXT_LOST || err == EGL_BAD_CONTEXT) {
            EGL_LOGI("Recreate EGLContext on ContextLost|BadContext");
            Terminate();
            InitEGLContext();
        }
        return err;
    }
    return EGL_SUCCESS;
}


WmEGLWindowContext::WmEGLWindowContext() : nWindow(nullptr),
                                         isGLESInit(false),
                                         isES3Supported(false) {}

WmEGLWindowContext::WmEGLWindowContext(EGLDisplay sharedDisplay, EGLContext sharedContext) :
                                                                                        WmEGLContext(sharedDisplay, sharedContext),
                                                                                        nWindow(nullptr),
                                                                                           isGLESInit(false),
                                                                                           isES3Supported(
                                                                                                   false) {}

bool WmEGLWindowContext::Init(ANativeWindow *window) {
    if (isEGLInit) {
        return true;
    }
    nWindow = window;
    // 锟斤拷始锟斤拷eglDisplay & eglSurface(eglSurface锟斤拷eglContext锟睫癸拷锟斤拷)
    bool flag = InitEGLSurface();
    if (flag) {
        flag = InitEGLContext();
    }
    // 锟斤拷锟斤拷丫锟街达拷泄锟斤拷锟绞硷拷锟�(锟斤拷锟叫匡拷锟杰筹拷始锟斤拷失锟斤拷)
    isEGLInit = true;

    return flag;
}


bool WmEGLWindowContext::InitEGLSurface() {
    if (eglDisplay == EGL_NO_DISPLAY) {
        eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    }
    eglInitialize(eglDisplay, 0, 0);

    const EGLint attribs[] = {EGL_RENDERABLE_TYPE,
                              EGL_OPENGL_ES2_BIT,
                              EGL_SURFACE_TYPE,
                              EGL_WINDOW_BIT,
                              EGL_ALPHA_SIZE,
                              8,
                              EGL_BLUE_SIZE,
                              8,
                              EGL_GREEN_SIZE,
                              8,
                              EGL_RED_SIZE,
                              8,
                              EGL_DEPTH_SIZE,
                              24,
                              EGL_STENCIL_SIZE,
                              8,
            // 锟斤拷锟斤拷msaa
             EGL_SAMPLE_BUFFERS, 1,
             EGL_SAMPLES, 4,
                              EGL_NONE};
    colorSize = 8;
    depthSize = 24;

    EGLint num_configs;
    eglChooseConfig(eglDisplay, attribs, &eglConfig, 1, &num_configs);

    EGLint s;
    eglGetConfigAttrib(eglDisplay, eglConfig, EGL_STENCIL_SIZE, &s);

    if (!num_configs) {
        // Fall back to 16bit depth buffer
        const EGLint attribs[] = {EGL_RENDERABLE_TYPE,
                                  EGL_OPENGL_ES2_BIT,
                                  EGL_SURFACE_TYPE,
                                  EGL_WINDOW_BIT,
                                  EGL_ALPHA_SIZE,
                                  8,
                                  EGL_BLUE_SIZE,
                                  8,
                                  EGL_GREEN_SIZE,
                                  8,
                                  EGL_RED_SIZE,
                                  8,
                                  EGL_DEPTH_SIZE,
                                  16,
                                  EGL_STENCIL_SIZE,
                                  8,
                                  EGL_NONE};
        eglChooseConfig(eglDisplay, attribs, &eglConfig, 1, &num_configs);
        depthSize = 16;
    }

    if (!num_configs) {
        return false;
    }

    std::string scene = "InitEGLSurface";
    eglSurface = CreateWindowSurface(scene);
    return eglSurface != EGL_NO_SURFACE;
}


bool WmEGLWindowContext::InitEGLContext() {
    EGL_LOGI("InitEGLContext:start");

    EGLint targetVersion = 3;
    EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION,
                                targetVersion,  // Request opengl ES Version
                                EGL_NONE};
    bool is3 = false;
    eglContext = eglCreateContext(eglDisplay, eglConfig, eglSharedContext, context_attribs);
    if (eglContext == EGL_NO_CONTEXT) {
        is3 = false;
        targetVersion = 2;
        context_attribs[1] = targetVersion;
        eglContext = eglCreateContext(eglDisplay, eglConfig, eglSharedContext, context_attribs);
    }
    glVersion = targetVersion;//is3 ? 3.0f : 2.0f;
    isES3Supported = is3;

    if (eglContext == EGL_NO_CONTEXT) {
        EGLint err = eglGetError();
        // TRACE_EXCEPTION("eglCreateContext_fail", "GLWindowContext.InitEGLContext, eglGetError=%x",
        //                err);
        return false;
    }

    // 锟斤拷始锟斤拷EGLContext时 强锟斤拷MakeCurrent
    EGLint result = MakeCurrentWithScene("InitEGLContext", false);
    EGL_LOGI("InitEGLContext:end,result=%i", (result == EGL_SUCCESS));

    return result == EGL_SUCCESS;
}


bool WmEGLWindowContext::Invalidate() {
    Terminate();
    isEGLInit = false;
    return true;
}


/**
 * 锟斤拷停锟斤拷锟斤拷锟斤拷锟斤拷删锟斤拷EGL Surface
 * FIXME 锟斤拷锟斤拷锟叫碉拷锟斤拷锟斤拷, 锟解部实锟斤拷锟斤拷锟斤拷DestroyEGLSurface
 */
void WmEGLWindowContext::Pause() {
    DestroyEGLSurfaceIf();
    nWindow = nullptr;
}


void WmEGLWindowContext::DestroyEGLSurfaceIf() {
    // 1.锟斤拷clear current锟斤拷锟斤拷锟斤拷gl锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷(锟斤拷锟斤拷gl锟斤拷锟斤拷)
    ClearCurrent();

    // 2.destroy eglSurface(未current锟斤拷surface锟斤拷锟斤拷锟斤拷删锟斤拷)
    if (eglSurface != EGL_NO_SURFACE) {
        EGLBoolean result = eglDestroySurface(eglDisplay, eglSurface);
        eglSurface = EGL_NO_SURFACE;
        EGL_LOGI("DestroySurface:eglDestroySurface()=%i", result);
    }
}


EGLint WmEGLWindowContext::Resume(ANativeWindow *window) {
    EGL_LOGI("Resume:window=%p", window);
    if (!isEGLInit) {
        Init(window);
        return EGL_SUCCESS;
    }

    // 锟斤拷锟絜gl surface锟斤拷效锟斤拷锟叫匡拷锟斤拷锟斤拷锟斤拷锟斤拷锟捷ｏ拷锟斤拷destroy surface(锟斤拷证resume执锟叫成癸拷)
    if (eglSurface != EGL_NO_SURFACE) {
        DestroyEGLSurfaceIf();
    }

    int32_t original_width = width;
    int32_t original_height = height;

    // 锟斤拷锟斤拷window
    nWindow = window;

    std::string scene = "Resume";
    eglSurface = CreateWindowSurface(scene);
    if (width != original_width || height != original_height) {
        EGL_LOGI("Resume:Surface resized");
    }

    EGLint result = MakeCurrentWithScene(scene.data());
    if (result == EGL_SUCCESS) {
        EGL_LOGI("Resume:Success:eglSurface=%p", eglSurface);
        return result;
    }

    // makeCurrent失锟杰ｏ拷锟叫匡拷锟斤拷锟斤拷context锟斤拷锟斤拷锟斤拷
    EGL_LOGI("Resume:MakeCurrent Fail");
    if (result == EGL_CONTEXT_LOST) {
        // Recreate context
        EGL_LOGI("Recreating egl context");
        return InitEGLContext();
    } else {
        // Recreate surface
        EGL_LOGI("Recreating Surface when Resume Fail");
        Terminate();
        InitEGLSurface();
        bool flag = InitEGLContext();
        return flag;
    }
}


bool WmEGLWindowContext::Resize(int32_t newWidth, int32_t newHeight) {
    // 只要锟斤拷Resize锟斤拷锟矫ｏ拷锟斤拷使锟竭达拷一锟斤拷锟斤拷也强锟狡达拷锟斤拷Resize锟竭硷拷(锟叫匡拷锟斤拷Surface锟窖撅拷锟斤拷锟斤拷锟戒化)
    EGL_LOGI("Resize:w=%i,h=%i", newWidth, newHeight);
    // 1.Destroy EGLSurface
    DestroyEGLSurfaceIf();

    // 2.Create New EGLSurface
    std::string scene = "Resize";
    eglSurface = CreateWindowSurface(scene);
    if (eglSurface == EGL_NO_SURFACE) {
        return false;
    }

    // 3.MakeCurrent
    EGLint result = MakeCurrentWithScene(scene.data());
    EGL_LOGI("Resize:finish,makeCurrent result=%i", result);

    return result == EGL_SUCCESS;
}


bool WmEGLWindowContext::MakeCurrent() {
    return MakeCurrentWithScene("Render") == EGL_SUCCESS;
}


// surface 状态锟斤拷锟�
EGLint WmEGLWindowContext::MakeCurrentWithScene(const char *scene, bool check) {
//    }

    // 锟叫讹拷锟角凤拷前
    if (check && eglContext != nullptr) {
        EGLContext p = eglGetCurrentContext();
        EGLSurface surf = eglGetCurrentSurface(EGL_DRAW);
        if (eglContext == p && surf == eglSurface) {
            return EGL_SUCCESS;
        }
    }

    // 锟斤拷View Destroy时锟斤拷锟斤拷执锟斤拷DestroySurface()锟斤拷然锟斤拷执锟斤拷canvas core锟侥伙拷锟秸ｏ拷锟斤拷时锟斤拷锟斤拷要锟斤拷锟斤拷gl锟接口ｏ拷
    // 锟斤拷锟斤拷锟斤拷锟斤拷锟� MakeCurrent 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷?
    // 1.preserve backbuffer锟斤拷锟斤拷锟皆ｏ拷锟斤拷pbb锟斤拷锟斤拷
    EGLBoolean result = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    // EGL_LOGI("eglMakeCurrent: surface=%p, context=%p, eglGetError=%x", eglSurface, eglContext, eglGetError());
    if (result != EGL_TRUE) {
        EGLint error = eglGetError();
        // TRACE_EXCEPTION("eglMakeCurrent_fail", "GLWindowContext.%s,eglGetError=%x", scene, error);
        return error;
    } else {
        currentEGLContext = eglContext;
        return EGL_SUCCESS;
    }
}


EGLSurface WmEGLWindowContext::CreateWindowSurface(std::string &scene) {
    if (nWindow == nullptr) { // 锟饺讹拷锟皆憋拷锟斤拷锟斤拷锟斤拷锟斤拷nWindow为null时crash
        // TRACE_EXCEPTION("CreateWindowSurface_fail", "Scene=%s,window is nullptr", scene.data());
        return EGL_NO_SURFACE;
    }

    EGL_LOGI("CreateWindowSurface:start:eglDisplay=%p,nWindow=%p,scene=%s", eglDisplay, nWindow,
             scene.data());
    EGLSurface createdSurface = eglCreateWindowSurface(eglDisplay, eglConfig, nWindow, nullptr);
    if (createdSurface == EGL_NO_SURFACE) { // 锟斤拷锟斤拷surface失锟斤拷
        EGLint error = eglGetError();
        EGL_LOGI("CreateWindowSurface:fail:eglSurface=%p,eglError=%x", createdSurface, error);
        if (error == EGL_BAD_ALLOC && scene == "resume") {
            // resume锟斤拷锟斤拷锟絪urface锟斤拷锟斤拷锟窖撅拷锟斤拷锟斤拷nWindow锟较ｏ拷锟斤拷时锟斤拷锟斤拷要锟较憋拷锟斤拷锟斤拷
        } else if (error != EGL_SUCCESS) { // 注锟解：锟截革拷锟斤拷锟斤拷eglSurface锟结报EGL_BAD_ALLOC锟斤拷锟斤拷
//            TRACE_EXCEPTION("eglCreateWindowSurface_fail", "Scene.%s, eglGetError=%x",
//                            scene.data(), error);
        }
        return EGL_NO_SURFACE;
    } else { // 锟斤拷锟斤拷surface锟缴癸拷
        eglQuerySurface(eglDisplay, createdSurface, EGL_WIDTH, &width);
        eglQuerySurface(eglDisplay, createdSurface, EGL_HEIGHT, &height);

        if (usePreserveBackBuffer) {
            eglGetError(); // 锟斤拷锟絜rror?
            eglSurfaceAttrib(eglDisplay, createdSurface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED);
            EGLint error = eglGetError();
            if (error != EGL_SUCCESS) {
//                TRACE_EXCEPTION("eglPreserveBackBuffer_fail",
//                                "GLWindowContext.%s, eglGetError=%x",
//                                scene.c_str(), eglGetError());
            }
        }
        EGL_LOGI("CreateWindowSurface:success:eglSurface=%p,preserveBB=%i,egl size(%i,%i)", createdSurface,
                 usePreserveBackBuffer, width, height);
    }

    return createdSurface;
}


bool WmEGLWindowContext::IsPreserveBackBuffer() {
    EGLint value;

    eglGetError();
    eglQuerySurface(eglDisplay, eglSurface, EGL_SWAP_BEHAVIOR, &value);

    EGLint error = eglGetError();
    if (error != EGL_SUCCESS) {
//        TRACE_EXCEPTION("QueryPreserveBackBuffer_fail", "GLWindowContext, eglGetError=%x", error);
    }

    return error == EGL_SUCCESS && value == EGL_BUFFER_PRESERVED;
}


void WmEGLWindowContext::Terminate() {
    if (eglDisplay != EGL_NO_DISPLAY) {
        if (eglContext != EGL_NO_CONTEXT) {
            eglDestroyContext(eglDisplay, eglContext);
        }

        DestroyEGLSurfaceIf();
        if (!IsSharingContext()) {
            eglTerminate(eglDisplay);
            eglDisplay = EGL_NO_DISPLAY;
        }
    }

    eglContext = EGL_NO_CONTEXT;
    eglSurface = EGL_NO_SURFACE;

    nWindow = nullptr;
}


WmEGLWindowContext::~WmEGLWindowContext() {
    Terminate();
}
