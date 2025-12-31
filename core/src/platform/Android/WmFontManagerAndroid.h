/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef GCANVAS_GFONTMANAGERANDROID_H
#define GCANVAS_GFONTMANAGERANDROID_H


class WmFont;

#include <WmFontManager.h>

#include "WmCanvas2DContextAndroid.h"
#include "wmcanvas/WmFontStyle.h"
#include "WmFontCache.h"

#define ANDROID_FONT_TEXTURE_SIZE 1024

class GFontCache;


class GFontManagerAndroid : public WmFontManager {

public:

    API_EXPORT GFontManagerAndroid(unsigned int w = ANDROID_FONT_TEXTURE_SIZE,
            unsigned int h = ANDROID_FONT_TEXTURE_SIZE);


    virtual ~GFontManagerAndroid();


    void DrawText(const unsigned short *text, unsigned int text_length, float x, float y,
                  bool isStroke, WmCanvasContext* context, float scaleX = 1, float scaleY = 1) override ;


    float MeasureText(const char *text, unsigned int text_length, wmcanvas::WmFontStyle *fontStyle) override;


    float* MeasureTextExt(const char *text, unsigned int text_length, wmcanvas::WmFontStyle *fontStyle) override;


    float* PreMeasureTextHeight(const char *text, unsigned int text_length, WmCanvasContext* context) override;


    GTexture* GetOrCreateFontTexture() override;


private:


    float* MeasureTextWidthHeight(const char *text, unsigned int text_length,
                            wmcanvas::WmFontStyle *fontStyle);


    void AdjustTextPenPoint(WmCanvasContext* context, std::vector<WmFont *>& font,
                            const unsigned short *text,
                            unsigned int textLength,
                            bool isStroke,
                            float &x, float &y, float sx, float sy);


    WmFont *GetFontByCharCode(wchar_t charCode, wmcanvas::WmFontStyle *fontStyle);


    void DrawTextInternal(WmCanvasContext *context, WmFont *font, bool isStroke, wchar_t text,
            float& x, float y, float sx, float sy);

    // global FontCache
    GFontCache *mFontCache;

};




#endif //GCANVAS_GFONTMANAGERANDROID_H
