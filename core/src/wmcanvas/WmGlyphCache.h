#ifndef GCANVAS_GGLYPHCACHE_H
#define GCANVAS_GGLYPHCACHE_H

#include "WmPoint.h"
#include "WmTexture.h"

#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>

class WmCanvasContext;

typedef std::tuple<std::string, wchar_t, std::string, bool> key_tuple;

struct key_hash : public std::unary_function<key_tuple, std::size_t>
{
    std::size_t operator()(const key_tuple &k) const
    {
        return std::hash<std::string>{}(std::get<0>(k)) ^ std::get<1>(k) ^
                std::hash<std::string>{}(std::get<2>(k)) ^ std::get<3>(k);
    }
};

struct key_equal : public std::binary_function<key_tuple, key_tuple, bool>
{
    bool operator()(const key_tuple &v0, const key_tuple &v1) const
    {
        return (
                std::get<0>(v0) == std::get<0>(v1) &&
                std::get<1>(v0) == std::get<1>(v1) &&
                std::get<2>(v0) == std::get<2>(v1) &&
                std::get<3>(v0) == std::get<3>(v1)
        );
    }
};


struct WmGlyphs
{
    /**
     * Wide character this glyph represents
     */
    wchar_t charcode;

    GTexture *texture;

    unsigned char *bitmapBuffer;

    /**
     * Glyph's width in pixels.
     */
    size_t width;

    /**
     * Glyph's height in pixels.
     */
    size_t height;

    /**
     * Glyph's left bearing expressed in integer pixels.
     */
    int offsetX;

    /**
     * Glyphs's top bearing expressed in integer pixels.
     *
     * Remember that this is the distance from the baseline to the top-most
     * glyph scanline, upwards y coordinates being positive.
     */
    int offsetY;

    /**
     * For horizontal text layouts, this is the horizontal distance (in
     * fractional pixels) used to increment the pen position when the glyph is
     * drawn as part of a string of text.
     */
    float advanceX;

    /**
     * For vertical text layouts, this is the vertical distance (in fractional
     * pixels) used to increment the pen position when the glyph is drawn as
     * part of a string of text.
     */
    float advanceY;

    /**
     * First normalized texture coordinate (x) of top-left corner
     */
    float s0;

    /**
     * Second normalized texture coordinate (y) of top-left corner
     */
    float t0;

    /**
     * First normalized texture coordinate (x) of bottom-right corner
     */
    float s1;

    /**
     * Second normalized texture coordinate (y) of bottom-right corner
     */
    float t1;

    /**
     * Glyph outline type (0 = None, 1 = line, 2 = inner, 3 = outer)
     */
    int outlineType;

    /**
     * Glyph outline thickness
     */
    float outlineThickness;

};

// key_tuple: <simpleFontName, charCode, fullFontName, isStroke>
typedef std::unordered_map<key_tuple, WmGlyphs, key_hash, key_equal> WmGlyphMap;


class WmFontManager;


class WmGlyphCache
{
public:
    WmGlyphCache(WmFontManager &fontManager);

    ~WmGlyphCache() {};

    const WmGlyphs *GetGlyph(const std::string &fontName, const wchar_t charcode,
                           const std::string &fullFontName, bool isStroke, bool autoLoadTexture = true);


    void Erase(const std::string &fontName, const wchar_t charcode, const std::string &font, bool isStroke);


    void
    Insert(const std::string &fontName, const wchar_t charcode, const std::string &font, bool isStroke,
           const WmGlyphs &glyph);

    void ClearGlyphsTexture();

private:

    bool LoadGlyphToTexture(WmGlyphs &glyph);

private:
    // WmCanvasContext *mContext;
    WmFontManager &mFontManager;
    WmGlyphMap mGlyphs;
};

#endif /* GCANVAS_GGLYPHCACHE_H */
