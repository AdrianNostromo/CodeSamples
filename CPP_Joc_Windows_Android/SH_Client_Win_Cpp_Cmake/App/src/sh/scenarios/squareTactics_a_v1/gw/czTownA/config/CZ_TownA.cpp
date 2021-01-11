#include "CZ_TownA.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include <base/statics/StaticsInit.h>
#include "../../../ScenarioFactory.h"
#include "../../../gw/config/entity/EntityIds.h"
#include <sh/scenarios/squareTactics_a_v1/gw/czGridBattleA/config/util/ZoneStartExtraData.h>
#include <rpg3D/gw/entity/template/platform/PlatformTemplate.h>
#include <rpg3D/gw/manager/IGPManager.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include <rpg3D/gw/entity/template/LivingEntityTemplate.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <base/opengl/util/GLUtil.h>
#include <sh/scenarios/startarea_a_v1/gw/config/ScenarioFactory.h>
#include <rpg3D/gw/config/entity/platform/T_Platform_P.h>
#include <rpg3D/gw/config/entity/doodad/TC_Bed_A_V1.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <worldGame3D/scripts/ScriptCommands.h>
#include <worldGame3D/gw/contentZones/IGamePartContentZones.h>
#include "../../czGridBattleA/config/CZ_GridBattleA.h"
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_GridXY.h>
#include <rpg3D/gw/config/entity/structure/TC_GroundOrWall.h>
#include <graphics/visual3d/util/VoxelSides.h>
#include <rpg3D/gw/contentZones/zone/contentData/ZoneContentData.h>
#include <base/memory/SharedPointer.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/playerUnit/generalActiveTool/LBGeneralActiveTools.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/customisedContent/IGamePartCustomisedContent.h>
#include <rpg3D/scenariohandler/customisedContentHandler/ICustomisedContentHandler.h>
#include <rpg3D/gw/config/entity/unit/UnitTemplates_PlayerA.h>
#include <rpg3D/gw/config/team/TeamsIndexesList.h>
#include <rpg3D/gw/entity/module/controller/IControllerModule.h>
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>

#include <sh/scenarios/squareTactics_a_v1/gw/config/entity/custom/T_UnitValidatorHaulerPlatform.h>
#include <sh/scenarios/squareTactics_a_v1/gw/config/entity/platform/T_PlatformSlimeTeleporter.h>
#include <sh/scenarios/squareTactics_a_v1/gw/config/entity/environment/T_Doorframe_A_V1.h>
#include <sh/scenarios/squareTactics_a_v1/gw/config/entity/environment/T_Pillar_A_V1.h>
#include <sh/scenarios/squareTactics_a_v1/gw/config/entity/environment/T_WallElement_A_V1.h>
#include <sh/scenarios/squareTactics_a_v1/gw/partyCandidates/IGPPartyCandidates.h>
#include <rpg3D/gw/entity/module/stackable/IStackableModule.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>

#include <base/view/snackbar/util/WrappedSnackbarView.h>
#include <base/view/snackbar/ISnackbarView.h>
#include <base/view/snackbar/util/Config.h>
#include <base/app/IApp.h>
#include <base/viewsHandler/IViewsHandler.h>

using namespace squareTactics_a_v1;
using namespace squareTactics_a_v1::czTownA;

std::string CZ_TownA::NAME = "TownA";

std::shared_ptr<worldGame3D::ContentZone> CZ_TownA::FactoryFunction() {
	std::shared_ptr<worldGame3D::ContentZone> ret = std::make_shared<CZ_TownA>();

	return ret;
}

CZ_TownA::CZ_TownA()
	: super(NAME, true/*useAutoPlayerPositioning*/, EntityIds::defaultZoneEntrance/*defaultZoneEntrance*/)
{
	// Include from other scenarios because this entire system will be converted to some database logic.
	if (squareTactics_a_v1::T_PlatformSlimeTeleporter::TEMPLATE == nullptr) {
		throw LogicException(LOC);
	}
	(void)squareTactics_a_v1::T_UnitValidatorHaulerPlatform::TEMPLATE;
	(void)squareTactics_a_v1::T_Doorframe_A_V1::TEMPLATE;
	(void)squareTactics_a_v1::T_Pillar_A_V1::TEMPLATE;
	(void)squareTactics_a_v1::T_WallElement_A_V1::TEMPLATE;
}

