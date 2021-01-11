#pragma once

#include <sh/scenarios/battlezones_a_v1/gw/content/GamePartContent.h>
#include <sh/scenarios/battlezones_a_v1/gw/online/GPOnline.h>
#include <sh/scenarios/battlezones_a_v1/gw/manager/GPManager.h>
#include <sh/scenarios/battlezones_a_v1/gw/sim/GamePartSim.h>
#include <rpg3D/gw/GameWorldExtendable.h>

namespace battlezones_a_v1 {
class GWLBase : public rpg3D::GameWorldExtendable {
private: typedef rpg3D::GameWorldExtendable super;
public:
	GWLBase(GWLBase const&) = delete;
	GWLBase(GWLBase &&) = default;
	GWLBase& operator=(GWLBase const&) = delete;
	GWLBase& operator=(GWLBase &&) = default;

	explicit GWLBase(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig);

	~GWLBase() override;
protected:
    void disposeMain() override;

};
};
