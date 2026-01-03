#include "../support/Log.h"
#include "WmPath2D.h"
#include "WmPath.h"


WmPath2D::WmPath2D() {

}


WmPath2D::~WmPath2D() {
    ClearPath();
}


WmPath2D::WmPath2D(const WmPath2D &other) {
    ClearPath();

    AddPaths(const_cast<std::vector<WmPathCmd *>&>(other.cmds));
}


void WmPath2D::ClearPath() {
    for (int i = 0; i < cmds.size(); i++) {
        WmPathCmd *cmd = cmds[i];
        delete cmd;
    }
    cmds.clear();
}



void WmPath2D::AddPaths(std::vector<WmPathCmd*>& otherCmds, WmTransform* transform) {
    for (int i = 0; i < otherCmds.size(); i++) {
        WmPathCmd *item = otherCmds[i];

        // copy cmd
        WmPathCmd *cmd = new WmPathCmd();
        cmd->cmdType = item->cmdType;
        cmd->byteLen = item->byteLen;
        cmd->data = new uint8_t[cmd->byteLen];

        memcpy(cmd->data, item->data, cmd->byteLen);

        if (transform != nullptr && !WmTransformIsIdentity(*transform)) {
            TransformPathCmd(cmd, *transform);
        }

        cmds.push_back(cmd);
    }
}

void WmPath2D::AddPath(WmPath2D &other, WmTransform &transform) {
    AddPaths(other.cmds, &transform);
}


void WmPath2D::WriteToPath(WmPath &path) {
    for (int i = 0; i < cmds.size(); i++) {
        WmPathCmd *item = cmds[i];
        // decode and fill
        if (item->cmdType == MOVE_TO) {
            float *data = (float *) item->data;
            path.MoveTo(data[0], data[1]);
        } else if (item->cmdType == LINE_TO) {
            float *data = (float *) item->data;
            path.LineTo(data[0], data[1]);
        } else if (item->cmdType == CLOSE_PATH) {
            path.Close();
        } else if (item->cmdType == QUADRATIC_CURVE_TO) {
            float *data = (float *) item->data;
            path.QuadraticCurveTo(data[0], data[1], data[2], data[3]);
        } else if (item->cmdType == BEZIER_CURVE_TO) {
            float *data = (float *) item->data;
            path.BezierCurveTo(data[0], data[1], data[2], data[3], data[4], data[5]);
        } else if (item->cmdType == ARC_TO) {
            float *data = (float *) item->data;
            path.ArcTo(data[0], data[1], data[2], data[3], data[4]);
        } else if (item->cmdType == ARC) {
            float *data = (float *) item->data;
            path.Arc(data[0], data[1], data[2], data[3], data[4], data[7] == 1);
        } else if (item->cmdType == ELLIPSE) {
            float *data = (float *) item->data;
            path.Ellipse(data[0], data[1], data[2], data[3], data[4], data[5], data[6],
                         data[7] == 1);
        } else if (item->cmdType == RECT) {
            int *data = (int *) item->data;
            path.Rect(data[0], data[1], data[2], data[3]);
        }
    }
}


void WmPath2D::ClosePath() {
    WmPathCmd *cmd = new WmPathCmd();
    cmd->cmdType = CLOSE_PATH;
    cmds.push_back(cmd);
}


void WmPath2D::MoveTo(float x, float y) {
    WmPathCmd *cmd = new WmPathCmd();
    cmd->cmdType = MOVE_TO;
    cmd->byteLen = sizeof(float) * 2;

    float *data = new float[2];
    data[0] = x;
    data[1] = y;
    cmd->data = data;

    cmds.push_back(cmd);
}


void WmPath2D::LineTo(float x, float y) {
    WmPathCmd *cmd = new WmPathCmd();
    cmd->cmdType = WmPathCmdType::LINE_TO;
    cmd->byteLen = sizeof(float) * 2;

    float *data = new float[2];
    data[0] = x;
    data[1] = y;
    cmd->data = data;

    cmds.push_back(cmd);
}


void WmPath2D::QuadraticCurveTo(float cpx, float cpy, float x, float y, float scale) {
    WmPathCmd *cmd = new WmPathCmd();
    cmd->cmdType = WmPathCmdType::QUADRATIC_CURVE_TO;
    cmd->byteLen = sizeof(float) * 5;

    float *data = new float[5];
    data[0] = cpx;
    data[1] = cpy;
    data[2] = x;
    data[3] = y;
    data[4] = scale;
    cmd->data = data;

    cmds.push_back(cmd);
}


void WmPath2D::BezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x,
                            float y, float scale) {
    WmPathCmd *cmd = new WmPathCmd();
    cmd->cmdType = WmPathCmdType::BEZIER_CURVE_TO;
    cmd->byteLen = sizeof(float) * 7;

    float *data = new float[7];
    data[0] = cp1x;
    data[1] = cp1y;
    data[2] = cp2x;
    data[3] = cp2y;
    data[4] = x;
    data[5] = y;
    data[6] = scale;
    cmd->data = data;

    cmds.push_back(cmd);
}


void WmPath2D::ArcTo(float x1, float y1, float x2, float y2, float radius) {
    WmPathCmd *cmd = new WmPathCmd();
    cmd->cmdType = WmPathCmdType::ARC_TO;
    cmd->byteLen = sizeof(float) * 5;

    float *data = new float[5];
    data[0] = x1;
    data[1] = y1;
    data[2] = x2;
    data[3] = y2;
    data[4] = radius;
    cmd->data = data;

    cmds.push_back(cmd);
}


void WmPath2D::Arc(float x, float y, float radius, float startAngle, float endAngle,
                  bool antiClockwise2) {
    WmPathCmd *cmd = new WmPathCmd();
    cmd->cmdType = WmPathCmdType::ARC;
    cmd->byteLen = sizeof(float) * 6;

    float *data = new float[6];
    data[0] = x;
    data[1] = y;
    data[2] = radius;
    data[3] = startAngle;
    data[4] = endAngle;
    data[5] = antiClockwise2 ? 1 : 0;

    cmd->data = data;

    cmds.push_back(cmd);
}


void
WmPath2D::Ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle,
                 float endAngle, bool antiClockwise) {
    WmPathCmd *cmd = new WmPathCmd();
    cmd->cmdType = WmPathCmdType::ELLIPSE;
    cmd->byteLen = sizeof(float) * 8;

    float *data = new float[8];
    data[0] = x;
    data[1] = y;
    data[2] = radiusX;
    data[3] = radiusY;
    data[4] = rotation;
    data[5] = startAngle;
    data[6] = endAngle;
    data[7] = antiClockwise ? 1 : 0;

    cmd->data = data;

    cmds.push_back(cmd);
}


void WmPath2D::Rect(int x, int y, int w, int h) {
    WmPathCmd *cmd = new WmPathCmd();
    cmd->cmdType = WmPathCmdType::RECT;
    cmd->byteLen = sizeof(float) * 4;

    int *data = new int[4];
    data[0] = x;
    data[1] = y;
    data[2] = w;
    data[3] = h;
    cmd->data = data;

    cmds.push_back(cmd);
}

/**
 * TODO
 */
void WmPath2D::TransformPathCmd(WmPathCmd* cmd,  WmTransform& transform) {

}
