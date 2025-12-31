/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */


#include "WmCanvas2dContext.h"
#include "CharacterSet.h"
#include "wmcanvas/WmCanvas2dContext.h"
#include "support/Log.h"

void GCanvasContext::DrawTextWithLength(const char *text, int strLength, float x, float y,
                                        bool isStroke, float maxWidth) {
    if (strLength == 0) {
        strLength = static_cast<int>(strlen(text));
    }

    const WmCompositeOperation old_op = mCurrentState->mGlobalCompositeOp;
    DoSetGlobalCompositeOperation(COMPOSITE_OP_SOURCE_OVER, COMPOSITE_OP_SOURCE_OVER);
    // scaleWidth
    float scaleWidth = 1.0;
    if (fabs(maxWidth - SHRT_MAX) > 1) {
        // 对maxwidth进行判断，避免默认值导致的每次measure操作
        float measureWidth = MeasureTextWidth(text);
        if (measureWidth > maxWidth) {
            scaleWidth = maxWidth / measureWidth;
        }
    }


    Utf8ToUCS2 *lbData = new Utf8ToUCS2(text, strLength);
    FillText(lbData->ucs2, lbData->ucs2len, x, y,
             isStroke, scaleWidth);

    delete lbData;
    DoSetGlobalCompositeOperation(old_op, old_op);
}


void
GCanvasContext::DoSetGlobalCompositeOperation(WmCompositeOperation op, WmCompositeOperation alphaOp) {
    if (mCurrentState == nullptr)
        return;
    if (mCurrentState->mGlobalCompositeOp == op) {
        return;
    }
    SendVertexBufferToGPU();

    WmBlendOperationFuncs funcs = WmCompositeOperationFuncs(op);

    WmBlendOperationFuncs alphaFuncs = WmCompositeOperationFuncs(alphaOp);

    glBlendFuncSeparate(funcs.source, funcs.destination,
                        alphaFuncs.source, alphaFuncs.destination);

    mCurrentState->mGlobalCompositeOp = op;
}


