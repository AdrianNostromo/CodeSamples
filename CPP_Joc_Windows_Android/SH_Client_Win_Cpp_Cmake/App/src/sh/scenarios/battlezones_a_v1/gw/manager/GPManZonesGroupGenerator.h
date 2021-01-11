#pragma once

#include <sh/scenarios/battlezones_a_v1/gw/zone/ZoneTemplate.h>
#include "GPManZoneConfigs.h"

namespace battlezones_a_v1 {
class GPManZonesGroupGenerator : public GPManZoneConfigs {
private: typedef GPManZoneConfigs super;
protected:
	static base::GWEvent* GWEvent_onZonesGroupFinished;

public:
    GPManZonesGroupGenerator(GPManZonesGroupGenerator const&) = delete;
    GPManZonesGroupGenerator(GPManZonesGroupGenerator &&) = default;
    GPManZonesGroupGenerator& operator=(GPManZonesGroupGenerator const&) = delete;
    GPManZonesGroupGenerator& operator=(GPManZonesGroupGenerator &&) = default;

    explicit GPManZonesGroupGenerator(base::IGameWorld* gw);

    ~GPManZonesGroupGenerator() override;
protected:
	void generateNewZonesGroup();

private:
	ZoneConfig* buildZoneConfig(
		std::string& zonesGroupId, int zoneIndex,
		ZoneTemplate* zoneTemplate,
		bool isPreviousZoneNexus, bool isNextZoneNexus);

};
};
