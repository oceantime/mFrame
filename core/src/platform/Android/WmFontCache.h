/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#ifndef __WmCanvas__WmFontCache__
#define __WmCanvas__WmFontCache__

#include "WmFont.h"
#include <iostream>
#include <map>
#include <queue>
#include <string>


class WmCanvas2DContextAndroid;


class WmFont;
namespace wmcanvas
{
    class WmFontStyle;
}


struct WmFontKeySet {
    std::string fontFileName;

    std::string fallbackFontFileName;
};



class WmFontManager;

class WmFontCache
{
public:

    WmFontCache(WmFontManager* fontManager);


    ~WmFontCache();


#ifdef WMFONT_LOAD_BY_FREETYPE

    WmFont *GetOrCreateFont(wmcanvas::WmFontStyle *fontStyle, wchar_t charCode);

#else
    WmFont *GetOrCreateFont(const std::string &key);
#endif


private:

    void Clear();


    bool LazyInitFontLibrary();


    char *TryDefaultFont(const wchar_t charCode,
                         const char *currentFontLocation);


    char *TryDefaultFallbackFont(const wchar_t charCode,
                                 const char *currentFontLocation);


    char *TrySpecFont(const wchar_t charCode,
                      const char *currentFontLocation,
                      const char *specFontFile);


    char *TryFontFile(const wchar_t charCode, const char *fileFullPath, const char* fileName);


    WmFont* LoadAndSaveFont(const char* fontFileName);


private:

    WmFontManager* mFontManager;

    FT_Library mFtLibrary = nullptr;

    std::unordered_map<std::string, WmFontKeySet> mFontRefMap;

    std::unordered_map<std::string, WmFont*> mFontMap;

};


#endif