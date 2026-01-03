#include "WmWebGLRenderContext.hpp"
#include "WmWebGLRenderContextInner.hpp"
#include <algorithm>
#include "../support/Log.h"
#include "WmGL.h"



namespace wmcanvas {

namespace WebGL {

JSBindingPixels::JSBindingPixels():
    textureId(0), width(0), height(0), pixels(nullptr)
{

}

JSBindingPixels::~JSBindingPixels()
{

}


WmWebGLRenderContext::WmWebGLRenderContext(std::string contextId)
    : mContextId(contextId), mDrawCall(0),mNeed_draw(false),
    mUnpackFlipY_WebGL(false), mPremultiplyAlpha_WebGL(false)
{

}

WmWebGLRenderContext::WmWebGLRenderContext(void* data, std::string contextId)
    : mData(data), mContextId(contextId), mDrawCall(0),
      mNeed_draw(false), mUnpackFlipY_WebGL(false), mPremultiplyAlpha_WebGL(false)
{

}

WmWebGLRenderContext::~WmWebGLRenderContext()
{
    ReleaseResource();
}

void WmWebGLRenderContext::FetchPixels(uint64_t instanceId, int format, JSBindingPixels* pixels)
{
    if (mFetchPixelsFunc)
    {
        mFetchPixelsFunc(mData, instanceId, pixels);
    }
}

void WmWebGLRenderContext::BindFramebufferNull()
{
    if( mBindFramebufferNullFunc )
    {
        mBindFramebufferNullFunc(mData);
    }
}

void WmWebGLRenderContext::AddGLResource(GLResourceType resType, GLuint v)
{
    if( mAddGLResourceFunc )
    {
        mAddGLResourceFunc( mData, resType, v );
    }
    
    switch (resType)
    {
        case GLResourceType::Buffer:
        {
            mBufferSet.insert(v);
            break;
        }
        case GLResourceType::FrameBuffer:
        {
            mFrameBuferSet.insert(v);
            break;
        }
        case GLResourceType::Program:
        {
            mProgramSet.insert(v);
            break;
        }
        case GLResourceType::RenderBuffer:
        {
            mRenderBufferSet.insert(v);
            break;
        }
        case GLResourceType::Shader:
        {
            mShaderSet.insert(v);
            break;
        }
        case GLResourceType::VertexArray:
        {
            mVertexArraySet.insert(v);
            break;
        }
        default:
        {
            break;
        }
    }
}

void WmWebGLRenderContext::DeleteGLResource(GLResourceType resType, GLuint v)
{
    switch (resType)
    {
        case GLResourceType::Buffer:
        {
            mBufferSet.erase(v);
            break;
        }
        case GLResourceType::FrameBuffer:
        {
            mFrameBuferSet.erase(v);
            break;
        }
        case GLResourceType::Program:
        {
            mProgramSet.erase(v);
            break;
        }
        case GLResourceType::RenderBuffer:
        {
            mRenderBufferSet.erase(v);
            break;
        }
        case GLResourceType::Shader:
        {
            mShaderSet.erase(v);
            break;
        }
        case GLResourceType::VertexArray:
        {
            mVertexArraySet.erase(v);
            break;
        }
        default:
        {
            break;
        }
    }
}

void WmWebGLRenderContext::ReleaseResource() {
    std::for_each(mBufferSet.begin(), mBufferSet.end(), [](GLuint v) {
        deleteBuffer(v);
    });
    mBufferSet.clear();

    std::for_each(mFrameBuferSet.begin(), mFrameBuferSet.end(), [](GLuint v) {
        deleteFramebuffer(v);
    });
    mFrameBuferSet.clear();

    std::for_each(mProgramSet.begin(), mProgramSet.end(), [](GLuint v) {
        deleteProgram(v);
    });
    mProgramSet.clear();

    std::for_each(mRenderBufferSet.begin(), mRenderBufferSet.end(), [](GLuint v) {
        deleteRenderbuffer(v);
    });
    mRenderBufferSet.clear();

    std::for_each(mShaderSet.begin(), mShaderSet.end(), [](GLuint v) {
        deleteShader(v);
    });
    mShaderSet.clear();

    std::for_each(mTextureSet.begin(), mTextureSet.end(), [](GLuint v) {
        deleteTexture(v);
    });
    mTextureSet.clear();
}




} //namespace WebGL
} //namespace wmcanvas

