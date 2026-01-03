#ifndef WmCanvas_WmCanvasLINKNATIVE_H
#define WmCanvas_WmCanvasLINKNATIVE_H


namespace WmCanvas {
    class WmCanvasLinkNative {
    public:
        static const char* CallNative(const char* ContextId,int type,const char* args);
    };
}

#endif //WmCanvas_WmCanvasLINKNATIVE_H
