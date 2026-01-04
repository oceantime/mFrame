
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
        // ���е�swapBuffer fail�Ķ�trace���� (�п��ܵ������󣬵������ݱ���?)
        // TRACE_EXCEPTION("eglSwapBuffers_fail", "eglGetError=%x", err);
        if (err == EGL_BAD_SURFACE) { // ��Ҫ���Իָ������䱨��
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


WmEGLWindowContext::WmEGLWindowContext() : nWindow(NULL),
                                         isGLESInit(false),
                                         isES3Supported(false) {}

WmEGLWindowContext::WmEGLWindowContext(EGLDisplay sharedDisplay, EGLContext sharedContext) :
                                                                                        WmEGLContext(sharedDisplay, sharedContext),
                                                                                        nWindow(NULL),
                                                                                           isGLESInit(false),
                                                                                           isES3Supported(
                                                                                                   false) {}

bool WmEGLWindowContext::Init(ANativeWindow *window) {
    if (isEGLInit) {
        return true;
    }
    nWindow = window;
    // ��ʼ��eglDisplay & eglSurface(eglSurface��eglContext�޹���)
    bool flag = InitEGLSurface();
    if (flag) {
        flag = InitEGLContext();
    }
    // ����Ѿ�ִ�й���ʼ��(���п��ܳ�ʼ��ʧ��)
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
            // ����msaa
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

    // ��ʼ��EGLContextʱ ǿ��MakeCurrent
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
 * ��ͣ��������ɾ��EGL Surface
 * FIXME �����е�����, �ⲿʵ������DestroyEGLSurface
 */
void WmEGLWindowContext::Pause() {
    DestroyEGLSurfaceIf();
    nWindow = nullptr;
}


void WmEGLWindowContext::DestroyEGLSurfaceIf() {
    // 1.��clear current������gl��������������(����gl����)
    ClearCurrent();

    // 2.destroy eglSurface(δcurrent��surface������ɾ��)
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

    // ���egl surface��Ч���п����������ݣ���destroy surface(��֤resumeִ�гɹ�)
    if (eglSurface != EGL_NO_SURFACE) {
        DestroyEGLSurfaceIf();
    }

    int32_t original_width = width;
    int32_t original_height = height;

    // ����window
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

    // makeCurrentʧ�ܣ��п�����context������
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
    // ֻҪ��Resize���ã���ʹ�ߴ�һ����Ҳǿ�ƴ���Resize�߼�(�п���Surface�Ѿ������仯)
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


// surface ״̬���
EGLint WmEGLWindowContext::MakeCurrentWithScene(const char *scene, bool check) {
//    }

    // �ж��Ƿ�ǰ
    if (check && eglContext != nullptr) {
        EGLContext p = eglGetCurrentContext();
        EGLSurface surf = eglGetCurrentSurface(EGL_DRAW);
        if (eglContext == p && surf == eglSurface) {
            return EGL_SUCCESS;
        }
    }

    // ��View Destroyʱ����ִ��DestroySurface()��Ȼ��ִ��canvas core�Ļ��գ���ʱ����Ҫ����gl�ӿڣ�
    // ��������� MakeCurrent ������������?
    // 1.preserve backbuffer�����ԣ���pbb����
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
    if (nWindow == nullptr) { // �ȶ��Ա���������nWindowΪnullʱcrash
        // TRACE_EXCEPTION("CreateWindowSurface_fail", "Scene=%s,window is null", scene.data());
        return EGL_NO_SURFACE;
    }

    EGL_LOGI("CreateWindowSurface:start:eglDisplay=%p,nWindow=%p,scene=%s", eglDisplay, nWindow,
             scene.data());
    EGLSurface createdSurface = eglCreateWindowSurface(eglDisplay, eglConfig, nWindow, NULL);
    if (createdSurface == EGL_NO_SURFACE) { // ����surfaceʧ��
        EGLint error = eglGetError();
        EGL_LOGI("CreateWindowSurface:fail:eglSurface=%p,eglError=%x", createdSurface, error);
        if (error == EGL_BAD_ALLOC && scene == "resume") {
            // resume�����surface�����Ѿ�����nWindow�ϣ���ʱ����Ҫ�ϱ�����
        } else if (error != EGL_SUCCESS) { // ע�⣺�ظ�����eglSurface�ᱨEGL_BAD_ALLOC����
//            TRACE_EXCEPTION("eglCreateWindowSurface_fail", "Scene.%s, eglGetError=%x",
//                            scene.data(), error);
        }
        return EGL_NO_SURFACE;
    } else { // ����surface�ɹ�
        eglQuerySurface(eglDisplay, createdSurface, EGL_WIDTH, &width);
        eglQuerySurface(eglDisplay, createdSurface, EGL_HEIGHT, &height);

        if (usePreserveBackBuffer) {
            eglGetError(); // ���error?
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
