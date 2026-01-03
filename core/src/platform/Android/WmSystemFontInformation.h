#ifndef __WmCanvas_WmSystemFontInformation__
#define __WmCanvas_WmSystemFontInformation__

#include "WmFontFamily.h"
#include "support/Lesser.h"


namespace wmcanvas {
    class SystemFontInformation final {
    public:
        SystemFontInformation();

        ~SystemFontInformation();

        API_EXPORT void SetDefaultFontFile(const char *fontFile);

        char *GetDefaultFontFile() { return mDefaultFontFile; }


        API_EXPORT void SetSystemFontLocation(const char *systemFontLocation);


        char *GetSystemFontLocation() { return mSystemFontLocation; }


        API_EXPORT void SetExtraFontLocation(const char *extraFontLocation);

        char *GetExtraFontLocation() { return mExtraFontLocation; }


        API_EXPORT bool InsertFontFamily(const char *fontName,
                                         std::list<const char *> &fontFileList);

        API_EXPORT void InsertFallbackFontFamily(std::list<const char *> &fontFileList);

        API_EXPORT WmFontFamily *FindFontFamily(const char *fontName);


        char *GetClosestFontFamily(void* ftLibrary,
                                   const char *currentFontLocation,
                                   const wchar_t charCode, const float width, const float height,
                                   WmFontStyle &fontStyle);

        static SystemFontInformation *GetSystemFontInformation() {
            return &sSystemFontInformation;
        }

    private:
        API_EXPORT static SystemFontInformation sSystemFontInformation;

        char *mDefaultFontFile;
        char *mSystemFontLocation;
        char *mExtraFontLocation;
        std::map<const char *, WmFontFamily, Lesser> mFontFamilies;
        std::list<const char *> mFallbackFontFileList;
        std::map<const char *, const char *, Lesser> mFontFiles;
    };
}

#endif // __WmCanvas_WmSystemFontInformation__
