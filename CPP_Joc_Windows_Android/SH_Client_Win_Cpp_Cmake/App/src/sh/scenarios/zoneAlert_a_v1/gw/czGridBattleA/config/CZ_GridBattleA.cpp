#include "CZ_GridBattleA.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include <base/statics/StaticsInit.h>
#include "../../../gw/config/entity/EntityIds.h"
#include <worldGame3D/gw/contentZones/zone/util/ZoneStartExtraData.h>
#include <rpg3D/gw/entity/template/platform/PlatformTemplate.h>
#include <rpg3D/gw/manager/IGPManager.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include <rpg3D/gw/entity/template/LivingEntityTemplate.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <base/opengl/util/GLUtil.h>
#include "../../czTownA/config/CZ_TownA.h"
#include <rpg3D/gw/config/entity/platform/T_Platform_X.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <worldGame3D/scripts/ScriptCommands.h>
#include <rpg3D/gw/contentZones/zone/contentData/ZoneContentData.h>
#include <worldGame3D/gw/contentZones/IGamePartContentZones.h>
#include <sh/scenarios/zoneAlert_a_v1/gw/czCommon/config/util/ZoneStartExtraData.h>

using namespace zoneAlert_a_v1;
using namespace zoneAlert_a_v1::czGridBattleA;

std::string CZ_GridBattleA::NAME = "GridBattleA";

std::shared_ptr<worldGame3D::ContentZone> CZ_GridBattleA::FactoryFunction() {
	std::shared_ptr<worldGame3D::ContentZone> ret = std::make_shared<CZ_GridBattleA>();

	return ret;
}

CZ_GridBattleA::CZ_GridBattleA()
	: super(NAME, false/*useAutoPlayerPositioning*/, nullptr/*defaultZoneEntrance*/)
{
	//void
}

void CZ_GridBattleA::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	//if (event.type == rpg3D::IGPManager::GWEvent_onPlayerActivatedPlatformNonSpawn->type) {
		//if (event.getEntityData()->getInstanceId() == EntityIds::enterGridBattleA) {
		//	worldGame3D::IGamePartContentZones* gpContentZones = gw->getComponentAs<worldGame3D::IGamePartContentZones*>(true/*mustExist*/);
		//	gpContentZones->setActiveZone(
		//		czGridBattleA::CZ_GridBattleA::NAME, nullptr/*zoneStartExtraData*/, true/*restartZoneIfActive*/,
		//		true/*useDelayedOnLoopActivation*/
		//	);
		//} else {
		//	// Unhandled platform. Script platforms are already processed by the script logic and never get here.
		//	throw LogicException(LOC);
		//}
	//} else {
		throw LogicException(LOC);
	//}
}

sp<rpg3D::ZoneContentData> CZ_GridBattleA::CreateDefaultZoneContentData() {
	sp<rpg3D::ZoneContentData> zoneContentData = rpg3D::ZoneContentData::GetNewBasicZone(
		rpg3D::ZoneContentData::GroundType::GrassDirt_2Layers/*groundType*/, true/*createTallUnderground*/,
		true/*createWallsTiledXY*/,
		false/*createStarterAreaTeleporter*/
	);

	{

		float gridCellSizeM = zoneContentData->tileCellSizeCm / 100.0f;
		Vector3 pos{};
		Quaternion rot{};
		zoneContentData->entitiesInfosList.appendEmplace(
			rpg3D::T_Platform_X::TEMPLATE/*entityTemplate*/,
			*pos.set((0.5f + 4.0f) * gridCellSizeM, (0.5f + 4.0f) * gridCellSizeM, 0.0f)/*posInRoom*/,
			*rot.set(Vector3::Z, 0.0f)/*rot*/,
			EntityIds::defaultZoneEntrance/*instanceId*/,
			nullptr/*materialsList*/, StringUtil::EMPTY/*extraDataId_materialsList*/,
			nullptr/*tilingInformation*/,
			std::make_shared<std::string>(
				std::string("zone set ") + czTownA::CZ_TownA::NAME
				+ " spawnTarget " + *EntityIds::enterGridBattleA + " keepTeleporterOffset"
			)/*platformActivationScript*/
		);
	}

	return zoneContentData;
}

