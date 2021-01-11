#pragma once

#include <base/gh.h>
#include "../../zoneCommon/config/ContentZoneCommon.h"
#include <base/list/Array3D.h>
#include <base/list/ArrayList.h>
#include <memory>

class Visual3DModelMaterial;
class EntityTemplate;
class IWorldEntity;
namespace rpg3D {
	class ILogicBlock;
	class EffectPayload;
	class IOGPConditionalEntities;
};

namespace roomgeons_a_v1 {
namespace ArenaRoom {
class ContentZoneArenaRoom : public common::ContentZoneCommon {priv typedef common::ContentZoneCommon super;pub dCtor(ContentZoneArenaRoom);
	pub static std::string NAME;
	pub static std::shared_ptr<worldGame3D::ContentZone> FactoryFunction();
	
	priv const int roomsInSequenceCount = 5;

	priv int zoneInSequenceIndex = 0;

    pub explicit ContentZoneArenaRoom();
	
	pub int getZoneInSequenceIndex();

	prot void onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;
	prot void onZoneDeactivate_pre(base::IGameWorld* gw) override;

    pub ~ContentZoneArenaRoom() override;
};
};
};
