/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#ifndef GCANVAS_GPATH_H
#define GCANVAS_GPATH_H

#include "WmPoint.h"
#include "WmTransform.h"
#include "WmPathStroker.h"
#include <float.h>
#include <iostream>
#include <vector>

class WmCanvasContext;



typedef struct {
    std::vector<GPoint> points;
    bool isClosed;
} GSubPath;




class WmPath {

public:
    WmPath();

    WmPath(const WmPath &other);

    ~WmPath();


    void MoveTo(float x, float y);

    void LineTo(float x, float y);

    void QuadraticCurveTo(float cpx, float cpy, float x, float y);

    void BezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y);

    void ArcTo(float x1, float y1, float x2, float y2, float radius);

    void Arc(float x, float y, float radius, float startAngle, float endAngle,
             bool antiClockwise2);

    void Ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle,
                 float endAngle, bool antiClockwise);


    void Rect(float x, float y, float w, float h);

    void Close();

    void Reset();


    void ClipRegion(WmCanvasContext *context);

    void Stroke(WmCanvasContext *context);

    void Stroke(WmCanvasContext *context, WmColorRGBA color, std::vector<WmVertex> *vertexVec);


    void Fill(WmCanvasContext *context, GFillRule rule, GFillTarget target = FILL_TARGET_COLOR,
              bool needTransform = false);


    void StencilRectForStroke(WmCanvasContext *context, std::vector<WmVertex> &vertexVec);

    void DrawVertexToContext(WmCanvasContext *context, std::vector<WmVertex> &vertexVec);

    void GetRect(WmRectf &rect);

    static void GetRectCoverVertex(WmRectf &rect, std::vector<WmVertex> &vertexVec);

    static void SubdivideCubicTo(WmPath *path, GPoint points[4], int level = 4);

    static void ChopCubicAt(GPoint src[4], GPoint dst[7], float t);

    static inline GPoint Interp(const GPoint &v0, const GPoint &v1, const GPoint &t);

private:


    std::vector<GSubPath*> *CreateLineDashPath(WmCanvasContext *context);


    void PushPoint(GPoint pt);


    void PushPoint(float x, float y);


    void EnsureSubPathAtPoint(float x, float y);


    void ClearSubPath();


    void NewSubPath(float x, float y);


    GPoint TransformPoint(float x, float y);


    GSubPath* GetCurPath();

//    void PushTriangleFanPoints(WmCanvasContext *context, tSubPath* subPath, WmColorRGBA color);

//    void RestoreStencilForClip(WmCanvasContext *context);

    void SetStencilForClip();


private:
    GPoint mStartPosition;
    GPoint mCurrentPosition;

    std::vector<GSubPath*> mPathStack;

    GPoint mMinPosition;
    GPoint mMaxPosition;

    bool needNewSubPath;

    GPathStroker stroker;

    friend GPathStroker;

public:
    GFillRule mClipFillRule;
    WmTransform mTransform;

};

#endif /* GCANVAS_GPATH_H */
