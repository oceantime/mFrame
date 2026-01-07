#ifndef WMCANVAS_WMCANVASSTATE_H
#define WMCANVAS_WMCANVASSTATE_H

#include "WmShader.h"
#include "WmPath.h"
#include "WmTransform.h"
#include "WmFillStyle.h"
#include "WmFontStyle.h"
#include "WmTextDefine.h"
#include "WmFontStyle.h"

#include <stdio.h>

const int InvalidateTextureId = -1;


class WmCanvasState
{
public:
    WmCanvasState();

    WmCanvasState(const WmCanvasState &state);

    WmCanvasState &operator=(const WmCanvasState &state);

    ~WmCanvasState();

    void Clone(const WmCanvasState&st);

    void ClearStyle();

    void ClearClip();
    
    //transfrom
    WmTransform mTransform;
    
    // Clip Paths
    std::vector<WmPath*> clipPaths;


    //fill style,stroke style,shadow
    WmColorRGBA mFillColor;
    WmFillStyle *mFillStyle;

    WmColorRGBA mStrokeColor;
    WmFillStyle *mStrokeStyle;

    WmColorRGBA mShadowColor;
    float mShadowBlur;
    float mShadowOffsetX;
    float mShadowOffsetY;
    
    //line style
    WmLineCap mLineCap;
    WmLineJoin mLineJoin;
    float mLineWidth;
    float mMiterLimit;
    
    std::vector<float> mLineDash;
    float mLineDashOffset;
    
    //font style
    wmcanvas::WmFontStyle *mFont;
    WmTextAlign mTextAlign;
    WmTextBaseline mTextBaseline;
    
    //global alpha / composite
    float mGlobalAlpha;
    WmCompositeOperation mGlobalCompositeOp;

    // TODO manager shader pointer
    // shader & texture
    WmShader *mShader;
    int mTextureId = InvalidateTextureId;

};


#endif /* WMCANVAS_WMCANVASSTATE_H */
