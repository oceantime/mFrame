package com.honghu.wmcanvas.demo;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.pm.PackageManager;
import android.graphics.ImageFormat;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCaptureSession;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CaptureRequest;
import android.media.Image;
import android.media.ImageReader;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Size;
import android.webkit.JavascriptInterface;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Toast;

import java.nio.ByteBuffer;
import java.util.Arrays;

/**
 * WebView加载H5页面示例
 * 集成WMCanvas相机功能
 */
public class WebViewActivity extends AppCompatActivity {

    private static final int CAMERA_PERMISSION_REQUEST = 1001;
    
    private WebView mWebView;
    private Handler mCameraHandler;
    private HandlerThread mCameraThread;
    private boolean mCameraRunning = false;
    
    
    private CameraManager mCameraManager;
    private CameraDevice mCameraDevice;
    private CameraCaptureSession mCaptureSession;
    private ImageReader mImageReader;
    private String mCameraId;
    private Size mPreviewSize = new Size(640, 480);

    @SuppressLint("SetJavaScriptEnabled")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_webview);

        mWebView = findViewById(R.id.webview);

        
        WebSettings webSettings = mWebView.getSettings();
        
        
        webSettings.setJavaScriptEnabled(true);
        
        
        webSettings.setSupportZoom(true);
        webSettings.setBuiltInZoomControls(true);
        webSettings.setDisplayZoomControls(false);
        
        
        webSettings.setUseWideViewPort(true);
        webSettings.setLoadWithOverviewMode(true);
        
        
        webSettings.setAllowFileAccess(true);
        webSettings.setAllowFileAccessFromFileURLs(true);
        webSettings.setAllowUniversalAccessFromFileURLs(true);
        
        
        webSettings.setCacheMode(WebSettings.LOAD_DEFAULT);
        webSettings.setDomStorageEnabled(true);
        
        
        webSettings.setRenderPriority(WebSettings.RenderPriority.HIGH);
        
        
        mWebView.setWebViewClient(new WebViewClient() {
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url) {
                view.loadUrl(url);
                return true;
            }

            @Override
            public void onPageFinished(WebView view, String url) {
                super.onPageFinished(view, url);
            }
        });

        
        mWebView.setWebChromeClient(new WebChromeClient() {
            @Override
            public void onProgressChanged(WebView view, int newProgress) {
                super.onProgressChanged(view, newProgress);
            }
        });

        
        // 注入JavaScript接口
        mWebView.addJavascriptInterface(new AndroidBridge(), "AndroidBridge");
        mWebView.addJavascriptInterface(new WMCanvasCameraBridge(), "WMCanvasCamera");

        
        
        mWebView.loadUrl("file:///android_asset/index.html");
    }

    /**
     * Android 通用 JavaScript Bridge
     * 提供通用API给前端调用
     */
    public class AndroidBridge {
        @JavascriptInterface
        public void showToast(final String message) {
            android.util.Log.d("AndroidBridge", "showToast: " + message);
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(WebViewActivity.this, message, Toast.LENGTH_SHORT).show();
                }
            });
        }

        @JavascriptInterface
        public String getDeviceInfo() {
            android.util.Log.d("AndroidBridge", "getDeviceInfo");
            String manufacturer = android.os.Build.MANUFACTURER;
            String model = android.os.Build.MODEL;
            String version = android.os.Build.VERSION.RELEASE;
            return String.format("%s %s (Android %s)", manufacturer, model, version);
        }
    }

    /**
     * WMCanvas Camera JavaScript Bridge
     * 提供相机相关API给前端调用
     */
    public class WMCanvasCameraBridge {
        @JavascriptInterface
        public String start(int width, int height) {
            
            android.util.Log.d("WMCanvasCamera", "Camera start: " + width + "x" + height);
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(WebViewActivity.this, "正在启动相机...", Toast.LENGTH_SHORT).show();
                    startRealCamera();
                }
            });
            
            return "{\"success\":true,\"message\":\"Camera starting\"}";
        }

        @JavascriptInterface
        public String stop() {
            
            android.util.Log.d("WMCanvasCamera", "Camera stop");
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    stopRealCamera();
                }
            });
            
            return "{\"success\":true,\"message\":\"Camera stopped\"}";
        }

        @JavascriptInterface
        public void setFrameEnabled(boolean enabled) {
            
            android.util.Log.d("WMCanvasCamera", "Frame enabled: " + enabled);
        }

        @JavascriptInterface
        public String takePicture() {
            
            android.util.Log.d("WMCanvasCamera", "Take picture");
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(WebViewActivity.this, "📸 拍照!", Toast.LENGTH_SHORT).show();
                }
            });
            
            return "{\"success\":true,\"message\":\"Picture taken\",\"path\":\"/sdcard/picture.jpg\"}";
        }

        @JavascriptInterface
        public String getCapabilities() {
            
            return "{\"camera\":true,\"canvas2d\":true,\"webgl\":false}";
        }
    }

    /**
     * 启动真实相机
     */
    private void startRealCamera() {
        
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.CAMERA}, CAMERA_PERMISSION_REQUEST);
            return;
        }
        
        if (mCameraRunning) {
            return;
        }
        
        
        mCameraThread = new HandlerThread("CameraThread");
        mCameraThread.start();
        mCameraHandler = new Handler(mCameraThread.getLooper());
        
        
        mCameraManager = (CameraManager) getSystemService(CAMERA_SERVICE);
        try {
            
            for (String cameraId : mCameraManager.getCameraIdList()) {
                CameraCharacteristics characteristics = mCameraManager.getCameraCharacteristics(cameraId);
                Integer facing = characteristics.get(CameraCharacteristics.LENS_FACING);
                if (facing != null && facing == CameraCharacteristics.LENS_FACING_BACK) {
                    mCameraId = cameraId;
                    break;
                }
            }
            
            if (mCameraId == null && mCameraManager.getCameraIdList().length > 0) {
                mCameraId = mCameraManager.getCameraIdList()[0];
            }
            
            
            mImageReader = ImageReader.newInstance(
                mPreviewSize.getWidth(),
                mPreviewSize.getHeight(),
                ImageFormat.YUV_420_888,
                2
            );
            
            mImageReader.setOnImageAvailableListener(new ImageReader.OnImageAvailableListener() {
                @Override
                public void onImageAvailable(ImageReader reader) {
                    Image image = null;
                    try {
                        image = reader.acquireLatestImage();
                        if (image != null) {
                            processImage(image);
                        }
                    } finally {
                        if (image != null) {
                            image.close();
                        }
                    }
                }
            }, mCameraHandler);
            
            
            mCameraManager.openCamera(mCameraId, new CameraDevice.StateCallback() {
                @Override
                public void onOpened(@NonNull CameraDevice camera) {
                    mCameraDevice = camera;
                    createCaptureSession();
                    android.util.Log.d("WMCanvasCamera", "Camera opened successfully");
                }
                
                @Override
                public void onDisconnected(@NonNull CameraDevice camera) {
                    camera.close();
                    mCameraDevice = null;
                    android.util.Log.d("WMCanvasCamera", "Camera disconnected");
                }
                
                @Override
                public void onError(@NonNull CameraDevice camera, int error) {
                    camera.close();
                    mCameraDevice = null;
                    android.util.Log.e("WMCanvasCamera", "Camera error: " + error);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(WebViewActivity.this, "相机打开失败", Toast.LENGTH_SHORT).show();
                        }
                    });
                }
            }, mCameraHandler);
            
            mCameraRunning = true;
            
        } catch (CameraAccessException e) {
            android.util.Log.e("WMCanvasCamera", "Camera access error", e);
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(WebViewActivity.this, "无法访问相机", Toast.LENGTH_SHORT).show();
                }
            });
        }
    }
    
    /**
     * 创建相机捕获会话
     */
    private void createCaptureSession() {
        if (mCameraDevice == null || mImageReader == null) {
            return;
        }
        
        try {
            mCameraDevice.createCaptureSession(
                Arrays.asList(mImageReader.getSurface()),
                new CameraCaptureSession.StateCallback() {
                    @Override
                    public void onConfigured(@NonNull CameraCaptureSession session) {
                        mCaptureSession = session;
                        startPreview();
                        android.util.Log.d("WMCanvasCamera", "Capture session configured");
                    }
                    
                    @Override
                    public void onConfigureFailed(@NonNull CameraCaptureSession session) {
                        android.util.Log.e("WMCanvasCamera", "Capture session configuration failed");
                    }
                },
                mCameraHandler
            );
        } catch (CameraAccessException e) {
            android.util.Log.e("WMCanvasCamera", "Create capture session error", e);
        }
    }
    
    /**
     * 开始预�?
     */
    private void startPreview() {
        if (mCameraDevice == null || mCaptureSession == null) {
            return;
        }
        
        try {
            CaptureRequest.Builder builder = mCameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW);
            builder.addTarget(mImageReader.getSurface());
            builder.set(CaptureRequest.CONTROL_MODE, CaptureRequest.CONTROL_MODE_AUTO);
            
            mCaptureSession.setRepeatingRequest(builder.build(), null, mCameraHandler);
            android.util.Log.d("WMCanvasCamera", "Camera preview started");
            
        } catch (CameraAccessException e) {
            android.util.Log.e("WMCanvasCamera", "Start preview error", e);
        }
    }
    
    /**
     * 处理相机图像�?
     */
    private void processImage(Image image) {
        if (!mCameraRunning) {
            return;
        }
        
        try {
            
            ByteBuffer yBuffer = image.getPlanes()[0].getBuffer();
            ByteBuffer uBuffer = image.getPlanes()[1].getBuffer();
            ByteBuffer vBuffer = image.getPlanes()[2].getBuffer();
            
            int ySize = yBuffer.remaining();
            int uSize = uBuffer.remaining();
            int vSize = vBuffer.remaining();
            
            byte[] nv21 = new byte[ySize + uSize + vSize];
            yBuffer.get(nv21, 0, ySize);
            vBuffer.get(nv21, ySize, vSize);
            uBuffer.get(nv21, ySize + vSize, uSize);
            
            
            android.graphics.YuvImage yuvImage = new android.graphics.YuvImage(
                nv21,
                ImageFormat.NV21,
                image.getWidth(),
                image.getHeight(),
                null
            );
            
            java.io.ByteArrayOutputStream out = new java.io.ByteArrayOutputStream();
            yuvImage.compressToJpeg(
                new android.graphics.Rect(0, 0, image.getWidth(), image.getHeight()),
                70,
                out
            );
            
            byte[] imageBytes = out.toByteArray();
            final String base64Image = "data:image/jpeg;base64," + 
                android.util.Base64.encodeToString(imageBytes, android.util.Base64.NO_WRAP);
            
            
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (mWebView != null) {
                        String js = "javascript:if(window.updateCameraFrame){window.updateCameraFrame('" + base64Image + "');}";
                        mWebView.evaluateJavascript(js, null);
                    }
                }
            });
            
        } catch (Exception e) {
            android.util.Log.e("WMCanvasCamera", "Process image error", e);
        }
    }
    
    /**
     * 停止真实相机
     */
    private void stopRealCamera() {
        mCameraRunning = false;
        
        if (mCaptureSession != null) {
            try {
                mCaptureSession.stopRepeating();
                mCaptureSession.close();
            } catch (CameraAccessException e) {
                android.util.Log.e("WMCanvasCamera", "Stop capture session error", e);
            }
            mCaptureSession = null;
        }
        
        if (mCameraDevice != null) {
            mCameraDevice.close();
            mCameraDevice = null;
        }
        
        if (mImageReader != null) {
            mImageReader.close();
            mImageReader = null;
        }
        
        if (mCameraThread != null) {
            mCameraThread.quitSafely();
            try {
                mCameraThread.join();
            } catch (InterruptedException e) {
                android.util.Log.e("WMCanvasCamera", "Camera thread join error", e);
            }
            mCameraThread = null;
        }
        
        android.util.Log.d("WMCanvasCamera", "Camera stopped");
    }
    
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == CAMERA_PERMISSION_REQUEST) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                Toast.makeText(this, "Camera permission granted", Toast.LENGTH_SHORT).show();
                startRealCamera();
            } else {
                Toast.makeText(this, "Camera permission required", Toast.LENGTH_LONG).show();
            }
        }
    }

    @Override
    public void onBackPressed() {
        if (mWebView.canGoBack()) {
            mWebView.goBack();
        } else {
            super.onBackPressed();
        }
    }

    @Override
    protected void onDestroy() {
        stopRealCamera();
        
        if (mWebView != null) {
            mWebView.loadDataWithBaseURL(null, "", "text/html", "utf-8", null);
            mWebView.clearHistory();
            mWebView.destroy();
            mWebView = null;
        }
        super.onDestroy();
    }
}

