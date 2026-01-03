/**
* Created by G-Canvas Open Source Team.
* Copyright (c) 2017, Alibaba, Inc. All rights reserved.
*
* This source code is licensed under the Apache Licence 2.0.
* For the full copyright and license information, please view
* the LICENSE file in the root directory of this source tree.
*/

#ifndef WM_CANVAS_COMMANDDECODER_H
#define WM_CANVAS_COMMANDDECODER_H

#include <stdio.h>
#include <memory>
#include <functional>
#include "WmCommandBuffer.hpp"


namespace wmcanvas
{

enum WmResType
{
    WmResType_NONE = 0,
    WmResType_NULL,
    WmResType_Bool,
    WmResType_Int32,
    WmResType_UInt32,
    WmResType_UInt32_CreateBuffer,
    WmResType_UInt32_CreateTexture,
    WmResType_UInt32_CreateFrameBuffer,
    WmResType_UInt32_CreateShader,
    WmResType_UInt32_CreateProgram,
    WmResType_UInt32_CreateRenderBuffer,
    WmResType_UInt32_CreateVertexArray,//extension
    WmResType_Float,
    WmResType_BoolPtr,
    WmResType_CharPtr,
    WmResType_Int32Ptr,
    WmResType_UInt32Ptr,
    WmResType_FloatPtr,
    WmResType_CharPtr_GetExtention,
    WmResType_CharPtr_GetSupportedExtention,
    WmResType_GetContextAttributes

};

/**
    `WmDecodeRes` struct save the value of each batch render while has result
 */
struct WmDecodeRes
{
    bool hasResult;
    WmResType type;
    //result is a typed value
    union
    {
        uint8_t         boolValue;
        int32_t         intValue;
        uint32_t        uintValue;
        float           floatValue;
    };

    //multi return value, while point value not nullptr, delete it before use
    union
    {
        char *          charPtrValue;
        int32_t *       intPtrValue;
        uint32_t *      uintPtrValue;
        float *         floatPtrValue;
    };
    //size of element
    size_t length;
    //size of byte size
    size_t byteSize;
    
    size_t maxSize;
    
    WmDecodeRes()
    {
        maxSize = 2048;
        hasResult = false;
        type = WmResType_NONE;
        uintValue = 0;
        charPtrValue = new char[maxSize];
    }
    
    ~WmDecodeRes()
    {
        if( charPtrValue )
        {
            delete charPtrValue;
            charPtrValue = nullptr;
        }
    }
    
    inline void checkSize(size_t size)
    {
        if( size <= maxSize ) return;
        
        maxSize += size*2;
        char * newCharPtrValue = new char[maxSize];
        if( newCharPtrValue ){
            delete charPtrValue;
            charPtrValue = newCharPtrValue;
        }
    }
    
    //reset before use
    inline void reset()
    {
        hasResult = false;
        type = WmResType_NONE;
        uintValue = 0;
        length = byteSize = 0;
    }
};

//define WebGL API function
//using WmCommandFunc = std::function< void(WmCommandBuffer& buffer, WmDecodeRes& res ) >;
typedef void (*WmCommandFuncPtr)(WmCommandBuffer& , WmDecodeRes& );

//convert utf16 string to utf8 string
std::string U16StrToU8Str( char16_t* u16char );

class WmCommandDecoder
{
public:
    WmCommandDecoder(){};
    ~WmCommandDecoder(){};

    void SetChangeContextFunc( std::function<void(unsigned int)> func){
        mChangeContextFunc = func;
    }

    std::function<void(unsigned int)> GetChangeContextFunc(){
        return mChangeContextFunc;
    }

    virtual void InitCommands() = 0;
    virtual bool Decode(WmCommandBuffer& buffer, WmDecodeRes& res) = 0;
    virtual bool IsSyncCommand(uint32_t commandId) = 0;

protected:
    std::function<void(unsigned int)> mChangeContextFunc;
};

} //namespace wmcanvas

#endif /* WM_CANVAS_COMMANDDECODER_H */
