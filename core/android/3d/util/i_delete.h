
#ifndef WM_CANVAS_I_DELETE_H
#define WM_CANVAS_I_DELETE_H
namespace wm {
    namespace wm_engine {
        namespace WmCanvas {
            class IDelete {
            public:
                IDelete() {}

                virtual void Delete() = 0;

            protected:
                virtual ~IDelete() {}

            };
        }
    }
}
#endif //WM_CANVAS_I_DELETE_H
