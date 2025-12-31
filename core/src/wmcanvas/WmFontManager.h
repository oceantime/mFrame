/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#ifndef GCANVAS_GFONTMANAGER_H
#define GCANVAS_GFONTMANAGER_H

#include "WmPoint.h"
#include "WmTexture.h"
#include "WmGlyphCache.h"
#include "WmTreemap.h"
#include <map>
#include <string>
#include <vector>


class WmCanvasContext;

#define FontTextureWidth        2048
#define FontTextureHeight       2048

namespace wmcanvas
{
    class WmFontStyle;
}

class WmFontManager
{
public:

    static WmFontManager *NewInstance();
    virtual  ~WmFontManager();

    bool PrepareGlyphTexture(int w, int h, GRect& rect);

    bool AddGlyph(std::string& fontFileName, std::string& glyphKey, WmGlyphs& glyph, bool isStroke);

    bool LoadGlyphToTexture(WmGlyphs& glyph);

    
    virtual GTexture* GetOrCreateFontTexture();
        
    virtual void DrawText(const unsigned short *text, unsigned int text_length, float x, float y,
                          bool isStroke, WmCanvasContext* context, float scaleX=1, float scaleY=1)=0;

    virtual float MeasureText(const char *text, unsigned int text_length, wmcanvas::WmFontStyle *fontStyle)=0;
    
    // deprecated, use MeasureTextMetrics
    virtual float* MeasureTextExt(const char *text, unsigned int text_length, wmcanvas::WmFontStyle *fontStyle)=0;

    //return float[4]，0：top，1：height，2：ascender，3：descender
    float* MeasureTextMetrics(const char *text, unsigned int text_length, wmcanvas::WmFontStyle *fontStyle);

    virtual float* PreMeasureTextHeight(const char *text, unsigned int text_length, WmCanvasContext* context);

protected:

    WmFontManager(unsigned w = FontTextureWidth, unsigned h = FontTextureHeight);

    void ClearFontBuffer();

public:

    GTexture* mFontTexture;

    WmGlyphCache mGlyphCache;
    GTreemap mTreemap;
};

#endif /* GCANVAS_GFONTMANAGER_H */
