#include "WmGlyphCache.h"
#include "WmCanvas2dContext.h"

WmGlyphCache::WmGlyphCache(WmFontManager &fontManager) :mFontManager(fontManager) {

}

const WmGlyphs *WmGlyphCache::GetGlyph(const std::string& fontName,
                                    const wchar_t charCode,
                                    const std::string &glyphKey,
                                    bool isStroke, bool autoLoadTexture) {
    WmGlyphMap::iterator iter = mGlyphs.find(make_tuple(fontName, charCode, glyphKey, isStroke));
    if (iter != mGlyphs.end()) {
        if (autoLoadTexture) {
            printf("the autoLoadTexture \n");   
            if (!iter->second.texture) {
                // if texture is empty, we save glyph to texture and then get textureId
                if (!mFontManager.LoadGlyphToTexture(iter->second)) {
                    LOG_E("GetGlyph:LoadGlyphToTexture fail,char=%i", charCode);
                    return nullptr;
                }
            }
        }
        return &(iter->second);
    } else { // no glyph cache
        //        fontName.data(), glyphKey.data(), charCode, isStroke);
    }
    return nullptr;
}


void WmGlyphCache::ClearGlyphsTexture() {
    for (WmGlyphMap::iterator i = mGlyphs.begin(); i != mGlyphs.end(); ++i) {
        unsigned char *buffer = i->second.bitmapBuffer;
        if (buffer != nullptr) {
            delete[] buffer;
            buffer = nullptr;
        }
        i->second.bitmapBuffer = nullptr;
        i->second.texture = nullptr;
    }
    mGlyphs.clear();
}


void WmGlyphCache::Erase(const std::string& fontName,
                        const wchar_t charCode,
                        const std::string &font,
                        bool isStroke) {
    mGlyphs.erase(make_tuple(fontName, charCode, font,isStroke));
}


void WmGlyphCache::Insert(const std::string& fontName,
                         const wchar_t charCode,
                         const std::string &glyphKey,
                         bool isStroke,
                         const WmGlyphs &glyph) {
    mGlyphs.insert(WmGlyphMap::value_type(make_tuple(fontName, charCode, glyphKey, isStroke), glyph));
}
