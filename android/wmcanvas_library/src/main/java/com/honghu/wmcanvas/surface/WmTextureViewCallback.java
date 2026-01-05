package com.honghu.wmcanvas.surface;

import android.annotation.TargetApi;
import android.graphics.SurfaceTexture;
import android.os.Build;
import android.text.TextUtils;
import android.view.Surface;
import android.view.TextureView;

import com.honghu.wmcanvas.WmCanvas;
import com.honghu.wmcanvas.WmCanvasJNI;
import com.honghu.wmcanvas.util.WmLog;

import java.util.ArrayList;

import android.annotation.TargetApi;
import android.graphics.SurfaceTexture;
import android.os.Build;
import android.text.TextUtils;
import android.view.Surface;
import android.view.TextureView;

import com.honghu.wmcanvas.WmCanvasJNI;
import com.honghu.wmcanvas.util.WmLog;


import java.util.ArrayList;

public class WmTextureViewCallback implements TextureView.SurfaceTextureListener {
    private final String mKey;
    private String mBackgroundColor = "#ffffff";
    private Surface mSurface;
    private TextureView mTextureview;

    private ArrayList<TextureView.SurfaceTextureListener> mDelegateLists;

    static {
        WmCanvasJNI.load();
    }

    public WmTextureViewCallback(TextureView v, String id) {
        this.mKey = id;
        this.mTextureview = v;
        WmCanvasJNI.setWrapperContextType(id, 0);
    }


    public void addSurfaceTextureListener(TextureView.SurfaceTextureListener listener) {
        if (null == mDelegateLists) {
            mDelegateLists = new ArrayList<>(1);
        }

        if (!mDelegateLists.contains(listener)) {
            mDelegateLists.add(listener);
        }
    }

    public void removeSurfaceTextureListener(TextureView.SurfaceTextureListener listener) {
        if (null == mDelegateLists) {
            return;
        }

        mDelegateLists.remove(listener);
    }

    public void setBackgroundColor(String color) {
        if (!TextUtils.isEmpty(color)) {
            mBackgroundColor = color;
        }
    }

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        WmLog.d("on surfaceTexture Available.");
        if (mSurface == null) {
            mSurface = new Surface(surface);
        }
        onSurfaceChanged(this.mKey, mSurface, 0, width, height, mBackgroundColor);
        WmCanvasJNI.refreshArguments(mKey);
        if (WmCanvasJNI.sendEvent(mKey)) {
            if (mTextureview instanceof WmTextureView) {
                WmLog.d("start to send event in GSurfaceCallback.");
                ((WmTextureView) mTextureview).sendEvent();
            }
        }

        if (null != mDelegateLists) {
            for (TextureView.SurfaceTextureListener listener : mDelegateLists) {
                listener.onSurfaceTextureAvailable(surface, width, height);
            }
        }
    }

    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        WmLog.d("on surfaceTexture changed.");

        if (mSurface == null) {
            mSurface = new Surface(surface);
        }

        onSurfaceChanged(this.mKey, mSurface, 0, width, height, mBackgroundColor);

        if (null != mDelegateLists) {
            for (TextureView.SurfaceTextureListener listener : mDelegateLists) {
                listener.onSurfaceTextureSizeChanged(surface, width, height);
            }
        }
    }

    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        WmLog.d("on surfaceTexture destroyed.");

        if (null != mDelegateLists) {
            for (TextureView.SurfaceTextureListener listener : mDelegateLists) {
                listener.onSurfaceTextureDestroyed(surface);
            }
        }

        onSurfaceDestroyed(this.mKey, mSurface);

        if (null != mSurface && mSurface.isValid()) {
            mSurface.release();
            mSurface = null;
        }
        return true;
    }

    public void onSurfaceTextureUpdated(SurfaceTexture surface) {

    }

    public void onRequestExit() {
        WmLog.d("on RequestExit");

        onRenderExit(this.mKey);

        if (null != mDelegateLists) {
            mDelegateLists.clear();
        }
    }

    public String getKey() {
        return mKey;
    }

    private native void onSurfaceCreated(String key, Surface surface);

    private native void onSurfaceChanged(String key, Surface surface, int format, int width, int height, String color);

    private native void onSurfaceDestroyed(String key, Surface surface);

    private native void onRenderExit(String key);


}

