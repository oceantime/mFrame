/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#ifndef GCANVAS_GCANVASCONTEXT_H
#define GCANVAS_GCANVASCONTEXT_H

#include "WmGL.h"
#include "WmCanvasState.h"
#include "WmFrameBufferObject.h"
#include "WmTexture.h"
#include "WmConvert.h"
#include "WmTreemap.h"
#include "WmFontManager.h"
#include "../support/Log.h"
#include "WmShaderManager.h"
#include "WmPath2D.h"

#include <iostream>

extern WmColorRGBA BlendColor(WmCanvasContext *context, WmColorRGBA color);
extern WmColorRGBA BlendWhiteColor(WmCanvasContext *context);
extern WmColorRGBA BlendFillColor(WmCanvasContext *context);
extern WmColorRGBA BlendStrokeColor(WmCanvasContext *context);


#ifdef WMCANVAS
typedef void (*GWebGLTxtImage2DFunc)(GLenum target, GLint level, GLenum internalformat,
                                     GLenum format, GLenum type, const char *src);
typedef void (*GWebGLTxtSubImage2DFunc)(GLenum target, GLint level, GLint xoffset, GLint yoffset,
                                        GLenum format, GLenum type, const char *src);
typedef void (*GWebGLBindToGLKViewFunc)(std::string contextId);
#endif

class WmCanvas;

struct WmCanvasConfig {
    bool flip;              //deafult is false,
    bool useFbo;            //default is true
    bool sharedFont;
    bool sharedShader;
};


enum ShaderType {

    TEXTURE,

};


/**
 * texture id to vertex range map
 */
struct GCanvasVertexShaderProperty {

    std::string shaderType;

    int vertexStartIndex = 0;

    int vertexCount = 0;

    // shader property
    int textureId = -1;

};




class WmCanvasContext {
public:

    static const int GCANVAS_STATE_STACK_SIZE = 16;


    static const int GCANVAS_VERTEX_BUFFER_SIZE = 2048;


    static const int MAX_BLUE_RADIUS = 127;


    WmCanvasContext(short w, short h, const WmCanvasConfig &config, WmCanvasHooks *hooks = nullptr);


    virtual ~WmCanvasContext();

    //------------------GL----------------------
    API_EXPORT bool InitializeGLEnvironment();
    bool InitializeGLShader();
    void ResetStateStack();
    
    void BindVertexBuffer();
    void ClearGeometryDataBuffers();
    API_EXPORT void SendVertexBufferToGPU(const GLenum geometry_type = GL_TRIANGLES);

    void SendVertexBufferToGPUOptim(const GLenum geometry_type = GL_TRIANGLES);

    void BindPositionVertexBuffer();
    GLuint PositionSlot();
    
    void UpdateProjectTransform();
    GTransform CalculateProjectTransform(int width, int height, bool needFlipY = false);


    //------------------Pipeline----------------------
    void SaveRenderPipeline();
    void RestoreRenderPipeline();

    API_EXPORT void UseDefaultRenderPipeline();
    void UseTextureRenderPipeline();
    void UseShadowRenderPipeline();
    void UseBlurRenderPipeline();
    void UseBlurRenderPipeline(double radius);
    void UsePatternRenderPipeline(bool isStroke = false);
    void UseLinearGradientPipeline(bool isStroke = false);
    void UseRadialGradientPipeline(bool isStroke = false);
    void ApplyFillStylePipeline(bool isStroke = false);
    float GetCurrentAlphaOfStyle(bool isStroke = false);

    void SetTexture(int textureId);
    bool flagHack=true;
    
    //----------------Push Vertex------------------------
    void PushTriangle(GPoint v1, GPoint v2, GPoint v3, WmColorRGBA color,
                     GTransform transform, std::vector<WmVertex> *vec = NULL);
    void PushQuad(GPoint v1, GPoint v2, GPoint v3, GPoint v4, WmColorRGBA color, GTransform transform, std::vector<WmVertex> *vec = NULL);
    //fbo size same with texutre
    void PushRectangle(float x, float y, float w, float h, float tx, float ty, float tw, float th,
                       WmColorRGBA color, GTransform transform = GTransformIdentity, bool flipY = false, std::vector<WmVertex> *vec = NULL);

