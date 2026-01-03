/**
* Created by G-Canvas Open Source Team.
* Copyright (c) 2017, Alibaba, Inc. All rights reserved.
*
* This source code is licensed under the Apache Licence 2.0.
* For the full copyright and license information, please view
* the LICENSE file in the root directory of this source tree.
*/

#ifndef WMCANVAS_WMCOMMANDDECODERWEBGL_H
#define WMCANVAS_WMCOMMANDDECODERWEBGL_H

#include "WmCommandDecoder.hpp"
#include "WmWebGLRenderContext.hpp"
#include <string>


namespace wmcanvas
{

class WmCommandDecoderWebGL : public WmCommandDecoder
{
    enum WMWEBGL_FUNC_ID
    {
        WMWEBGL_FUNC_activeTexture = 1,
        WMWEBGL_FUNC_attachShader,
        WMWEBGL_FUNC_bindAttribLocation,
        WMWEBGL_FUNC_bindBuffer,
        WMWEBGL_FUNC_bindFramebuffer,
        WMWEBGL_FUNC_bindRenderbuffer,
        WMWEBGL_FUNC_bindTexture,
        WMWEBGL_FUNC_blendColor,
        WMWEBGL_FUNC_blendEquation,
        WMWEBGL_FUNC_blendEquationSeparate,
        WMWEBGL_FUNC_blendFunc,
        WMWEBGL_FUNC_blendFuncSeparate,
        WMWEBGL_FUNC_bufferData_size,
        WMWEBGL_FUNC_bufferData_buffer,
        WMWEBGL_FUNC_bufferSubData,
        WMWEBGL_FUNC_checkFramebufferStatus,
        WMWEBGL_FUNC_clear,
        WMWEBGL_FUNC_clearColor,
        WMWEBGL_FUNC_clearDepth,
        WMWEBGL_FUNC_clearStencil,
        WMWEBGL_FUNC_colorMask,
        WMWEBGL_FUNC_commit,
        WMWEBGL_FUNC_compileShader,
        WMWEBGL_FUNC_compressedTexImage2D,
        WMWEBGL_FUNC_compressedTexSubImage2D,
        WMWEBGL_FUNC_copyTexImage2D,
        WMWEBGL_FUNC_copyTexSubImage2D,
        WMWEBGL_FUNC_createBuffer,
        WMWEBGL_FUNC_createFramebuffer,
        WMWEBGL_FUNC_createProgram,
        WMWEBGL_FUNC_createRenderbuffer,
        WMWEBGL_FUNC_createShader,
        WMWEBGL_FUNC_createTexture,
        WMWEBGL_FUNC_cullFace,
        WMWEBGL_FUNC_deleteBuffer,
        WMWEBGL_FUNC_deleteFramebuffer,
        WMWEBGL_FUNC_deleteProgram,
        WMWEBGL_FUNC_deleteRenderbuffer,
        WMWEBGL_FUNC_deleteShader,
        WMWEBGL_FUNC_deleteTexture,
        WMWEBGL_FUNC_depthFunc,
        WMWEBGL_FUNC_depthMask,
        WMWEBGL_FUNC_depthRange,
        WMWEBGL_FUNC_detachShader,
        WMWEBGL_FUNC_disable,
        WMWEBGL_FUNC_disableVertexAttribArray,
        WMWEBGL_FUNC_drawArrays,
        WMWEBGL_FUNC_drawElements,
        WMWEBGL_FUNC_enable,
        WMWEBGL_FUNC_enableVertexAttribArray,
        WMWEBGL_FUNC_finish,
        WMWEBGL_FUNC_flush,
        WMWEBGL_FUNC_framebufferRenderbuffer,
        WMWEBGL_FUNC_framebufferTexture2D,
        WMWEBGL_FUNC_frontFace,
        WMWEBGL_FUNC_generateMipmap,
        WMWEBGL_FUNC_getActiveAttrib,
        WMWEBGL_FUNC_getActiveUniform,
        WMWEBGL_FUNC_getAttachedShaders,
        WMWEBGL_FUNC_getAttribLocation,
        WMWEBGL_FUNC_getBufferParameter,
        WMWEBGL_FUNC_getContextAttributes,
        WMWEBGL_FUNC_getError,
        WMWEBGL_FUNC_getExtension,
        WMWEBGL_FUNC_getFramebufferAttachmentParameter,
        WMWEBGL_FUNC_getParameter,
        WMWEBGL_FUNC_getProgramInfoLog,
        WMWEBGL_FUNC_getProgramParameter,
        WMWEBGL_FUNC_getRenderbufferParameter,
        WMWEBGL_FUNC_getShaderInfoLog,
        WMWEBGL_FUNC_getShaderParameter,
        WMWEBGL_FUNC_getShaderPrecisionFormat,
        WMWEBGL_FUNC_getShaderSource,
        WMWEBGL_FUNC_getSupportedExtensions,
        WMWEBGL_FUNC_getTexParameter,
        WMWEBGL_FUNC_getUniform,
        WMWEBGL_FUNC_getUniformLocation,
        WMWEBGL_FUNC_getVertexAttrib,
        WMWEBGL_FUNC_getVertexAttribOffset,
        WMWEBGL_FUNC_hint,
        WMWEBGL_FUNC_isBuffer,
        WMWEBGL_FUNC_isContextLost,
        WMWEBGL_FUNC_isEnabled,
        WMWEBGL_FUNC_isFramebuffer,
        WMWEBGL_FUNC_isProgram,
        WMWEBGL_FUNC_isRenderbuffer,
        WMWEBGL_FUNC_isShader,
        WMWEBGL_FUNC_isTexture,
        WMWEBGL_FUNC_lineWidth,
        WMWEBGL_FUNC_linkProgram,
        WMWEBGL_FUNC_pixelStorei,
        WMWEBGL_FUNC_polygonOffset,
        WMWEBGL_FUNC_readPixels,
        WMWEBGL_FUNC_renderbufferStorage,
        WMWEBGL_FUNC_sampleCoverage,
        WMWEBGL_FUNC_scissor,
        WMWEBGL_FUNC_shaderSource,
        WMWEBGL_FUNC_stencilFunc,
        WMWEBGL_FUNC_stencilFuncSeparate,
        WMWEBGL_FUNC_stencilMask,
        WMWEBGL_FUNC_stencilMaskSeparate,
        WMWEBGL_FUNC_stencilOp,
        WMWEBGL_FUNC_stencilOpSeparate,
        WMWEBGL_FUNC_texImage2D,
        WMWEBGL_FUNC_texImage2D_null,
        WMWEBGL_FUNC_texImage2D_pixels,
        WMWEBGL_FUNC_texParameterf,
        WMWEBGL_FUNC_texParameteri,
        WMWEBGL_FUNC_texSubImage2D,
        WMWEBGL_FUNC_texSubImage2D_null,
        WMWEBGL_FUNC_texSubImage2D_pixels,
        WMWEBGL_FUNC_uniform1f,
        WMWEBGL_FUNC_uniform2f,
        WMWEBGL_FUNC_uniform3f,
        WMWEBGL_FUNC_uniform4f,
        WMWEBGL_FUNC_uniform1i,
        WMWEBGL_FUNC_uniform2i,
        WMWEBGL_FUNC_uniform3i,
        WMWEBGL_FUNC_uniform4i,
        WMWEBGL_FUNC_uniform1fv,
        WMWEBGL_FUNC_uniform2fv,
        WMWEBGL_FUNC_uniform3fv,
        WMWEBGL_FUNC_uniform4fv,
        WMWEBGL_FUNC_uniform1iv,
        WMWEBGL_FUNC_uniform2iv,
        WMWEBGL_FUNC_uniform3iv,
        WMWEBGL_FUNC_uniform4iv,
        WMWEBGL_FUNC_uniformMatrix2fv,
        WMWEBGL_FUNC_uniformMatrix3fv,
        WMWEBGL_FUNC_uniformMatrix4fv,
        WMWEBGL_FUNC_useProgram,
        WMWEBGL_FUNC_validateProgram,
        WMWEBGL_FUNC_vertexAttrib1f,
        WMWEBGL_FUNC_vertexAttrib2f,
        WMWEBGL_FUNC_vertexAttrib3f,
        WMWEBGL_FUNC_vertexAttrib4f,
        WMWEBGL_FUNC_vertexAttrib1fv,
        WMWEBGL_FUNC_vertexAttrib2fv,
        WMWEBGL_FUNC_vertexAttrib3fv,
        WMWEBGL_FUNC_vertexAttrib4fv,
        WMWEBGL_FUNC_vertexAttribPointer,
        WMWEBGL_FUNC_viewport,
        
