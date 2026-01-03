/**
* Created by G-Canvas Open Source Team.
* Copyright (c) 2017, Alibaba, Inc. All rights reserved.
*
* This source code is licensed under the Apache Licence 2.0.
* For the full copyright and license information, please view
* the LICENSE file in the root directory of this source tree.
*/
#ifndef GCANVAS_GPATHSTROKER_H
#define GCANVAS_GPATHSTROKER_H


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
struct GPathOutLine {
    GPoint from;
    GPoint to;

    float offsetX;
    float offsetY;

    GPoint fromIn;
    GPoint fromOut;

    GPoint toIn;
    GPoint toOut;
};



class GPathStroker {

public:

    void StrokePath(WmCanvasContext *context, WmPath *path, std::vector<WmVertex> *vertexVec) ;


private:


    void FilterTooClosePoints(std::vector<GPoint> &pts);


    void DrawLineJoin(WmCanvasContext* context, float halfLineWidth, GPathOutLine& lineOne, GPathOutLine& lineTwo,
                             WmColorRGBA color, WmTransform& transform, std::vector<WmVertex> *vertexVec);


    void DrawArcForCapOrJoin(WmCanvasContext *context, float halfLineWidth,
                                           GPoint& center, GPoint& p1, GPoint& p2,
                                           WmColorRGBA color, WmTransform& transform, std::vector<WmVertex> *vec,
                                           float samePointThreshold);


    void DrawMiterJoin(WmCanvasContext *context, float halfLineWidth,
                       const GPoint &center, const GPoint &p1, const GPoint &p2,
                       WmColorRGBA color, WmTransform transform, std::vector<WmVertex> *vec);


    void DrawLineCap(WmCanvasContext *context, float halfLineWidth,
                                   GPoint &center, GPoint &p1, GPoint &p2, float deltaX, float deltaY,
                                   WmColorRGBA color, WmTransform& transform, std::vector<WmVertex> *vec,
                                   float samePointThreshold = 0.001f);
};




#endif //GCANVAS_GPATHSTROKER_H
