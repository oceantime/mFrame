
#ifndef WM_CANVAS_REF_COUNT_H
#define WM_CANVAS_REF_COUNT_H

namespace wm {
    namespace wm_engine {
        namespace wmcanvas {
            class RefCount {
            public:
                RefCount() : mRefCount(0) {}

                void AddRef() {
                    ++mRefCount;
                }

                void ReleaseRef() {
                    if (--mRefCount <= 0)
                        delete this;
                }

            protected:
                virtual ~RefCount() {}

            protected:
                unsigned int mRefCount;
            };
        }
    }
}

#endif //WM_CANVAS_REF_COUNT_H
