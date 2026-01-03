#ifndef LESSER_H_
#define LESSER_H_

#include <string.h>

namespace wmcanvas
{
struct Lesser
{
public:
    bool operator()(const char *l, const char *r) const
    {
        return strcmp(l, r) < 0;
    }
};
}

#endif /* LESSER_H_ */
