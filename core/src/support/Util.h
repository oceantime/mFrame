#ifndef WMCANVAS_UTIL_H
#define WMCANVAS_UTIL_H


#ifdef ANDROID

#include <sys/types.h>
#include <semaphore.h>
#include <string>

#endif

#include "../wmcanvas/WmPoint.h"
#include "../wmcanvas/WmContext2dType.h"


namespace wmcanvas {

    // flip the pixels by y axis
    void FlipPixel(unsigned char *pixels, int w, int h);

    // read pixels form texture
    void ReadPixelsFromTexture(unsigned char *pixels, int width, int height, int textureId);


    bool IsSamePoint(GPoint& p1, GPoint& p2, float min);


    bool IsNanPoint(const GPoint& p);


    bool HasSamePoint(GPoint& p1, GPoint& p2, GPoint& p3, float minValue);


    bool IsTrianglePointsValid(GPoint& p1, GPoint& p2, GPoint& p3, float minValue);


    float CalcPointAngle(const GPoint &director, const GPoint &center);


    float PointDistance(GPoint& s, GPoint& t);


//    inline float Determinate(float a, float b, float c, float d) {
//    }
//
//
//    bool LineLineIntersect(GPoint& output,
//                           float x1, float y1, //Line 1 start
//                           float x2, float y2, //Line 1 end
//                           float x3, float y3, //Line 2 start
//                           float x4, float y4 //Line 2 end
//    );


#ifdef ANDROID

    API_EXPORT WmCompositeOperation StringToGlobalCompositeOp(const char *str);


    API_EXPORT std::string GlobalCompositeOpToString(WmCompositeOperation &op);

    void waitUtilTimeout(sem_t *sem, uint ms);

#endif

}

#endif /* WMCANVAS_UTIL_H */
