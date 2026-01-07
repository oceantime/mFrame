#ifndef WMCANVAS_WMPATHSTROKER_H
#define WMCANVAS_WMPATHSTROKER_H


#include "WmPoint.h"
#include "WmTransform.h"


class WmPath;
class WmCanvasContext;



/**
 *    [out]           [out]
 *     |              |
 *    from  ------->  to
 *     |              |
 *    [in]            [in]
 */
struct WmPathOutLine {
    WmPoint from;
    WmPoint to;

    float offsetX;
    float offsetY;

    WmPoint fromIn;
    WmPoint fromOut;

    WmPoint toIn;
    WmPoint toOut;
};

class WmPathStroker {

public:

    void StrokePath(WmCanvasContext *context, WmPath *path, std::vector<WmVertex> *vertexVec) ;


private:


    void FilterTooClosePoints(std::vector<WmPoint> &pts);


    void DrawLineJoin(WmCanvasContext* context, float halfLineWidth, WmPathOutLine& lineOne, WmPathOutLine& lineTwo,
                             WmColorRGBA color, WmTransform& transform, std::vector<WmVertex> *vertexVec);


    void DrawArcForCapOrJoin(WmCanvasContext *context, float halfLineWidth,
                                           WmPoint& center, WmPoint& p1, WmPoint& p2,
                                           WmColorRGBA color, WmTransform& transform, std::vector<WmVertex> *vec,
                                           float samePointThreshold);


    void DrawMiterJoin(WmCanvasContext *context, float halfLineWidth,
                       const WmPoint &center, const WmPoint &p1, const WmPoint &p2,
                       WmColorRGBA color, WmTransform transform, std::vector<WmVertex> *vec);


    void DrawLineCap(WmCanvasContext *context, float halfLineWidth,
                                   WmPoint &center, WmPoint &p1, WmPoint &p2, float deltaX, float deltaY,
                                   WmColorRGBA color, WmTransform& transform, std::vector<WmVertex> *vec,
                                   float samePointThreshold = 0.001f);
};

#endif //WMCANVAS_WMPATHSTROKER_H
