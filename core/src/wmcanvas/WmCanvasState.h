/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef GCANVAS_GCANVASSTATE_H
#define GCANVAS_GCANVASSTATE_H

#include "WmShader.h"
#include "WmPath.h"
#include "WmTransform.h"
#include "WmFillStyle.h"
#include "WmFontStyle.h"
#include "WmTextDefine.h"
#include "WmFontStyle.h"

#include <stdio.h>

const int InvalidateTextureId = -1;


class GCanvasState
{
public:
    GCanvasState();

    GCanvasState(const GCanvasState &state);

    GCanvasState &operator=(const GCanvasState &state);

    ~GCanvasState();

    void Clone(const GCanvasState&st);

    void ClearStyle();

    void ClearClip();
    
    //transfrom
    WmTransform mTransform;
    
    // Clip Paths
    std::vector<WmPath*> clipPaths;


    //fill style,stroke style,shadow
    WmColorRGBA mFillColor;
    GFillStyle *mFillStyle;

    WmColorRGBA mStrokeColor;
    GFillStyle *mStrokeStyle;

    WmColorRGBA mShadowColor;
    float mShadowBlur;
    float mShadowOffsetX;
    float mShadowOffsetY;
    
    //line style
    GLineCap mLineCap;
    GLineJoin mLineJoin;
    float mLineWidth;
    float mMiterLimit;
    
    std::vector<float> mLineDash;
    float mLineDashOffset;
    
    //font style
    wmcanvas::WmFontStyle *mFont;
    WmTextAlign mTextAlign;
    GTextBaseline mTextBaseline;
    
    //global alpha / composite
    float mGlobalAlpha;
    WmCompositeOperation mGlobalCompositeOp;

    // TODO manager shader pointer
    // shader & texture
    WmShader *mShader;
    int mTextureId = InvalidateTextureId;

};


#endif /* GCANVAS_GCANVASSTATE_H */
