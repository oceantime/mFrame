#include "WmCanvas2DContextAndroid.h"

#include "WmFontCache.h"
#include "WmPoint.h"
#include "WmFontManagerAndroid.h"
#include "WmFrameBufferObject.h"
#include "WmShaderManager.h"
#include "support/Log.h"


extern bool g_use_pre_compile;
extern std::string g_shader_cache_path;


WmCanvas2DContextAndroid::WmCanvas2DContextAndroid(uint32_t w, uint32_t h, WmCanvasConfig &config) :
        WmCanvasContext(w, h, config, nullptr) {
    Create();
}


WmCanvas2DContextAndroid::WmCanvas2DContextAndroid(uint32_t w, uint32_t h, WmCanvasConfig &config, WmCanvasHooks* hooks) :
        WmCanvasContext(w, h, config, hooks) {
    Create();
}


void WmCanvas2DContextAndroid::Create() {
    mShaderManager = nullptr;

    if (!mConfig.sharedShader) {
        mShaderManager = new WmShaderManager();
    }
}


WmCanvas2DContextAndroid::~WmCanvas2DContextAndroid() {
    if (!mConfig.sharedShader) {
        if (mShaderManager != nullptr) {
            delete mShaderManager;
            mShaderManager = nullptr;
        }
    }
}


void WmCanvas2DContextAndroid::SetUseShaderBinaryCache(bool v) {
    g_use_pre_compile = true;
}


void WmCanvas2DContextAndroid::SetShaderBinaryCachePath(const std::string& v) {
    g_shader_cache_path = v;
}



void WmCanvas2DContextAndroid::InitFBO() {
    if (0 != mContextType) return;

    if (!mConfig.useFbo) {
        return;
    }

    if (!mIsFboSupported) {
        return;
    }

    if (mFboMap.find(DefaultFboName) == mFboMap.end()) {
        std::vector<WmCanvasLog> logVec;
        mIsFboSupported = mFboMap[DefaultFboName].InitFBO(mWidth, mHeight,
                                                          GColorTransparent, mEnableFboMsaa,
                                                          &logVec);

        LOG_EXCEPTION_VECTOR(mHooks, mContextId.c_str(), logVec);
    }
}



void WmCanvas2DContextAndroid::ClearColorToTransparent()
{
    WmColorRGBA c = GColorTransparent;
    ClearColor(c);
}


void WmCanvas2DContextAndroid::ClearColor(WmColorRGBA& c) {
    glClearColor(c.rgba.r, c.rgba.g, c.rgba.b, c.rgba.a);
    glClear(GL_COLOR_BUFFER_BIT);
}


void WmCanvas2DContextAndroid::GetRawImageData(int width, int height, uint8_t *pixels) {
    SendVertexBufferToGPU();
    glReadPixels(0, 0, width, height, GL_RGBA,
                 GL_UNSIGNED_BYTE, pixels);
}


//{
//    WmBlendOperationFuncs funcs = WmCompositeOperationFuncs(op);
//    WmBlendOperationFuncs alphaFuncs = WmCompositeOperationFuncs(alphaOp);
//
//    glBlendFuncSeparate(funcs.source, funcs.destination,
//                        alphaFuncs.source, alphaFuncs.destination);
//}


void WmCanvas2DContextAndroid::BeginDraw(bool is_first_draw) {
    glEnable(GL_DEPTH_TEST);

    if (mConfig.useFbo) {
        // ���test һֱ����
        // glEnable(GL_DEPTH_TEST);
        BindFBO();
    } else {
        if (is_first_draw) {
            // ��fboģʽ�£��״���Ⱦǰִ��һ������
            ClearScreen();
        }
    }
}


void WmCanvas2DContextAndroid::EndDraw() {
    if (!mConfig.useFbo) {
        return;
    }

    // FBOģʽ
    UnbindFBO();
    // ����fboǰ���Ĭ��frameBuffer���� (��Ϊ������ʾ�����ǵ��ӻ�����fbo�ϵ�)
    ClearScreen();
    // ����FBO
    DrawFBO(DefaultFboName);
}


WmTexture *WmCanvas2DContextAndroid::GetFBOTextureData() {
    return &(mFboMap[DefaultFboName].mFboTexture);
}


