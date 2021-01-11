#include "GamePartInit.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <rpg3D/gw/config/entity/unit/UnitTemplates_PlayerA.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include <worldGame3D/gw/util/GameCameraModeBitGroups.h>
#include <rpg3D/gw/sim/GamePartSim.h>
#include "../config/zoneConfigs/ContentZoneMain.h"
#include <worldGame3D/gw/contentZones/GamePartContentZones.h>
#include <graphics/environment/environment3D/cinematic/ICinematicController.h>
#include <rpg3D/gw/ui/IGamePartUserInterface.h>
#include <base/opengl/util/GLUtil.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/scenariohandler/customisedContentHandler/ICustomisedContentHandler.h>
#include <rpg3D/gw/customisedContent/IGamePartCustomisedContent.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <base/app/IAppAssets.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveDPadLocalFrontBack/ToolMDPadLocalFrontBack.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveGlobalDestination_TMoveDirection/ToolMGlobalDestination_TMoveDirection.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveGlobalDirection_TMoveDirection/ToolMGlobalDirection_TMoveDirection.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveGlobalDirection/ToolMGlobalDirection.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveLocalStrafe/ToolMLocalStrafe.h>

#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/turnDPASSY_LMOSP/ToolTDPASSY_LMOSP.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/turnGlobalFocusDirectionYaw/ToolTGlobalFocusDirectionYaw.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/turnMouseOrSensorPitchYaw/ToolTMouseOrSensorPitchYaw.h>

#include <rpg3D/gw/entity/module/controller/logicBlock/playerUnit/generalActiveTool/LBGeneralActiveTools.h>
#include <rpg3D/gw/entity/module/controller/IControllerModule.h>
#include <rpg3D/gw/userEntitiesController/IGPUserEntitiesController.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <rpg3D/gw/userEntitiesController/util/EntityControlType.h>
#include <worldGame3D/gw/entityGroups/util/IEntsGroup.h>

using namespace islandSurvival_a_v1;

