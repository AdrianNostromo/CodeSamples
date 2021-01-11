#pragma once

#include <base/gh.h>
#include <string>
#include <memory>
#include <base/memory/SharedPointer.h>

namespace base {
    class IWrappedValue;
};
namespace worldGame3D {
    class WrappedWorldFlag;
};

namespace worldGame3D {
class WorldFlag {pub dCtor(WorldFlag);
    pub std::string id;
    pub std::shared_ptr<base::IWrappedValue> wValue;
    pub bool isZoneOwned;

    pub sp<WrappedWorldFlag> wrapper;

    pub explicit WorldFlag(std::string& id, std::shared_ptr<base::IWrappedValue> wValue, bool isZoneOwned);

    pub ~WorldFlag();
};
};
