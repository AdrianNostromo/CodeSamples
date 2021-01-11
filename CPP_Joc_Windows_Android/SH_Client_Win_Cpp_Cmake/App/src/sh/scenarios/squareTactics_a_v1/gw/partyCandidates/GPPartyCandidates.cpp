#include "GPPartyCandidates.h"
#include <base/exceptions/LogicException.h>
#include <base/map/ExtraData.h>
#include <base/object/IWrappedValue.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/module/controller/IControllerModule.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <rpg3D/gw/entity/template/TeamTemplate.h>
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/playerUnit/generalActiveTool/LBGeneralActiveTools.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveGlobalDestination_TMoveDirection/ToolMGlobalDestination_TMoveDirection.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigDirect.h>
#include <base/app/IAppAssets.h>

using namespace squareTactics_a_v1;

GPPartyCandidates::GPPartyCandidates(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

ListDL<IGPPartyCandidates::EntitiesCreateInfo>& GPPartyCandidates::getPartyCandidatesConfigsListRef() {
	return partyCandidatesConfigsList;
}

ListDL<sp<base::WrappedWorldEntity>>& GPPartyCandidates::getActivePartyMemberEntitiesListRef() {
	return activePartyMemberEntitiesList;
}

GPPartyCandidates::EntitiesCreateInfo* GPPartyCandidates::getEntityPartyCandidateConfigIfExists(IWorldEntity* entity) {
	if (entity == nullptr) {
		throw LogicException(LOC);
	}

	for (ListDL<EntitiesCreateInfo>::Entry* listEntry = partyCandidatesConfigsList.getFirst(); listEntry != nullptr; listEntry = listEntry->next) {
		EntitiesCreateInfo& config = listEntry->data;

		if (config.wEntity != nullptr && config.wEntity->entity == entity) {
			return &config;
		}
	}

	return nullptr;
}

void GPPartyCandidates::addCandidateToActiveParty(EntitiesCreateInfo& entityConfig) {
	// Find the config for the entity.
	if (entityConfig.wEntity == nullptr || entityConfig.wEntity->entity == nullptr || entityConfig.isEntityInActiveParty) {
		throw LogicException(LOC);
	}

	entityConfig.isEntityInActiveParty = true;
}

void GPPartyCandidates::removeAllEntitiesFromActiveParty() {
	for (ListDL<EntitiesCreateInfo>::Entry* listEntry = partyCandidatesConfigsList.getFirst(); listEntry != nullptr; listEntry = listEntry->next) {
		EntitiesCreateInfo& config = listEntry->data;

		if (config.isEntityInActiveParty) {
			config.isEntityInActiveParty = false;
		}
	}
}

void GPPartyCandidates::removeActivePartyConfigs() {
	// Use enumeration to allow for entries removing.
	partyCandidatesConfigsList.enumerateDirect([](ListDL<EntitiesCreateInfo>::Entry* listEntry, EntitiesCreateInfo& impactedEntityInfo, bool& stopEnumeration) ->void {
		EntitiesCreateInfo& config = listEntry->data;

		if (config.isEntityInActiveParty) {
			listEntry->remove();
		}
	});
}

void GPPartyCandidates::disposeUnusedPartyCandidated() {
	for (ListDL<EntitiesCreateInfo>::Entry* listEntry = partyCandidatesConfigsList.getFirst(); listEntry != nullptr; listEntry = listEntry->next) {
		EntitiesCreateInfo& config = listEntry->data;

		if (config.wEntity != nullptr && !config.isEntityInActiveParty) {
			if (config.wEntity->entity == nullptr) {
				throw LogicException(LOC);
			}

			config.wEntity->entity->markEntityForRemoval(IWorldEntity::ENT_REM_REASON_PURPOSE_FINISHED, false);

			config.wEntity = nullptr;
		}
	}
}

void GPPartyCandidates::disposeAllPartyCandidated() {
	for (ListDL<EntitiesCreateInfo>::Entry* listEntry = partyCandidatesConfigsList.getFirst(); listEntry != nullptr; listEntry = listEntry->next) {
		EntitiesCreateInfo& config = listEntry->data;

		if (config.wEntity != nullptr) {
			if (config.wEntity->entity == nullptr) {
				throw LogicException(LOC);
			}

			config.wEntity->entity->markEntityForRemoval(IWorldEntity::ENT_REM_REASON_PURPOSE_FINISHED, false);

			config.wEntity = nullptr;
		}
	}
}

IWorldEntity* GPPartyCandidates::createPartyCandidateEntity(
	EntitiesCreateInfo& createConfig,
	Vector3& pos, Quaternion& rot)
{
	if (createConfig.wEntity != nullptr) {
		// Entity already exists.
		throw LogicException(LOC);
	}

	std::shared_ptr<ExtraData> extraData = std::make_shared<ExtraData>();

	extraData->setData(createConfig.extraDataId_voxelGridZXY, base::IWrappedValue::new_Custom<Array3D<signed char>*>(createConfig.voxelGridZXY, -1));
	extraData->setData(createConfig.extraDataId_materialsList, base::IWrappedValue::new_Custom<ArrayList<Visual3DModelMaterial*>*>(createConfig.materialsList, -1));

	IWorldEntity* entity = gw->getGPMain()->createEntity(
		createConfig.entityTemplate,
		pos,
		&rot/*rot*/, nullptr/*cardinalRotation*/,
		nullptr/*instanceId*/,
		0,
		extraData,
		false/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		[&createConfig, this](IWorldEntity* entity) -> void {
			rpg3D::ITeamModule* entity_team = entity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
			entity_team->setTeamIndex(createConfig.teamIndex);

			rpg3D::IControllerModule* entity_controller = entity->getComponentAs<rpg3D::IControllerModule*>(true/*mustExist*/);

			rpg3D::IToolsHandlerModule* entity_toolsHandler = entity->getComponentAs<rpg3D::IToolsHandlerModule*>(true/*mustExist*/);

			//asd_01;
			//entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolMDPadLocalFrontBack>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			//entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolMGlobalDestination_TMoveDirection>(
			//	-1/*movementTypeOverride*/, false/*usesGridMovement*/, rpg3D::playerUnit::ToolMGlobalDestination_TMoveDirection::DestinationType::GroundAndSomeEntities/*destinationType*/
			//), gw->getAppAssets()->getEmbeddedAssetsManager());
			//entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolMGlobalDirection_TMoveDirection>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			//entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolMGlobalDirection>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			//entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolMLocalStrafe>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			//entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolTDPASSY_LMOSP>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			//entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolTGlobalFocusDirectionYaw>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			//entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolTMouseOrSensorPitchYaw>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolMGlobalDestination_TMoveDirection>(
				rpg3D::MovementConfigDirect::TYPE/*movementTypeOverride*/, true/*usesGridMovement*/, rpg3D::playerUnit::ToolMGlobalDestination_TMoveDirection::DestinationType::GroundOnly/*destinationType*/
			), gw->getAppAssets()->getEmbeddedAssetsManager());

			rpg3D::playerUnit::LBGeneralActiveTools* lBGeneralActiveTools = new rpg3D::playerUnit::LBGeneralActiveTools();
			entity_controller->registerLogicBlock(lBGeneralActiveTools, rpg3D::playerUnit::ILBGeneralActiveTools::ID, static_cast<rpg3D::playerUnit::ILBGeneralActiveTools*>(lBGeneralActiveTools));

			entity_controller->setSingleActiveGroupLogicBlock(rpg3D::playerUnit::ILBGeneralActiveTools::GroupID, rpg3D::playerUnit::ILBGeneralActiveTools::ID);
		}, true/*isActive*//*the entity is activated by the zone start*/
	);

	//asd_01;
	//sp<rpg3D::Tool> tool1 = EntityTemplatesList::Tool_StackablesHauler_A_V1->createNewTool(
	//	gw->getAppAssets()
	//);
	//rpg3D::IToolsHandlerModule* entity_toolsHandler = entity->getComponentAs<rpg3D::IToolsHandlerModule*>(true/*mustExist*/);
	//entity_toolsHandler->addTool(
	//	tool1
	//);
	//entity_toolsHandler->selectTool(tool1, -1);

	createConfig.wEntity = entity->getWrapperB();

	return entity;
}

GPPartyCandidates::~GPPartyCandidates() {
	//void
}