    void PushRectangleFormat(float x, float y, float w, float h, float tx, float ty, float tw, float th,
                       WmColorRGBA color, GTransform transform = GTransformIdentity, bool flipY = false,
                       std::vector<WmVertex> *vec = NULL, bool formatIntVertex = false);
    //fbo size isn't with texutre, tw,th vaule is tw=fbo.w/texture.w, th=fbo.h/texture.h
    void PushRectangle4TextureArea(float x, float y, float w, float h,
                                   float tx, float ty, float tw, float th,WmColorRGBA color,
                                   GTransform transform = GTransformIdentity, bool flipY = false);
    void PushReverseRectangle(float x, float y, float w, float h, float tx, float ty,
                              float tw, float th, WmColorRGBA color);
    void PushPoints(const std::vector<GPoint> &points, WmColorRGBA color);
    void PushVertexs(const std::vector<WmVertex> &vertexs);
    void PushTriangleFanPoints(const std::vector<GPoint> &points, WmColorRGBA color);

    void SaveVertexShaderProperty(int offset, int count);

    //----------------Getter & Setter------------------------
    API_EXPORT bool IsUseFbo();

    API_EXPORT float GetCanvasDimensionWidthScale();
    API_EXPORT float GetCanvasDimensionHeightScale();

    API_EXPORT float GetCurrentScaleX();
    API_EXPORT float GetCurrentScaleY();

    API_EXPORT void SetDevicePixelRatio(const float ratio);
    API_EXPORT float GetDevicePixelRatio();

    API_EXPORT void SetCanvasDimension(const int w, const int h, bool resetStatus = false);

    API_EXPORT int GetCanvasWidth();
    API_EXPORT int GetCanvasHeight();

    API_EXPORT short GetWidth() const;
    API_EXPORT short GetHeight() const;

    API_EXPORT void SetWidth(short v) { mWidth = v; }
    API_EXPORT void SetHeight(short v) { mHeight = v; }

    API_EXPORT void SetClearColor(const WmColorRGBA &c);
    API_EXPORT WmColorRGBA GetClearColor() const { return mClearColor; }
    
    bool IsFboSupport() const { return mIsFboSupported; }
    
    void SetHiQuality(bool isHiQuality) { mHiQuality = isHiQuality; }
    bool GetHiQuality() { return mHiQuality; }
    
    bool HasClipRegion() const { return mHasClipRegion; }
    void SetClipFlag(const bool flag) { mHasClipRegion = flag; }
    
    void SetContextLost(bool v) { mContextLost = v; }
    
    GCanvasState *GetCurrentState() { return mCurrentState; }
    
    GTexture *GetFboTexture();

    //non-w3c API
    API_EXPORT void ClearScreen();
    API_EXPORT void Resize(int w, int h); //Android only


    //////////////////////////////////////////////////////////////////////////////
    ///   Context2D Property
    //////////////////////////////////////////////////////////////////////////////
    //global
    API_EXPORT float GlobalAlpha();
    API_EXPORT void SetGlobalAlpha(float a);

    API_EXPORT WmCompositeOperation GlobalCompositeOperation();
    API_EXPORT void SetGlobalCompositeOperation(int op);
    API_EXPORT void DoSetGlobalCompositeOperation(WmCompositeOperation op,
                                                  WmCompositeOperation alphaOp = COMPOSITE_OP_LIGHTER);

    //font
    API_EXPORT WmTextAlign TextAlign() const { return mCurrentState->mTextAlign; }
    API_EXPORT void SetTextAlign(WmTextAlign align);

    API_EXPORT GTextBaseline TextBaseline() const { return mCurrentState->mTextBaseline; }
    API_EXPORT void SetTextBaseline(GTextBaseline baseLine);
    
    API_EXPORT void SetFont(const char *font);

