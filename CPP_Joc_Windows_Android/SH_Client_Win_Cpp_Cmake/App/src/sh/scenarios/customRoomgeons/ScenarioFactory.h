#pragma once

#include <base/gh.h>
#include <worldGame3D/scenariotemplate/ScenarioTemplate.h>

namespace customRoomgeons {
class ScenarioFactory {
	priv class GameWorldFactory : virtual public ScenarioTemplate::IWorldFactory {
		pub base::IGameWorld* newInstance(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig) override;
    };
	
	pub static ScenarioTemplate* scenarioTemplate;

};
};