void CZ_TownA::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if (event.type == rpg3D::IGPManager::GWEvent_onPlayerActivatedPlatformNonSpawn->type) {
		if (event.getEntityData()->getInstanceId() == EntityIds::enterGridBattleA) {
			IGPPartyCandidates::EntitiesCreateInfo* partyCandidateConfig_0 = nullptr;
			IGPPartyCandidates::EntitiesCreateInfo* partyCandidateConfig_1 = nullptr;
			IGPPartyCandidates::EntitiesCreateInfo* partyCandidateConfig_2 = nullptr;
			
			{
				IWorldEntity* partyValidationPlatform_0 = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::arenaUnitValidatorHaulerPlatform_0, true/*mustExist*/);
				IWorldEntity* partyValidationPlatform_1 = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::arenaUnitValidatorHaulerPlatform_1, true/*mustExist*/);
				IWorldEntity* partyValidationPlatform_2 = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::arenaUnitValidatorHaulerPlatform_2, true/*mustExist*/);

				rpg3D::IStackableModule* pvp_0_stackable = partyValidationPlatform_0->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);
				rpg3D::IStackableModule* pvp_1_stackable = partyValidationPlatform_1->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);
				rpg3D::IStackableModule* pvp_2_stackable = partyValidationPlatform_2->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);

				rpg3D::IStackableModule* partyEntity_0_stackable = pvp_0_stackable->getCarrying();
				rpg3D::IStackableModule* partyEntity_1_stackable = pvp_1_stackable->getCarrying();
				rpg3D::IStackableModule* partyEntity_2_stackable = pvp_2_stackable->getCarrying();
				
				bool isPartyValid = true;
				if (isPartyValid) {
					if (partyEntity_0_stackable == nullptr || partyEntity_1_stackable == nullptr || partyEntity_2_stackable == nullptr) {
						isPartyValid = false;
					}
				}

				if (isPartyValid) {
					IWorldEntity* partyEntity_0 = partyEntity_0_stackable->getEntity();
					IWorldEntity* partyEntity_1 = partyEntity_1_stackable->getEntity();
					IWorldEntity* partyEntity_2 = partyEntity_2_stackable->getEntity();

					if (!partyEntity_0->getTemplate()->hasTag(rpg3D::EntityTags::PLAYER_UNIT)
						|| !partyEntity_1->getTemplate()->hasTag(rpg3D::EntityTags::PLAYER_UNIT)
						|| !partyEntity_2->getTemplate()->hasTag(rpg3D::EntityTags::PLAYER_UNIT)
						|| partyEntity_0_stackable->getIsCarrying()
						|| partyEntity_1_stackable->getIsCarrying()
						|| partyEntity_2_stackable->getIsCarrying()
						|| (partyCandidateConfig_0 = gpPartyCandidates->getEntityPartyCandidateConfigIfExists(partyEntity_0)) == nullptr
						|| (partyCandidateConfig_1 = gpPartyCandidates->getEntityPartyCandidateConfigIfExists(partyEntity_1)) == nullptr
						|| (partyCandidateConfig_2 = gpPartyCandidates->getEntityPartyCandidateConfigIfExists(partyEntity_2)) == nullptr)
					{
						isPartyValid = false;
					}
				}

				if (!isPartyValid) {
					// show a snackbar.
					if (snackbarWView_partyIsNotValid == nullptr || snackbarWView_partyIsNotValid->view == nullptr) {
						snackbarWView_partyIsNotValid = gw->getApp()->getComponentAs<base::IViewsHandler*>(true/*mustExist*/)->getNewSnackbarViewAutoDisposable();
						snackbarWView_partyIsNotValid->view->showInOrderManaged(
							true,
							std::make_shared<base::snackbar::Config>(
								"Party is not valid."/*text*/,
								4.0f/*autoCloseTimerS*/,
								nullptr/*dynamicButtonConfig*/
							)
						);
					}

					return;
				}
			}

			{
				rpg3D::IStackableModule* playerUnit_stackable = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);
				if (playerUnit_stackable != nullptr) {
					if (playerUnit_stackable->getIsHauled()) {
						throw LogicException(LOC);
					}

					if (playerUnit_stackable->getIsCarrying()) {
						playerUnit_stackable->setCarrying(nullptr);
					}
				}
			}

			worldGame3D::IGamePartContentZones* gpContentZones = gw->getComponentAs<worldGame3D::IGamePartContentZones*>(true/*mustExist*/);

			sp<Vector3> teleportTargetOffset = new Vector3(gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getPos());
			teleportTargetOffset->sub(event.getEntityData()->getPos());

			std::shared_ptr<czGridBattleA::ZoneStartExtraData> zoneStartExtraData = std::make_shared<czGridBattleA::ZoneStartExtraData>(
				nullptr/*teleportTargetEntityInstanceID*/, nullptr/*teleportTargetOffset*/,
				nullptr/*zeroHpZoneName*/, nullptr/*zeroHpZonePlayerPosEntityId*/
			);

			// Make the entities to be in the party list.
			gpPartyCandidates->addCandidateToActiveParty(*partyCandidateConfig_0);
			gpPartyCandidates->addCandidateToActiveParty(*partyCandidateConfig_1);
			gpPartyCandidates->addCandidateToActiveParty(*partyCandidateConfig_2);

			zoneStartExtraData->partyEntities.appendDirect(partyCandidateConfig_0->wEntity);
			zoneStartExtraData->partyEntities.appendDirect(partyCandidateConfig_1->wEntity);
			zoneStartExtraData->partyEntities.appendDirect(partyCandidateConfig_2->wEntity);

			gpContentZones->setActiveZone(
				czGridBattleA::CZ_GridBattleA::NAME, zoneStartExtraData, true/*restartZoneIfActive*/,
				true/*useDelayedOnLoopActivation*/
			);
		} else {
			// Unhandled platform. Script platforms are already processed by the script logic and never get here.
			throw LogicException(LOC);
		}
	} else {
		throw LogicException(LOC);
	}
}

