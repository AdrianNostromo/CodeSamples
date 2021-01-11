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
#include <sh/scenarios/zoneAlert_a_v1/gw/czCommon/config/util/ZoneStartExtraData.h>
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

using namespace zoneAlert_a_v1;
using namespace zoneAlert_a_v1::czTownA;

std::string CZ_TownA::NAME = "TownA";

std::shared_ptr<worldGame3D::ContentZone> CZ_TownA::FactoryFunction() {
	std::shared_ptr<worldGame3D::ContentZone> ret = std::make_shared<CZ_TownA>();

	return ret;
}

CZ_TownA::CZ_TownA()
	: super(NAME, true/*useAutoPlayerPositioning*/, EntityIds::defaultZoneEntrance/*defaultZoneEntrance*/)
{
	//void
}

void CZ_TownA::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if (event.type == rpg3D::IGPManager::GWEvent_onPlayerActivatedPlatformNonSpawn->type) {
		if (event.getEntityData()->getInstanceId() == EntityIds::enterGridBattleA) {
			worldGame3D::IGamePartContentZones* gpContentZones = gw->getComponentAs<worldGame3D::IGamePartContentZones*>(true/*mustExist*/);

			sp<Vector3> teleportTargetOffset = new Vector3(gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getPos());
			teleportTargetOffset->sub(event.getEntityData()->getPos());

			std::shared_ptr<czCommon::ZoneStartExtraData> zoneStartExtraData = std::make_shared<czCommon::ZoneStartExtraData>(
				EntityIds::defaultZoneEntrance/*teleportTargetEntityInstanceID*/, teleportTargetOffset/*teleportTargetOffset*/,
				nullptr/*zeroHpZoneName*/, nullptr/*zeroHpZonePlayerPosEntityId*/
			);

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

		// Create 3 beds.
		zoneContentData->entitiesInfosList.appendEmplace(
			rpg3D::TC_Bed_A_V1::GetOrCreateTemplate(10/*voxelSizeCM*/)/*entityTemplate*/,
			*pos.set((0.5f + 6.0f) * gridCellSizeM, (0.5f + 6.0f) * gridCellSizeM, 0.0f)/*posInRoom*/,
			*rot.set(Vector3::Z, 0.0f)/*rot*/,
			EntityIds::bed_0/*instanceId*/,
			nullptr/*materialsList*/, StringUtil::EMPTY/*extraDataId_materialsList*/,
			nullptr/*tilingInformation*/,
			nullptr/*platformActivationScript*/
		);
		zoneContentData->entitiesInfosList.appendEmplace(
			rpg3D::TC_Bed_A_V1::GetOrCreateTemplate(10/*voxelSizeCM*/)/*entityTemplate*/,
			*pos.set((0.5f + 6.0f) * gridCellSizeM, (0.5f + 6.0f) * gridCellSizeM, 0.0f)/*posInRoom*/,
			*rot.set(Vector3::Z, 0.0f)/*rot*/,
			EntityIds::bed_1/*instanceId*/,
			nullptr/*materialsList*/, StringUtil::EMPTY/*extraDataId_materialsList*/,
			nullptr/*tilingInformation*/,
			nullptr/*platformActivationScript*/
		);
		zoneContentData->entitiesInfosList.appendEmplace(
			rpg3D::TC_Bed_A_V1::GetOrCreateTemplate(10/*voxelSizeCM*/)/*entityTemplate*/,
			*pos.set((0.5f + 6.0f) * gridCellSizeM, (0.5f + 6.0f) * gridCellSizeM, 0.0f)/*posInRoom*/,
			*rot.set(Vector3::Z, 0.0f)/*rot*/,
			EntityIds::bed_2/*instanceId*/,
			nullptr/*materialsList*/, StringUtil::EMPTY/*extraDataId_materialsList*/,
			nullptr/*tilingInformation*/,
			nullptr/*platformActivationScript*/
		);
	}
	
	return zoneContentData;
}

void CZ_TownA::onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivated(zoneStartExtraData, gw);

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
}

void CZ_TownA::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);

	localAutoListenersList.clear();
}

CZ_TownA::~CZ_TownA() {
	//void
}
