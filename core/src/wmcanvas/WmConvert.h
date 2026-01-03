/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef WMCANVAS_WMCONVERT_H
#define WMCANVAS_WMCONVERT_H

#include "export.h"
#include "WmPoint.h"

#define WmColorTransparent       WmColorRGBA({0,0,0,0})
#define WmColorTransparentWhite  WmColorRGBA({1.0,1.0,1.0,0})
#define WmColorWhite             WmColorRGBA({1.0,1.0,1.0,1.0})

// Backward compatibility
#define GColorTransparent       WmColorTransparent
#define GColorTransparentWhite  WmColorTransparentWhite
#define GColorWhite             WmColorWhite
namespace wmcanvas{
    API_EXPORT WmColorRGBA StrValueToColorRGBA(const char *value);


    API_EXPORT WmColorRGBA StrValueToColorRGBALegacy(const char *value);



    API_EXPORT WmColorRGBA IntValueToColorRGBA(int value);


    API_EXPORT std::string ColorToString(const WmColorRGBA &color);
}


#endif /* WMCANVAS_WMCONVERT_H */
