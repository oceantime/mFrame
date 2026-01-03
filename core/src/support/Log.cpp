#include "Log.h"


//namespace wmcanvas {

    #define TRACE_TAG_BUF_SIZE 128
    #define TRACE_MSG_BUF_SIZE 512

    #define LOG_MSG_BUF_SIZE 1024


    // external system log handler
    WmCanvasSystemLog wmcanvasSystemLog;

    // default log level : Error
    LogLevel g_log_level = LOG_LEVEL_ERROR;

    LogLevel GetLogLevel() { return g_log_level; };

    void SetLogLevel(LogLevel logLevel) {
        if (g_log_level == logLevel) return;
#ifdef DEBUG
        LogLevel oldLevel = g_log_level;
        LOG_I("[SetLogLevel] %d=>%d", oldLevel, logLevel);
#endif
        g_log_level = logLevel;
    };


    // should rename to GCanvasTraceData
    WmCanvasLog::WmCanvasLog() {
        tag.resize(TRACE_TAG_BUF_SIZE, '\0');
        detail.resize(TRACE_MSG_BUF_SIZE, '\0');
    }


    // @deprecated, will delete soon
    void fillLogInfo(WmCanvasLog &log, const char *tag, const char *format, ...) {
        snprintf((char*)log.tag.data(), TRACE_TAG_BUF_SIZE, "%s", tag);
        va_list va;
        va_start(va, format);
        vsnprintf((char*)log.detail.data(), TRACE_MSG_BUF_SIZE, format, va);
        va_end(va);
    }



    void FillLogInfo(WmCanvasLog &log, const char *tag, const char *format, ...) {
        snprintf((char*)log.tag.data(), TRACE_TAG_BUF_SIZE, "%s", tag);
        va_list va;
        va_start(va, format);
        vsnprintf((char*)log.detail.data(), TRACE_MSG_BUF_SIZE, format, va);
        va_end(va);
    }


    void AppendErrorLogInfo(std::vector<WmCanvasLog> *errVec, const char *tag, const char *format, ...)
    {
        if (errVec == nullptr) {
            return;
        }

        WmCanvasLog log;

        snprintf((char*)log.tag.data(), TRACE_TAG_BUF_SIZE, "%s", tag);

        va_list va;
        va_start(va, format);
        vsnprintf((char*)log.detail.data(), TRACE_MSG_BUF_SIZE, format, va);
        va_end(va);

        errVec->push_back(log);
    }


#if defined(__ANDROID__)

    int TransLogLevel(LogLevel logLevel) {
        if (LOG_LEVEL_DEBUG == logLevel) return ANDROID_LOG_DEBUG;
        if (LOG_LEVEL_INFO == logLevel) return ANDROID_LOG_INFO;
        if (LOG_LEVEL_WARN == logLevel) return ANDROID_LOG_WARN;
        if (LOG_LEVEL_ERROR == logLevel) return ANDROID_LOG_ERROR;
        if (LOG_LEVEL_FATAL == logLevel) return ANDROID_LOG_FATAL;
        return ANDROID_LOG_DEFAULT;
    }

#endif

    void LogExt(LogLevel logLevel, const char *tag, const char *format, ...) {
        if (g_log_level > logLevel) return;

        va_list va;
        char buffer[LOG_MSG_BUF_SIZE];

        va_start(va, format);
        vsnprintf(buffer, LOG_MSG_BUF_SIZE, format, va);
        va_end(va);

#if defined(__ANDROID__)
        if (wmcanvasSystemLog) {
            wmcanvasSystemLog(logLevel, tag, buffer);
        } else {
            __android_log_write(TransLogLevel(logLevel), tag, buffer);
        }
#else
        if (wmcanvasSystemLog) {
            wmcanvasSystemLog(logLevel, tag, buffer);
        } else {
            printf("%s\n", buffer);
        }
#endif
    }



    void LogException(WmCanvasHooks *hooks, std::string contextId, const char *tag, const char *format, ...)
    {
        va_list va;
        char buffer[LOG_MSG_BUF_SIZE];

        va_start(va, format);
        vsnprintf(buffer, LOG_MSG_BUF_SIZE, format, va);
        va_end(va);


#if defined(__ANDROID__)
        if (hooks && hooks->WmCanvasException) {
            hooks->WmCanvasException(contextId.data(), tag, buffer, hooks);
        } else {
            __android_log_write(TransLogLevel(g_log_level), tag, buffer);
        }
#else
        if (hooks && hooks->WmCanvasException) {
            hooks->WmCanvasException(contextId.c_str(), tag, buffer, nullptr);
        }
#endif
    }


    API_EXPORT void LogExceptionVector(WmCanvasHooks *hooks, std::string contextId, std::vector<WmCanvasLog> &vec)
    {
        if (vec.size()) {
            for (int i = 0; i < vec.size(); i++) {
                WmCanvasLog& log = vec[i];
                LogException(hooks, contextId, log.tag.data(), log.detail.data());
            }
        }
    }
//}
