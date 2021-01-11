#pragma once

#include "GPManPlatforms.h"

namespace battlezones_a_v1 {
class GPManZonesChanger : public GPManPlatforms {
private: typedef GPManPlatforms super;
public:
	GPManZonesChanger(GPManZonesChanger const&) = delete;
	GPManZonesChanger(GPManZonesChanger &&) = default;
	GPManZonesChanger& operator=(GPManZonesChanger const&) = delete;
	GPManZonesChanger& operator=(GPManZonesChanger &&) = default;

	explicit GPManZonesChanger(base::IGameWorld* gw);

	~GPManZonesChanger() override;
private:
	void onEvent(IEventListener& eventListener, base::GWEvent& event);

};
};
