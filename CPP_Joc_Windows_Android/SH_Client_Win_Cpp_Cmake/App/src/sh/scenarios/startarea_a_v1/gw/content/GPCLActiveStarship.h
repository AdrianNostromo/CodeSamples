#pragma once

#include "GPCLBase.h"

namespace startarea_a_v1 {
class GPCLActiveStarship : public GPCLBase {
private: typedef GPCLBase super;
private:
	IWorldEntity* activeStarship = nullptr;

public:
    GPCLActiveStarship(GPCLActiveStarship const&) = delete;
    GPCLActiveStarship(GPCLActiveStarship &&) = default;
    GPCLActiveStarship& operator=(GPCLActiveStarship const&) = delete;
    GPCLActiveStarship& operator=(GPCLActiveStarship &&) = default;

    explicit GPCLActiveStarship(base::IGameWorld* gw);

	IWorldEntity* getActiveStarship() override;
    void setActiveStarship(IWorldEntity* activeStarship);

    ~GPCLActiveStarship() override;

protected:
	void createBMain() override;

private:
	void onEvent(IEventListener& eventListener, base::GWEvent& event);

	void onEntityRemoved_Pre(IWorldEntity* entity);

};
};
