#include "WmStrSeparator.h"

#include <ctype.h>
#include <string.h>

namespace wmcanvas
{
WmStrSeparator::WmStrSeparator() { memset(mPointers, 0, sizeof(mPointers)); }

short WmStrSeparator::SepStrByCharArray(char *str, const char *byteArray,
                                        short byteCount, short maxCount)
{
    if (maxCount < 0)
    {
        maxCount = PARSESEPSTRMAXCOUNT;
    }
    short sepCount = 0, i = 0;
    bool lastMark = true;
    char *cur = str;

    for (; *cur; cur++)
    {
        for (i = 0; i < byteCount; i++)
        {
            if (*cur == byteArray[i])
            {
                lastMark = true;
                *cur = 0;
                break;
            }
        }
        if (*cur && lastMark)
        {
            if (sepCount < maxCount)
            {
                mPointers[sepCount++] = cur;
            }
            lastMark = false;
        }
    }
    return sepCount;
};

short WmStrSeparator::SepStrBySpace(char *str, short maxCount)
{
    if (maxCount < 0)
    {
        maxCount = PARSESEPSTRMAXCOUNT;
    }

    short sepCount = 0;
    bool lastMark = true;
    char *cur = str;

    for (; *cur; cur++)
    {
        if (isspace((unsigned char)*cur))
        {
            lastMark = true;
            *cur = 0;
        }
        else
        {
            if (lastMark)
            {
                if (sepCount < maxCount)
                {
                    mPointers[sepCount++] = cur;
                }
                lastMark = false;
            }
        }
    }
    return sepCount;
};
}
