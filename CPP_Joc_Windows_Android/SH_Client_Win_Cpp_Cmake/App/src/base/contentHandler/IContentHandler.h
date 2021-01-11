#pragma once

#include <base/gh.h>
#include <base/component/IComponent_AppComponent.h>

namespace base {
class IContentHandler : virtual public IComponent_AppComponent {priv typedef IComponent_AppComponent super;
    // void

    pub ~IContentHandler() override = default;
};
};
