#include "WmCommandBuffer.hpp"
#include "../support/Log.h"
namespace wmcanvas
{

    WmCommandBuffer::WmCommandBuffer():
            mCurrentPos(0), mBufferData(nullptr), mBufferSize(0)
    {

    }

    WmCommandBuffer::WmCommandBuffer(int8_t* buffer, size_t size):
            mCurrentPos(0), mBufferData(buffer), mBufferSize(size)
    {
        //解析meta信息
        parseArray<uint32_t>(mMeta, 2);

        //计算buffersize
        mBufferSize = mMeta[0] * 4;
    }

    WmCommandBuffer::~WmCommandBuffer()
    {

    }

    void WmCommandBuffer::resetBuffer(int8_t* bufffer, size_t size)
    {
        mBufferData = bufffer, mBufferSize = size, mCurrentPos = 0;

        //解析meta信息
        parseArray<uint32_t>(mMeta, 2);

        //计算buffersize
        mBufferSize = mMeta[0] * 4;

    }

    int8_t* WmCommandBuffer::getCurrentBufferPtr()
    {
        if( mCurrentPos < mBufferSize ) {
            return (mBufferData + mCurrentPos);
        }
        return NULL;
    }

    template <typename T>
    T* WmCommandBuffer::parseValue()
    {
        size_t size = sizeof(T);
        if( checkBufferSize(size) )
        {
            T* ptr = (T*)(mBufferData + mCurrentPos);
            mCurrentPos += size;
            return ptr;
        }
        return nullptr;
    }
    template uint32_t* WmCommandBuffer::parseValue();
    template float* WmCommandBuffer::parseValue();
    template uint64_t* WmCommandBuffer::parseValue();

    template <typename T>
    bool WmCommandBuffer::parseArray(T *arr, size_t length)
    {
        size_t size = sizeof(T) * length;
        if( checkBufferSize(size) )
        {
            T* basePtr = (T*)(mBufferData + mCurrentPos);
            for (int i=0; i<length; i++)
            {
                arr[i] = *(basePtr + i);
            }
            mCurrentPos += size;
            return true;
        }
        return false;
    }

    template bool WmCommandBuffer::parseArray(int32_t* arr, size_t length);
    template bool WmCommandBuffer::parseArray(uint32_t* arr, size_t length);
    template bool WmCommandBuffer::parseArray(float* arr, size_t length);

    const void* WmCommandBuffer::parseBuffer(size_t size)
    {
        if( checkBufferSize(size) )
        {
            const void *bufPtr = (mBufferData + mCurrentPos);
            mCurrentPos += size;
            return bufPtr;
        }
        return nullptr;
    }

    const void* WmCommandBuffer::parseBufferAlign(size_t size)
    {
        uint8_t delta = size % 4;
        size_t realSize = size;
        if( delta != 0 )
        {
            realSize = (size + 4 ) - delta;
        }

        if( checkBufferSize(realSize) )
        {
            const void *bufPtr = (mBufferData + mCurrentPos);
            mCurrentPos += realSize;
            return bufPtr;
        }
        return nullptr;
    }



    inline bool WmCommandBuffer::checkBufferSize(size_t size)
    {
        if( size <= 0 ) return false;
        return mCurrentPos+size <= mBufferSize;
    }


} //namespace wmcanvas
