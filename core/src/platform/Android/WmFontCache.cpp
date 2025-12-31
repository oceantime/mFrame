/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#include <sstream>
#include <assert.h>

#include "WmFontCache.h"
#include "WmSystemFontInformation.h"
#include "support/Log.h"
#include "WmFontManager.h"
#include "WmCanvas2dContext.h"
#include "WmFreeTypeWrap.h"



using namespace wmcanvas;



WmFontCache::WmFontCache(WmFontManager *fontManager) : mFontManager(fontManager) {
    mFtLibrary = nullptr;
}

WmFontCache::~WmFontCache() { Clear(); }


void WmFontCache::Clear() {
    size_t mapSize = mFontMap.size();
    auto iter = mFontMap.begin();
    for (; iter != mFontMap.end(); ++iter) {
        delete iter->second;
    }
    mFontMap.clear();

    size_t fontRefSize = mFontRefMap.size();
    mFontRefMap.clear();

    wmcanvas::WmFT_DisposeLibrarySafe(mFtLibrary);
    mFtLibrary = nullptr;

    LOG_E("WmFontCache(%p) Clear:fontRefMap size=%u, fontMap size=%u",
            this, fontRefSize, mapSize);
}


WmFont *WmFontCache::GetOrCreateFont(WmFontStyle *fontStyle, wchar_t charCode) {
    if (!LazyInitFontLibrary()) {
        return nullptr;
    }

    // char key[256] = {0};
    // snprintf(key, 256, "%s_%s_", contextId.c_str(), GetScaleFontName(context, sx, sy).c_str());
    std::string fontStyleKey = fontStyle->GetFullFontStyle();
    if (fontStyleKey.empty()) {
        fontStyleKey = "DefaultFontStyle";
    }

    auto iter = mFontRefMap.find(fontStyleKey);
    if (iter != mFontRefMap.end()) {
        std::string fontFileName = iter->second.fontFileName;

        auto fontIter = mFontMap.find(fontFileName);
        if (fontIter != mFontMap.end()) {
            if (fontIter->second->IsCharInFont(charCode)) {
                return fontIter->second;
            }
        }

        auto fallbackFontIter = mFontMap.find(iter->second.fallbackFontFileName);
        if (fallbackFontIter != mFontMap.end()) {
            if (fallbackFontIter->second->IsCharInFont(charCode)) {
                return fallbackFontIter->second;
            }
        }
    }

    const char *defaultSystemFontLocation = "/system/fonts/";
    SystemFontInformation* systemFontInformation = SystemFontInformation::GetSystemFontInformation();
    auto systemFontLocation = systemFontInformation->GetSystemFontLocation();
    const char *currentFontLocation = (systemFontLocation != nullptr) ? systemFontLocation : defaultSystemFontLocation;
    const char *currentFontFile = nullptr;
    const char *currentFontFileCopy = nullptr;

    auto fontFamily = systemFontInformation->FindFontFamily(fontStyle->GetFamily().data());
    if (fontFamily != nullptr) {
        currentFontFile = fontFamily->MatchFamilyStyle(*fontStyle);
        currentFontFileCopy = currentFontFile;
        // LOG_E("TrySpecFont MatchFamilyStyle  %s", currentFontFile);
    }

    // try match specific font
    // LOG_E("TrySpecFont defaultSystemLocation");
    if (nullptr != currentFontFile) {
        currentFontFile = TrySpecFont(charCode, currentFontLocation, currentFontFile);
    }

    // external font logic
    // LOG_E("TrySpecFont GetExtraFontLocation");
    const char *extraFontLocation = systemFontInformation->GetExtraFontLocation();
    if (currentFontFile == nullptr && currentFontFileCopy != nullptr && extraFontLocation != nullptr) {
        currentFontFile = TrySpecFont(charCode, extraFontLocation, currentFontFileCopy);
        if (currentFontFile != nullptr) {
            currentFontLocation = extraFontLocation;
        }
    }

    // LOG_E("TrySpecFont TryDefaultFont");
    if (currentFontFile == nullptr) {
        currentFontFile = TryDefaultFont(charCode, currentFontLocation);
        if (nullptr == currentFontFile) {
            currentFontFile = TryDefaultFallbackFont(charCode, currentFontLocation);
            if (nullptr == currentFontFile) {
                // LOG_E("TrySpecFont GetClosestFontFamily");
                currentFontFile = systemFontInformation->GetClosestFontFamily(mFtLibrary, currentFontLocation, charCode,
                                               12, 12, *fontStyle);
            }
        }
    }


    std::string fontFileFullPath = currentFontLocation;
    if (currentFontFile[0] == '/') {
        fontFileFullPath = currentFontFile;
    } else  {
        fontFileFullPath += currentFontFile;
    }

    WmFont* font = nullptr;
    auto newIter = mFontMap.find(fontFileFullPath);
    if (mFontMap.find(fontFileFullPath) != mFontMap.end()) {
        font = newIter->second;
    } else {
        font = LoadAndSaveFont(fontFileFullPath.data());
    }

    // save to font ref map
    WmFontKeySet& fontKeySet = mFontRefMap[fontStyleKey];
    if (fontKeySet.fontFileName.empty()) {
        fontKeySet.fallbackFontFileName = font->GetFontFileName();
    } else {
        fontKeySet.fontFileName = font->GetFontFileName();
    }

    // LOG_E("GetOrCreateFont: style=%s, font=%s", fontStyleKey.data(), font->GetFontFileName().data());
    return font;
}


