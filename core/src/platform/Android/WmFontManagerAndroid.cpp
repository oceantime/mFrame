/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#include "WmFont.h"
#include "WmCanvas.hpp"
#include "WmFontCache.h"
#include "support/CharacterSet.h"
#include "WmFontManagerAndroid.h"
#include "WmCanvas2DContextAndroid.h"
#include "wmcanvas/WmFontManager.h"
#include "wmcanvas/WmCanvas2dContext.h"
#include "support/Log.h"
#include "support/CharacterSet.h"
#include "wmcanvas/WmTextDefine.h"

#include <assert.h>



WmFontManager *WmFontManager::NewInstance() {
    return new WmFontManagerAndroid(ANDROID_FONT_TEXTURE_SIZE, ANDROID_FONT_TEXTURE_SIZE);
}


WmFontManagerAndroid::WmFontManagerAndroid(unsigned w, unsigned h) : WmFontManager(w, h) {
    mFontCache = new WmFontCache(this);
}


WmFontManagerAndroid::~WmFontManagerAndroid() {
    if (mFontCache != nullptr) {
        delete mFontCache;
        mFontCache = nullptr;
    }
}


WmTexture* WmFontManagerAndroid::GetOrCreateFontTexture() {
    if (!mFontTexture) {
        std::vector<WmCanvasLog> logVec;
        mFontTexture = new WmTexture(ANDROID_FONT_TEXTURE_SIZE, ANDROID_FONT_TEXTURE_SIZE,
                GL_ALPHA, nullptr, &logVec);
        // FIXME
        // LOG_EXCEPTION_VECTOR(mHooks, "SharedFontTexture", logVec);
    }
    return mFontTexture;
}


void WmFontManagerAndroid::DrawText(const unsigned short *text, unsigned int text_length, float x, float y,
                                   bool isStroke, WmCanvasContext *context, float sx, float sy) {
    if (text == nullptr || text_length == 0) {
        return;
    }

    std::vector<WmFont*> fonts;
    wmcanvas::WmFontStyle* fontStyle = context->mCurrentState->mFont;
    for (unsigned int i = 0; i < text_length; ++i) {
        fonts.push_back(GetFontByCharCode(text[i], fontStyle));
    }

    // get current font scale
    AdjustTextPenPoint(context, fonts, text, text_length, isStroke, x, y, sx, sy);

    for (unsigned int i = 0; i < text_length; ++i) {
        DrawTextInternal(context, fonts[i], isStroke, text[i], x, y, sx, sy);
    }
}


float *WmFontManagerAndroid::MeasureTextWidthHeight(const char *text, unsigned int textLength,
                                                    wmcanvas::WmFontStyle *fontStyle) {
    if (text == nullptr || textLength == 0) {
        float *ret = new float[4];
        ret[0] = ret[1] = ret[2] = ret[3] = 0.0;
        return ret;
    }

    Utf8ToUCS2 *lbData = new Utf8ToUCS2(text, textLength);
    unsigned short *ucs = lbData->ucs2;
    textLength = lbData->ucs2len;

    std::vector<WmFont*> fonts;
    float sx = 1;
    float sy = 1;

    for (unsigned int i = 0; i < textLength; ++i) {
        fonts.push_back(GetFontByCharCode(ucs[i], fontStyle));
    }

    float deltaX = 0;
    float maxHeight = 0;

    float fontSize = fontStyle->GetSize();

    for (unsigned int i = 0; i < textLength; ++i) {
        auto glyph = fonts[i]->GetOrLoadGlyph(fontStyle, ucs[i], false, sx, sy);

        if (glyph != nullptr) {
            deltaX += glyph->advanceX / sx;
        }
    }

    if (fonts.size() > 0) {
        //文本高度本来应该去除上下空白的，但是laya1，还是有点问题，所以这个高度，还是用老的高度ascender+descender绝对值
        maxHeight = fabs(fonts[0]->GetMetrics()->ascender / sy) +
                    fabs(fonts[0]->GetMetrics()->descender / sy);
    }

    // 如果满足条件则，返回不带空白的文字高度
    float m_ascender = fontStyle->GetAscender();
    float m_descender = fontStyle->GetDescender();
    if (m_ascender > 0 && m_descender > 0) {
        maxHeight = m_ascender + m_descender;
    }

    delete lbData;

    float *ret = new float[4];
    ret[0] = deltaX;
    ret[1] = maxHeight;
    ret[2] = ret[3] = 0.0;
    return ret;
}

float WmFontManagerAndroid::MeasureText(const char *text,
                                       unsigned int textLength, wmcanvas::WmFontStyle *fontStyle) {
    float *tmpMeasure = MeasureTextWidthHeight(text, textLength, fontStyle);
    float width = tmpMeasure[0];
    delete tmpMeasure;
    return width;
}


float *WmFontManagerAndroid::MeasureTextExt(const char *text, unsigned int textLength,
                                           wmcanvas::WmFontStyle *fontStyle) {
    return  MeasureTextWidthHeight(text, textLength, fontStyle);
}


