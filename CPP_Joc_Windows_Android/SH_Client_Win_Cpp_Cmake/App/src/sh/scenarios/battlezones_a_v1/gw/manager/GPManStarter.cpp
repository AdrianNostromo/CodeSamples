#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityTemplatesList.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityIds.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include <worldGame3D/gw/util/GameCameraModeBitGroups.h>
#include "GPManStarter.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <sh/scenarios/battlezones_a_v1/gw/zone/ZoneTemplatesList.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/module/toolsHandler/ToolsHandlerModule.h>
#include <rpg3D/gw/config/entity/unit/UnitTemplates_PlayerA.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/rangeInertialProjectile/ToolRangeInertialProjectile.h>
#include <rpg3D/gw/userEntitiesController/util/EntityControlType.h>
#include <sh/scenarios/battlezones_a_v1/gw/sim/GamePartSim.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/projectile/ProjectileConfigsList.h>
#include <rpg3D/gw/entity/module/controller/ControllerModule.h>
#include <rpg3D/gw/ui/IGamePartUserInterface.h>
#include <graphics/environment/environment3D/cinematic/ICinematicController.h>
#include <rpg3D/gw/ui/IGamePartUserInterface.h>
#include <worldGame3D/gw/handler/IHandler_GameWorld.h>
#include "../view/mainui/MainInGameUIView.h"
#include <base/opengl/util/GLUtil.h>
#include <rpg3D/gw/userEntitiesController/IGPUserEntitiesController.h>
#include <worldGame3D/gw/main/IGamePartMain.h>

using namespace battlezones_a_v1;

GPManStarter::GPManStarter(base::IGameWorld* gw)
    : super(gw)
{
    //void
}

void GPManStarter::gwInitFinished() {
	super::gwInitFinished();

	// Create the view. This scenario doesn't use zones so it has to st the view class manually.
	rpg3D::IGamePartUserInterface* gpUI = gw->getComponentAs<rpg3D::IGamePartUserInterface*>(true/*mustExist*/);
	MainInGameUIView* v = new MainInGameUIView(gw->getHandler()->getApp(), gw);
	gpUI->setNewMainUIViewUninitialised(v);

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

	gw->getGPSim()->setCameraMode(GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation, false);

	gw->getComponentAs<rpg3D::IGPUserEntitiesController*>(true/*mustExist*/)->setEntityControlType(rpg3D::EntityControlType::DPadCameraAxesDirectionMove_LookAtCursorOrAutoRotToMoveDirection);

	// Enable the user interface.
	gw->getComponentAs<rpg3D::IGamePartUserInterface*>(true/*mustExist*/)->showViewIncremental();

	// Create the Nexus zone util and push it.
	generateNexusZoneConfig();

	setActiveZone(
		GLOBAL_ZONES_GROUP_ID, ZONE_ID_NEXUS,
		EntityIds::platform_Teleporter_Home,
		false,
		false
	);

	Color col{ "#64B0FFFF" };
	GLUtil::gl->setCachedClearColor(col);
}

void GPManStarter::generateNexusZoneConfig() {
	ZoneTemplate* zoneTemplate = ZoneTemplatesList::Nexus_A_V1;

	ZoneConfig* zoneConfig = createNewZoneConfig(GLOBAL_ZONES_GROUP_ID, ZONE_ID_NEXUS);
	zoneConfig->zoneTemplateId = zoneTemplate->id;

	//Validate if needed teleporters exist in the template.
	if(zoneTemplate->getEnvironmentEntityWithInstanceId(EntityIds::platform_zonesGroupStart) == nullptr
	   || zoneTemplate->getEnvironmentEntityWithInstanceId(EntityIds::defaultSpawnLocation) == nullptr)
	{
		// Template doesn't contain the needed teleporters.
		throw LogicException(LOC);
	}

	// Set the 2 teleporters as needed.
	zoneConfig->addTeleporterTarget(
		*EntityIds::platform_zonesGroupStart,
		GENERATED_ZONES_GROUP_ID, std::to_string(0), EntityIds::platform_zoneTeleporter_previous
	);
}

GPManStarter::~GPManStarter() {
    //void
}
