#ifndef WMCANVAS_WMFONTMANAGERANDROID_H
#define WMCANVAS_WMFONTMANAGERANDROID_H


class WmFont;

#include <WmFontManager.h>

#include "WmCanvas2DContextAndroid.h"
#include "wmcanvas/WmFontStyle.h"
#include "WmFontCache.h"

#define ANDROID_FONT_TEXTURE_SIZE 1024

class WmFontCache;


class WmFontManagerAndroid : public WmFontManager {

public:

        API_EXPORT WmFontManagerAndroid(unsigned int w = ANDROID_FONT_TEXTURE_SIZE,
            unsigned int h = ANDROID_FONT_TEXTURE_SIZE);


        virtual ~WmFontManagerAndroid();


    void DrawText(const unsigned short *text, unsigned int text_length, float x, float y,
                  bool isStroke, WmCanvasContext* context, float scaleX = 1, float scaleY = 1) override ;


    float MeasureText(const char *text, unsigned int text_length, wmcanvas::WmFontStyle *fontStyle) override;


    float* MeasureTextExt(const char *text, unsigned int text_length, wmcanvas::WmFontStyle *fontStyle) override;


    float* PreMeasureTextHeight(const char *text, unsigned int text_length, WmCanvasContext* context) override;


    WmTexture* GetOrCreateFontTexture() override;


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
    WmFontCache *mFontCache;

};




#endif //WMCANVAS_WMFONTMANAGERANDROID_H
