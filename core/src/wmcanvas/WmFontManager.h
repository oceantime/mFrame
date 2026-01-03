#ifndef WMCANVAS_WMFONTMANAGER_H
#define WMCANVAS_WMFONTMANAGER_H

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

    bool PrepareGlyphTexture(int w, int h, WmRect& rect);

    bool AddGlyph(std::string& fontFileName, std::string& glyphKey, WmGlyphs& glyph, bool isStroke);

    bool LoadGlyphToTexture(WmGlyphs& glyph);

    
    virtual WmTexture* GetOrCreateFontTexture();
        
    virtual void DrawText(const unsigned short *text, unsigned int text_length, float x, float y,
                          bool isStroke, WmCanvasContext* context, float scaleX=1, float scaleY=1)=0;

    virtual float MeasureText(const char *text, unsigned int text_length, wmcanvas::WmFontStyle *fontStyle)=0;
    
    // deprecated, use MeasureTextMetrics
    virtual float* MeasureTextExt(const char *text, unsigned int text_length, wmcanvas::WmFontStyle *fontStyle)=0;

    float* MeasureTextMetrics(const char *text, unsigned int text_length, wmcanvas::WmFontStyle *fontStyle);

    virtual float* PreMeasureTextHeight(const char *text, unsigned int text_length, WmCanvasContext* context);

protected:

    WmFontManager(unsigned w = FontTextureWidth, unsigned h = FontTextureHeight);

    void ClearFontBuffer();

public:

    WmTexture* mFontTexture;

    WmGlyphCache mGlyphCache;
    WmTreemap mTreemap;
};

#endif /* WMCANVAS_WMFONTMANAGER_H */
