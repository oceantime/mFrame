#ifndef WMCANVAS_GLUTIL_H
#define WMCANVAS_GLUTIL_H

#include "WmGL.h"
#include "../../support/Log.h"
#include <vector>

namespace wmcanvas{
    GLuint PixelsBindTexture(const unsigned char *rgbaData, WmLint format, unsigned int width,
                             unsigned int height, std::vector<WmCanvasLog> *errVec = nullptr);
    void PixelsSampler(int inWidth, int inHeight, int *inPixels, int outWidth, int outHeight, int *outPixels);
}

#endif /* WMCANVAS_GLUTIL_H */
