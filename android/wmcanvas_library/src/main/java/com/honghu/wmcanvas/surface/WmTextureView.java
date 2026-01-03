package com.honghu.wmcanvas.surface;

import android.annotation.TargetApi;
import android.content.Context;
import android.os.Build;
import android.util.AttributeSet;
import android.view.TextureView;

import com.honghu.wmcanvas.util.WmLog;

/**
 * @author ertong
 * create at 2017/8/1
 */

public class WmTextureView extends TextureView {
    private WmTextureViewCallback mCallback;

    public WmTextureView(Context context, String id) {
        super(context);
        init(id);
    }

    public WmTextureView(Context context, String id, AttributeSet attrs) {
        super(context, attrs);
        init(id);
    }

    public WmTextureView(Context context, String id, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(id);
    }

    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    public WmTextureView(Context context, String id, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        init(id);
    }

    public void setBackgroundColor(String color) {
        if (mCallback != null) {
            mCallback.setBackgroundColor(color);
        }
    }


    public void addSurfaceTextureListener(TextureView.SurfaceTextureListener listener) {
        if (null == listener) {
            return;
        }

        if (null != mCallback) {
            mCallback.addSurfaceTextureListener(listener);
        }
    }

    public void removeSurfaceTextureListener(TextureView.SurfaceTextureListener listener) {
        if (null == listener) {
            return;
        }

        if(null != mCallback){
            mCallback.removeSurfaceTextureListener(listener);
        }
    }



    private void init(String id) {
        mCallback = new WmTextureViewCallback(this, id);
        this.setSurfaceTextureListener(mCallback);
        this.setOpaque(false);
        this.setLayerType(LAYER_TYPE_HARDWARE, null);
    }

    public void resume() {
    }

    public void pause() {
    }

    @Override
    protected void onWindowVisibilityChanged(int visibility) {
        super.onWindowVisibilityChanged(visibility);
        WmLog.d("on window visibility changed.visibility=" + visibility);
    }

    public void requestExit() {
        WmLog.d("on request Exit in GSurfaceView.");
        if (mCallback != null) {
            WmLog.d("start to request Exit.");
            mCallback.onRequestExit();
        }
    }

    public void sendEvent() {

    }

    public String getCanvasKey() {
        if (null != mCallback) {
            return mCallback.getKey();
        }

        return "";
    }
}

