#pragma once

#include <base/gh.h>
#include <memory>
#include <string>
#include <base/memory/SharedPointer.h>

namespace base {
	class IWrappedValue;
};
namespace worldGame3D {
	class WorldFlag;
	class WrappedWorldFlag;
};

namespace worldGame3D {
namespace gw {
class IFlags {
	pub virtual sp<WrappedWorldFlag> get(std::string& id, bool mustExist) = 0;
	pub virtual sp<WrappedWorldFlag> set(std::string& id, std::shared_ptr<base::IWrappedValue> wValue, bool isZoneOwned) = 0;

	pub virtual void removeAllZoneOwnedFlags() = 0;

    pub virtual ~IFlags() = default;
};
};
};
