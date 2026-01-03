#ifndef WMCANVAS_LOG_H
#define WMCANVAS_LOG_H

#if defined(__ANDROID__)
#include <android/log.h>
#endif
#include <vector>
#include <stdarg.h>  
#include <string>
#include "export.h"




//namespace wmcanvas
//{

    typedef void (*WmCanvasSystemLog)(int level, const char *tag, const char *log);

    API_EXPORT extern WmCanvasSystemLog wmcanvasSystemLog;


    typedef enum {
        LOG_LEVEL_DEBUG = 0,
        LOG_LEVEL_INFO,
        LOG_LEVEL_WARN,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_FATAL
    } LogLevel;


    struct WmCanvasHooks;

    typedef void (*WmCanvasExceptionHandler)(const char *canvasId, const char *tag, const char *detail, WmCanvasHooks* selfHooks);


    struct WmCanvasHooks
    {
        WmCanvasExceptionHandler WmCanvasException;
    };



    struct WmCanvasLog
    {
        WmCanvasLog();
        std::string tag;
        std::string detail;
    };

    void fillLogInfo(WmCanvasLog &log, const char *tag, const char *format, ...);


    void FillLogInfo(WmCanvasLog &log, const char *tag, const char *format, ...);


    void AppendErrorLogInfo(std::vector<WmCanvasLog> *errVec, const char *tag, const char *format, ...);


    #if defined(__ANDROID__)
    int TransLogLevel(LogLevel logLevel);
    #endif

    void LogExt(LogLevel logLevel, const char *tag, const char *format, ...);
    void SetLogLevel(LogLevel logLevel);
    LogLevel GetLogLevel();


    API_EXPORT void LogException(WmCanvasHooks *hooks, std::string contextId, const char *tag, const char *format, ...);


    API_EXPORT void LogExceptionVector(WmCanvasHooks *hooks, std::string contextId, std::vector<WmCanvasLog> &vec);


    #ifndef DEBUG
    #define DISABLE_LOG
    #endif
    #define DISABLE_LOG
    #ifdef DISABLE_LOG
    #define LOG_D(...) ;
    #define LOG_I(...) ;
    #define LOG_W(...) ;
    #define LOG_E(...) ;
    #define LOG_F(...) ;
    #define LOG_EXCEPTION(hooks, contextId, tag, ...);
    #define LOG_EXCEPTION_VECTOR(hooks, contextId, vec);
    #else
    #define LOG_D(...) LogExt(LOG_LEVEL_DEBUG, LOG_TAG_NAME, __VA_ARGS__)
    #define LOG_I(...) LogExt(LOG_LEVEL_INFO, LOG_TAG_NAME, __VA_ARGS__)
    #define LOG_W(...) LogExt(LOG_LEVEL_WARN, LOG_TAG_NAME, __VA_ARGS__)
    #define LOG_E(...) LogExt(LOG_LEVEL_ERROR, LOG_TAG_NAME, __VA_ARGS__)
    #define LOG_F(...) LogExt(LOG_LEVEL_FATAL, LOG_TAG_NAME, __VA_ARGS__)
    #define LOG_EXT LogExt
    #define LOG_EXCEPTION(hooks, contextId, tag, ...) LogException(hooks, contextId, tag, __VA_ARGS__)
    #define LOG_EXCEPTION_VECTOR(hooks, contextId, vec) LogExceptionVector(hooks, contextId, vec)
    #endif
//}

#endif /* WMCANVAS_LOG_H */
