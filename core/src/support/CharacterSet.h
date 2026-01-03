#ifndef __WMCANVAS_CHARACTER_SET_H__
#define __WMCANVAS_CHARACTER_SET_H__

#include <export.h>

struct Utf8ToUCS2
{
    const char *utf8;
    int utf8len;
    unsigned short *ucs2;
    int ucs2len;

    API_EXPORT Utf8ToUCS2(const char *utf8In, int utf8lenIn);
    virtual ~Utf8ToUCS2();

private:
    Utf8ToUCS2(const Utf8ToUCS2 &);
    void getUcs2();
};

#endif
