#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>
#include "../../dungeonContent/DungeonContent.h"

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
class IGamePartExplorableRooms : virtual public IGamePart {priv typedef IGamePart super;
	pub static int TYPE;
    DEC_componentBasics();

	pub virtual void initLevelZoneRooms(std::shared_ptr<DungeonContent::LevelZone> levelZoneConfig) = 0;
	
	pub virtual void preActivateRoomIfInnactive(int roomIndex) = 0;

    pub ~IGamePartExplorableRooms() override = default;
};
};
};
