#include <base/log/GlobalAccessLogHandler.h>
#include "GPManZoneConfigs.h"

using namespace battlezones_a_v1;

std::string GPManZoneConfigs::GLOBAL_ZONES_GROUP_ID = "GLOBAL_ZONES_GROUP_ID";
std::string GPManZoneConfigs::ZONE_ID_NEXUS = "ZONE_ID_NEXUS";

std::string GPManZoneConfigs::GENERATED_ZONES_GROUP_ID = "GENERATED_ZONES_GROUP_ID";

GPManZoneConfigs::GPManZoneConfigs(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

ZoneConfig* GPManZoneConfigs::getZoneConfig(std::string& zonesGroupId, std::string& zoneName) {
	std::shared_ptr<ZoneConfig> entry = zoneConfigsMap.getDirect(zonesGroupId, zoneName, nullptr);
	if(entry != nullptr) {
		return entry.get();
	}

	return nullptr;
}

ZoneConfig* GPManZoneConfigs::createNewZoneConfig(std::string& zonesGroupId, std::string zoneName) {
	GlobalAccessLogHandler::devLogHandler->postDebug(std::string("GW zone_config. Config created: ") + zonesGroupId + ", " + zoneName + ".");

	if(getZoneConfig(zonesGroupId, zoneName) != nullptr) {
		throw LogicException(LOC);
	}

	std::shared_ptr<ZoneConfig> entry = std::make_shared<ZoneConfig>(zonesGroupId, zoneName);

	return zoneConfigsMap.putReference(zonesGroupId, zoneName, entry)->get();
}

void GPManZoneConfigs::removeZoneConfigsWithGroupId(std::string& zonesGroupId) {
	GlobalAccessLogHandler::devLogHandler->postDebug(std::string("GW zone_config. Group cleared: ") + zonesGroupId + ".");

	return zoneConfigsMap.clear(zonesGroupId);
}

GPManZoneConfigs::~GPManZoneConfigs() {
	//void
}
