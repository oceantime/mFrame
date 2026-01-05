package com.honghu.wmcanvas.util;

import android.util.Log;

import com.honghu.wmcanvas.WmCanvasJNI;

public class WmLog {

    public static final String TAG = "CANVAS";

    public static final int LEVEL_FORCE = 0;
    public static final int LEVEL_VERBOSE = 2;
    public static final int LEVEL_DEBUG = 3;
    public static final int LEVEL_INFO = 4;
    public static final int LEVEL_WARN = 5;
    public static final int LEVEL_ERROR = 6;
    public static final int LEVEL_ASSERT = 7;
    public static final int LEVEL_NULL = 9;

    public static int logLevel = LEVEL_DEBUG;


    private static WmCanvasTracer sWmCanvasTracerClz;


    public static void bindWmCanvasTracer(WmCanvasTracer tracer) {
        sWmCanvasTracerClz = tracer;
        registerJavaTracer(tracer);
    }



    private static native void registerJavaTracer(WmCanvasTracer tracer);

    public static void setLevel(String level) {
        if (level == null) {
            return;
        }

        if (level.equals("force")) {
            logLevel = LEVEL_FORCE;
        } else if (level.equals("debug")) {
            logLevel = LEVEL_DEBUG;
        } else if (level.equals("info")) {
            logLevel = LEVEL_INFO;
        } else if (level.equals("warn")) {
            logLevel = LEVEL_WARN;
        } else if (level.equals("error")) {
            logLevel = LEVEL_ERROR;
        } else if (level.equals("fatal")) {
            logLevel = LEVEL_ASSERT;
        }

        WmCanvasJNI.setLogLevel(level);
    }

    public static int getLevel() {
        return logLevel;
    }

    public static void force(String Tag, String message) {
        Log.e(Tag, message);
    }

    public static void i(String message) {
        WmLog.i(TAG, message);
    }

    public static void i(String Tag, String message) {
        if (logLevel == LEVEL_FORCE) {
            WmLog.force(Tag, message);
        } else if (logLevel <= LEVEL_INFO) {
            Log.i(Tag, message);
        }
    }

    public static void i(String Tag, String message, Throwable e) {
        if (logLevel <= LEVEL_INFO) {
            Log.i(Tag, message, e);
        }
    }

    public static void d(String message) {
        WmLog.d(TAG, message);
    }

    public static void d(String Tag, String message) {
        if (logLevel == LEVEL_FORCE) {
            WmLog.force(Tag, message);
        } else if (logLevel <= LEVEL_DEBUG) {
            Log.i(Tag, message);
        }
    }

    public static void d(String Tag, String message, Throwable e) {
        if (logLevel <= LEVEL_DEBUG) {
            Log.i(Tag, message, e);
        }
    }

    public static void e(String message) {
        WmLog.e(TAG, message);
    }

    public static void e(String Tag, String message) {
        if (logLevel <= LEVEL_ERROR) {
            Log.e(Tag, message);
        }
    }

    public static void e(String Tag, String message, Throwable e) {
        if (logLevel <= LEVEL_ERROR) {
            Log.e(Tag, message, e);
        }
    }

    public static void w(String message) {
        WmLog.w(TAG, message);
    }

    public static void w(String Tag, String message) {
        if (logLevel == LEVEL_FORCE) {
            WmLog.force(Tag, message);
        } else if (logLevel <= LEVEL_WARN) {
            Log.w(Tag, message);
        }
    }

    public static void w(String Tag, String message, Throwable e) {
        if (logLevel <= LEVEL_WARN) {
            Log.w(Tag, message, e);
        }
    }

    public static void v(String message) {
        WmLog.v(TAG, message);
    }

    public static void v(String Tag, String message) {
        if (logLevel == LEVEL_FORCE) {
            WmLog.force(Tag, message);
        } else if (logLevel <= LEVEL_VERBOSE) {
            Log.v(Tag, message);
        }
    }

    public static void v(String Tag, String message, Throwable e) {
        if (logLevel <= LEVEL_VERBOSE) {
            Log.v(Tag, message, e);
        }
    }

}
