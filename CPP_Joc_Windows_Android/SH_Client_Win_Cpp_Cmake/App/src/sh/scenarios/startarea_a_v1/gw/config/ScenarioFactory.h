#pragma once

#include <base/gh.h>
#include <worldGame3D/scenariotemplate/ScenarioTemplate.h>

namespace startarea_a_v1 {
class ScenarioFactory {
	priv class ILocalWorldFactory : virtual public ScenarioTemplate::IWorldFactory {
    public:
        base::IGameWorld* newInstance(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig) override;
    };
	
	pub static ScenarioTemplate* scenarioTemplate;

};
};
