#include "WmFontManager.h"
#include "WmCanvas2dContext.h"
#include <assert.h>
#include "support/Log.h"
#include "WmFont.h"
#include "utils.hpp"
#include "WmFontCacheLinux.hpp"
#include "support/CharacterSet.h"
#include <stdlib.h>

class WmFontManagerImplementLinux : public WmFontManager
{
public:
    WmFontManagerImplementLinux(unsigned w, unsigned h);

    virtual ~WmFontManagerImplementLinux()
    {
        delete mFontCache;
    };

    void DrawText(const unsigned short *text, unsigned int text_length, float x, float y,
                  bool isStroke, WmCanvasContext *context, float scaleX = 1, float scaleY = 1) override;

    float MeasureText(const char *text, unsigned int text_length, wmcanvas::WmFontStyle *fontStyle) override;

    //空实现
    float *MeasureTextExt(const char *text, unsigned int text_length, wmcanvas::WmFontStyle *fontStyle) override
    {
        return nullptr;
    }

    //空实现
    float *PreMeasureTextHeight(const char *text, unsigned int text_length, WmCanvasContext *context) override
    {
        return nullptr;
    }


    GTexture *GetOrCreateFontTexture() override;

private:
    void AdjustTextPenPoint(WmCanvasContext *context, const std::vector<WmFont *> &font,
                            const unsigned short *text,
                            unsigned int textLength,
                            bool isStroke,
                            float &x, float &y, float sx, float sy);

    WmFont *GetFontByCharCode(wchar_t charCode, wmcanvas::WmFontStyle *fontStyle);

    void DrawTextInternal(WmCanvasContext *context, WmFont *font, bool isStroke, wchar_t text,
                          float &x, float y, float sx, float sy);
    WmFontCache *mFontCache = nullptr;
};