    //shadow
    API_EXPORT void SetShadowColor(const char *str);
    API_EXPORT void SetShadowBlur(float blur);
    API_EXPORT void SetShadowOffsetX(float x);
    API_EXPORT void SetShadowOffsetY(float y);

    //style
    WmColorRGBA FillStyle() const { return mCurrentState->mFillColor; }
    API_EXPORT void SetFillStyle(WmColorRGBA c);
    API_EXPORT void SetFillStyle(const char *str);


    WmColorRGBA StrokeStyle() const { return mCurrentState->mStrokeColor; }
    API_EXPORT void SetStrokeStyle(const WmColorRGBA &c);
    API_EXPORT void SetStrokeStyle(const char *str);

    API_EXPORT void SetFillStylePattern(int textureId, int width, int height,
                                        const char *repeatMode, bool isStroke = false);

    API_EXPORT void SetFillStyleLinearGradient(float startArr[], float endArr[], int stop_count,
                                               const float posArray[],
                                               const std::string colorArray[],
                                               bool isStroke = false);

    API_EXPORT void SetFillStyleRadialGradient(float startArr[], float endArr[], int stop_count,
                                               const float posArray[],
                                               const std::string colorArray[],
                                               bool isStroke = false);

    //path
    API_EXPORT float LineWidth() const { return mCurrentState->mLineWidth; }
    API_EXPORT void SetLineWidth(float w) { mCurrentState->mLineWidth = w; }
    
    API_EXPORT GLineCap LineCap() const { return mCurrentState->mLineCap; }
    API_EXPORT void SetLineCap(GLineCap cap) { mCurrentState->mLineCap = cap; }
    API_EXPORT void SetLineCap(const char *p);

    
    API_EXPORT GLineJoin LineJoin() const { return mCurrentState->mLineJoin; }
    API_EXPORT void SetLineJoin(GLineJoin join) { mCurrentState->mLineJoin = join; }
    API_EXPORT void SetLineJoin(const char *p);


    API_EXPORT float MiterLimit() const { return mCurrentState->mMiterLimit; }
    API_EXPORT void SetMiterLimit(float limit) { mCurrentState->mMiterLimit = limit; }


    API_EXPORT std::vector<float> &GetLineDash();
    API_EXPORT std::vector<float> LineDash() const { return mCurrentState->mLineDash; }
    API_EXPORT void SetLineDash(std::vector<float> lineDash) { mCurrentState->mLineDash = lineDash; }
    
    API_EXPORT float LineDashOffset() const { return mCurrentState->mLineDashOffset; }
    API_EXPORT void SetLineDashOffset(float offset) { mCurrentState->mLineDashOffset = offset; }
    

    //////////////////////////////////////////////////////////////////////////////
    ///   Context2D Method
    //////////////////////////////////////////////////////////////////////////////

    //trasnfrom
    API_EXPORT void SetTransform(float a, float b, float c, float d, float tx, float ty);
    API_EXPORT void Transfrom(float a, float b, float c, float d, float tx, float ty);
    API_EXPORT void ResetTransform();
    API_EXPORT void DoResetTransform(); //deprcated
    API_EXPORT void Scale(float sx, float sy);
    API_EXPORT void DoScale(float x, float y);
    API_EXPORT void Rotate(float angle);
    API_EXPORT void DoRotate(float angle); //deprcated
    API_EXPORT void Translate(float tx, float ty);
    API_EXPORT void DoTranslate(float tx, float ty);

    //GCanvasState save & restore
    API_EXPORT void Save();
    API_EXPORT bool Restore();