float *WmFontManagerAndroid::PreMeasureTextHeight(const char *text, unsigned int text_length,
                                                WmCanvasContext *context) {
    if (text == nullptr || text_length == 0) {
        float *ret = new float[4];
        ret[0] = ret[1] = ret[2] = ret[3] = 0.0;
        return ret;
    }

    Utf8ToUCS2 *lbData = new Utf8ToUCS2(text, text_length);

    unsigned short *ucs = lbData->ucs2;
    text_length = lbData->ucs2len;

    std::vector<WmFont*> fonts;

    wmcanvas::WmFontStyle* fontStyle = context->mCurrentState->mFont;
    float sx = GTransformGetScaleX(context->mCurrentState->mTransform);
    float sy = GTransformGetScaleY(context->mCurrentState->mTransform);

    for (unsigned int i = 0; i < text_length; ++i) {
        fonts.push_back(GetFontByCharCode(ucs[i], fontStyle));
    }

    float top = 0;
    float height = 0;
    float ascender = 0;
    float descender = 0;

    for (unsigned int i = 0; i < text_length; ++i) {
        //todo fixme
        auto glyph = fonts[i]->GetOrLoadGlyph(fontStyle, ucs[i], false, sx, sy);
        if (glyph != nullptr) {
            top = glyph->offsetY / sy;
            height = glyph->height / sy;
            ascender = fonts[i]->GetMetrics()->ascender / sy;
            descender = fonts[0]->GetMetrics()->descender / sy;
        }
    }

    delete lbData;

    float *ret = new float[4];
    ret[0] = top;
    ret[1] = height;
    ret[2] = ascender;
    ret[3] = descender;
    return ret;
}


void WmFontManagerAndroid::AdjustTextPenPoint(WmCanvasContext *context, std::vector<WmFont*>& font,
                                             const unsigned short *text, unsigned int textLength,
                                             bool isStroke, float &x, float &y, float sx, float sy) {
    wmcanvas::WmFontStyle *fontStyle = context->mCurrentState->mFont;

    if (context->mCurrentState->mTextAlign != WmTextAlign::TEXT_ALIGN_START &&
        context->mCurrentState->mTextAlign != WmTextAlign::TEXT_ALIGN_LEFT) {
        auto left_x = x;
        auto delta_x = 0.0f;
        for (unsigned int i = 0; i < textLength; ++i) {
            auto glyph = font[i]->GetOrLoadGlyph(fontStyle, text[i], isStroke, sx, sy,
            context->mCurrentState->mLineWidth,context->GetDevicePixelRatio());
            if (glyph != nullptr) {
                delta_x += glyph->advanceX / sx;
            }
        }

        if (context->mCurrentState->mTextAlign == WmTextAlign::TEXT_ALIGN_CENTER) {
            x = left_x - delta_x / 2.0f;
        } else { // textAlign is "Right" or "End"
            x = left_x - delta_x;
        }
    }

    WmFont *font0 = font[0];
    // update font metrics
    const WmGlyphs* glyph = font0->GetOrLoadGlyph(fontStyle, text[0], isStroke, sx, sy,
       context->mCurrentState->mLineWidth,context->GetDevicePixelRatio());
    if (glyph == nullptr) { // fail
        return;
    }

    auto font_metrics = font0->GetMetrics();
    auto ascender = font_metrics->ascender / sy;
    auto descender = font_metrics->descender / sy;

    //m_ascender，m_descender不能除scale，因为在PreMeasureTextHeight获取这两个值时，已经除过了
    float m_ascender = fontStyle->GetAscender();
    float m_descender = fontStyle->GetDescender();

    switch (context->mCurrentState->mTextBaseline) {
        case TEXT_BASELINE_TOP:
        case TEXT_BASELINE_HANGING:
            if (m_ascender > 0 && m_descender > 0) {
                y += fabs(m_ascender);
            } else {
                y += fabs(ascender);
            }
            break;
        case TEXT_BASELINE_BOTTOM:
        case TEXT_BASELINE_IDEOGRAPHIC:
            if (m_ascender > 0 && m_descender > 0) {
                y -= fabs(m_descender);
            } else {
                y -= fabs(descender);
            }
            break;
        case TEXT_BASELINE_MIDDLE:
            if (m_ascender > 0 && m_descender > 0) {
                y += (fabs(m_ascender) - fabs(m_descender)) / 2.0f;
            } else {
                y += (fabs(ascender) - fabs(descender)) / 2.0f;
            }
            break;
        case TEXT_BASELINE_ALPHABETIC:
        default:
            break;
    }
}



void WmFontManagerAndroid::DrawTextInternal(WmCanvasContext *context, WmFont *font, bool isStroke, wchar_t charCode,
                                           float &x, float y, float sx, float sy) {
    if (isStroke) {
        font->DrawText(context, charCode, x, y, context->StrokeStyle(), sx, sy, isStroke);
    } else {
        font->DrawText(context, charCode, x, y, context->FillStyle(), sx, sy, isStroke);
    }
}


/**
 * query font by charcode
 */
WmFont *WmFontManagerAndroid::GetFontByCharCode(wchar_t charCode, wmcanvas::WmFontStyle *fontStyle) {
    return mFontCache->GetOrCreateFont(fontStyle, charCode);
}