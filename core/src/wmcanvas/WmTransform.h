#ifndef WMCANVAS_WMTRANSFORM_H
#define WMCANVAS_WMTRANSFORM_H

#include "WmPoint.h"

#include <math.h>
#include <float.h>


#define G_FLOAT_EQUAL(a,b)    fabs((a)-(b)) < FLT_EPSILON

// -----------------------------------------------------------
// --    Transform utility class
// --    Used by Canvas.setTransform
// --    Populates the vertexArr of a Quad
// -----------------------------------------------------------

struct WmTransform
{
    float a, b, c, d, tx, ty;

    WmTransform() : a(1), b(0), c(0), d(1), tx(0), ty(0) {}
    WmTransform(float v1, float v2, float v3, float v4, float v5, float v6)
        : a(v1), b(v2), c(v3), d(v4), tx(v5), ty(v6)
    {
    }
};

/* The identity transform: [ 1 0 0 1 0 0 ]. */
//const WmTransform WmTransformIdentity(1, 0, 0, 1, 0, 0);

#define  WmTransformIdentity  WmTransform(1, 0, 0, 1, 0, 0)

/*
 * Return the transform [ a b c d tx ty ]
 * [a   c   tx]
 * [b   d   ty]
 * [0   0    1]
 * */
static inline WmTransform WmTransformMake(float a, float b, float c, float d,
                                        float tx, float ty)
{
    WmTransform trans(a, b, c, d, tx, ty);
    return trans;
}

/* Return a transform which translates by `(tx, ty)':
 t' = [ 1 0 0 1 tx ty ] */
static inline WmTransform WmTransformMakeTranslation(float tx, float ty)
{
    WmTransform trans(1, 0, 0, 1, tx, ty);
    return trans;
}

/* Return a transform which scales by `(sx, sy)':
 t' = [ sx 0 0 sy 0 0 ] */
static inline WmTransform WmTransformMakeScale(float sx, float sy)
{
    return WmTransform(sx, 0, 0, sy, 0, 0);
}

static inline float WmTransformGetScaleX(const WmTransform& t)
{
    return sqrt(t.a*t.a+t.c*t.c);
}

static inline float WmTransformGetScaleY(const WmTransform& t)
{
    return sqrt(t.b*t.b+t.d*t.d);
}

static inline float WmTransformGetScale(const WmTransform& t)
{
    float sx = WmTransformGetScaleX(t);
    float sy = WmTransformGetScaleY(t);
    return std::max<float>(sx, sy);
}

//for font scale, min value is 1, max value is 72
static inline float WmTransformGetMaxScale(const WmTransform& t)
{
    float sx = WmTransformGetScaleX(t);
    float sy = WmTransformGetScaleY(t);
    return std::min<float>(72,std::max<float>(1, std::max<float>(sx, sy)) );
}


/* Return a transform which rotates by `angle' radians:
 t' = [ cos(angle) sin(angle) -sin(angle) cos(angle) 0 0 ] */
static inline WmTransform WmTransformMakeRotation(float angle)
{
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    WmTransform trans;
    trans.a = cosAngle;
    trans.b = sinAngle;
    trans.c = -sinAngle;
    trans.d = cosAngle;
    trans.tx = trans.ty = 0;
    return trans;
}

/* Return true if `t' is the identity transform, false otherwise. */
static inline bool WmTransformIsIdentity(const WmTransform& t)
{
    return (t.a == 1 && t.b == 0 && t.c == 0 && t.d == 1 && t.tx == 0 &&
            t.ty == 0);
}


static inline bool WmTransformIsSimilarity(const WmTransform& t)
{
    // translate or equal scale (no rotation or skew)
    return (t.a == t.d && t.b == 0 && t.c == 0);
}


/* Return true if `t1' and `t2' are equal, false otherwise. */
static inline bool WmTransformEqualToTransform(const WmTransform& t1, const WmTransform& t2)
{
    return G_FLOAT_EQUAL(t1.a, t2.a) && G_FLOAT_EQUAL(t1.b, t2.b) &&
          G_FLOAT_EQUAL(t1.c, t2.c) && G_FLOAT_EQUAL(t1.d, t2.d) &&
          G_FLOAT_EQUAL(t1.tx, t2.tx) && G_FLOAT_EQUAL(t1.ty, t2.ty);
}

