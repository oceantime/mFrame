#ifndef __WmCanvas__WmFont__
#define __WmCanvas__WmFont__

#include "WmPoint.h"
#include "WmTexture.h"
#include "WmFontStyle.h"
#include "WmGlyphCache.h"
#include "WmTreemap.h"

#include <map>
#include <string>
#include <vector>

#include "WmFreeTypeWrap.h"


class WmCanvasContext;



std::string GetGlyphKey(std::string& fontFileName, wmcanvas::WmFontStyle* fontStyle,
        float scaleFontX, float scaleFontY);


//std::string FontStyleNameForScale(std::string& fontName, float scaleFontX, float scaleFontY);


//std::string GetScaleFontName(WmCanvasContext *context, float scaleFontX, float scaleFontY);


//std::string GetCurrentScaleFontName(WmCanvasContext *context);



typedef struct WmFontMetrics
{
    WmFontMetrics();
    // units of EM square: font units
    unsigned short unitsPerEM;
    // sized ascender: 26.6 pixel format
    float ascender;
    // sized descender" 26.6 pixel format: This could be negative.
    float descender;
} WmFontMetrics;



class WmFontManager;


/**
 * Font Draw Impl
 * A WmFont is a FreeType face wrap, different style(weight, italic) use different WmFont,
 * a WmFont can generate any size glyph and font bitmap
 */
class WmFont
{
public:
#ifdef WMFONT_LOAD_BY_FREETYPE
    WmFont(WmFontManager& fontManager, const char *fontFileName);
#else
    WmFont(const char *fontDefinition);
#endif
    ~WmFont();


    void DrawText(WmCanvasContext *context, wchar_t text, float &x, float y,
                  WmColorRGBA color, float scaleX, float scaleY, bool isStroke);


    void DrawText(WmCanvasContext *context, const wchar_t *text, float &x,
                  float y, WmColorRGBA color, float scaleX, float scaleY, bool isStroke);


    const WmGlyphs *GetOrLoadGlyph(wmcanvas::WmFontStyle* fontStyle, const wchar_t charcode, bool isStroke,
                           float scaleX, float scaleY,float lineWidth=1.0,float deviceRatio=1.0);


    // deprecated
    void RemoveGlyph(const wchar_t charcode, bool isStroke);


    WmFontMetrics *GetMetrics() { return &mFontMetrics; }


    const std::string &GetFontFileName() const;


    static void SetFontCallback(void *(*getFontCallback)(const char *fontDefinition),
                    bool (*getFontImageCallback)(
                            void *font, wchar_t charcode, int &ftBitmapWidth,
                            int &ftBitmapHeight, unsigned char *&bitmapBuffer,
                            int &left, int &top, float &advanceX, float &advanceY));


    bool IsCharInFont(const wchar_t charCode);


    void SetFtLibrary(FT_Library libraryPtr);

private:

  void DrawGlyph(WmCanvasContext *context, const WmGlyphs *glyph, float x, float y,
                float scaleX, float scaleY, WmColorRGBA color, bool needDrawShadow);


    static void *(*getFontCallback)(const char *fontDefinition);


    static bool (*getFontImageCallback)(void *font, wchar_t charCode,
                                        int &ftBitmapWidth, int &ftBitmapHeight,
                                        unsigned char *&bitmapBuffer, int &left,
                                        int &top, float &advanceX,
                                        float &advanceY);

#ifdef WMFONT_LOAD_BY_FREETYPE

    void LoadGlyphs(wmcanvas::WmFontStyle* style, const wchar_t *charCodes, bool isStroke,
                    float scaleX, float scaleY,float lineWidth,float deviceRatio);

#endif

    bool LoadStroke(const char *filename, FT_Stroker *stroker, float scaleX, float scaleY,float lineWidth,float deviceRatio);


    bool LoadFaceIfNot();


    bool PrepareLoadGlyph(float fontSize, float scaleX, float scaleY);


    void UpdateCurrentTextMetrics();


private:

    WmFontManager& mFontManager;

#ifdef WMFONT_LOAD_BY_FREETYPE
    float mPointSize;

    // font file path
    std::string mFontFileName;

    int mHinting;            // whether to use autohint when rendering font
    int mOutlineType;        //(0 = None, 1 = line, 2 = inner, 3 = outer)
    float mOutlineThickness;
#endif

    std::string mFontDefinition;
    bool mHasSetMetrics;

    WmFontMetrics mFontMetrics;

    // not own FT_Library, set from external, do not dispose!!
    FT_Library mLibrary = nullptr;

    FT_Face mFace = nullptr;

    // FT_Stroker mStroker = nullptr;

};

#endif
