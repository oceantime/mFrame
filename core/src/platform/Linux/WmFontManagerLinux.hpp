#include "WmFontManager.h"
#include "WmCanvas2dContext.h"
#include <assert.h>
#include "support/Log.h"
#include "WmFont.h"
#include "utils.hpp"
#include "WmFontCacheLinux.hpp"
#include "support/CharacterSet.h"
#include <stdlib.h>

class GFontManagerImplementLinux : public GFontManager
{
public:
    GFontManagerImplementLinux(unsigned w, unsigned h);

    virtual ~GFontManagerImplementLinux()
    {
        delete mFontCache;
    };

    void DrawText(const unsigned short *text, unsigned int text_length, float x, float y,
                  bool isStroke, GCanvasContext *context, float scaleX = 1, float scaleY = 1) override;

    float MeasureText(const char *text, unsigned int text_length, wmcanvas::GFontStyle *fontStyle) override;

    //空实现
    float *MeasureTextExt(const char *text, unsigned int text_length, wmcanvas::GFontStyle *fontStyle) override
    {
        return nullptr;
    }

    //空实现
    float *PreMeasureTextHeight(const char *text, unsigned int text_length, GCanvasContext *context) override
    {
        return nullptr;
    }


    GTexture *GetOrCreateFontTexture() override;

private:
    void AdjustTextPenPoint(GCanvasContext *context, const std::vector<GFont *> &font,
                            const unsigned short *text,
                            unsigned int textLength,
                            bool isStroke,
                            float &x, float &y, float sx, float sy);

    GFont *GetFontByCharCode(wchar_t charCode, wmcanvas::GFontStyle *fontStyle);

    void DrawTextInternal(GCanvasContext *context, GFont *font, bool isStroke, wchar_t text,
                          float &x, float y, float sx, float sy);
    GFontCache *mFontCache = nullptr;
};