char *WmFontCache::TrySpecFont(const wchar_t charCode,
                              const char *currentFontLocation,
                              const char *specFontFile) {
    // LOG_E("TrySpecFont: %s, %s", currentFontLocation, specFontFile);
    std::string fontFileFullPath = currentFontLocation;
    if (specFontFile[0] == '/') {
        fontFileFullPath = specFontFile;
    } else {
        fontFileFullPath += specFontFile;
    }
    return TryFontFile(charCode, fontFileFullPath.data(), specFontFile);
}


char *WmFontCache::TryDefaultFont(const wchar_t charCode,
                                 const char *currentFontLocation) {
    auto defaultFontFile =
            SystemFontInformation::GetSystemFontInformation()->GetDefaultFontFile();

    if (defaultFontFile == nullptr)  {
        return nullptr;
    }

    std::string fontFileFullPath = currentFontLocation;
    fontFileFullPath += defaultFontFile;

    return TryFontFile(charCode, fontFileFullPath.data(), defaultFontFile);
}


char *WmFontCache::TryDefaultFallbackFont(const wchar_t charCode,
                                         const char *currentFontLocation) {
    auto defaultFontFile = "DroidSans.ttf";

    std::string fontFileFullPath = currentFontLocation;
    fontFileFullPath += defaultFontFile;

    return TryFontFile(charCode, fontFileFullPath.data(), defaultFontFile);
}


bool WmFontCache::LazyInitFontLibrary() {
    if (mFtLibrary == nullptr) {
        return wmcanvas::WmFT_InitLibrary(&mFtLibrary);
    }
    return true;
}


char * WmFontCache::TryFontFile(const wchar_t charCode, const char *fileFullPath, const char* fileName) {
    char* result = nullptr;
    auto fontIter = mFontMap.find(fileFullPath);
    if (fontIter != mFontMap.end()) {
        if (fontIter->second->IsCharInFont(charCode)) {
            result = (char*)fileName;
        }
    } else {
        WmFont* font = LoadAndSaveFont(fileFullPath);
        if (font->IsCharInFont(charCode)) {
            result = (char*)fileName;
        }
    }
    if (result != nullptr) {
        // LOG_E("TryFontFile:match, charCode=%i, fontFile=%s", charCode, fileFullPath);
    }
    return result;
}


WmFont* WmFontCache::LoadAndSaveFont(const char* fontFileName) {
    WmFont *font = new WmFont(*mFontManager, fontFileName);
    font->SetFtLibrary(mFtLibrary);
    // save to font map
    mFontMap.insert(std::pair<std::string, WmFont*>(fontFileName, font));
    // LOG_E("LoadAndSaveFont:%s, font=%p", fontFileName, font);
    return font;
}

