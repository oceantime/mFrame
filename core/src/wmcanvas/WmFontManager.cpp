//
// Created by zhanqu on 2020/3/25.
//

#include "../support/Log.h"
#include "WmFontManager.h"
#include "GL/WmTexture.h"
#include <vector>


WmFontManager::WmFontManager(unsigned w, unsigned h) : mGlyphCache(*this), mTreemap(w, h) {
    mFontTexture = nullptr;
};


WmFontManager::~WmFontManager() {
    ClearFontBuffer();
}


GTexture *WmFontManager::GetOrCreateFontTexture() {
    if (mFontTexture == nullptr) {
        std::vector<WmCanvasLog> logVec;
        mFontTexture = new GTexture(mTreemap.GetWidth(), mTreemap.GetHeight(), GL_ALPHA, nullptr,
                                    &logVec);
        // FIXME
        // LOG_EXCEPTION_VECTOR(mHooks, mContextId.c_str(), logVec);
    }
    return mFontTexture;
}


/**
 * dumb impl
 */
float* WmFontManager::PreMeasureTextHeight(const char *text, unsigned int text_length, WmCanvasContext* context) {
    float *ret = new float[4];
    return ret;
}


float* WmFontManager::MeasureTextMetrics(const char *text, unsigned int text_length, wmcanvas::WmFontStyle *fontStyle) {
    return MeasureTextExt(text, text_length, fontStyle);
}


void WmFontManager::ClearFontBuffer() {
    mTreemap.Clear();
    mGlyphCache.ClearGlyphsTexture();

    if (mFontTexture != nullptr) {
        delete mFontTexture;
        mFontTexture = nullptr;
    }
}


bool WmFontManager::LoadGlyphToTexture(WmGlyphs &glyph) {
    GTexture *texture = GetOrCreateFontTexture();
    GRect rect;
    bool flag = PrepareGlyphTexture((int)glyph.width, (int)glyph.height, rect);
    if (!flag) {
        ClearFontBuffer();
        texture = GetOrCreateFontTexture();
        flag = PrepareGlyphTexture((int)glyph.width, (int)glyph.height, rect);
    }
    if(texture==nullptr){
        printf("GetOrCreateFontTexture is null \n");
    }
    if (!flag) {
        return false;
    } else {
        unsigned int texWidth = texture->GetWidth();
        unsigned int texHeight = texture->GetHeight();
        if (glyph.bitmapBuffer != nullptr) {
            texture->UpdateTexture(glyph.bitmapBuffer, rect.x, rect.y, rect.width, rect.height);
            glyph.texture = texture;
            glyph.s0 = (float) rect.x / texWidth;
            glyph.t0 = (float) rect.y / texHeight;
            glyph.s1 = (float) (rect.x + rect.width) / texWidth;
            glyph.t1 = (float) (rect.y + rect.height) / texHeight;
//
//            LOG_E("LoadGlyphToTexture: s0=%f, t0=%f, s1=%f, s2=%f, textur=%p, bitmapBuffer=%p",
//                glyph.s0, glyph.t0, glyph.s1, glyph.t1, texture, glyph.bitmapBuffer);

            delete[] glyph.bitmapBuffer;
            glyph.bitmapBuffer = nullptr;

            return true;
        } else {
            return false;
        }
    }
}


bool WmFontManager::AddGlyph(std::string& fontFileName, std::string& glyphKey, WmGlyphs& glyph, bool isStroke) {
    bool flag = LoadGlyphToTexture(glyph);
    if (flag) {
        mGlyphCache.Insert(fontFileName, glyph.charcode, glyphKey, isStroke, glyph);
    } else {
        return false;
    }
    return true;
}


bool WmFontManager::PrepareGlyphTexture(int w, int h, GRect& rect) {
    GSize size(w, h);
    return mTreemap.Add(size, rect);
}

