#pragma once

#include <string>
#include <memory>
#include <rpg3D/gw/config/entity/EntityIds.h>

namespace gridAdventure_a_v1 {
class EntityIds : public rpg3D::EntityIds {
public:
	static std::shared_ptr<std::string> enterGridBattleA;
	static std::shared_ptr<std::string> enterNextGridBattleLevel;

};
};
