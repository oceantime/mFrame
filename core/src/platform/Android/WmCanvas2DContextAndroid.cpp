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
        // 锟斤拷锟絫est 一直锟斤拷锟斤拷
        // glEnable(GL_DEPTH_TEST);
        BindFBO();
    } else {
        if (is_first_draw) {
            // 锟斤拷fbo模式锟铰ｏ拷锟阶达拷锟斤拷染前执锟斤拷一锟斤拷锟斤拷锟斤拷
            ClearScreen();
        }
    }
}


void WmCanvas2DContextAndroid::EndDraw() {
    if (!mConfig.useFbo) {
        return;
    }

    // FBO模式
    UnbindFBO();
    // 锟斤拷锟斤拷fbo前锟斤拷锟侥�锟斤拷frameBuffer锟斤拷锟斤拷 (锟斤拷为锟斤拷锟斤拷锟斤拷示锟斤拷锟斤拷锟角碉拷锟接伙拷锟斤拷锟斤拷fbo锟较碉拷)
    ClearScreen();
    // 锟斤拷锟斤拷FBO
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
 * 锟节改憋拷canvas view锟斤拷小时锟斤拷锟斤拷锟斤拷锟捷革拷锟斤拷
 * 锟铰斤拷fbo, 锟斤拷锟斤拷锟斤拷fbo锟斤拷锟斤拷
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
        // 1.锟叫伙拷锟斤拷锟斤拷fbo
        UnbindFBO();
        // 2.锟铰达拷锟斤拷fbo
        WmFrameBufferObject newFbo;

        // 锟斤拷录锟铰达拷锟斤拷init fbo vector锟斤拷锟届常
        std::vector<WmCanvasLog> logVec;
        mIsFboSupported = newFbo.InitFBO(mWidth, mHeight, GColorTransparent, mEnableFboMsaa, &logVec);
        LOG_EXCEPTION_VECTOR(mHooks, mContextId, logVec);

        // FIXME 锟斤拷锟斤拷麓锟斤拷锟絝bo失锟杰ｏ拷锟斤拷fbo锟竭达拷锟街诧拷锟皆ｏ拷锟斤拷锟斤拷锟斤拷锟狡必讹拷锟斤拷锟斤拷

        // 锟角凤拷锟斤拷诰傻锟絝bo, 锟斤拷锟斤拷锟斤拷蚩奖锟斤拷锟絝bo锟斤拷删锟斤拷锟缴碉拷
        if (mFboMap.find(DefaultFboName) != mFboMap.end()) {
            // 3.锟斤拷锟斤拷fbo
            CopyFBO(mFboMap[DefaultFboName], newFbo);
            // 4.删锟斤拷锟缴碉拷fbo锟斤拷锟斤拷锟斤拷锟斤拷fbo锟斤拷源
            mFboMap.erase(DefaultFboName);
        }

        // 5.锟斤拷锟斤拷锟斤拷FBO锟斤拷map锟斤拷(锟斤拷锟狡讹拷锟斤拷锟节达拷, 锟斤拷锟斤拷锟节的讹拷锟斤拷锟斤拷然锟斤拷锟斤拷)
        std::string key = DefaultFboName;
        // 锟芥换锟斤拷锟缴碉拷写锟诫方锟斤拷
        mFboMap.emplace(key, std::move(newFbo));

        // 6.锟叫伙拷锟斤拷锟斤拷FBO锟斤拷(锟斤拷锟皆伙拷锟斤拷)
        BindFBO();
    }

    // 锟斤拷锟斤拷锟斤拷锟斤拷叽锟轿�0锟斤拷锟斤拷示要锟斤拷锟斤拷surface锟戒换锟斤拷锟斤拷锟斤拷锟斤拷(0,0)锟斤拷锟斤拷dimension锟斤拷锟斤拷锟斤拷锟铰的变换锟斤拷锟斤拷
    if (shouldChangeDimension) {
        SetCanvasDimension(0, 0);
    }

    if (mContextType == 0) {
        // 锟斤拷锟斤拷viewport锟戒换
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

    // 锟斤拷锟斤拷锟斤拷桑锟斤拷谢锟斤拷锟絝bo
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

    // 锟斤拷锟铰尺达拷
    mWidth = width;
    mHeight = height;

    bool shouldChangeDimension = (mCanvasWidth <= 0 && mCanvasHeight <= 0);
    // 锟斤拷始锟斤拷锟斤拷图锟斤拷
    if (rgbaData != nullptr) {
        CopyImageToCanvas(width, height, rgbaData, imgWidth, imgHeight);
    }

    // 锟斤拷锟斤拷锟斤拷锟斤拷叽锟轿�0锟斤拷锟斤拷示要锟斤拷锟斤拷surface锟戒换锟斤拷锟斤拷锟斤拷锟斤拷(0,0)锟斤拷锟斤拷dimension锟斤拷锟斤拷锟斤拷锟铰的变换锟斤拷锟斤拷
    if (shouldChangeDimension) {
        SetCanvasDimension(0, 0);
    }
    // 锟斤拷锟斤拷viewport锟戒换
    glViewport(0, 0, width, height);
}


void WmCanvas2DContextAndroid::CopyImageToCanvas(int width, int height,
                                                const unsigned char *rgbaData, int imgWidth,
                                                int imgHeight) {
    ResetGLBeforeCopyFrame(width, height);
    // 锟斤拷图锟斤拷锟斤拷锟斤拷
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
