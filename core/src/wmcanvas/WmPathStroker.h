/**
* Created by G-Canvas Open Source Team.
* Copyright (c) 2017, Alibaba, Inc. All rights reserved.
*
* This source code is licensed under the Apache Licence 2.0.
* For the full copyright and license information, please view
* the LICENSE file in the root directory of this source tree.
*/
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

using GPathOutLine = WmPathOutLine;



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

using GPathStroker = WmPathStroker;



#endif //WMCANVAS_WMPATHSTROKER_H