/* Translate `t' by `(tx, ty)' and return the result:
 t' = [ 1 0 0 1 tx ty ] * t */
static inline WmTransform WmTransformTranslate(const WmTransform & t, float tx, float ty)
{
    WmTransform result;
    result.a = t.a;
    result.b = t.b;
    result.c = t.c;
    result.d = t.d;
    result.tx = t.a * tx + t.c * ty + t.tx;
    result.ty = t.b * tx + t.d * ty + t.ty;
    
    return result;
}

/* Scale `t' by `(sx, sy)' and return the result:
 t' = [ sx 0 0 sy 0 0 ] * t */

static inline WmTransform WmTransformScale(const WmTransform& t, float sx, float sy)
{
    WmTransform result = t;
    result.a *= sx;
    result.b *= sx;
    result.c *= sy;
    result.d *= sy;
    
    return result;
}

/* Rotate `t' by `angle' radians and return the result:
 t' =  [ cos(angle) sin(angle) -sin(angle) cos(angle) 0 0 ] * t */

static inline WmTransform WmTransformRotate(const WmTransform& t, float angle)
{
    double cosAngle = cos(angle);
    double sinAngle = sin(angle);

    WmTransform result = t;
    result.a = t.a*cosAngle    + t.c*sinAngle;
    result.b = t.b*cosAngle    + t.d*sinAngle;
    result.c = t.a*(-sinAngle) + t.c*cosAngle;
    result.d = t.b*(-sinAngle) + t.d*cosAngle;
    
    return result;
}

/* Concatenate `t2' to `t1' and return the result:
 t' = t1 * t2 */
static inline WmTransform WmTransformConcat(const WmTransform& t2, const WmTransform& t1)
{
    WmTransform result;
    result.a = t1.a * t2.a + t1.b * t2.c;
    result.b = t1.a * t2.b + t1.b * t2.d;
    result.c = t1.c * t2.a + t1.d * t2.c;
    result.d = t1.c * t2.b + t1.d * t2.d;
    result.tx = t1.tx * t2.a + t1.ty * t2.c + t2.tx;
    result.ty = t1.tx * t2.b + t1.ty * t2.d + t2.ty;
    return result;
}

/* Transform `point' by `t' and return the result:
 p' = p * t
 where p = [ x y 1 ]. */
static inline WmPoint WmPointApplyWmTransform(const WmPoint& point, const WmTransform& t)
{
    if(WmTransformIsIdentity(t)) {
        return point;
    }
    WmPoint p;
    p.x = (t.a * point.x + t.c * point.y + t.tx);
    p.y = (t.b * point.x + t.d * point.y + t.ty);
    return p;
}

static inline void WmPointApplyWmTransformInPlace(WmPoint& point, const WmTransform& t)
{
    float oldX = point.x;
    float oldY = point.y;
    point.x = (t.a * oldX + t.c * oldY + t.tx);
    point.y = (t.b * oldX + t.d * oldY + t.ty);
}


/**
 * copy transform data to float array
 */
static inline void WmTransformCopy(WmTransform& t, float out[])
{
    out[0] = t.a;
    out[1] = t.b;
    out[2] = t.c;
    out[3] = t.d;
    out[4] = t.tx;
    out[5] = t.ty;
}

static inline WmTransform WmTransformInvert(const WmTransform& t)
{
    if( WmTransformIsIdentity(t) ){
        return t;
    }
    double k = t.a * t.d - t.b * t.c;
    
    if( fabs(k) < FLT_EPSILON ){
        return t;
    }
    
    WmTransform result;
    result.a = (float) ( t.d / k );
    result.b = (float) ( -t.b / k );
    result.c = (float) ( -t.c / k );
    result.d = (float) ( t.a / k );
    result.tx = (float) (t.c * t.ty - t.d * t.tx) / k;
    result.ty = (float) (t.b * t.tx - t.a * t.ty) / k;
    return result;
}

#endif /* WMCANVAS_WMTRANSFORM_H */
