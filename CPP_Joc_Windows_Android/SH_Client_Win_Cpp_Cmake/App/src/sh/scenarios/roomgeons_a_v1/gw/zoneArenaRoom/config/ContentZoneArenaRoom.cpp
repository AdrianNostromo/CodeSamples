#include "ContentZoneArenaRoom.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include <base/statics/StaticsInit.h>
#include "../../../ScenarioFactory.h"
#include "../gw/playLogicHandler/GamePartPlayLogicHandler.h"
#include "../../../gw/config/entity/EntityIds.h"
#include <rpg3D/gw/entity/module/platform/IPlatformModule.h>
#include <rpg3D/gw/manager/IGPManager.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include <rpg3D/gw/entity/template/LivingEntityTemplate.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <base/opengl/util/GLUtil.h>
#include <rpg3D/gw/config/entity/platform/T_Platform_P.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include "util/ZoneStartExtraData.h"
#include <rpg3D/gw/config/entity/platform/T_Platform_X.h>
#include <rpg3D/gw/config/entity/mineral/T_StoneRock_A_V1.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/melleArea/ToolMelleArea.h>
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <rpg3D/gw/zoneFlags/OGPZoneFlags.h>
#include <rpg3D/gw/mobsSpawnSystem/OGPMobsSpawnSystem.h>
#include <rpg3D/gw/conditionalEntities/OGPConditionalEntities.h>
#include <base/memory/SharedPointer.h>
#include <base/container/flags/ManagedFlags.h>
#include "../roomGenerator/config/CfgRoomGenerator.h"
#include "../roomGenerator/RoomGenerator.h"
#include "../roomContent/RoomContent.h"

using namespace roomgeons_a_v1;
using namespace roomgeons_a_v1::ArenaRoom;

std::string ContentZoneArenaRoom::NAME = "ArenaRoom";

std::shared_ptr<worldGame3D::ContentZone> ContentZoneArenaRoom::FactoryFunction() {
	std::shared_ptr<worldGame3D::ContentZone> ret = std::make_shared<ContentZoneArenaRoom>();

	return ret;
}

ContentZoneArenaRoom::ContentZoneArenaRoom()
	: super(NAME)
{
	//void
}

int ContentZoneArenaRoom::getZoneInSequenceIndex() {
	return zoneInSequenceIndex;
}

void ContentZoneArenaRoom::onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivated(zoneStartExtraData, gw);

	ZoneStartExtraData* cZoneStartExtraData = zoneStartExtraData != nullptr ? zoneStartExtraData->getAs<ZoneStartExtraData*>() : nullptr;

	if (cZoneStartExtraData != nullptr) {
		zoneInSequenceIndex = cZoneStartExtraData->zoneInSequenceIndex;
	} else {
		zoneInSequenceIndex = 0;
	}

	GlobalAccessLogHandler::devLogHandler->post_debug("RoomGeons zone activated with room index in sequence: %i", zoneInSequenceIndex);

	// Create the zone specific game parts.
	gw->addGamePartUninitialised(new GamePartPlayLogicHandler(gw));

	gw->addGamePartUninitialised(new rpg3D::OGPZoneFlags(gw));

	rpg3D::IOGPMobsSpawnSystem* gpMobsSpawnSystem = new rpg3D::OGPMobsSpawnSystem(gw);
	gw->addGamePartUninitialised(gpMobsSpawnSystem);

	rpg3D::IOGPConditionalEntities* gpConditionalEntities = new rpg3D::OGPConditionalEntities(gw);
	gw->addGamePartUninitialised(gpConditionalEntities);

	sp<CfgRoomGenerator> cfgRoomGenerator = new CfgRoomGenerator();
	cfgRoomGenerator->tileSizeCm = 60;
	cfgRoomGenerator->roomPrefabsCount.set(3, 3);

	cfgRoomGenerator->isFirstRoomInSequence = zoneInSequenceIndex == 0 ? true : false;
	cfgRoomGenerator->isLastRoomInSequence = zoneInSequenceIndex == roomsInSequenceCount - 1 ? true : false;

	RoomGenerator roomGenerator{ cfgRoomGenerator };
	roomGenerator.generate();

	sp<RoomContent> roomContent = roomGenerator.getRoomContent();
	roomContent->createWorldContent(gw, gpConditionalEntities, gpMobsSpawnSystem);

	// This is used if playerSpawnTargetPlatform is nullptr.
	IWorldEntity* playerSpawnTargetPlatform = nullptr;
	Vector3 playerSpawnPos{};
	bool isPlayerSpawnPosSet = false;

	if (zoneStartExtraData != nullptr && zoneStartExtraData->teleportTargetEntityInstanceID != nullptr) {
		playerSpawnTargetPlatform = gw->getGPMain()->getEntityWithInstanceId(*zoneStartExtraData->teleportTargetEntityInstanceID);
		if (playerSpawnTargetPlatform == nullptr) {
			throw LogicException(LOC);
		}
	} else {
		if (cZoneStartExtraData == nullptr) {
			IWorldEntity* homeTeleporterPlatform = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::zoneEntranceIndicator);
			if (homeTeleporterPlatform == nullptr) {
				// At first spawn, must use the home teleporter.
				throw LogicException(LOC);
			}

			playerSpawnTargetPlatform = homeTeleporterPlatform;
		} else {
			// Use the configured platform.
			IWorldEntity* homeTeleporterPlatform = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::zoneEntranceIndicator);
			if (homeTeleporterPlatform == nullptr) {
				// At first spawn, must use the home teleporter.
				throw LogicException(LOC);
			}
		}
	}
	//playerSpawnTargetPlatform may be nullptr if it is a conditional entity. playerSpawnPos should have a correct value.
	if (!isPlayerSpawnPosSet) {
		if (playerSpawnTargetPlatform == nullptr) {
			throw LogicException(LOC);
		}

		playerSpawnPos.set(playerSpawnTargetPlatform->getPos());
	}

	if (playerSpawnTargetPlatform != nullptr) {
		if (playerSpawnTargetPlatform->hasComponent(rpg3D::IPlatformModule::TYPE)) {
			// Ignore the platform activation for a few frames if the player was spawned on it.
			gw->getManagerAs<rpg3D::IGPManager*>()->addPlayerSpawnIgnoredPlatformsActivation(playerSpawnTargetPlatform->getInstanceId());
		}
	}

	// Give the player full hp. This is also done in case he loses all hp.
	rpg3D::ILivingEntityModule* livingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<rpg3D::ILivingEntityModule*>(true/*mustExist*/);
	// Don't heal the playter.
	// livingEntity->setCurrentHP(livingEntity->getT()->maxHP);

	gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->setPos(playerSpawnPos);

	IMovingEntityModule* movingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	movingEntity->addAction_teleportToDestination(playerSpawnPos);

	Color col{ "#64B0FFFF" };
	GLUtil::gl->setCachedClearColor(col);
}

void ContentZoneArenaRoom::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);

	gw->disposeGamePart<rpg3D::IOGPConditionalEntities*>();
	gw->disposeGamePart<rpg3D::IOGPMobsSpawnSystem*>();
	gw->disposeGamePart<rpg3D::IOGPZoneFlags*>();

	gw->disposeGamePart<IGamePartPlayLogicHandler*>();
}

ContentZoneArenaRoom::~ContentZoneArenaRoom() {
	//void
}
