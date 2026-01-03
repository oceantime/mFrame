/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

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
