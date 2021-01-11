#pragma once

#include <string>
#include <memory>
#include <rpg3D/gw/config/entity/EntityIds.h>

namespace randDungeons_a_v1 {
class EntityIds : public rpg3D::EntityIds {
public:
	static std::shared_ptr<std::string> enterRandomDungeonA;
	static std::shared_ptr<std::string> enterTurnBasedRandomDungeonA;

	static std::shared_ptr<std::string> enterPrefabRoomsDungeonA;
	
	static std::shared_ptr<std::string> teleportDungeonEntryPoint;
	static std::shared_ptr<std::string> teleporterExitDungeon;
	static std::string teleportPrefix;

};
};