void WmCanvas2DContextAndroid::ResizeCanvas(int width, int height) {
    mWidth = width;
    mHeight = height;

    mCanvasWidth = width;
    mCanvasHeight = height;

    if (mContextType == 0) {
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mVertexBufferIndex = 0;
        UpdateProjectTransform();

        if (mCurrentState != nullptr) {
            WmTransform old = mCurrentState->mTransform;
            mCurrentState->mTransform = WmTransformIdentity;
            if (!WmTransformEqualToTransform(old, mCurrentState->mTransform)) {
                // update shader transform
                SetTransformOfShader(mProjectTransform);
            }
        }

        ResetStateStack();
        DoSetGlobalCompositeOperation(COMPOSITE_OP_SOURCE_OVER, COMPOSITE_OP_SOURCE_OVER);
        UseDefaultRenderPipeline();

        ClearScreen();
    }

    mFboMap.erase(WmCanvasContext::DefaultFboName);
    InitFBO();
    BindFBO();
}



/**
 * �ڸı�canvas view��Сʱ�������ݸ���
 * �½�fbo, ������fbo����
 */
void WmCanvas2DContextAndroid::ResizeCopyUseFbo(int width, int height) {
    bool sizeChanged = mWidth != width || height != mHeight;
    mWidth = width;
    mHeight = height;
    if (!sizeChanged) {
        return;
    }

    bool shouldChangeDimension = (mCanvasWidth <= 0 && mCanvasHeight <= 0);
    if (0 == mContextType && mIsFboSupported && mWidth > 0 && mHeight > 0) {
        // 1.�л�����fbo
        UnbindFBO();
        // 2.�´���fbo
        WmFrameBufferObject newFbo;

        // ��¼�´���init fbo vector���쳣
        std::vector<WmCanvasLog> logVec;
        mIsFboSupported = newFbo.InitFBO(mWidth, mHeight, GColorTransparent, mEnableFboMsaa, &logVec);
        LOG_EXCEPTION_VECTOR(mHooks, mContextId, logVec);

        // FIXME ����´���fboʧ�ܣ���fbo�ߴ��ֲ��ԣ��������Ʊض�����

        // �Ƿ���ھɵ�fbo, ������򿽱���fbo��ɾ���ɵ�
        if (mFboMap.find(DefaultFboName) != mFboMap.end()) {
            // 3.����fbo
            CopyFBO(mFboMap[DefaultFboName], newFbo);
            // 4.ɾ���ɵ�fbo��������fbo��Դ
            mFboMap.erase(DefaultFboName);
        }

        // 5.������FBO��map��(���ƶ����ڴ�, �����ڵĶ�����Ȼ����)
        std::string key = DefaultFboName;
        // �滻���ɵ�д�뷽��
        mFboMap.emplace(key, std::move(newFbo));

        // 6.�л�����FBO��(���Ի���)
        BindFBO();
    }

    // ��������ߴ�Ϊ0����ʾҪ����surface�任��������(0,0)����dimension�������µı任����
    if (shouldChangeDimension) {
        SetCanvasDimension(0, 0);
    }

    if (mContextType == 0) {
        // ����viewport�任
        glViewport(0, 0, mWidth, mHeight);
    }
}


void WmCanvas2DContextAndroid::CopyFBO(WmFrameBufferObject &srcFbo, WmFrameBufferObject &destFbo) {
    if (!mIsFboSupported) {
        return;
    }

    if (nullptr == mCurrentState || nullptr == mCurrentState->mShader) {
        return;
    }

    destFbo.BindFBO();

    ResetGLBeforeCopyFrame(destFbo.mWidth, destFbo.mHeight);

    WmColorRGBA color = GColorWhite;
    glBindTexture(GL_TEXTURE_2D, srcFbo.mFboTexture.GetTextureID());
    PushRectangle(-1, -1, 2, 2, 0, 0, 1, 1, color);
    glDrawArrays(GL_TRIANGLES, 0, mVertexBufferIndex);
    mVertexBufferIndex = 0;

    RestoreGLAfterCopyFrame();

    // ������ɣ��л���fbo
    UnbindFBO();
}


