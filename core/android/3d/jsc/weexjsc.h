#ifndef _WEEX_JSC_H_
#define _WEEX_JSC_H_

#include <jni.h>
#include <sys/types.h>

namespace WEEXJSC
{
    typedef const char* (*FunType)(const char*, int, const char*);
    const char* callWmCanvasFun(FunType fp, const char* conextId, int x, const char* args);
    extern void Inject_WmCanvasFunc(FunType fp);

}

#endif //_WEEX_JSC_H_