    //path
    API_EXPORT void BeginPath();
    API_EXPORT void ClosePath();
    API_EXPORT void MoveTo(float x, float y);
    API_EXPORT void LineTo(float x, float y);
    API_EXPORT void Rect(float x, float y, float w, float h);
    API_EXPORT void Arc(float x, float y, float r, float startAngle, float endAngle, bool anticlosewise=false);
    API_EXPORT void ArcTo(float x1, float y1, float x2, float y2, float radius);
    API_EXPORT void QuadraticCurveTo(float cpx, float cpy, float x, float y);
    API_EXPORT void BezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y);
    
    //rect
    API_EXPORT void FillRect(float x, float y, float w, float h);
    API_EXPORT void DoFillRect(float x, float y, float w, float h);
    API_EXPORT void StrokeRect(float x, float y, float w, float h);
    API_EXPORT void DoStrokeRect(float x, float y, float w, float h);
    API_EXPORT void GenStrokeRectPath(WmPath &path, float x, float y, float w, float h);
    API_EXPORT void ClearRect(float x, float y, float w, float h);
    
    //clip / stroke / fill
    API_EXPORT void Clip(GFillRule rule = FILL_RULE_NONZERO);
    API_EXPORT void Clip(GPath2D& path2d, GFillRule rule = FILL_RULE_NONZERO);

    API_EXPORT void ResetClip();

    API_EXPORT void Fill(GFillRule rule = FILL_RULE_NONZERO);
    API_EXPORT void Fill(GPath2D& path2d, GFillRule rule = FILL_RULE_NONZERO);

    API_EXPORT void Stroke();
    API_EXPORT void Stroke(GPath2D& path2d);


    //text
    API_EXPORT float MeasureTextWidth(const char *text, int strLength = 0);
    API_EXPORT void DrawText(const char *text, float x, float y, float maxWidth = SHRT_MAX);
    API_EXPORT void StrokeText(const char *text, float x, float y, float maxWidth = SHRT_MAX);
    
    //image
    API_EXPORT void DrawImage(int textureId, int textureWidth, int textureHeight,
                                  float sx, float sy, float sw, float sh,
                                  float dx, float dy, float dw, float dh,
                                  bool flipY = false);

    API_EXPORT void DoDrawImage(float w, float h, int TextureId, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh, bool flipY = false);
    
    API_EXPORT void PutImageData(const unsigned char *rgbaData,
                                 int tw, int th, int x, int y,
                                 int sx, int sy, int sw, int sh,
                                 bool is_src_flip_y = false);

    API_EXPORT void GetImageData(int x, int y, int width, int height, uint8_t *pixels);
    
    //Android only
    API_EXPORT int BindImage(const unsigned char *rgbaData, GLint format, unsigned int width,
                             unsigned int height);


    API_EXPORT void DrawTexture(int textureId, float* vertexList);

    API_EXPORT void DrawTextureArray(int textureId, int count, float* vertexList, float* texList);


    //----------------Other------------------------
    API_EXPORT void SetTransformOfShader(const GTransform &trans);

    void ClipRegion();
    void BeforeClip();
    void AfterClip();

//    void ClipRegionNew(GFillRule rule = FILL_RULE_NONZERO);

    void DrawClip();
    void DrawClipPath(WmPath* path);


    API_EXPORT void FillText(const unsigned short *text, unsigned int text_length,
                             float x, float y, bool isStroke, float scaleWidth = 1.0);

    //fbo
    virtual void InitFBO();
    API_EXPORT void BindFBO();
    API_EXPORT void UnbindFBO();

    //Dump
    API_EXPORT long DrawCallCount();
    API_EXPORT void ClearDrawCallCount();

    //----------------Weex API------------------------
#ifdef WMCANVAS
    API_EXPORT void ApplyTransform(float m11, float m12, float m21, float m22,
                                   float dx, float dy);
    API_EXPORT void ApplyTransform(GTransform t);
    
    GWebGLTxtImage2DFunc GetGWebGLTxtImage2DFunc() { return mWebGLTexImage2dFunc; }
    void SetGWebGLTxtImage2DFunc(GWebGLTxtImage2DFunc func) { mWebGLTexImage2dFunc = func; }

    GWebGLTxtSubImage2DFunc GetGWebGLTxtSubImage2DFunc() { return mWebGLTexSubImage2dFunc; }
    void SetGWebGLTxtSubImage2DFunc(GWebGLTxtSubImage2DFunc func) { mWebGLTexSubImage2dFunc = func; }

