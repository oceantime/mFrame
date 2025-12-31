//
// Created by yuantong on 2017/8/1.
//



#ifndef WM_CANVAS_G3D_JNI_H
#define WM_CANVAS_G3D_JNI_H

#include <jni.h>

extern "C" {

JNIEXPORT void JNICALL
Java_com_taobao_wmcanvas_surface_WmTextureViewCallback_onSurfaceCreated(JNIEnv *, jobject, jstring,
                                                                      jobject);

JNIEXPORT void JNICALL
Java_com_taobao_wmcanvas_surface_WmTextureViewCallback_onSurfaceChanged(JNIEnv *, jobject, jstring,
                                                                      jobject, jint, jint, jint, jstring);


JNIEXPORT void JNICALL
Java_com_taobao_wmcanvas_surface_WmTextureViewCallback_onSurfaceDestroyed(JNIEnv *, jobject, jstring,
                                                                        jobject);


JNIEXPORT void JNICALL
Java_com_taobao_wmcanvas_surface_WmTextureViewCallback_onRenderExit(JNIEnv *env,
                                                                  jobject instance,
                                                                  jstring key);
}

#endif //WM_CANVAS_G3D_JNI_H
