#ifndef WMCANVAS_WMGLUTIL_H
#define WMCANVAS_WMGLUTIL_H

#include "WmGL.h"
#include "../../support/Log.h"
#include <vector>

namespace wmcanvas{
    GLuint PixelsBindTexture(const unsigned char *rgbaData, GLint format, unsigned int width,
                             unsigned int height, std::vector<WmCanvasLog> *errVec = nullptr);
    void PixelsSampler(int inWidth, int inHeight, int *inPixels, int outWidth, int outHeight, int *outPixels);
}

#endif /* WMCANVAS_WMGLUTIL_H */
