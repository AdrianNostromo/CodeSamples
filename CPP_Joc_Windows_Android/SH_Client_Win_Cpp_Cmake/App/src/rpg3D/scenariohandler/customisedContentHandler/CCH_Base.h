#pragma once

#include <base/gh.h>
#include <base/component/ComponentsHandler.h>
#include "ICustomisedContentHandler.h"
#include <base/component/IComponent_ScenariosHandlerComponent.h>

class IApp;
namespace rpg3D {
    class IGamePartCustomisedContent;
};

namespace rpg3D {
class CCH_Base : public base::ManagedObject, virtual public ICustomisedContentHandler, public virtual IComponent_ScenariosHandlerComponent {priv typedef base::ManagedObject super;pub dCtor(CCH_Base);
    prot static std::string CUSTOMISED_CONTENT_CACHE_GROUP;

    prot IApp* app;

    priv base::IGameWorld* activeGameWorld = nullptr;
    priv rpg3D::IGamePartCustomisedContent* activeWorld_gpCustomisedContent = nullptr;

    pub explicit CCH_Base(IApp* app);

    prot base::IGameWorld* getActiveGameWorld();
    pub void setActiveGameWorld(base::IGameWorld* activeGameWorld) final;

    pub ~CCH_Base() override;
};
};
