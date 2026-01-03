#ifndef __WmCanvas_WmFontFamily__
#define __WmCanvas_WmFontFamily__

#include "WmFontStyle.h"

#include <export.h>
#include <vector>

namespace wmcanvas
{


    /**
     * font family内的单个字体
     */
    struct WmFontFamilyItem {

        const char* font_file_name;

        WmFontStyle::Style style;

        WmFontStyle::Variant variant;

        WmFontStyle::Weight  weight;


        WmFontFamilyItem();

    };



class WmFontFamily
{
public:
    WmFontFamily(std::list< const char * > &fontFamily);
    virtual ~WmFontFamily() {}

    API_EXPORT const char *MatchFamilyStyle(WmFontStyle &fontStyle);
    char *GetProperFontFile();

private:
    void InitFontFamily(std::list<const char *> &fontFamily);


    // @depreated
//    const char *mFontNormal= nullptr;
//    const char *mFontLight= nullptr;
//    const char *mFontBold= nullptr;
//    const char *mFontItalic= nullptr;
//    const char *mFontBoldItalic= nullptr;

    std::vector<WmFontFamilyItem> fontItems;

    };
}

#endif
