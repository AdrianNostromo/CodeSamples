#pragma once

#include <base/gh.h>
#include <base/component/ComponentsHandler.h>
#include "IViewsHandler.h"
#include <base/component/IComponent_AppComponent.h>
#include <base/list/ArrayList.h>
#include <memory>

class IApp;
class IContainer2D;
class IEventListener;
namespace base {
    namespace snackbar {
        class SnackbarViewsInOrderDisplayHandler;
    };
};

namespace base {
class VH_Base : public ComponentsHandler, public virtual IViewsHandler, public virtual IComponent_AppComponent {priv typedef ComponentsHandler super;pub dCtor(VH_Base);
    prot IApp* app;

    prot ArrayList<std::shared_ptr<IEventListener>> autoListenersList{};

    pub IContainer2D* layer2DGameWorld = nullptr;
    pub IContainer2D* layerSidebar = nullptr;
    pub IContainer2D* layer2D_debug = nullptr;
    pub IContainer2D* layerDialogs = nullptr;
    pub IContainer2D* layerStatusView = nullptr;

    pub IContainer2D* layer2D_splashScreen = nullptr;

    pub base::snackbar::SnackbarViewsInOrderDisplayHandler* snackbarViewsInOrderDisplayHandler = nullptr;

    pub explicit VH_Base(IApp* app);

    pub ~VH_Base() override;
};
};
