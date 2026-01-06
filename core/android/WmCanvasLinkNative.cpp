#include "WmCanvasLinkNative.h"
#include "WmCanvas.hpp"
#include "WmCanvasManager.h"

using namespace std;
using namespace wmcanvas;


__attribute__ ((visibility ("default"))) const char *
WmCanvasLinkNative::CallNative(const char *ContextId, int type, const char *args) {
    if (ContextId == nullptr || args == nullptr || !strcmp(args, "")) {
//        }
//        }
        return nullptr;
    }

    WmCanvasManager *theManager = WmCanvasManager::GetManager();
    string sContextId = ContextId;
    wmcanvas::WmCanvas *theCanvas = static_cast<wmcanvas::WmCanvas *>(theManager->GetCanvas(sContextId));
    string sArgs = args;
    if (theCanvas != nullptr) {
        return theCanvas->CallNative(type, sArgs);
    } else {
        LOG_E(""
              "callnative CAN NOT FIND Canvas, id = %s, cmd=%s", sContextId.c_str(),
              sArgs.c_str());
    }
    return nullptr;
}
