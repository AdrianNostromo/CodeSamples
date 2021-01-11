#pragma once

#include "GPManZonesGroupGenerator.h"

namespace battlezones_a_v1 {
class GPManAZBase : public GPManZonesGroupGenerator {
private: typedef GPManZonesGroupGenerator super;
protected:
	ZoneConfig* activeZoneConfig = nullptr;
	base::ManagedFlags activeZoneFlags{};

public:
    GPManAZBase(GPManAZBase const&) = delete;
    GPManAZBase(GPManAZBase &&) = default;
    GPManAZBase& operator=(GPManAZBase const&) = delete;
    GPManAZBase& operator=(GPManAZBase &&) = default;

    explicit GPManAZBase(base::IGameWorld* gw);

    ~GPManAZBase() override;
protected:
	void createMain() override;

	virtual void onActiveZoneDeactivate_pre();
	virtual void onActiveZoneChanged();

	virtual void onZoneFlagsChange();

};
};
