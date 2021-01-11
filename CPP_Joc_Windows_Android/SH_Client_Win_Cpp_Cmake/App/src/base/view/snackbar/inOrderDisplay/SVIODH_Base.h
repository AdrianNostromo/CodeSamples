#pragma once

#include <base/gh.h>
#include <base/component/ComponentsHandler.h>
#include "ISnackbarViewsInOrderDisplayHandler.h"
#include <base/component/IComponent_AppComponent.h>

class IApp;

namespace base {
namespace snackbar {
class SVIODH_Base : public base::ManagedObject, public virtual ISnackbarViewsInOrderDisplayHandler, public virtual IComponent_AppComponent {priv typedef base::ManagedObject super;pub dCtor(SVIODH_Base);
    prot IApp* app;

    pub explicit SVIODH_Base(IApp* app);

    pub ~SVIODH_Base() override;
};
};
};
