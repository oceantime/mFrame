#include "../../src/WmCanvasManager.h"

namespace wmcanvas {
    void WmCanvasManager::AddtoQueue(const std::string contextId, struct WmCanvasCmd *p) {
        std::map<std::string, std::queue<struct WmCanvasCmd *> *>::iterator it = mCmdQueue.find(
                contextId);
        if (it != mCmdQueue.end()) {
            std::queue<struct WmCanvasCmd *> *queue = it->second;
            queue->push(p);
        } else {
            std::queue<struct WmCanvasCmd *> *queue = new std::queue<struct WmCanvasCmd *>;
            queue->push(p);
            mCmdQueue[contextId] = queue;
        }
    }

    std::queue<struct WmCanvasCmd *> *
    WmCanvasManager::getQueueByContextId(const std::string contextId) {
        std::map<std::string, std::queue<struct WmCanvasCmd *> *>::iterator it = mCmdQueue.find(
                contextId);
        if (it != mCmdQueue.end()) {
            std::queue<struct WmCanvasCmd *> *queue = it->second;
            it->second = 0;
            return queue;
        }

        return nullptr;
    }

    void WmCanvasManager::clearQueue(std::queue<struct WmCanvasCmd *> *queue) {
        if (queue != nullptr) {
            while (!queue->empty()) {
                struct WmCanvasCmd *p = reinterpret_cast<struct WmCanvasCmd *> (queue->front());
                queue->pop();
                delete p;
            }
        }
    }

    void WmCanvasManager::clearQueueByContextId(const std::string contextId) {
        std::queue<struct WmCanvasCmd *> *queue = this->getQueueByContextId(contextId);
        if (queue != nullptr) {
            clearQueue(queue);
        }
    }
}
