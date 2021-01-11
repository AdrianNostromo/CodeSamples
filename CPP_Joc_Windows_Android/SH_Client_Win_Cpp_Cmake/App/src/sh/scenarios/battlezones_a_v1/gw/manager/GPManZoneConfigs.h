#pragma once

#include <sh/scenarios/battlezones_a_v1/gw/manager/zone/ZoneConfig.h>
#include "GPManBase.h"
#include <base/map/Map2D.h>

namespace battlezones_a_v1 {
class GPManZoneConfigs : public GPManBase {
private: typedef GPManBase super;
protected:
	static std::string GLOBAL_ZONES_GROUP_ID;
	static std::string ZONE_ID_NEXUS;

	static std::string GENERATED_ZONES_GROUP_ID;

private:
	Map2D<std::string, std::string, std::shared_ptr<ZoneConfig>> zoneConfigsMap{};

public:
    GPManZoneConfigs(GPManZoneConfigs const&) = delete;
    GPManZoneConfigs(GPManZoneConfigs &&) = default;
    GPManZoneConfigs& operator=(GPManZoneConfigs const&) = delete;
    GPManZoneConfigs& operator=(GPManZoneConfigs &&) = default;

    explicit GPManZoneConfigs(base::IGameWorld* gw);

	ZoneConfig* getZoneConfig(std::string& zonesGroupId, std::string& zoneName);
	ZoneConfig* createNewZoneConfig(std::string& zonesGroupId, std::string zoneName);
	void removeZoneConfigsWithGroupId(std::string& zonesGroupId);

    ~GPManZoneConfigs() override;
};
};
