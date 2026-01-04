#import <Foundation/Foundation.h>
#include "../../wmcanvas/WmFontManager.h"

class WmFontManagerIOS : public WmFontManager
{
public:
    WmFontManagerIOS();
    virtual ~WmFontManagerIOS();

    void DrawText(const unsigned short *text, unsigned int text_length, float x, float y,
                  bool isStroke, WmCanvasContext* context, float scaleX=1, float scaleY=1);
    
    float MeasureText(const char *text, unsigned int text_length, wmcanvas::WmFontStyle *fontStyle);
    
    float* MeasureTextExt(const char *text, unsigned int text_length, wmcanvas::WmFontStyle *fontStyle);
};




