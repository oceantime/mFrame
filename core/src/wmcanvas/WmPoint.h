#ifndef WMCANVAS_WMPOINT_H
#define WMCANVAS_WMPOINT_H

#include "WmGL.h"
#include "WmContext2dType.h"
#include "export.h"

#include <cmath>
#include <iostream>


typedef struct
{
    GLfloat x;
    GLfloat y;
} WmPoint;

typedef union
{
    struct
    {
        GLfloat r, g, b, a;
    } rgba;
    GLfloat components[4];
} WmColorRGBA;


typedef struct
{
    WmPoint pos;
    WmPoint uv;
    WmColorRGBA color;
} WmVertex;

static inline WmPoint PointMake(float x, float y)
{
    WmPoint p = {x, y};
    return p;
}

static inline WmPoint PointAdd(WmPoint a, WmPoint b)
{
    WmPoint p = {a.x + b.x, a.y + b.y};
    return p;
}

static inline WmPoint PointSub(WmPoint a, WmPoint b)
{
    WmPoint p = {a.x - b.x, a.y - b.y};
    return p;
}

//static inline WmPoint PointMultiply(WmPoint a, WmPoint b)
//{
//    WmPoint p = {a.x * b.x, a.y * b.y};
//}


static inline WmPoint PointNormalize(WmPoint v)
{
    double ln = sqrtf(v.x * v.x + v.y * v.y);
    if (ln == 0)
    {
        return v;
    }

    v.x = (float) (v.x / ln);
    v.y = (float) (v.y / ln);
    return v;
}

struct WmRectf
{
    bool isTransformed = false; //坐标是否经过transform变换
    WmPoint leftTop = {0, 0};
    WmPoint bottomRight = {0, 0};

    float Width() const
    {
        return bottomRight.x - leftTop.x;
    }

    float Height() const
    {
        return bottomRight.y - leftTop.y;
    }

    void Enlarge(float x, float y)
    {
        leftTop.x -= x;
        leftTop.y -= y;
        bottomRight.x += x;
        bottomRight.y += y;
    }
};

inline WmRectf operator *(const WmRectf &rect, float factor )
{
    WmRectf result = rect;
    result.leftTop.x *= factor;
    result.leftTop.y *= factor;
    result.bottomRight.x *= factor;
    result.bottomRight.y *= factor;
    return result;
}



#endif /* WMCANVAS_WMPOINT_H */
