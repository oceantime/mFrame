#ifndef WMCANVAS_WMPATH2D_H
#define WMCANVAS_WMPATH2D_H


#include "GL/WmGL.h"
#include "WmTransform.h"
#include "WmPath.h"
#include <vector>


enum WmPathCmdType {
    MOVE_TO,
    LINE_TO,
    QUADRATIC_CURVE_TO,
    BEZIER_CURVE_TO,
    ARC_TO,
    ARC,
    ELLIPSE,
    RECT,
    CLOSE_PATH
};

struct WmPathCmd {

    WmPathCmd () {
        byteLen = 0;
        data = nullptr;
    }

    ~WmPathCmd() {
        if (data != nullptr) {
            free (data);
        }
    }

    WmPathCmdType cmdType;
    size_t byteLen;
    void* data;
};

/**
 * WmPath2D object, for implement Canvas Path2d interface
 */
class WmPath2D {

public:

    API_EXPORT WmPath2D();


    API_EXPORT ~WmPath2D();


    API_EXPORT WmPath2D(const WmPath2D &other);


    API_EXPORT void AddPath(WmPath2D&, WmTransform& transform);


    API_EXPORT void ClosePath();


    API_EXPORT void MoveTo(float x, float y);


    API_EXPORT  void LineTo(float x, float y);


    API_EXPORT void QuadraticCurveTo(float cpx, float cpy, float x, float y, float scale);


    API_EXPORT void BezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x,
                                  float y, float scale);

    API_EXPORT void ArcTo(float x1, float y1, float x2, float y2, float radius);


    API_EXPORT void Arc(float x, float y, float radius, float startAngle, float endAngle,
                        bool antiClockwise2);


    API_EXPORT void Ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle,
                            float endAngle, bool antiClockwise);


    API_EXPORT void Rect(int x, int y, int w, int h);


    void WriteToPath(WmPath& path);


private:

    void ClearPath();


    void TransformPathCmd(WmPathCmd* cmd,  WmTransform& transform);


    void AddPaths(std::vector<WmPathCmd*>&, WmTransform* transform = nullptr);


private:


    std::vector<WmPathCmd*> cmds;


};

#endif //WMCANVAS_WMPATH2D_H
