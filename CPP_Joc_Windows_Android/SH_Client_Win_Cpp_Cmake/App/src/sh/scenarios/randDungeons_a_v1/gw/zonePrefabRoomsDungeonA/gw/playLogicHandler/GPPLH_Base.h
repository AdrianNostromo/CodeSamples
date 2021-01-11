#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include "IGamePartPlayLogicHandler.h"

class EntityRemoveReason;

namespace randDungeons_a_v1 {
namespace PrefabRoomsDungeonA {
	class IMainInGameUIView;

class GPPLH_Base : public GamePart, virtual public IGamePartPlayLogicHandler {priv typedef GamePart super;pub dCtor(GPPLH_Base);
	priv std::shared_ptr< DungeonContent> dungeonContent = nullptr;

	priv std::shared_ptr<std::string> exitZoneName;
	priv std::shared_ptr<std::string> exitZonePlayerPosEntityId;
	
	priv std::shared_ptr<std::string> zeroHpZoneName;
	priv std::shared_ptr<std::string> zeroHpZonePlayerPosEntityId;

	pub explicit GPPLH_Base(base::IGameWorld* gw);

	prot std::shared_ptr< DungeonContent> getDungeonContent();

	prot std::shared_ptr<std::string> getExitZoneName();
	prot std::shared_ptr<std::string> getExitZonePlayerPosEntityId();
	
	prot std::shared_ptr<std::string> getZeroHpZoneName();
	prot std::shared_ptr<std::string> getZeroHpZonePlayerPosEntityId();

	pub void setData(
		std::shared_ptr< DungeonContent> dungeonContent,
		std::shared_ptr<std::string> exitZoneName, std::shared_ptr<std::string> exitZonePlayerPosEntityId,
		std::shared_ptr<std::string> zeroHpZoneName, std::shared_ptr<std::string> zeroHpZonePlayerPosEntityId) final;

    pub ~GPPLH_Base() override;
};
};
};
