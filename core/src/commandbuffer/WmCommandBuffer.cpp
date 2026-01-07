#include "WmCommandBuffer.hpp"
#include "../support/Log.h"

namespace wmcanvas {

/**
 * @brief 默认构造函数
 */
WmCommandBuffer::WmCommandBuffer() : mCurrentPos(0), mBufferData(nullptr), mBufferSize(0) {}

/**
 * @brief 带参数的构造函数
 * @param buffer 缓冲区指针
 * @param size 缓冲区大小
 */
WmCommandBuffer::WmCommandBuffer(int8_t* buffer, size_t size)
    : mCurrentPos(0), mBufferData(buffer), mBufferSize(size) {
    // 解析meta信息
    parseArray<uint32_t>(mMeta, 2);

    // 计算buffersize（以字节为单位）
    mBufferSize = mMeta[0] * 4;
}

/**
 * @brief 析构函数
 */
WmCommandBuffer::~WmCommandBuffer() {}

/**
 * @brief 重置缓冲区
 * @param buffer 新的缓冲区指针
 * @param size 新的缓冲区大小
 */
void WmCommandBuffer::resetBuffer(int8_t* buffer, size_t size) {
    mBufferData = buffer;
    mBufferSize = size;
    mCurrentPos = 0;

    // 解析meta信息
    parseArray<uint32_t>(mMeta, 2);

    // 计算buffersize（以字节为单位）
    mBufferSize = mMeta[0] * 4;
}

/**
 * @brief 获取当前缓冲区指针
 * @return 当前位置的缓冲区指针，如果超出范围则返回nullptr
 */
int8_t* WmCommandBuffer::getCurrentBufferPtr() {
    if (mCurrentPos < mBufferSize) {
        return (mBufferData + mCurrentPos);
    }
    return nullptr;
}

/**
 * @brief 解析单个值
 * @tparam T 值的类型
 * @return 解析得到的值指针，如果失败则返回nullptr
 */
template <typename T>
T* WmCommandBuffer::parseValue() {
    size_t size = sizeof(T);
    if (checkBufferSize(size)) {
        T* ptr = (T*)(mBufferData + mCurrentPos);
        mCurrentPos += size;
        return ptr;
    }
    return nullptr;
}

// 显式实例化模板
template uint32_t* WmCommandBuffer::parseValue();
template float* WmCommandBuffer::parseValue();
template uint64_t* WmCommandBuffer::parseValue();

/**
 * @brief 解析数组
 * @tparam T 数组元素类型
 * @param arr 目标数组指针
 * @param length 数组长度
 * @return 解析是否成功
 */
template <typename T>
bool WmCommandBuffer::parseArray(T* arr, size_t length) {
    size_t size = sizeof(T) * length;
    if (checkBufferSize(size)) {
        T* basePtr = (T*)(mBufferData + mCurrentPos);
        for (size_t i = 0; i < length; i++) {
            arr[i] = *(basePtr + i);
        }
        mCurrentPos += size;
        return true;
    }
    return false;
}

// 显式实例化模板
template bool WmCommandBuffer::parseArray(int32_t* arr, size_t length);
template bool WmCommandBuffer::parseArray(uint32_t* arr, size_t length);
template bool WmCommandBuffer::parseArray(float* arr, size_t length);

/**
 * @brief 解析缓冲区数据
 * @param size 需要解析的字节数
 * @return 解析得到的缓冲区指针，如果失败则返回nullptr
 */
const void* WmCommandBuffer::parseBuffer(size_t size) {
    if (checkBufferSize(size)) {
        const void* bufPtr = (mBufferData + mCurrentPos);
        mCurrentPos += size;
        return bufPtr;
    }
    return nullptr;
}

/**
 * @brief 解析对齐的缓冲区数据（4字节对齐）
 * @param size 需要解析的字节数
 * @return 解析得到的缓冲区指针，如果失败则返回nullptr
 */
const void* WmCommandBuffer::parseBufferAlign(size_t size) {
    uint8_t delta = size % 4;
    size_t realSize = size;
    // 如果不是4字节对齐，计算对齐后的大小
    if (delta != 0) {
        realSize = (size + 4) - delta;
    }

    if (checkBufferSize(realSize)) {
        const void* bufPtr = (mBufferData + mCurrentPos);
        mCurrentPos += realSize;
        return bufPtr;
    }
    return nullptr;
}

/**
 * @brief 检查缓冲区剩余大小是否足够
 * @param size 需要的字节数
 * @return 如果足够返回true，否则返回false
 */
inline bool WmCommandBuffer::checkBufferSize(size_t size) {
    if (size <= 0) {
        return false;
    }
    return mCurrentPos + size <= mBufferSize;
}

}  // namespace wmcanvas
