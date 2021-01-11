#pragma once

#include <string>
#include <memory>
#include <rpg3D/gw/config/entity/EntityIds.h>

namespace towerDefense_a_v1 {
class EntityIds : public rpg3D::EntityIds {
public:
    static std::shared_ptr<std::string> mobsSpawner;
    static std::shared_ptr<std::string> mobsDestination;
    static std::shared_ptr<std::string> gridMob;
    
	static std::shared_ptr<std::string> turretSpawner_1;
	static std::shared_ptr<std::string> turretSpawner_2;

};
};