sp<rpg3D::ZoneContentData> CZ_TownA::CreateDefaultZoneContentData() {
	sp<rpg3D::ZoneContentData> zoneContentData = rpg3D::ZoneContentData::GetNewBasicZone(
		rpg3D::ZoneContentData::GroundType::GrassDirt_2Layers/*groundType*/, true/*createTallUnderground*/,
		true/*createWallsTiledXY*/,
		true/*createStarterAreaTeleporter*/
	);

	{
		float gridCellSizeM = zoneContentData->tileCellSizeCm / 100.0f;
		Vector3 pos{};
		Quaternion rot{};
		
		zoneContentData->entitiesInfosList.appendEmplace(
			rpg3D::T_Platform_P::TEMPLATE/*entityTemplate*/,
			*pos.set((0.5f + 4.0f) * gridCellSizeM, (0.5f + 4.0f) * gridCellSizeM, 0.0f)/*posInRoom*/,
			*rot.set(Vector3::Z, 0.0f)/*rot*/,
			EntityIds::enterGridBattleA/*instanceId*/,
			nullptr/*materialsList*/, StringUtil::EMPTY/*extraDataId_materialsList*/,
			nullptr/*tilingInformation*/,
			nullptr/*platformActivationScript*/
		);

		{
			// Create 3 party unit validation hauyler platform.
			float gridCellSizeM = zoneContentData->tileCellSizeCm / 100.0f;
			Vector3 pos{};
			Quaternion rot{};

			zoneContentData->entitiesInfosList.appendEmplace(
				T_UnitValidatorHaulerPlatform::TEMPLATE/*entityTemplate*/,
				*pos.set((0.5f + 4.0f) * gridCellSizeM, (0.5f + 4.0f) * gridCellSizeM, 0.0f)/*posInRoom*/,
				*rot.set(Vector3::Z, 0.0f)/*rot*/,
				EntityIds::arenaUnitValidatorHaulerPlatform_0/*instanceId*/,
				nullptr/*materialsList*/, StringUtil::EMPTY/*extraDataId_materialsList*/,
				nullptr/*tilingInformation*/,
				nullptr/*platformActivationScript*/
			);
			zoneContentData->entitiesInfosList.appendEmplace(
				T_UnitValidatorHaulerPlatform::TEMPLATE/*entityTemplate*/,
				*pos.set((0.5f + 4.0f) * gridCellSizeM, (0.5f + 4.0f) * gridCellSizeM, 0.0f)/*posInRoom*/,
				*rot.set(Vector3::Z, 0.0f)/*rot*/,
				EntityIds::arenaUnitValidatorHaulerPlatform_1/*instanceId*/,
				nullptr/*materialsList*/, StringUtil::EMPTY/*extraDataId_materialsList*/,
				nullptr/*tilingInformation*/,
				nullptr/*platformActivationScript*/
			);
			zoneContentData->entitiesInfosList.appendEmplace(
				T_UnitValidatorHaulerPlatform::TEMPLATE/*entityTemplate*/,
				*pos.set((0.5f + 4.0f) * gridCellSizeM, (0.5f + 4.0f) * gridCellSizeM, 0.0f)/*posInRoom*/,
				*rot.set(Vector3::Z, 0.0f)/*rot*/,
				EntityIds::arenaUnitValidatorHaulerPlatform_2/*instanceId*/,
				nullptr/*materialsList*/, StringUtil::EMPTY/*extraDataId_materialsList*/,
				nullptr/*tilingInformation*/,
				nullptr/*platformActivationScript*/
			);
		}

		//{
		//	// Create 3 beds.
		//	zoneContentData->entitiesInfosList.appendEmplace(
		//		rpg3D::TC_Bed_A_V1::GetOrCreateTemplate(10/*voxelSizeCM*/)/*entityTemplate*/,
		//		*pos.set((0.5f + 6.0f) * gridCellSizeM, (0.5f + 6.0f) * gridCellSizeM, 0.0f)/*posInRoom*/,
		//		*rot.set(Vector3::Z, 0.0f)/*rot*/,
		//		EntityIds::bed_0/*instanceId*/,
		//		nullptr/*materialsList*/, StringUtil::EMPTY/*extraDataId_materialsList*/,
		//		nullptr/*tilingInformation*/,
		//		nullptr/*platformActivationScript*/
		//	);
		//	zoneContentData->entitiesInfosList.appendEmplace(
		//		rpg3D::TC_Bed_A_V1::GetOrCreateTemplate(10/*voxelSizeCM*/)/*entityTemplate*/,
		//		*pos.set((0.5f + 6.0f) * gridCellSizeM, (0.5f + 6.0f) * gridCellSizeM, 0.0f)/*posInRoom*/,
		//		*rot.set(Vector3::Z, 0.0f)/*rot*/,
		//		EntityIds::bed_1/*instanceId*/,
		//		nullptr/*materialsList*/, StringUtil::EMPTY/*extraDataId_materialsList*/,
		//		nullptr/*tilingInformation*/,
		//		nullptr/*platformActivationScript*/
		//	);
		//	zoneContentData->entitiesInfosList.appendEmplace(
		//		rpg3D::TC_Bed_A_V1::GetOrCreateTemplate(10/*voxelSizeCM*/)/*entityTemplate*/,
		//		*pos.set((0.5f + 6.0f) * gridCellSizeM, (0.5f + 6.0f) * gridCellSizeM, 0.0f)/*posInRoom*/,
		//		*rot.set(Vector3::Z, 0.0f)/*rot*/,
		//		EntityIds::bed_2/*instanceId*/,
		//		nullptr/*materialsList*/, StringUtil::EMPTY/*extraDataId_materialsList*/,
		//		nullptr/*tilingInformation*/,
		//		nullptr/*platformActivationScript*/
		//	);
		//}
	}
	
	return zoneContentData;
}