GamePartInit::GamePartInit(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		base::IGameWorld::GWEvent_createContent->type,
		std::bind(&GamePartInit::onGWEvent_createContent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GamePartInit::onGWEvent_createContent(IEventListener& eventListener, base::GWEvent& event) {
	//Create the player unit and handle settings initialisation.
	createPlayerUnit(
		0.0f,
		0.0f,
		0.0f
	);

	//Zoom not used with perspective camera.
	// cc->setCamera3DZoom(3.5f);
	Vector3 offsetDir;
	Vector3 focusPointOffset;
	float offsetDist;

	offsetDir.set(0.0f, 0.0f, 1.0f)->nor();
	offsetDist = 0.5f;
	gw->getGPSim()->configCamera_followerFirstPersonCamera(
		offsetDir, offsetDist
	);

	offsetDir.set(-1.0f, -1.0f, 3.0f)->nor();
	focusPointOffset.set(0.0f, 0.0f, 0.0f);
	gw->getGPSim()->configCamera_followerThirdPersonCamera(
		offsetDir,
		4.0f, 6.0f,
		6.0f, 11.0f,
		focusPointOffset
	);

	gw->getGPMain()->getCC()->setSunLightDirection(0.0f, 2.0f, -4.0f);

	//gw->getGPSim()->setCameraMode(GameCameraModeBitGroups::followerFirstPersonTargetRotation, false);
	gw->getGPSim()->setCameraMode(GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation, false);

	// gw->getComponentAs<rpg3D::IGPUserEntitiesController*>(true/*mustExist*/)->setEntityControlType(rpg3D::EntityControlType::IndirectMoveToTarget_AutoRotToMoveDirection);
	// gw->getComponentAs<rpg3D::IGPUserEntitiesController*>(true/*mustExist*/)->setEntityControlType(rpg3D::EntityControlType::DPadCameraAxesDirectionMove_AutoRotToMoveDirection);
	gw->getComponentAs<rpg3D::IGPUserEntitiesController*>(true/*mustExist*/)->setEntityControlType(rpg3D::EntityControlType::DPadCameraAxesDirectionMove_LookAtCursorOrAutoRotToMoveDirection);
	//gw->getComponentAs<rpg3D::IGPUserEntitiesController*>(true/*mustExist*/)->setEntityControlType(rpg3D::EntityControlType::DPadUnitAxesStrafe_CursorRotationBothAxes);

	// Enable the user interface.
	gw->getComponentAs<rpg3D::IGamePartUserInterface*>(true/*mustExist*/)->showViewIncremental();

	Color col{ "#64B0FFFF" };
	GLUtil::gl->setCachedClearColor(col);
}

void GamePartInit::createPlayerUnit(float x, float y, float z) {
	std::shared_ptr<ExtraData> extraData = std::make_shared<ExtraData>();

	rpg3D::IGamePartCustomisedContent* gpCustomisedContent = gw->getComponentAs<rpg3D::IGamePartCustomisedContent*>(true/*mustExist*/);
	std::shared_ptr<rpg3D::ICustomisedContentHandler::EntityMaterialsAndVoxelsGrid> playerUnitMaterialsAndVoxelsGrid = gpCustomisedContent->getCustomisedContentHandler()->getPlayerUnitMaterialsAndVoxelsGrid();
	extraData->setData(rpg3D::UnitTemplates_PlayerA::DATA_KEY_voxelGridZXY, base::IWrappedValue::new_Custom<Array3D<signed char>*>(playerUnitMaterialsAndVoxelsGrid->voxelGrid, -1));
	extraData->setData(rpg3D::UnitTemplates_PlayerA::DATA_KEY_materials, base::IWrappedValue::new_Custom<ArrayList<Visual3DModelMaterial*>*>(playerUnitMaterialsAndVoxelsGrid->materialsList, -1));

	extraData->setData(rpg3D::Physics3DTemplate::DATA_KEY_extraInteractionFilters, base::IWrappedValue::new_Custom(rpg3D::PhysicsCollissionConfig::MODIFIER_MainPlayerUnit, -1));

	Vector3 pos{ x, y, z };
	Quaternion rot{ Vector3::Z, -90.0f };
	IWorldEntity* entity = gw->getGPMain()->createEntity(
		rpg3D::EntityTemplatesList::Unit_PlayerA,
		pos,
		&rot/*rot*/, nullptr/*cardinalRotation*/,
		nullptr/*instanceId*/,
		0,
		extraData,
		false/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		[this](IWorldEntity* entity) -> void {

			rpg3D::IControllerModule* entity_controller = entity->getComponentAs<rpg3D::IControllerModule*>(true/*mustExist*/);

			rpg3D::IToolsHandlerModule* entity_toolsHandler = entity->getComponentAs<rpg3D::IToolsHandlerModule*>(true/*mustExist*/);

			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolMDPadLocalFrontBack>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolMGlobalDestination_TMoveDirection>(
				-1/*movementTypeOverride*/, false/*usesGridMovement*/, rpg3D::playerUnit::ToolMGlobalDestination_TMoveDirection::DestinationType::GroundAndSomeEntities/*destinationType*/
			), gw->getAppAssets()->getEmbeddedAssetsManager());
			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolMGlobalDirection_TMoveDirection>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolMGlobalDirection>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolMLocalStrafe>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolTDPASSY_LMOSP>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolTGlobalFocusDirectionYaw>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolTMouseOrSensorPitchYaw>(), gw->getAppAssets()->getEmbeddedAssetsManager());

			rpg3D::playerUnit::LBGeneralActiveTools* lBGeneralActiveTools = new rpg3D::playerUnit::LBGeneralActiveTools();
			entity_controller->registerLogicBlock(lBGeneralActiveTools, rpg3D::playerUnit::ILBGeneralActiveTools::ID, static_cast<rpg3D::playerUnit::ILBGeneralActiveTools*>(lBGeneralActiveTools));

			entity_controller->setSingleActiveGroupLogicBlock(rpg3D::playerUnit::ILBGeneralActiveTools::GroupID, rpg3D::playerUnit::ILBGeneralActiveTools::ID);
		}, false/*isActive*//*the entity is activated by the zone start*/
	);

	gw->getGPContent()->getPlayerEntitiesGroup()->addEntity(entity);
	gw->getGPContent()->setIsSelectorEnabled(true);
	gw->getGPContent()->setSelectManagedPlayerEntity(entity);

	gw->getSimAs<base::GamePartSimExtendable*>()->setCameraFollowTarget(gw->getContent()->getSelectedPlayerEntity(true/*mustExist*/));
}

GamePartInit::~GamePartInit() {
	//void
}
