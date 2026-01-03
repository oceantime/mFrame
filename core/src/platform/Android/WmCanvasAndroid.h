#ifndef WMCANVAS_WMCANVASANDROID_H
#define WMCANVAS_WMCANVASANDROID_H


#include "WmCanvas.hpp"
#include "WmCanvas2DContextAndroid.h"


class WmCanvasAndroid : public wmcanvas::WmCanvas {


public:


    API_EXPORT WmCanvasAndroid(std::string contextId, const WmCanvasConfig& config, WmCanvasHooks *hooks = nullptr);


    void CreateContext() override;


    API_EXPORT WmCanvas2DContextAndroid* GetCanvasContextAndroid();


};


#endif //WMCANVAS_WMCANVASANDROID_H
