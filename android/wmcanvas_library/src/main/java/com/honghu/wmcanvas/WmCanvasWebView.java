package com.honghu.wmcanvas;

import android.view.View;

public abstract class WmCanvasWebView {

    private View mView;

    public WmCanvasWebView(View view) {
        mView = view;
    }

    public View getWebView() {
        return mView;
    }

    public void setWebView(View view) {
        mView = view;
    }

    public abstract String getUserAgentString();

    public abstract void setUserAgentString(String ua);

    public abstract String getOriginalUrl();

    public abstract String getUrl();

}

