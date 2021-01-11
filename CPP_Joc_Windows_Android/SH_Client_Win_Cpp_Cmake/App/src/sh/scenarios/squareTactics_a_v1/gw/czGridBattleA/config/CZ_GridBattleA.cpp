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
#include <rpg3D/gw/config/entity/indicator/T_GridOriginIndicator60Cm_A_V1.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <worldGame3D/scripts/ScriptCommands.h>
#include <rpg3D/gw/contentZones/zone/contentData/ZoneContentData.h>
#include <worldGame3D/gw/contentZones/IGamePartContentZones.h>
#include <sh/scenarios/squareTactics_a_v1/gw/czCommon/config/util/ZoneStartExtraData.h>
#include <sh/scenarios/squareTactics_a_v1/gw/partyCandidates/IGPPartyCandidates.h>
#include "util/ZoneStartExtraData.h"

#include <rpg3D/gw/playPhases/GCPlayPhases.h>
#include <rpg3D/gw/playPhases/util/TurnPhaseConfig.h>
#include <rpg3D/gw/config/team/TeamsIndexesList.h>
#include <rpg3D/gw/grid2DContent/GPGrid2DContent.h>
#include <rpg3D/gw/userEntitiesController/IGPUserEntitiesController.h>
#include <rpg3D/gw/userEntitiesController/util/EntityControlType.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/capabilities/ICapabilities.h>
#include <worldGame3D/gw/sim/IGPSim.h>
#include <worldGame3D/gw/util/GameCameraModeBitGroups.h>
#include <worldGame3D/gw/entityGroups/util/IEntsGroup.h>

using namespace squareTactics_a_v1;
using namespace squareTactics_a_v1::czGridBattleA;

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
			rpg3D::T_GridOriginIndicator60Cm_A_V1::TEMPLATE/*entityTemplate*/,
			*pos.set((0.5f + 4.0f) * gridCellSizeM, (0.5f + 4.0f) * gridCellSizeM, 0.0f)/*posInRoom*/,
			*rot.set(Vector3::Z, 0.0f)/*rot*/,
			EntityIds::gridOriginIndicator/*instanceId*/,
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

void CZ_GridBattleA::onZoneActivatedPre(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivatedPre(zoneStartExtraData, gw);

	cachedPrevousStackableGWCapability = gw->getCapabilities().getWithBitIndex(worldGame3D::gw::ICapabilities::STACKABLES_HAULER_BIT_INDEX);
	if (cachedPrevousStackableGWCapability) {
		gw->getCapabilities().setWithBitIndex(worldGame3D::gw::ICapabilities::STACKABLES_HAULER_BIT_INDEX, false);
	}

	gw->addGamePartUninitialised(gpGrid2DContent = new rpg3D::GPGrid2DContent(
		gw, Vector2(0.6f, 0.6f),
		true/*allowMultipleEntitiesPerGCell*/, true/*allowEntitiesOutsideOfTheGridArea*/)
	);
	
	//asd_011;// grab the value from a zone config flag, not a entity.
	Vector2 gridOriginOffset{
		3.0f,
		3.0f
	};
	gpGrid2DContent->setGridOriginOffset(gridOriginOffset);

	sp<Array2D<unsigned char>> gridTrackingCells = new Array2D<unsigned char>{
		3/*lengthA*/, 3/*lengthB*/, false/*invertA*/, true/*invertB*/, new unsigned char[3 * 3] {
			1,1,0,
			1,1,1,
			1,1,1
	}};
	gpGrid2DContent->initTrackingRegion_cellsGrid(gridTrackingCells);

	{
		asd;// need some custom local code for tese, maybe use a object.
		sp<Array1D<rpg3D::playPhases::TurnPhaseConfig>> playPhaseConfigsList = new Array1D<rpg3D::playPhases::TurnPhaseConfig>{ 3, new rpg3D::playPhases::TurnPhaseConfig[3] {
			rpg3D::playPhases::TurnPhaseConfig("Prep", true/*isUserEndable*/),
			rpg3D::playPhases::TurnPhaseConfig("PreAutoBattle", false/*isUserEndable*/),
			rpg3D::playPhases::TurnPhaseConfig("AutoBattle", false/*isUserEndable*/)
		}};

		gw->addGamePartUninitialised(gcTurnBasedPlay = new rpg3D::playPhases::GCPlayPhases(
			gw,
			playPhaseConfigsList
		));
	}
}

