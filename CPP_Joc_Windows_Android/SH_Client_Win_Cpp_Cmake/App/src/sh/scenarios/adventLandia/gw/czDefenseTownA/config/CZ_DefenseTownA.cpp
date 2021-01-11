#include "CZ_DefenseTownA.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include <base/statics/StaticsInit.h>
#include "../../../ScenarioFactory.h"
#include "../../../gw/config/entity/EntityIds.h"
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

#include <sh/scenarios/adventLandia/gw/config/entity/custom/T_UnitValidatorHaulerPlatform.h>
#include <sh/scenarios/adventLandia/gw/config/entity/platform/T_PlatformSlimeTeleporter.h>
#include <sh/scenarios/adventLandia/gw/config/entity/environment/T_Doorframe_A_V1.h>
#include <sh/scenarios/adventLandia/gw/config/entity/environment/T_Pillar_A_V1.h>
#include <sh/scenarios/adventLandia/gw/config/entity/environment/T_WallElement_A_V1.h>
#include <rpg3D/gw/entity/module/stackable/IStackableModule.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>

#include <base/view/snackbar/util/WrappedSnackbarView.h>
#include <base/view/snackbar/ISnackbarView.h>
#include <base/view/snackbar/util/Config.h>
#include <base/app/IApp.h>
#include <base/viewsHandler/IViewsHandler.h>
#include <rpg3D/gw/config/entity/platform/T_Platform_X.h>
#include "../../czTownA/config/CZ_TownA.h"

using namespace adventLandia;
using namespace adventLandia::czDefenseTownA;

std::string CZ_DefenseTownA::NAME = "DefenseTownA";

std::shared_ptr<worldGame3D::ContentZone> CZ_DefenseTownA::FactoryFunction() {
	std::shared_ptr<worldGame3D::ContentZone> ret = std::make_shared<CZ_DefenseTownA>();

	return ret;
}

CZ_DefenseTownA::CZ_DefenseTownA()
	: super(NAME, true/*useAutoPlayerPositioning*/, EntityIds::defaultZoneEntrance/*defaultZoneEntrance*/)
{
	// Include from other scenarios because this entire system will be converted to some database logic.
	if (adventLandia::T_PlatformSlimeTeleporter::TEMPLATE == nullptr) {
		throw LogicException(LOC);
	}
	(void)adventLandia::T_UnitValidatorHaulerPlatform::TEMPLATE;
	(void)adventLandia::T_Doorframe_A_V1::TEMPLATE;
	(void)adventLandia::T_Pillar_A_V1::TEMPLATE;
	(void)adventLandia::T_WallElement_A_V1::TEMPLATE;
}

void CZ_DefenseTownA::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	// Implement code when custom platforms are used.
	throw LogicException(LOC);
}

sp<rpg3D::ZoneContentData> CZ_DefenseTownA::CreateDefaultZoneContentData() {
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
				+ " spawnTarget " + *EntityIds::gotoDefenseTownATeleport + " keepTeleporterOffset"
				)/*platformActivationScript*/
		);
	}

	return zoneContentData;
}

void CZ_DefenseTownA::onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivated(zoneStartExtraData, gw);

	sp<rpg3D::ZoneContentData> zoneContentData = loadedZoneData;
	if (zoneContentData == nullptr) {
		zoneContentData = CreateDefaultZoneContentData();
	}

	// Create the zone specific game parts.
	gw->addEventListener(
		rpg3D::IGPManager::GWEvent_onPlayerActivatedPlatformNonSpawn->type,
		std::bind(&CZ_DefenseTownA::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		localAutoListenersList
	);

	zoneContentData->createContent(gw);

	// Give the player full hp. This is also done in case he loses all hp.
	rpg3D::ILivingEntityModule* livingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<rpg3D::ILivingEntityModule*>(true/*mustExist*/);
	livingEntity->setCurrentHP(livingEntity->getT()->maxHP);

	Color col{ "#64B0FFFF" };
	GLUtil::gl->setCachedClearColor(col);
}

void CZ_DefenseTownA::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);

	localAutoListenersList.clear();
}

CZ_DefenseTownA::~CZ_DefenseTownA() {
	//void
}
