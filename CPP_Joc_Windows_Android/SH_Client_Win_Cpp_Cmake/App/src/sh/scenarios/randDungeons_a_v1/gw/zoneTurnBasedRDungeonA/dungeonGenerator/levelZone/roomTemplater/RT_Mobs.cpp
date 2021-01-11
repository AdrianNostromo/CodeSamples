#include "RT_Mobs.h"
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <rpg3D/gw/projectile/EffectPayload.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::TurnBasedRDungeonA;

RT_Mobs::RT_Mobs()
	: super()
{
	//void
}

void RT_Mobs::bakeMobs() {
	float gCellSizeM = config->tileSize_cm / 100.0f;
	
	// Place a test mob in each room except from the start one.
	for (int i = 0; i < in_roomElementsPopulatorResult->mobsList.count(); i++) {
		Result_RoomElementsPopulator::MobInfo& mobInfo = in_roomElementsPopulatorResult->mobsList.getReference(i);

		Vector3 elementPos{
			(mobInfo.roomGPos.x + 0.5f) * gCellSizeM,
			(mobInfo.roomGPos.y + 0.5f) * gCellSizeM,
			(0.0f) * gCellSizeM
		};

		std::string t_nSpace = config->templateIdentifier_mob.nSpace;
		std::string t_id = config->templateIdentifier_mob.id;

		DungeonContent::LevelZone::Room::Mob* mob = result->roomContent->mobsList.append_emplace(
			elementPos, 0.0f,
			t_nSpace, t_id
		);

		mob->impactPayloadsList.appendDirect(std::make_shared<rpg3D::EffectPayload>(
			rpg3D::EffectPayload::Types::physicalDamage, rpg3D::EffectPayload::Target::OTHER_ENTITY, 1.0f
		));
	}
}

RT_Mobs::~RT_Mobs() {
	//void
}
