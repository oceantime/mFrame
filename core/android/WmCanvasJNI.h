/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#include <jni.h>
/* Header for class com_taobao_wmcanvas_WmCanvasJNI */

#ifndef _Included_com_taobao_wmcanvas_WmCanvasJNI
#define _Included_com_taobao_wmcanvas_WmCanvasJNI
#ifdef __cplusplus
extern "C" {
#endif

#ifdef WmCanvas

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_newCanvas(JNIEnv *,
                                                                    jclass,
                                                                    jstring,
                                                                    jint,
                                                                    jstring);

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_freeCanvas(JNIEnv *,
                                                                     jclass,
                                                                     jstring);

/*
 * Class:     com_taobao_wmcanvas_WmCanvasJNI
 * Method:    setBackgroundColor
 * Signature: (III)V
 */

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_setClearColor(
        JNIEnv *, jclass, jstring, jstring);

/*
 * Class:     com_taobao_wmcanvas_WmCanvasJNI
 * Method:    setOrtho
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_setOrtho(JNIEnv *,
                                                                   jclass,
                                                                   jstring,
                                                                   jint, jint);

/*
 * Class:     com_taobao_wmcanvas_WmCanvasJNI
 * Method:    addTexture
 * Signature: (IIII)V
 */
JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_addTexture(
        JNIEnv *, jclass, jstring, jint, jint, jint, jint);

/*
 * Class:     com_taobao_wmcanvas_WmCanvasJNI
 * Method:    addPngTexture
 * Signature:
 * (Ljava/lang/Object;Ljava/lang/String;ILcom/taobao/gcanvas/WmCanvasTextureDimension;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_addPngTexture(
        JNIEnv *, jclass, jstring, jobject, jstring, jint, jobject);

/*
 * Class:     com_taobao_wmcanvas_WmCanvasJNI_addPngTextureByStream
 * Method:    addPngTexture
 * Signature:
 * (Ljava/lang/Object;Ljava/lang/String;ILcom/taobao/gcanvas/WmCanvasTextureDimension;)Z
 */
JNIEXPORT jboolean JNICALL
Java_com_taobao_wmcanvas_WmCanvasJNI_addPngTextureByStream(JNIEnv *, jclass,
                                                         jstring, jbyteArray,
                                                         jint, jobject);

/*
 * Class:     com_taobao_wmcanvas_WmCanvasJNI
 * Method:    removeTexture
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_removeTexture(
        JNIEnv *, jclass, jstring, jint);

/*
 * Class:     com_taobao_wmcanvas_WmCanvasJNI
 * Method:    render
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_render(
        JNIEnv *je, jclass jc, jstring contextId,
        jstring renderCommands);

/*
 * Class:     com_taobao_wmcanvas_WmCanvasJNI
 * Method:    surfaceChanged
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_surfaceChanged(
        JNIEnv *, jclass, jstring, jint, jint);

/*
 * Class:     com_taobao_wmcanvas_WmCanvasJNI
 * Method:    captureGLLayer
 * Signature: (Ljava/lang/String;IIIILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_captureGLLayer(
        JNIEnv *, jclass, jstring, jstring, jint, jint, jint, jint, jstring);

/*
 * Class:     com_taobao_wmcanvas_WmCanvasJNI
 * Method:    release
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_release(JNIEnv *,
                                                                  jclass);

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_init(JNIEnv *,
                                                               jclass);

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_uninit(JNIEnv *,
                                                                 jclass);

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_setTyOffsetFlag(
        JNIEnv *je, jclass jc, jstring contextId, jboolean flag);

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_setHiQuality(
        JNIEnv *je, jclass jc, jstring contextId, jboolean isHiQuality);

JNIEXPORT jboolean JNICALL
Java_com_taobao_wmcanvas_WmCanvasJNI_isNeonSupport(JNIEnv *, jclass);

JNIEXPORT jboolean JNICALL
Java_com_taobao_wmcanvas_WmCanvasJNI_isFboSupport(JNIEnv *, jclass, jstring);

JNIEXPORT jstring JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_getImageData(
        JNIEnv *je, jclass jc, jstring contextId, jint x, jint y, jint width,
        jint height);

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_setDevicePixelRatio(
        JNIEnv *je, jclass jc, jstring contextId, jdouble ratio);
#endif

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_setFallbackFont(
        JNIEnv *je, jclass jc, jstring fallback_font_name,
        jstring system_font_location);

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_addFontFamily(
        JNIEnv *je, jclass jc, jobjectArray font_names, jobjectArray font_files);

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_addFallbackFontFamily(
        JNIEnv *je, jclass jc, jobjectArray font_files);

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_setLogLevel(
        JNIEnv *je, jclass jc, jstring logLevel);

#ifdef WmCanvas

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_setPreCompilePath(
        JNIEnv *je, jclass jc, jstring path);

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_setContextType(
        JNIEnv *je, jclass jc, jstring contextId, jint type);

JNIEXPORT jstring JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_getAllParameter(
        JNIEnv *je, jclass jc, jstring contextId);

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_setConfig(JNIEnv *je,
                                                                    jclass jc,
                                                                    jstring key,
                                                                    jint value);
JNIEXPORT jstring JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_exeSyncCmd
        (JNIEnv *je, jclass jc, jstring ContextID, jint type, jstring args);

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_bindTexture(
        JNIEnv *je, jclass jc, jstring contextId, jobject bitmap, jint id,
        jint target, jint level, jint internalformat, jint format, jint type);

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_texSubImage2D(
        JNIEnv *je, jclass jc, jstring contextId, jobject bitmap, jint id,
        jint target, jint level, jint xoffset, jint yoffset, jint format, jint type);


JNIEXPORT bool JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_sendEvent(
		JNIEnv *je, jclass jc, jstring contextId);

JNIEXPORT void JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_registerCallback(JNIEnv *je, jclass jc, jstring soPath, jint version);

JNIEXPORT jint JNICALL Java_com_taobao_wmcanvas_WmCanvasJNI_getNativeFps(JNIEnv *je, jclass jc, jstring contextId);

#endif

#ifdef __cplusplus
}
#endif
#endif
