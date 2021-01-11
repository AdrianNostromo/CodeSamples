#pragma once

#include <base/gh.h>
#include "OGPZF_Base.h"

namespace rpg3D {
class OGPZoneFlags : public OGPZF_Base {priv typedef OGPZF_Base super;pub dCtor(OGPZoneFlags);
    priv base::ManagedFlags* zoneFlags;

    pub explicit OGPZoneFlags(base::IGameWorld* gw);

    pub base::ManagedFlags* getZoneFlags() final;

    prot void disposeMain() override;
    pub ~OGPZoneFlags() override;
};
};
