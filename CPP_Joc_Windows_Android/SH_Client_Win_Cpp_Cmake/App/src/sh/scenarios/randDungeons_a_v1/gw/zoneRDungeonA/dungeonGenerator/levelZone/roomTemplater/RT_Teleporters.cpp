#include "RT_Teleporters.h"
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/EntityIds.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::RDungeonA;

RT_Teleporters::RT_Teleporters()
	: super()
{
	//void
}

void RT_Teleporters::bakeTeleporters() {
	float gCellSizeM = config->tileSize_cm / 100.0f;

	// Place the teleporters.
	for (int i = 0; i < in_roomElementsPopulatorResult->teleportersList.count(); i++) {
		Result_RoomElementsPopulator::TeleporterInfo& teleporterInfo = in_roomElementsPopulatorResult->teleportersList.getReference(i);

		std::string t_nSpace = "";
		std::string t_id = "";
		std::shared_ptr<std::string> instanceId = nullptr;
		int teleportTargetZoneLevelIndex = -1;
		std::string teleportTargetEntityInstanceID = "";
		if (teleporterInfo.type == Result_RoomElementsPopulator::TeleporterInfo::Type::EntryPointStairs) {
			t_nSpace = config->templateIdentifier_teleporter_exitStairs.nSpace;
			t_id = config->templateIdentifier_teleporter_exitStairs.id;

			instanceId = EntityIds::teleportDungeonEntryPoint;
			teleportTargetZoneLevelIndex = -1;
			teleportTargetEntityInstanceID = "";
		}else if (teleporterInfo.type == Result_RoomElementsPopulator::TeleporterInfo::Type::ExitStairs) {
			t_nSpace = config->templateIdentifier_teleporter_exitStairs.nSpace;
			t_id = config->templateIdentifier_teleporter_exitStairs.id;

			instanceId = EntityIds::teleporterExitDungeon;
			teleportTargetZoneLevelIndex = -1;
			teleportTargetEntityInstanceID = "";
		}else if (teleporterInfo.type == Result_RoomElementsPopulator::TeleporterInfo::Type::StairsDown) {
			t_nSpace = config->templateIdentifier_teleporter_stairsDown.nSpace;
			t_id = config->templateIdentifier_teleporter_stairsDown.id;

			instanceId = std::make_shared<std::string>(EntityIds::teleportPrefix + "down1");
			teleportTargetZoneLevelIndex = zoneIndex + 1;
			teleportTargetEntityInstanceID = EntityIds::teleportPrefix + "up1";
		}else if (teleporterInfo.type == Result_RoomElementsPopulator::TeleporterInfo::Type::StairsUp) {
			t_nSpace = config->templateIdentifier_teleporter_stairsUp.nSpace;
			t_id = config->templateIdentifier_teleporter_stairsUp.id;

			instanceId = std::make_shared<std::string>(EntityIds::teleportPrefix + "up1");
			teleportTargetZoneLevelIndex = zoneIndex - 1;
			teleportTargetEntityInstanceID = EntityIds::teleportPrefix + "down1";
		} else {
			throw LogicException(LOC);
		}
		if (t_nSpace.empty() || t_id.empty()) {
			throw LogicException(LOC);
		}

		Vector3 elementPos{
			(teleporterInfo.roomGPos.x + 0.5f)* gCellSizeM,
			(teleporterInfo.roomGPos.y + 0.5f) * gCellSizeM,
			(0.0f) * gCellSizeM
		};

		result->roomContent->teleportersList.append_emplace(
			elementPos, 0.0f,
			t_nSpace, t_id,
			instanceId,
			teleportTargetZoneLevelIndex, teleportTargetEntityInstanceID
		);
	}
}

RT_Teleporters::~RT_Teleporters() {
	//void
}
