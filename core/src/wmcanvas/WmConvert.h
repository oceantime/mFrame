/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef GCANVAS_GCONVERT_H
#define GCANVAS_GCONVERT_H

#include "export.h"
#include "WmPoint.h"

#define GColorTransparent       WmColorRGBA({0,0,0,0})
#define GColorTransparentWhite  WmColorRGBA({1.0,1.0,1.0,0})
#define GColorWhite             WmColorRGBA({1.0,1.0,1.0,1.0})
namespace wmcanvas{
    API_EXPORT WmColorRGBA StrValueToColorRGBA(const char *value);


    API_EXPORT WmColorRGBA StrValueToColorRGBALegacy(const char *value);



    API_EXPORT WmColorRGBA IntValueToColorRGBA(int value);


    API_EXPORT std::string ColorToString(const WmColorRGBA &color);
}


#endif /* GCANVAS_GCONVERT_H */
