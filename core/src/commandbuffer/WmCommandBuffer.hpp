#ifndef WM_CANVAS_COMMANDBUFFER_H
#define WM_CANVAS_COMMANDBUFFER_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <export.h>

namespace wmcanvas {

/**
 commandbuffer编码具体格式:
    meta: 2个uint32
        meta[0] - 4字节长度
        meta[1] - 保留字段

    funcId+param+...+funcId+param
 */

    class WmCommandBuffer {
    public:
        API_EXPORT WmCommandBuffer();

        API_EXPORT WmCommandBuffer(int8_t *buffer, size_t size);

        API_EXPORT ~WmCommandBuffer();

        API_EXPORT void resetBuffer(int8_t *bufffer, size_t size);

        size_t getCurrentPos() { return mCurrentPos; }

        size_t getBufferSize() { return mBufferSize; }

        int8_t *getCurrentBufferPtr();

        template<typename T>
        T *parseValue();

        template<typename T>
        bool parseArray(T *arr, size_t length);

        const void *parseBuffer(size_t size);

        const void *parseBufferAlign(size_t size);

    private:
        inline bool checkBufferSize(size_t size);

        uint32_t mMeta[2];              //meta
        int8_t *mBufferData;
        size_t mBufferSize;             //字节长度
        size_t mBufferFourByteSize;     //4字节长度
        size_t mCurrentPos;

    };

} //namespace wmcanvas


#endif /* WM_CANVAS_COMMANDBUFFER_H */
