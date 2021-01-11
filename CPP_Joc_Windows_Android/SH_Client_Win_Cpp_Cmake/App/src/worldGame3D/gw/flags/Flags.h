#pragma once

#include <base/gh.h>
#include "IFlags.h"
#include <base/list/ListDL.h>
#include <base/map/Map1D.h>

namespace worldGame3D {
    class WorldFlag;
};

namespace worldGame3D {
namespace gw {
class Flags : public virtual IFlags {pub dCtor(Flags);
    priv ListDL<WorldFlag> flagsList{};
    priv Map1D<std::string, WorldFlag*> flagsMap{};

    pub explicit Flags();

    pub sp<WrappedWorldFlag> get(std::string& id, bool mustExist) final;
    pub sp<WrappedWorldFlag> set(std::string& id, std::shared_ptr<base::IWrappedValue> wValue, bool isZoneOwned) final;

    pub void removeAllZoneOwnedFlags() final;

    pub ~Flags() override;
};
};
};
