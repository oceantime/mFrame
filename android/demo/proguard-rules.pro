# Add project specific ProGuard rules here.
-keepattributes *Annotation*
-keepattributes JavascriptInterface

# Keep JavaScript interface
-keepclassmembers class com.taobao.gcanvas.demo.WebViewActivity$JsInterface {
    public *;
}

# Keep native methods
-keepclasseswithmembernames class * {
    native <methods>;
}

# WebView
-keepclassmembers class * extends android.webkit.WebViewClient {
    public void *(android.webkit.WebView, java.lang.String, android.graphics.Bitmap);
    public boolean *(android.webkit.WebView, java.lang.String);
}
