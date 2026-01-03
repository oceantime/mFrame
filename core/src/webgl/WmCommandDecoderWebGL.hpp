/**
* Created by G-Canvas Open Source Team.
* Copyright (c) 2017, Alibaba, Inc. All rights reserved.
*
* This source code is licensed under the Apache Licence 2.0.
* For the full copyright and license information, please view
* the LICENSE file in the root directory of this source tree.
*/

#ifndef GCANVAS_GCOMMANDDECODERWEBGL_H
#define GCANVAS_GCOMMANDDECODERWEBGL_H

#include "WmCommandDecoder.hpp"
#include "WmWebGLRenderContext.hpp"
#include <string>


namespace wmcanvas
{

class GCommandDecoderWebGL : public WmCommandDecoder
{
    enum GWEBGL_FUNC_ID
    {
        GWEBGL_FUNC_activeTexture = 1,
        GWEBGL_FUNC_attachShader,
        GWEBGL_FUNC_bindAttribLocation,
        GWEBGL_FUNC_bindBuffer,
        GWEBGL_FUNC_bindFramebuffer,
        GWEBGL_FUNC_bindRenderbuffer,
        GWEBGL_FUNC_bindTexture,
        GWEBGL_FUNC_blendColor,
        GWEBGL_FUNC_blendEquation,
        GWEBGL_FUNC_blendEquationSeparate,
        GWEBGL_FUNC_blendFunc,
        GWEBGL_FUNC_blendFuncSeparate,
        GWEBGL_FUNC_bufferData_size,
        GWEBGL_FUNC_bufferData_buffer,
        GWEBGL_FUNC_bufferSubData,
        GWEBGL_FUNC_checkFramebufferStatus,
        GWEBGL_FUNC_clear,
        GWEBGL_FUNC_clearColor,
        GWEBGL_FUNC_clearDepth,
        GWEBGL_FUNC_clearStencil,
        GWEBGL_FUNC_colorMask,
        GWEBGL_FUNC_commit,
        GWEBGL_FUNC_compileShader,
        GWEBGL_FUNC_compressedTexImage2D,
        GWEBGL_FUNC_compressedTexSubImage2D,
        GWEBGL_FUNC_copyTexImage2D,
        GWEBGL_FUNC_copyTexSubImage2D,
        GWEBGL_FUNC_createBuffer,
        GWEBGL_FUNC_createFramebuffer,
        GWEBGL_FUNC_createProgram,
        GWEBGL_FUNC_createRenderbuffer,
        GWEBGL_FUNC_createShader,
        GWEBGL_FUNC_createTexture,
        GWEBGL_FUNC_cullFace,
        GWEBGL_FUNC_deleteBuffer,
        GWEBGL_FUNC_deleteFramebuffer,
        GWEBGL_FUNC_deleteProgram,
        GWEBGL_FUNC_deleteRenderbuffer,
        GWEBGL_FUNC_deleteShader,
        GWEBGL_FUNC_deleteTexture,
        GWEBGL_FUNC_depthFunc,
        GWEBGL_FUNC_depthMask,
        GWEBGL_FUNC_depthRange,
        GWEBGL_FUNC_detachShader,
        GWEBGL_FUNC_disable,
        GWEBGL_FUNC_disableVertexAttribArray,
        GWEBGL_FUNC_drawArrays,
        GWEBGL_FUNC_drawElements,
        GWEBGL_FUNC_enable,
        GWEBGL_FUNC_enableVertexAttribArray,
        GWEBGL_FUNC_finish,
        GWEBGL_FUNC_flush,
        GWEBGL_FUNC_framebufferRenderbuffer,
        GWEBGL_FUNC_framebufferTexture2D,
        GWEBGL_FUNC_frontFace,
        GWEBGL_FUNC_generateMipmap,
        GWEBGL_FUNC_getActiveAttrib,
        GWEBGL_FUNC_getActiveUniform,
        GWEBGL_FUNC_getAttachedShaders,
        GWEBGL_FUNC_getAttribLocation,
        GWEBGL_FUNC_getBufferParameter,
        GWEBGL_FUNC_getContextAttributes,
        GWEBGL_FUNC_getError,
        GWEBGL_FUNC_getExtension,
        GWEBGL_FUNC_getFramebufferAttachmentParameter,
        GWEBGL_FUNC_getParameter,
        GWEBGL_FUNC_getProgramInfoLog,
        GWEBGL_FUNC_getProgramParameter,
        GWEBGL_FUNC_getRenderbufferParameter,
        GWEBGL_FUNC_getShaderInfoLog,
        GWEBGL_FUNC_getShaderParameter,
        GWEBGL_FUNC_getShaderPrecisionFormat,
        GWEBGL_FUNC_getShaderSource,
        GWEBGL_FUNC_getSupportedExtensions,
        GWEBGL_FUNC_getTexParameter,
        GWEBGL_FUNC_getUniform,
        GWEBGL_FUNC_getUniformLocation,
        GWEBGL_FUNC_getVertexAttrib,
        GWEBGL_FUNC_getVertexAttribOffset,
        GWEBGL_FUNC_hint,
        GWEBGL_FUNC_isBuffer,
        GWEBGL_FUNC_isContextLost,
        GWEBGL_FUNC_isEnabled,
        GWEBGL_FUNC_isFramebuffer,
        GWEBGL_FUNC_isProgram,
        GWEBGL_FUNC_isRenderbuffer,
        GWEBGL_FUNC_isShader,
        GWEBGL_FUNC_isTexture,
        GWEBGL_FUNC_lineWidth,
        GWEBGL_FUNC_linkProgram,
        GWEBGL_FUNC_pixelStorei,
        GWEBGL_FUNC_polygonOffset,
        GWEBGL_FUNC_readPixels,
        GWEBGL_FUNC_renderbufferStorage,
        GWEBGL_FUNC_sampleCoverage,
        GWEBGL_FUNC_scissor,
        GWEBGL_FUNC_shaderSource,
        GWEBGL_FUNC_stencilFunc,
        GWEBGL_FUNC_stencilFuncSeparate,
        GWEBGL_FUNC_stencilMask,
        GWEBGL_FUNC_stencilMaskSeparate,
        GWEBGL_FUNC_stencilOp,
        GWEBGL_FUNC_stencilOpSeparate,
        GWEBGL_FUNC_texImage2D,
        GWEBGL_FUNC_texImage2D_null,
        GWEBGL_FUNC_texImage2D_pixels,
        GWEBGL_FUNC_texParameterf,
        GWEBGL_FUNC_texParameteri,
        GWEBGL_FUNC_texSubImage2D,
        GWEBGL_FUNC_texSubImage2D_null,
        GWEBGL_FUNC_texSubImage2D_pixels,
        GWEBGL_FUNC_uniform1f,
        GWEBGL_FUNC_uniform2f,
        GWEBGL_FUNC_uniform3f,
        GWEBGL_FUNC_uniform4f,
        GWEBGL_FUNC_uniform1i,
        GWEBGL_FUNC_uniform2i,
        GWEBGL_FUNC_uniform3i,
        GWEBGL_FUNC_uniform4i,
        GWEBGL_FUNC_uniform1fv,
        GWEBGL_FUNC_uniform2fv,
        GWEBGL_FUNC_uniform3fv,
        GWEBGL_FUNC_uniform4fv,
        GWEBGL_FUNC_uniform1iv,
        GWEBGL_FUNC_uniform2iv,
        GWEBGL_FUNC_uniform3iv,
        GWEBGL_FUNC_uniform4iv,
        GWEBGL_FUNC_uniformMatrix2fv,
        GWEBGL_FUNC_uniformMatrix3fv,
        GWEBGL_FUNC_uniformMatrix4fv,
        GWEBGL_FUNC_useProgram,
        GWEBGL_FUNC_validateProgram,
        GWEBGL_FUNC_vertexAttrib1f,
        GWEBGL_FUNC_vertexAttrib2f,
        GWEBGL_FUNC_vertexAttrib3f,
        GWEBGL_FUNC_vertexAttrib4f,
        GWEBGL_FUNC_vertexAttrib1fv,
        GWEBGL_FUNC_vertexAttrib2fv,
        GWEBGL_FUNC_vertexAttrib3fv,
        GWEBGL_FUNC_vertexAttrib4fv,
        GWEBGL_FUNC_vertexAttribPointer,
        GWEBGL_FUNC_viewport,
        
        //extension
        GWEBGL_FUNC_drawArraysInstancedANGLE = 200,
        GWEBGL_FUNC_drawElementsInstancedANGLE,
        GWEBGL_FUNC_vertexAttribDivisorANGLE,
        GWEBGL_FUNC_createVertexArrayOES,
        GWEBGL_FUNC_deleteVertexArrayOES,
        GWEBGL_FUNC_isVertexArrayOES,
        GWEBGL_FUNC_bindVertexArrayOES,
        
        //API数量
        GWEBGL_FUNC_COUNT,
    };
    
    
public:
    API_EXPORT GCommandDecoderWebGL(WebGL::GWebGLRenderContext* renderContext);
    API_EXPORT ~GCommandDecoderWebGL();

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
//    GCommandFunc mWebGLFuncs[GWEBGL_FUNC_COUNT];
//    GCommandFuncNew mWebGLFuncs[GWEBGL_FUNC_COUNT] = { nullptr };
    std::unordered_set<unsigned int> mWebGLSyncFuncSet;
//    WebGL::GWebGLRenderContext* mRenderContext;
};

} //namespace wmcanvas

#endif /* GCANVAS_GCOMMANDDECODERWEBGL_H */
