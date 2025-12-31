/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#ifndef __GCanvas__GFontCache__
#define __GCanvas__GFontCache__

#include "WmFont.h"
#include <iostream>
#include <map>
#include <queue>
#include <string>

class WmFont;
namespace wmcanvas
{
    class WmFontStyle;
}

struct GFontSet
{
    WmFont *font = nullptr;
    WmFont *fallbackFont = nullptr;
};

class WmFontManager;

class GFontCache
{
public:
    GFontCache(WmFontManager &fontManager);

    ~GFontCache();

#ifdef WMFONT_LOAD_BY_FREETYPE

    WmFont *GetOrCreateFont(wmcanvas::WmFontStyle *fontStyle, wchar_t charCode, const float size);

#else
    WmFont *GetOrCreateFont(const std::string &key);
#endif

    void ReadyToRemoveCacheForFonts(
        const std::map<WmFont *, std::vector<wchar_t>> &fontsToBeDeleted, bool isStroke);

    void RemoveCacheForFonts(
        const std::map<WmFont *, std::vector<wchar_t>> &fontsToBeDeleted, bool isStroke);

private:
    void Clear();

    bool LoadFace(FT_Library *library, const char *filename, const float size,
                  FT_Face *face);

    char *TryDefaultFont(const wchar_t charCode, const float size,
                         const char *currentFontLocation);

    char *TryDefaultFallbackFont(const wchar_t charCode, const float size,
                                 const char *currentFontLocation);

    char *TryOtherFallbackFont(WmCanvasContext *context, const wchar_t charCode, const float size,
                               const char *currentFontLocation, wmcanvas::WmFontStyle *fontStyle);

    char *TrySpecFont(const wchar_t charCode, const float size,
                      const char *currentFontLocation,
                      const char *specFontFile);

    bool IsGlyphExistedInFont(const wchar_t charCode, const float size,
                              const std::string &filename);

private:
    WmFontManager &mFontManager;
    std::map<std::string, GFontSet> mFontCache;
    std::queue<std::map<WmFont *, std::vector<wchar_t>>> mCachedPages;
    FT_Library mFtLibrary = nullptr;
    bool LazyInitFontLibrary();
};

#endif