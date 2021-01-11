#pragma once

#include "GPManAZConditionalEntities.h"

namespace battlezones_a_v1 {
class GPManAZMobs : public GPManAZConditionalEntities {
private: typedef GPManAZConditionalEntities super;
private:
	// This includes mob projectiles, summons, structures, everything from their team.
	int mobsCount = 0;

public:
    GPManAZMobs(GPManAZMobs const&) = delete;
    GPManAZMobs(GPManAZMobs &&) = default;
    GPManAZMobs& operator=(GPManAZMobs const&) = delete;
    GPManAZMobs& operator=(GPManAZMobs &&) = default;

    explicit GPManAZMobs(base::IGameWorld* gw);

    ~GPManAZMobs() override;
protected:
	void onActiveZoneChanged() override;

private:
	void spawnZoneUnits();

	void onEvent(IEventListener& eventListener, base::GWEvent& event);

};
};