void CZ_TownA::onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivated(zoneStartExtraData, gw);

	gpPartyCandidates = gw->getComponentAs<IGPPartyCandidates*>(true/*mustExist*/);

	sp<rpg3D::ZoneContentData> zoneContentData = loadedZoneData;
	if (zoneContentData == nullptr) {
		zoneContentData = CreateDefaultZoneContentData();
	}

	// Create the zone specific game parts.
	gw->addEventListener(
		rpg3D::IGPManager::GWEvent_onPlayerActivatedPlatformNonSpawn->type,
		std::bind(&CZ_TownA::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		localAutoListenersList
	);

	zoneContentData->createContent(gw);

	// Give the player full hp. This is also done in case he loses all hp.
	rpg3D::ILivingEntityModule* livingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<rpg3D::ILivingEntityModule*>(true/*mustExist*/);
	livingEntity->setCurrentHP(livingEntity->getT()->maxHP);

	Color col{ "#64B0FFFF" };
	GLUtil::gl->setCachedClearColor(col);

	{
		// Create party candidates units.
		float gridCellSizeM = zoneContentData->tileCellSizeCm / 100.0f;
		Vector3 pos{};
		Quaternion rot{};

		int ct = -1;
		for (ListDL<IGPPartyCandidates::EntitiesCreateInfo>::Entry* listEntry = gpPartyCandidates->getPartyCandidatesConfigsListRef().getFirst(); listEntry != nullptr; listEntry = listEntry->next) {
			ct++;
			IGPPartyCandidates::EntitiesCreateInfo& entityConfig = listEntry->data;

			IWorldEntity* newEntity = gpPartyCandidates->createPartyCandidateEntity(
				entityConfig,
				*pos.set((0.5f + 0.0f + (ct % 3)) * gridCellSizeM, (0.5f + 0.0f + Math::floor(ct / 3)) * gridCellSizeM, 0.0f),
				*rot.set(Vector3::Z, -90.0f/*angleD*/)
			);

			//asd_01;_t;// For fast test, place the entities on the arena party platforms.
			if (ct == 0) {
				IWorldEntity* partyValidationPlatform_0 = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::arenaUnitValidatorHaulerPlatform_0, true/*mustExist*/);
				rpg3D::IStackableModule* pvp_0_stackable = partyValidationPlatform_0->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);

				newEntity->setPos(*partyValidationPlatform_0->getPos());
				pvp_0_stackable->setCarrying(newEntity->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/));
			} else if (ct == 1) {
				IWorldEntity* partyValidationPlatform_1 = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::arenaUnitValidatorHaulerPlatform_1, true/*mustExist*/);
				rpg3D::IStackableModule* pvp_1_stackable = partyValidationPlatform_1->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);

				newEntity->setPos(*partyValidationPlatform_1->getPos());
				pvp_1_stackable->setCarrying(newEntity->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/));
			} else if (ct == 2) {
				IWorldEntity* partyValidationPlatform_2 = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::arenaUnitValidatorHaulerPlatform_2, true/*mustExist*/);
				rpg3D::IStackableModule* pvp_2_stackable = partyValidationPlatform_2->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);

				newEntity->setPos(*partyValidationPlatform_2->getPos());
				pvp_2_stackable->setCarrying(newEntity->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/));
			}
		}
	}
}

void CZ_TownA::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);

	gpPartyCandidates->disposeUnusedPartyCandidated();

	localAutoListenersList.clear();
}

CZ_TownA::~CZ_TownA() {
	//void
}
