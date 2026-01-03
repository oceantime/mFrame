/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#ifndef WMCANVAS_WMCONTEXT2DTYPE_H
#define WMCANVAS_WMCONTEXT2DTYPE_H

#include "WmGL.h"

typedef enum
{
    LINE_CAP_BUTT = 0,
    LINE_CAP_ROUND,
    LINE_CAP_SQUARE
} WmLineCap;

using GLineCap = WmLineCap;

typedef enum
{
    LINE_JOIN_MITER = 0,
    LINE_JOIN_BEVEL,
    LINE_JOIN_ROUND
} WmLineJoin;

using GLineJoin = WmLineJoin;

typedef enum
{
    COMPOSITE_OP_SOURCE_OVER = 0,
    COMPOSITE_OP_SOURCE_ATOP = 1,
    COMPOSITE_OP_SOURCE_IN = 2,
    COMPOSITE_OP_SOURCE_OUT = 3,
    
    COMPOSITE_OP_DESTINATION_OVER = 4,
    COMPOSITE_OP_DESTINATION_ATOP = 5,
    COMPOSITE_OP_DESTINATION_IN = 6,
    COMPOSITE_OP_DESTINATION_OUT = 7,
    
    COMPOSITE_OP_LIGHTER = 8,
    COMPOSITE_OP_COPY = 9,
    COMPOSITE_OP_XOR = 10,
    
    COMPOSITE_OP_NONE,
} WmCompositeOperation;

typedef struct {
    GLenum source;
    GLenum destination;
} WmBlendOperationFuncs;

extern WmBlendOperationFuncs WmCompositeOperationFuncs(int index);

typedef enum
{
    FILL_RULE_NONZERO,
    FILL_RULE_EVENODD
} WmFillRule;

using GFillRule = WmFillRule;

typedef enum
{
    FILL_TARGET_COLOR,
    FILL_TARGET_DEPTH
} GFillTarget;

#endif /* WMCANVAS_WMCONTEXT2DTYPE_H */