#ifdef IOS
    GWebGLBindToGLKViewFunc GetGWebGLBindToGLKViewFunc(){return mWebGLBindGLKViewFunc;}
    void SetGWebGLBindToGLKViewFunc(GWebGLBindToGLKViewFunc func){mWebGLBindGLKViewFunc = func;}
#endif

#endif

    /**
     * set fontManager api, for shared font
     * @param fontManager WmFontManager
     */
    API_EXPORT void SetFontManager(WmFontManager* fontManager);
    bool NeedDrawShadow();
    void DrawShadow(const WmRectf &rect, std::function<void()> drawFun, bool isStroke = false);

protected:


    void CheckContextStatus();
    
    void DrawTextWithLength(const char *text, int strLength, float x, float y, bool isStroke = false, float maxWidth = SHRT_MAX);

    virtual GShaderManager *GetShaderManager();

    virtual GShader *FindShader(const char *name);
    

    void DoDrawShadowToFBO(GFrameBufferObjectPtr &shadowFbo, float dpr, const WmRectf &rect, std::function<void()> draw);
    void DoDrawShadowFBOToScreen(GFrameBufferObjectPtr &shadowFbo, const WmRectf &rect, std::vector<WmPath*>* recoveryClipPath);
    void DrawBlur(const WmRectf &rect, float blur, std::function<void()> draw, std::vector<WmPath*>* recoveryClipPath);
    void DoDrawBlur(const WmRectf &rect, float blur, std::function<void()> draw,
    GFrameBufferObjectPtr &inputFbo, GFrameBufferObjectPtr &outputFbo, float scale);
    
    //FBO
    void PrepareDrawElemetToFBO(GFrameBufferObject &fbo, float offsetX = 0, float offsetY = 0);
    void DrawFBOToFBO(GFrameBufferObject &src, GFrameBufferObject &dest);
    void DrawFBOToScreen(GFrameBufferObject &fbo, float x, float y, float w, float h,
                         WmColorRGBA color);
    
    //Vertex
    bool NeedSendVertexBufferToGPUWithSize(size_t size);


    void DoFillWithPath(WmPath& path, GFillRule rule);

    void DoStrokeWithPath(WmPath& path);

    void DoClipPath(WmPath* path, GFillRule rule);


public:
    WmCanvasConfig mConfig;
    GCanvasState *mCurrentState;
    float mDevicePixelRatio;
    GTransform mProjectTransform;

    bool mIsFboSupported;
    WmColorRGBA mClearColor;

    short mX;
    short mY;
    short mWidth;
    short mHeight;
    // canvas size
    int mCanvasWidth = 0;
    int mCanvasHeight = 0;

    bool mIsContextReady;
    
    std::map<std::string, GFrameBufferObject> mFboMap;
    static constexpr const char *DefaultFboName = "default";

    WmFontManager *mFontManager = nullptr;

    WmCanvasHooks *mHooks;
    
    //Weex
    int mContextType; // 0--2d;1--webgl

    bool mContextLost;

    // dumpInfo
    long mDrawCallCount;

    int mVertexBufferIndex;

    std::string mContextId;

protected:
    // Path is not affect by save/restore, so we keep persistent object
    WmPath mPath;

    std::vector<GCanvasState*> mStateStack;
    bool mHasClipRegion;

    GShader *mSaveShader;
    bool mSaveIsStroke;

    // gl vertex
    WmVertex CanvasVertexBuffer[WmCanvasContext::GCANVAS_VERTEX_BUFFER_SIZE];

    // save vertex shader map
    std::vector<GCanvasVertexShaderProperty> mVertexShaderProperties;
    
    bool mIsGLInited = false;

    GFrameBufferObjectPool mFrameBufferPool;

    bool mHiQuality;

#ifdef WMCANVAS
    GWebGLTxtImage2DFunc mWebGLTexImage2dFunc;
    GWebGLTxtSubImage2DFunc mWebGLTexSubImage2dFunc;
#ifdef IOS
    GWebGLBindToGLKViewFunc mWebGLBindGLKViewFunc;
#endif

#endif


};

#endif /* GCANVAS_GCANVASCONTEXT_H */
