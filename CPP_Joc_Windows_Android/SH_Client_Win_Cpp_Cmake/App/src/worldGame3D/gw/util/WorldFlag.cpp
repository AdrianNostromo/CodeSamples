#include "WorldFlag.h"
#include "WrappedWorldFlag.h"

using namespace worldGame3D;

WorldFlag::WorldFlag(std::string& id, std::shared_ptr<base::IWrappedValue> wValue, bool isZoneOwned)
	: id(id), wValue(wValue), isZoneOwned(isZoneOwned)
{
	wrapper = new WrappedWorldFlag(this);
}

WorldFlag::~WorldFlag() {
	wrapper = nullptr;
}