void CZ_GridBattleA::onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivated(zoneStartExtraData, gw);

	gpPartyCandidates = gw->getComponentAs<IGPPartyCandidates*>(true/*mustExist*/);

	ZoneStartExtraData* cZoneStartExtraData = zoneStartExtraData->getAs<ZoneStartExtraData*>();
	if (cZoneStartExtraData == nullptr) {
		throw LogicException(LOC);
	}

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

	if (zoneStartExtraData != nullptr && (zoneStartExtraData->teleportTargetEntityInstanceID != nullptr || zoneStartExtraData->teleportTargetOffset != nullptr)) {
		// This zone always has computed spawn position.
		throw LogicException(LOC);
	}

	// Give the player full hp. This is also done in case he loses all hp.
	rpg3D::ILivingEntityModule* livingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<rpg3D::ILivingEntityModule*>(true/*mustExist*/);
	livingEntity->setCurrentHP(livingEntity->getT()->maxHP);

	{
		//asd_011;// can't use a entity because the value must be available before this location.
		IWorldEntity* gridOriginIndicator = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::gridOriginIndicator);
		if (gridOriginIndicator == nullptr) {
			throw LogicException(LOC);
		}

		Vector3* gridOriginPos = gridOriginIndicator->getPos();
		float gridCellSizeM = zoneContentData->tileCellSizeCm / 100.0f;
		Vector3 pos{};

		{
			pos.set(
				gridOriginPos->x + 1.5f * gridCellSizeM,
				gridOriginPos->y + 1.5f * gridCellSizeM,
				gridOriginPos->z
			);

			// Position the player unit.
			gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->setPos(pos);
			IMovingEntityModule* movingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
			movingEntity->addAction_stopMovement(true/*stopPhysicsLinearVelocitiesAlso*/);
			movingEntity->addAction_teleportToDestination(pos);
		}

		// Position the party members.
		int ct = -1;
		for (ListDL<sp<base::WrappedWorldEntity>>::Entry* listEntry = cZoneStartExtraData->partyEntities.getFirst(); listEntry != nullptr; listEntry = listEntry->next) {
			ct++;
			sp<base::WrappedWorldEntity> wEntity = listEntry->data;

			if (wEntity == nullptr || wEntity->entity == nullptr) {
				throw LogicException(LOC);
			}

			gw->getGPContent()->getPlayerEntitiesGroup()->addEntity(wEntity->entity);

			float gridCellSizeM = zoneContentData->tileCellSizeCm / 100.0f;

			if (ct == 0) {
				pos.set(
					gridOriginPos->x + 0.5f * gridCellSizeM,
					gridOriginPos->y + 1.5f * gridCellSizeM,
					gridOriginPos->z
				);
			} else if (ct == 1) {
				pos.set(
					gridOriginPos->x + 1.5f * gridCellSizeM,
					gridOriginPos->y + 0.5f * gridCellSizeM,
					gridOriginPos->z
				);
			} else if (ct == 2) {
				pos.set(
					gridOriginPos->x + 0.5f * gridCellSizeM,
					gridOriginPos->y + 0.5f * gridCellSizeM,
					gridOriginPos->z
				);
			} else {
				throw LogicException(LOC);
			}

			wEntity->entity->setPos(pos);
			IMovingEntityModule* movingEntity = wEntity->entity->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
			movingEntity->addAction_ActionGeneralStop();
			movingEntity->addAction_teleportToDestination(pos);

			gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->setRot(Vector3::Z, 135.0f);
		}
	}
	
	Color col{ "#64B0FFFF" };
	GLUtil::gl->setCachedClearColor(col);

	{
		Vector3 offsetDir{ -1.0f, -1.0f, 3.0f };
		offsetDir.nor();
		Vector3 focusPointOffset{ 0.0f, 0.0f, 0.0f };
		gw->getGPSim()->configCamera_followerThirdPersonCamera(
			offsetDir,
			4.0f, 6.0f,
			6.0f, 11.0f,
			focusPointOffset
		);

		gw->getComponentAs<rpg3D::IGPUserEntitiesController*>(true/*mustExist*/)
			->setEntityControlType(rpg3D::EntityControlType::IndirectMoveToTarget_AutoRotToMoveDirection);
		gw->getGPSim()
			->setCameraMode(GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation, true);
	}
}

void CZ_GridBattleA::onZoneActivatedB(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivatedB(zoneStartExtraData, gw);

	gcTurnBasedPlay->setIsPlayPhasesEnabled(true);
}

void CZ_GridBattleA::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);

	gw->removeComponent(gcTurnBasedPlay, true/*doDispose*/);
	gw->removeComponent(gpGrid2DContent, true/*doDispose*/);

	// Dispose all party candidates entities. This is required in case a zone other than the Town gets activated.
	// This will dispose all entities because the party entities were released on the zone activation.
	gpPartyCandidates->disposeAllPartyCandidated();

	// Remove the entities configs from the list completly because they are no longer candidates.
	/// Do this removal here to avoid the player canceling the arena play and force_teleporting to the town and re-starting with the same party.
	gpPartyCandidates->removeActivePartyConfigs();

	gw->getCapabilities().setWithBitIndex(worldGame3D::gw::ICapabilities::STACKABLES_HAULER_BIT_INDEX, cachedPrevousStackableGWCapability);
}

CZ_GridBattleA::~CZ_GridBattleA() {
	//void
}
