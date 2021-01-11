#include "OGPZoneFlags.h"
#include <base/container/flags/ManagedFlags.h>

using namespace rpg3D;

OGPZoneFlags::OGPZoneFlags(base::IGameWorld* gw)
	: super(gw),
	zoneFlags(new base::ManagedFlags())
{
	//void
}

base::ManagedFlags* OGPZoneFlags::getZoneFlags() {
	return zoneFlags;
}

void OGPZoneFlags::disposeMain() {
	if (zoneFlags != nullptr) {
		delete zoneFlags;
		zoneFlags = nullptr;
	}

	super::disposeMain();
}

OGPZoneFlags::~OGPZoneFlags() {
	//void
}
