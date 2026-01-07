#ifndef WM_CANVAS_COMMANDBUFFER_H
#define WM_CANVAS_COMMANDBUFFER_H

#include <export.h>
#include <iostream>
#include <stdio.h>
#include <string>

namespace wmcanvas {

/**
 * @brief 命令缓冲区类：用于解析Canvas命令流
 *
 * 命令缓冲区编码格式：
 *   meta: 2个uint32
 *     meta[0] - 4字节长度
 *     meta[1] - 保留字段
 *
 *   数据格式: funcId+param+...+funcId+param
 */
class WmCommandBuffer {
   public:
    /**
     * @brief 默认构造函数
     */
    API_EXPORT WmCommandBuffer();

    /**
     * @brief 带参数的构造函数
     * @param buffer 缓冲区指针
     * @param size 缓冲区大小（字节）
     */
    API_EXPORT WmCommandBuffer(int8_t* buffer, size_t size);

    /**
     * @brief 析构函数
     */
    API_EXPORT ~WmCommandBuffer();

    /**
     * @brief 重置缓冲区
     * @param buffer 新的缓冲区指针
     * @param size 新的缓冲区大小（字节）
     */
    API_EXPORT void resetBuffer(int8_t* buffer, size_t size);

    /**
     * @brief 获取当前解析位置
     * @return 当前位置索引
     */
    size_t getCurrentPos() { return mCurrentPos; }

    /**
     * @brief 获取缓冲区大小
     * @return 缓冲区大小（字节）
     */
    size_t getBufferSize() { return mBufferSize; }

    /**
     * @brief 获取当前缓冲区指针
     * @return 当前位置的缓冲区指针
     */
    int8_t* getCurrentBufferPtr();

    /**
     * @brief 解析单个值
     * @tparam T 值的类型
     * @return 解析得到的值指针
     */
    template <typename T>
    T* parseValue();

    /**
     * @brief 解析数组
     * @tparam T 数组元素类型
     * @param arr 目标数组指针
     * @param length 数组长度
     * @return 解析是否成功
     */
    template <typename T>
    bool parseArray(T* arr, size_t length);

    /**
     * @brief 解析缓冲区数据
     * @param size 需要解析的字节数
     * @return 解析得到的缓冲区指针
     */
    const void* parseBuffer(size_t size);

    /**
     * @brief 解析对齐的缓冲区数据（4字节对齐）
     * @param size 需要解析的字节数
     * @return 解析得到的缓冲区指针
     */
    const void* parseBufferAlign(size_t size);

   private:
    /**
     * @brief 检查缓冲区剩余大小是否足够
     * @param size 需要的字节数
     * @return 如果足够返回true，否则返回false
     */
    inline bool checkBufferSize(size_t size);

    uint32_t mMeta[2];          // 元数据：meta[0]=4字节长度, meta[1]=保留
    int8_t* mBufferData;        // 缓冲区数据指针
    size_t mBufferSize;         // 缓冲区大小（字节）
    size_t mBufferFourByteSize; // 缓冲区大小（4字节单位）
    size_t mCurrentPos;         // 当前解析位置
};

}  // namespace wmcanvas

#endif /* WM_CANVAS_COMMANDBUFFER_H */
