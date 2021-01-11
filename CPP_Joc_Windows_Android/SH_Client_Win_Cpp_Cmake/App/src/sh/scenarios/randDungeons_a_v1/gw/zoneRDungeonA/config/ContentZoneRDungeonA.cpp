#include "ContentZoneRDungeonA.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include <base/statics/StaticsInit.h>
#include "../gw/playLogicHandler/GamePartPlayLogicHandler.h"
#include "../gw/explorableRooms/GamePartExplorableRooms.h"
#include "util/ZoneStartExtraData.h"
#include <base/exceptions/LogicException.h>
#include <rpg3D/gw/entity/module/platform/IPlatformModule.h>
#include <rpg3D/gw/manager/IGPManager.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/module/projectile/ProjectileModule.h>
#include <rpg3D/gw/config/team/TeamsIndexesList.h>
#include <rpg3D/gw/projectile/EffectPayload.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <base/opengl/util/GLUtil.h>
#include <base/app/IApp.h>
#include <base/view/debug/IDebugView.h>
#include <base/view/debug/entryUser/IDebugEntryUser.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/viewsHandler/IViewsHandler.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::RDungeonA;

std::string ContentZoneRDungeonA::NAME = "RDungeonA";

bool ContentZoneRDungeonA::IS_DEBUG_VIEW_LOG = true;

std::shared_ptr<worldGame3D::ContentZone> ContentZoneRDungeonA::FactoryFunction() {
	std::shared_ptr<worldGame3D::ContentZone> ret = std::make_shared<ContentZoneRDungeonA>();

	return ret;
}

ContentZoneRDungeonA::ContentZoneRDungeonA()
	: super(NAME)
{
	//void
}

void ContentZoneRDungeonA::onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivated(zoneStartExtraData, gw);

	if (zoneStartExtraData == nullptr) {
		throw LogicException(LOC);
	}
	ZoneStartExtraData* cZoneStartExtraData = zoneStartExtraData->getAs<ZoneStartExtraData*>();

	if (IS_DEBUG_VIEW_LOG) {
		dynamicDebugEntry = gw->getApp()->getComponentAs<base::IViewsHandler*>(true/*mustExist*/)->getDebugView()->addDebugEntry();
		dynamicDebugEntry->setPlaceholderedText("SH.RD. Dungeon levelIndex #0#, seed #1#");
		
		dynamicDebugEntry->appendTrackedDataValue(dynamicDebugEntry_levelIndexI = std::make_shared<base::WrappedValue<int>>(cZoneStartExtraData->levelZoneToActivate, base::IWrappedValue::Type::T_int));
		dynamicDebugEntry->appendTrackedDataValue(dynamicDebugEntry_seedI = std::make_shared<base::WrappedValue<int>>(cZoneStartExtraData->dungeonContent->seed, base::IWrappedValue::Type::T_int));
	}
	
	// Create the zone specific game parts.
	IGamePartPlayLogicHandler* gpPlayLogicHandler = new GamePartPlayLogicHandler(gw);
	gw->addGamePartUninitialised(gpPlayLogicHandler);

	IGamePartExplorableRooms* gpExplorableRooms = new GamePartExplorableRooms(gw);
	gw->addGamePartUninitialised(gpExplorableRooms);
	
	gpPlayLogicHandler->setData(
		cZoneStartExtraData->dungeonContent,
		cZoneStartExtraData->exitZoneName, cZoneStartExtraData->exitZonePlayerPosEntityId,
		cZoneStartExtraData->zeroHpZoneName, cZoneStartExtraData->zeroHpZonePlayerPosEntityId
	);

	gpExplorableRooms->initLevelZoneRooms(cZoneStartExtraData->dungeonContent->levelZonesList.getDirect(cZoneStartExtraData->levelZoneToActivate));
	gpExplorableRooms->preActivateRoomIfInnactive(0);

	Vector3 playerSpawnPos{};
	if (zoneStartExtraData->teleportTargetEntityInstanceID == nullptr) {
		// The dungeon must have a default in spawn point configured.
		throw LogicException(LOC);
	}
	
	IWorldEntity* spawnLocationEntity = gw->getGPMain()->getEntityWithInstanceId(*zoneStartExtraData->teleportTargetEntityInstanceID);
	if (spawnLocationEntity == nullptr) {
		throw LogicException(LOC);
	}
	playerSpawnPos.set(spawnLocationEntity->getPos());
	if (spawnLocationEntity->hasComponent(rpg3D::IPlatformModule::TYPE)) {
		// Ignore the platform activation for a few frames if the player was spawned on it.
		gw->getManagerAs<rpg3D::IGPManager*>()->addPlayerSpawnIgnoredPlatformsActivation(spawnLocationEntity->getInstanceId());
	}

	gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->setPos(playerSpawnPos);

	IMovingEntityModule* movingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	movingEntity->addAction_teleportToDestination(playerSpawnPos);

	Color col{ "#1a1a1aFF" };
	GLUtil::gl->setCachedClearColor(col);
}

void ContentZoneRDungeonA::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);

	gw->disposeGamePart<IGamePartPlayLogicHandler*>();
	gw->disposeGamePart<IGamePartExplorableRooms*>();

	if (dynamicDebugEntry != nullptr) {
		dynamicDebugEntry->remove();
		dynamicDebugEntry = nullptr;
	}
}

ContentZoneRDungeonA::~ContentZoneRDungeonA() {
	//void
}