        //extension
        WMWEBGL_FUNC_drawArraysInstancedANGLE = 200,
        WMWEBGL_FUNC_drawElementsInstancedANGLE,
        WMWEBGL_FUNC_vertexAttribDivisorANGLE,
        WMWEBGL_FUNC_createVertexArrayOES,
        WMWEBGL_FUNC_deleteVertexArrayOES,
        WMWEBGL_FUNC_isVertexArrayOES,
        WMWEBGL_FUNC_bindVertexArrayOES,
        
        //API����
        WMWEBGL_FUNC_COUNT,
    };
    
    
public:
    API_EXPORT WmCommandDecoderWebGL(WebGL::WmWebGLRenderContext* renderContext);
    API_EXPORT ~WmCommandDecoderWebGL();

    virtual void InitCommands();
    inline virtual bool IsSyncCommand(uint32_t cmdId){
        auto iter = mWebGLSyncFuncSet.find(cmdId);
        return iter != mWebGLSyncFuncSet.end();
    }
    API_EXPORT virtual bool Decode(WmCommandBuffer& buffer, WmDecodeRes& res);

    //WebGL1.0 API
    static void WebGL_activeTexture(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_attachShader(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_bindAttribLocation(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_bindBuffer(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_bindFramebuffer(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_bindRenderbuffer(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_bindTexture(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_blendColor(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_blendEquation(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_blendEquationSeparate(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_blendFunc(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_blendFuncSeparate(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_bufferData_size(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_bufferData_buffer(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_bufferSubData(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_checkFramebufferStatus(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_clear(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_clearColor(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_clearDepth(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_clearStencil(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_colorMask(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_commit(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_compileShader(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_compressedTexImage2D(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_compressedTexSubImage2D(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_copyTexImage2D(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_copyTexSubImage2D(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_createBuffer(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_createFramebuffer(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_createProgram(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_createRenderbuffer(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_createShader(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_createTexture(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_cullFace(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_deleteBuffer(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_deleteFramebuffer(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_deleteProgram(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_deleteRenderbuffer(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_deleteShader(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_deleteTexture(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_depthFunc(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_depthMask(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_depthRange(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_detachShader(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_disable(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_disableVertexAttribArray(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_drawArrays(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_drawElements(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_enable(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_enableVertexAttribArray(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_finish(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_flush(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_framebufferRenderbuffer(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_framebufferTexture2D(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_frontFace(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_generateMipmap(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getActiveAttrib(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getActiveUniform(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getAttachedShaders(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getAttribLocation(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getBufferParameter(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getContextAttributes(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getError(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getExtension(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getFramebufferAttachmentParameter(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getParameter(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getProgramInfoLog(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getProgramParameter(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getRenderbufferParameter(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getShaderInfoLog(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getShaderParameter(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getShaderPrecisionFormat(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getShaderSource(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getSupportedExtensions(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getTexParameter(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getUniform(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getUniformLocation(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getVertexAttrib(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_getVertexAttribOffset(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_hint(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_isBuffer(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_isContextLost(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_isEnabled(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_isFramebuffer(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_isProgram(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_isRenderbuffer(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_isShader(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_isTexture(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_lineWidth(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_linkProgram(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_pixelStorei(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_polygonOffset(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_readPixels(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_renderbufferStorage(WmCommandBuffer &buffer, WmDecodeRes &res);
    static void WebGL_sampleCoverage(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_scissor(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_shaderSource(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_stencilFunc(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_stencilFuncSeparate(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_stencilMask(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_stencilMaskSeparate(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_stencilOp(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_stencilOpSeparate(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_texImage2D(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_texImage2D_null(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_texImage2D_pixels(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_texParameterf(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_texParameteri(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_texSubImage2D(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_texSubImage2D_null(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_texSubImage2D_pixels(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniform1f(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniform2f(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniform3f(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniform4f(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniform1i(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniform2i(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniform3i(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniform4i(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniform1fv(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniform2fv(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniform3fv(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniform4fv(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniform1iv(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniform2iv(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniform3iv(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniform4iv(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniformMatrix2fv(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniformMatrix3fv(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_uniformMatrix4fv(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_useProgram(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_validateProgram(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_vertexAttrib1f(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_vertexAttrib2f(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_vertexAttrib3f(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_vertexAttrib4f(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_vertexAttrib1fv(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_vertexAttrib2fv(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_vertexAttrib3fv(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_vertexAttrib4fv(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_vertexAttribPointer(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_viewport(WmCommandBuffer& buffer, WmDecodeRes& res);
    
    //WebGL1.0 Extension
    static void WebGL_drawArraysInstancedANGLE(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_drawElementsInstancedANGLE(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_vertexAttribDivisorANGLE(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_createVertexArrayOES(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_deleteVertexArrayOES(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_isVertexArrayOES(WmCommandBuffer& buffer, WmDecodeRes& res);
    static void WebGL_bindVertexArrayOES(WmCommandBuffer& buffer, WmDecodeRes& res);
    
protected:
//    WmCommandFuncPtr mWebGLFuncs[WMWEBGL_FUNC_COUNT];
//    WmCommandFuncPtr mWebGLFuncs[WMWEBGL_FUNC_COUNT] = { nullptr };
    std::unordered_set<unsigned int> mWebGLSyncFuncSet;
//    WebGL::WmWebGLRenderContext* mRenderContext;
};

} //namespace wmcanvas

#endif /* WMCANVAS_WMCOMMANDDECODERWEBGL_H */

