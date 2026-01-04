#ifndef WMCANVAS_WMCANVAS2DCONTEXTANDROID_H
#define WMCANVAS_WMCANVAS2DCONTEXTANDROID_H


#include "WmCanvas2dContext.h"
#include "WmFont.h"
#include "WmSystemFontInformation.h"
#include "WmShaderManager.h"

#include <cstdint>




class WmCanvas2DContextAndroid : public WmCanvasContext {


public:


    API_EXPORT  WmCanvas2DContextAndroid(uint32_t w, uint32_t h, WmCanvasConfig &config);


    API_EXPORT WmCanvas2DContextAndroid(uint32_t width, uint32_t h, WmCanvasConfig &config, WmCanvasHooks* hooks);


    virtual ~WmCanvas2DContextAndroid();


    void InitFBO() override;


    API_EXPORT void SetUseShaderBinaryCache(bool v);


    API_EXPORT void SetShaderBinaryCachePath(const std::string& path);


    API_EXPORT void ClearColorToTransparent();


    API_EXPORT void ClearColor(WmColorRGBA& c);


    API_EXPORT void GetRawImageData(int width, int height, uint8_t *pixels);


    API_EXPORT void BeginDraw(bool is_first_draw = false);


    API_EXPORT void EndDraw();


    API_EXPORT WmTexture *GetFBOTextureData();


    API_EXPORT void CopyFBO(WmFrameBufferObject &srcFbo, WmFrameBufferObject &destFbo);


    API_EXPORT void CopyImageToCanvas(int width, int height, const unsigned char *rgbaData, int imgWidth,
                      int imgHeight);

//    API_EXPORT int BindImage(const unsigned char *rgbaData, GLint format, unsigned int width,
//                             unsigned int height);


    API_EXPORT void DrawFrame(bool clear);


    API_EXPORT void ResizeCanvas(int width, int height);


    API_EXPORT void ResizeCopyUseFbo(int width, int height);


    API_EXPORT  void ResizeCopyUseImage(int width, int height,
                                        const unsigned char *rgbaData, int imgWidth, int imgHeight);

    void SetEnableFboMsaa(bool v) { mEnableFboMsaa = v; }


    API_EXPORT void SetShaderManager(WmShaderManager* shaderManager);


    WmShaderManager *GetShaderManager() override;

protected:

    void Create();


    void ResetGLBeforeCopyFrame(int dest_fbo_w, int dest_fbo_h);


    void RestoreGLAfterCopyFrame();


    void DrawFBO(std::string fboName, WmCompositeOperation compositeOp = COMPOSITE_OP_SOURCE_OVER,
                 float sx = 0, float sy = 0, float sw = 1, float sh = 1, float dx = 0, float dy = 0,
                 float dw = 1, float dh = 1);


private:

    bool mEnableFboMsaa = false;

    WmShaderManager *mShaderManager = nullptr;

};


#endif //WMCANVAS_WMCANVAS2DCONTEXTANDROID_H
