#pragma once

#include "GPMan_PlayerUnitDeathHandling.h"

namespace battlezones_a_v1 {
class GPManager : public GPMan_PlayerUnitDeathHandling {
private: typedef GPMan_PlayerUnitDeathHandling super;
public:
    GPManager(GPManager const&) = delete;
    GPManager(GPManager &&) = default;
    GPManager& operator=(GPManager const&) = delete;
    GPManager& operator=(GPManager &&) = default;

    explicit GPManager(base::IGameWorld* gw);

    ~GPManager() override;
protected:
	void createBMain() override;

private:
	void onEvent(IEventListener& eventListener, base::GWEvent& event);

	void onPlayerEntityChanged();

};
};