void CZ_GridBattleA::onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivated(zoneStartExtraData, gw);

	sp<rpg3D::ZoneContentData> zoneContentData = loadedZoneData;
	if (zoneContentData == nullptr) {
		zoneContentData = CreateDefaultZoneContentData();
	}

	// Create the zone specific game parts.
	gw->addEventListener(
		rpg3D::IGPManager::GWEvent_onPlayerActivatedPlatformNonSpawn->type,
		std::bind(&CZ_GridBattleA::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		localAutoListenersList
	);

	zoneContentData->createContent(gw);

	Vector3 playerSpawnPos{};
	if (zoneStartExtraData != nullptr && zoneStartExtraData->teleportTargetEntityInstanceID != nullptr) {
		IWorldEntity* spawnLocationEntity = gw->getGPMain()->getEntityWithInstanceId(*zoneStartExtraData->teleportTargetEntityInstanceID);
		if (spawnLocationEntity == nullptr) {
			throw LogicException(LOC);
		}
		playerSpawnPos.set(spawnLocationEntity->getPos());

		if (spawnLocationEntity->hasComponent(rpg3D::IPlatformModule::TYPE)) {
			// Ignore the platform activation for a few frames if the player was spawned on it.
			gw->getManagerAs<rpg3D::IGPManager*>()->addPlayerSpawnIgnoredPlatformsActivation(spawnLocationEntity->getInstanceId());
		}
	} else {
		IWorldEntity* homeTeleporterPlatform = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::defaultZoneEntrance);
		if (homeTeleporterPlatform == nullptr) {
			throw LogicException(LOC);
		}

		playerSpawnPos.set(homeTeleporterPlatform->getPos());

		if (homeTeleporterPlatform->hasComponent(rpg3D::IPlatformModule::TYPE)) {
			// Ignore the platform activation for a few frames if the player was spawned on it.
			gw->getManagerAs<rpg3D::IGPManager*>()->addPlayerSpawnIgnoredPlatformsActivation(homeTeleporterPlatform->getInstanceId());
		}
	}

	if (zoneStartExtraData != nullptr && zoneStartExtraData->teleportTargetOffset != nullptr) {
		playerSpawnPos.add(*zoneStartExtraData->teleportTargetOffset);
	}

	// Give the player full hp. This is also done in case he loses all hp.
	rpg3D::ILivingEntityModule* livingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<rpg3D::ILivingEntityModule*>(true/*mustExist*/);
	livingEntity->setCurrentHP(livingEntity->getT()->maxHP);

	gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->setPos(playerSpawnPos);

	IMovingEntityModule* movingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	movingEntity->addAction_teleportToDestination(playerSpawnPos);

	Color col{ "#64B0FFFF" };
	GLUtil::gl->setCachedClearColor(col);
}

void CZ_GridBattleA::createGroundPatch(
	float x, float y, float z,
	Array3D<signed char>* voxelsGrid,
	ArrayList<Visual3DModelMaterial*>* materialsList,
	base::IGameWorld* gw)
{
	Vector3 pos{ x, y, z };
	Quaternion rot{ Vector3::Z, 0.0f };
	IWorldEntity* entity = gw->getGPMain()->createEntity(
		rpg3D::EntityTemplatesList::Ground_BasicPatchA_1m,
		pos,
		&rot/*rot*/, nullptr/*cardinalRotation*/,
		nullptr,
		0,
		nullptr/*extraData*/,
		true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		nullptr
	);

	IVoxelisedStructureModule* voxelisedStructure = entity->getComponentAs<IVoxelisedStructureModule*>(true/*mustExist*/);
	voxelisedStructure->setStructure(
		voxelsGrid,
		materialsList
	);
}

IWorldEntity* CZ_GridBattleA::createConfigEntity(
	float x, float y, float z,
	float rotZ,
	EntityTemplate* entityTemplate,
	std::shared_ptr<std::string> instanceId, std::shared_ptr<std::string> platformActivationScript,
	base::IGameWorld* gw)
{
	Vector3 pos{ x, y, z };
	Quaternion rot{ Vector3::Z, rotZ };
	IWorldEntity* entity = gw->getGPMain()->createEntity(
		entityTemplate,
		pos,
		&rot/*rot*/, nullptr/*cardinalRotation*/,
		instanceId,
		0,
		nullptr,
		true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		[platformActivationScript](IWorldEntity* entity) -> void {
			if (platformActivationScript != nullptr) {
				entity->getComponentAs<rpg3D::IPlatformModule*>(true/*mustExist*/)->setActivation_script(platformActivationScript);
			}
		}
	);
	
	return entity;
}

void CZ_GridBattleA::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);

	//void
}

CZ_GridBattleA::~CZ_GridBattleA() {
	//void
}
