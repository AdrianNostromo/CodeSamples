#pragma once

#include <base/gh.h>
#include <rpg3D/gw/GameWorldExtendable.h>

namespace customRoomgeons {
class GameWorld : public rpg3D::GameWorldExtendable {priv typedef rpg3D::GameWorldExtendable super;pub dCtor(GameWorld);
	pub explicit GameWorld(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig);
	prot void createGParts() override;

	pub void startLevelPlay() override;
	
	pub ~GameWorld() override;
};
};