void
WmCanvas2DContextAndroid::ResizeCopyUseImage(int width, int height, const unsigned char *rgbaData,
                                            int imgWidth,
                                            int imgHeight) {
    bool sizeChanged = (mWidth != width) || (height != mHeight);
    if (!sizeChanged) {
        // LOGE("sizeChanged not changed");
        return;
    }

    // ���³ߴ�
    mWidth = width;
    mHeight = height;

    bool shouldChangeDimension = (mCanvasWidth <= 0 && mCanvasHeight <= 0);
    // ��ʼ����ͼ��
    if (rgbaData != nullptr) {
        CopyImageToCanvas(width, height, rgbaData, imgWidth, imgHeight);
    }

    // ��������ߴ�Ϊ0����ʾҪ����surface�任��������(0,0)����dimension�������µı任����
    if (shouldChangeDimension) {
        SetCanvasDimension(0, 0);
    }
    // ����viewport�任
    glViewport(0, 0, width, height);
}


void WmCanvas2DContextAndroid::CopyImageToCanvas(int width, int height,
                                                const unsigned char *rgbaData, int imgWidth,
                                                int imgHeight) {
    ResetGLBeforeCopyFrame(width, height);
    // ��ͼ������
    GLuint glID = BindImage(rgbaData, GL_RGBA, (GLuint) imgWidth, (GLuint) imgHeight);
    WmColorRGBA color = GColorWhite;
    PushRectangle(-1, -1, 2, 2, 0, 0, 1, 1, color);
    mCurrentState->mShader->SetTransform(WmTransformIdentity);
    glDrawArrays(GL_TRIANGLES, 0, mVertexBufferIndex);
    mVertexBufferIndex = 0;

    glDeleteTextures(1, &glID);

    RestoreGLAfterCopyFrame();
}


void
WmCanvas2DContextAndroid::
DrawFBO(std::string fboName, WmCompositeOperation compositeOp, float sx, float sy, float sw,
        float sh, float dx, float dy, float dw, float dh) {
    if (!mIsFboSupported) {
        return;
    }

    if (nullptr == mCurrentState || nullptr == mCurrentState->mShader) {
        return;
    }


    Save();
    glViewport(mX, mY, mWidth, mHeight);

    WmFrameBufferObject &fbo = mFboMap[fboName];

    UseDefaultRenderPipeline();

    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);

    DoSetGlobalCompositeOperation(compositeOp, compositeOp);

    WmColorRGBA color = GColorWhite;
    mCurrentState->mShader->SetOverideTextureColor(0);
    mCurrentState->mShader->SetHasTexture(1);
    fbo.mFboTexture.Bind();

    PushRectangle(-1, -1, 2, 2, 0, 0, 1, 1, color);
    mCurrentState->mShader->SetTransform(WmTransformIdentity);
    glDrawArrays(GL_TRIANGLES, 0, mVertexBufferIndex);
    mVertexBufferIndex = 0;

    if (HasClipRegion()) {
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_DEPTH_TEST);
    }

    glViewport(0, 0, mWidth, mHeight);

    Restore();
}


void WmCanvas2DContextAndroid::ResetGLBeforeCopyFrame(int width, int height) {
    Save();
    WmColorRGBA c = mClearColor;
    SetClearColor(GColorTransparent);
    ClearScreen();
    SetClearColor(c);
    glViewport(0, 0, width, height);

    UseDefaultRenderPipeline();
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);

    WmCompositeOperation compositeOp = COMPOSITE_OP_SOURCE_OVER;
    DoSetGlobalCompositeOperation(compositeOp, compositeOp);

    mCurrentState->mShader->SetOverideTextureColor(0);
    mCurrentState->mShader->SetHasTexture(1);
    mCurrentState->mShader->SetTransform(WmTransformIdentity);
}


void WmCanvas2DContextAndroid::RestoreGLAfterCopyFrame() {
    if (HasClipRegion()) {
        glEnable(GL_STENCIL_TEST);
    }
    glEnable(GL_DEPTH_TEST);

    Restore();
}


void WmCanvas2DContextAndroid::DrawFrame(bool clear) {
    SendVertexBufferToGPU();
    if (clear) {
        ClearGeometryDataBuffers();
    }
}


void WmCanvas2DContextAndroid::SetShaderManager(WmShaderManager* shaderManager) {
    this->mShaderManager = shaderManager;
}


WmShaderManager *WmCanvas2DContextAndroid::GetShaderManager() {
    return this->mShaderManager;
}
