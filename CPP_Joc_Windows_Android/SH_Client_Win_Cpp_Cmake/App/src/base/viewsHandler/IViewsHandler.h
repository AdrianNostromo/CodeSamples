#pragma once

#include <base/gh.h>
#include <base/component/IComponent_AppComponent.h>
#include <base/sp.h>

class IContainer2D;
namespace base {
    class SplashScreenView;
    class IDebugView;
    class IListMenuCardPopupView;
    class IColorSelectorView;
    class IDialogView;
    namespace snackbar {
        class WrappedSnackbarView;
    };
};

namespace base {
class IViewsHandler : public virtual IComponent_AppComponent {priv typedef IComponent_AppComponent super;
    pub static int TYPE;
    DEC_componentBasics();

    pub virtual base::IDebugView* getDebugView() = 0;

    pub virtual base::IListMenuCardPopupView* getNewListMenuCardPopupViewAutoDisposable(IContainer2D* parentHolderOverride) = 0;

    pub virtual base::IColorSelectorView* getNewColorSelectorViewAutoDisposable() = 0;

    pub virtual base::IDialogView* getNewDialogViewAutoDisposable() = 0;

    //Use a wrapper because the dialog auto closes it-s self.
    pub virtual sp<base::snackbar::WrappedSnackbarView> getNewSnackbarViewAutoDisposable() = 0;

    pub ~IViewsHandler() override = default;
};
};
