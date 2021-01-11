#pragma once

#include <base/gh.h>
#include <memory>
#include <string>
#include <worldGame3D/gw/gamepart/IGamePart.h>

namespace randDungeons_a_v1 {
namespace PrefabRoomsDungeonA {
	class DungeonContent;

class IGamePartPlayLogicHandler : virtual public IGamePart {priv typedef IGamePart super;
	pub static int TYPE;
    DEC_componentBasics();

	pub virtual void setData(
		std::shared_ptr< DungeonContent> dungeonContent,
		std::shared_ptr<std::string> exitZoneName, std::shared_ptr<std::string> exitZonePlayerPosEntityId,
		std::shared_ptr<std::string> zeroHpZoneName, std::shared_ptr<std::string> zeroHpZonePlayerPosEntityId) = 0;

    pub ~IGamePartPlayLogicHandler() override = default;
};
};
};
