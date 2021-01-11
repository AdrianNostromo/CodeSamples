#include "ContentZoneMain.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/EntityTemplatesList.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/EntityIds.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/ScenarioFactory.h>
#include <sh/scenarios/randDungeons_a_v1/gw/config/ScenarioFactory.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/ScenarioFactory.h>
#include <sh/scenarios/spaceShipMissions_a_v1/gw/config/ScenarioFactory.h>
#include <sh/scenarios/islandSurvival_a_v1/gw/config/ScenarioFactory.h>
#include <sh/scenarios/gridAdventure_a_v1/ScenarioFactory.h>
#include <sh/scenarios/squareTactics_a_v1/ScenarioFactory.h>
#include <sh/scenarios/roomgeons_a_v1/ScenarioFactory.h>
#include <rpg3D/gw/config/team/TeamsIndexesList.h>
#include "../../zoneMain/localCacheContent/GamePartLocalCacheContent.h"
#include "../../../Namespace.h"
#include <base/statics/StaticsInit.h>
#include <worldGame3D/scenariotemplate/ScenarioTemplate.h>
#include "../ScenarioFactory.h"
#include "../../view/mainui/MainInGameUIView.h"
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/config/entity/text/Template_Text_Custom.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <worldGame3D/scripts/ScriptCommands.h>

using namespace startarea_a_v1;
using namespace startarea_a_v1::Main;

std::string ContentZoneMain::NAME = "Main";

std::shared_ptr<worldGame3D::ContentZone> ContentZoneMain::FactoryFunction() {
	std::shared_ptr<worldGame3D::ContentZone> ret = std::make_shared<ContentZoneMain>();

	return ret;
}

ContentZoneMain::ContentZoneMain()
	: super(NAME)
{
	//void
}

int ContentZoneMain::getMainUIViewID() {
	return IMainInGameUIView::ID;
}

rpg3D::MainInGameUIView* ContentZoneMain::getNewMainUIViewUninitialised(base::IGameWorld* gw) {
	MainInGameUIView* ret = new MainInGameUIView(gw->getHandler()->getApp(), gw);

	return ret;
}

void ContentZoneMain::onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivated(zoneStartExtraData, gw);
	
	// Create the zone specific game parts.
	gw->addGamePartUninitialised(new GamePartLocalCacheContent(gw));

	int eId = 0;
	int W = eId++;// Water
	int G = eId++;// Grass

	int T = eId++;// Tree
	int S = eId++;// Start position platform

	int v_creatureCreator = eId++;
	int c = v_creatureCreator;

	//asd;// not used currently.
	int b = eId++;// Platform Battle
	int d = eId++;// Teleporter - Rogue Zone
	int v_teleporterRogueZone = d;

	int v_smallFLowerA = eId++;
	int e = v_smallFLowerA;// Teleporter - Rogue Zone

	Array2D<int> elementsGrid(
		20, 20,
		false, true,
		new int[20 * 20]{
			W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
			W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
			W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
			W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
			W,W,W,W,W,W,W,W,W,W,W,W,T,T,T,W,W,W,W,W,
			W,W,W,W,W,W,W,W,T,T,T,T,T,T,T,T,W,W,W,W,
			W,W,W,W,W,W,W,W,T,T,G,G,G,G,T,T,W,W,W,W,
			W,W,W,W,W,W,W,W,T,G,c,G,G,G,T,T,W,W,W,W,
			W,W,W,W,W,W,W,W,T,G,G,G,G,G,T,W,W,W,W,W,
			W,W,W,W,W,W,W,W,T,G,G,G,G,G,T,W,W,W,W,W,
			W,W,W,W,W,T,T,T,T,G,G,G,G,T,T,W,W,W,W,W,
			W,W,W,W,W,T,e,G,G,T,T,T,T,T,T,W,W,W,W,W,
			W,W,W,W,W,T,G,S,G,T,W,W,W,W,W,W,W,W,W,W,
			W,W,W,W,W,T,G,G,e,T,W,W,W,W,W,W,W,W,W,W,
			W,W,W,W,W,T,T,T,T,T,W,W,W,W,W,W,W,W,W,W,
			W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
			W,W,W,W,W,W,W,W,W,W,W,d,b,W,W,W,W,W,W,W,
			W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
			W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
			W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W
		}
	);

	Vector3 playerSpawnGPos{};
	bool isPlayerPosOnGrid = false;

	for (int gx = 0; gx < elementsGrid.getLengthA(); gx++) {
		for (int gy = 0; gy < elementsGrid.getLengthB(); gy++) {
			int val = elementsGrid.getDirect(gx, gy);

			Array3D<signed char>* voxelsGrid = rpg3D::TestVoxelStructureComponents::groundPatchGrid;
			ArrayList<Visual3DModelMaterial*>* materialsList;
			float posZ;
			if (val == W) {
				posZ = -0.1f;

				materialsList = rpg3D::TestVoxelStructureComponents::waterAMaterialsList;
			} else {
				posZ = 0.0f;

				if (((gx + gy) % 2) == 0) {
					materialsList = rpg3D::TestVoxelStructureComponents::grassAMaterialsList;
				} else {
					materialsList = rpg3D::TestVoxelStructureComponents::grassBMaterialsList;
				}
			}

			createGroundPatch(
				-0.5f + gx,
				-0.5f + gy,
				posZ,
				voxelsGrid,
				materialsList,
				gw
			);

			EntityTemplate* entityTemplate = nullptr;
			std::shared_ptr<std::string> instanceId = nullptr;
			std::shared_ptr<std::string> platformActivationScript = nullptr;
			float rotZ = 0.0f;

			if (val == T) {
				entityTemplate = EntityTemplatesList::Flora_TreeA;
			} else if (val == S) {
				entityTemplate = EntityTemplatesList::Doodad_PlayerSpawnIndicatorA;
				playerSpawnGPos.set(gx, gy, 0.0f);
				isPlayerPosOnGrid = true;
			} else if (val == v_creatureCreator) {
				entityTemplate = EntityTemplatesList::Platform_CreatureCreatorPad;
				instanceId = EntityIds::platformCreatureCreator;
			} else if (val == b) {
				entityTemplate = EntityTemplatesList::Platform_MatchFindA;
			} else if (val == d) {
				entityTemplate = EntityTemplatesList::Platform_TeleporterA;
			} else if (val == v_smallFLowerA) {
				entityTemplate = EntityTemplatesList::Flora_SmallFlower_A_V1;

				rotZ = 0.0f;
			}

			if (entityTemplate != nullptr) {
				createConfigEntity(
					gx, gy, posZ,
					rotZ,
					entityTemplate,
					instanceId, platformActivationScript,
					gw
				);

				if (val == v_creatureCreator) {
					EntityTemplate* t = rpg3D::Template_Text_Custom::GetNewOrExistingTemplate("CREATURE\r   CREATOR", 0.05f/*voxelSize*/);

					createConfigEntity(
						gx - 0.61f, gy - 0.61f, posZ,
						-45.0f,
						t,
						nullptr/*instanceId*/, nullptr/*platformActivationScript*/,
						gw
					);
				} else if (val == v_teleporterRogueZone) {
					createConfigEntity(
						gx - 0.61f, gy - 0.61f, posZ,
						-45.0f,
						EntityTemplatesList::Text_TeleporterRogueZone,
						nullptr/*instanceId*/, nullptr/*platformActivationScript*/,
						gw
					);
				}
			}
		}
	}
	if (!isPlayerPosOnGrid) {
		throw LogicException(LOC);
	}

	// Create the dev teleporters (9 simple pads with numbers).
	createDevContent(gw, playerSpawnGPos.x, playerSpawnGPos.y);

	gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->setPos(playerSpawnGPos);

	IMovingEntityModule* movingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	movingEntity->addAction_teleportToDestination(playerSpawnGPos);

	gw->getComponentAs<IGamePartLocalCacheContent*>(true/*mustExist*/)->loadAllPersistentEntities();
}

void ContentZoneMain::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);

	gw->disposeGamePart<IGamePartLocalCacheContent*>();
}

void ContentZoneMain::createDevContent(base::IGameWorld* gw, float focusPosX, float focusPosY) {
	struct PlatformInfo {
		float posX;
		float posY;

		std::string scenarioTeplateId;
		std::string scenarioLabel;

		explicit PlatformInfo(float posX, float posY, std::string scenarioTeplateId, std::string scenarioLabel)
			: posX(posX), posY(posY), scenarioTeplateId(scenarioTeplateId), scenarioLabel(scenarioLabel)
		{}
	};

	ArrayList<PlatformInfo> platformInfos{};
	// Left side entries
	int gIndex = 0;
	
	ArrayList<std::string> devScenariosStartStrings{};
	ArrayList<std::string> labelTemplateIdsListsList{};
	
	platformInfos.append_emplace(
		1.0f + (gIndex % 6) * 1.0f - Math::floor(gIndex / 6.0f) * 1.2f,
		6.0f - (gIndex % 6) * 1.0f - Math::floor(gIndex / 6.0f) * 1.2f,
		randDungeons_a_v1::ScenarioFactory::scenarioTemplate->id,
		"RANDOM\rDUNGEONS"
	);
	gIndex++;
	
	platformInfos.append_emplace(
		1.0f + (gIndex % 6) * 1.0f - Math::floor(gIndex / 6.0f) * 1.2f,
		6.0f - (gIndex % 6) * 1.0f - Math::floor(gIndex / 6.0f) * 1.2f,
		towerDefense_a_v1::ScenarioFactory::scenarioTemplate->id,
		"TURRET\rDEFENSE"
	);
	gIndex++;
	
	platformInfos.append_emplace(
		1.0f + (gIndex % 6) * 1.0f - Math::floor(gIndex / 6.0f) * 1.2f,
		6.0f - (gIndex % 6) * 1.0f - Math::floor(gIndex / 6.0f) * 1.2f,
		spaceShipMissions_a_v1::ScenarioFactory::scenarioTemplate->id,
		"SPACE\rMISSIONS"
	);
	gIndex++;
	
	platformInfos.append_emplace(
		1.0f + (gIndex % 6) * 1.0f - Math::floor(gIndex / 6.0f) * 1.2f,
		6.0f - (gIndex % 6) * 1.0f - Math::floor(gIndex / 6.0f) * 1.2f,
		islandSurvival_a_v1::ScenarioFactory::scenarioTemplate->id,
		"ISLAND\rSURVIVAL"
	);
	gIndex++;
	
	platformInfos.append_emplace(
		1.0f + (gIndex % 6) * 1.0f - Math::floor(gIndex / 6.0f) * 1.2f,
		6.0f - (gIndex % 6) * 1.0f - Math::floor(gIndex / 6.0f) * 1.2f,
		gridAdventure_a_v1::ScenarioFactory::scenarioTemplate->id,
		"Grid\nAdventure"
	);
	gIndex++;
	
	platformInfos.append_emplace(
		1.0f + (gIndex % 6) * 1.0f - Math::floor(gIndex / 6.0f) * 1.2f,
		6.0f - (gIndex % 6) * 1.0f - Math::floor(gIndex / 6.0f) * 1.2f,
		squareTactics_a_v1::ScenarioFactory::scenarioTemplate->id,
		"Square\nTactics"
	);
	gIndex++;

	platformInfos.append_emplace(
		1.0f + (gIndex % 6) * 1.0f - Math::floor(gIndex / 6.0f) * 1.2f,
		6.0f - (gIndex % 6) * 1.0f - Math::floor(gIndex / 6.0f) * 1.2f,
		battlezones_a_v1::ScenarioFactory::scenarioTemplate->id,
		"Battle\nZones"
	);
	gIndex++;

	platformInfos.append_emplace(
		1.0f + (gIndex % 6) * 1.0f - Math::floor(gIndex / 6.0f) * 1.2f,
		6.0f - (gIndex % 6) * 1.0f - Math::floor(gIndex / 6.0f) * 1.2f,
		roomgeons_a_v1::ScenarioFactory::scenarioTemplate->id,
		"Room\nGeons"
	);
	gIndex++;
	
	for (int i = 0; i < platformInfos.count(); i++) {
		PlatformInfo& platformInfo = platformInfos.getReference(i);
		
		std::shared_ptr<std::string> platformActivationScript = nullptr;
		platformActivationScript = std::make_shared<std::string>(
			worldGame3D::ScriptCommands::cmd_setScenarioAndZone + worldGame3D::ScriptCommands::commandPartsDelimiter
			+ platformInfo.scenarioTeplateId
		);
		
		createConfigEntity(
			platformInfo.posX, platformInfo.posY, 0.0f,
			0.0f/*rotZ*/,
			EntityTemplatesList::Platform_TeleporterDevA,
			nullptr/*instanceId*/, 
			platformActivationScript/*platformActivationScript*/, gw
		);

		EntityTemplate* t = rpg3D::Template_Text_Custom::GetNewOrExistingTemplate(platformInfo.scenarioLabel, 0.025f/*voxelSize*/);
		createConfigEntity(
			platformInfo.posX - 0.61f, platformInfo.posY - 0.61f, 0.0f,
			-45.0f/*rotZ*/,
			t,
			nullptr/*instanceId*/, nullptr/*platformActivationScript*/,
			gw
		);
	}
}

void ContentZoneMain::createGroundPatch(
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

void ContentZoneMain::createConfigEntity(
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
	(void)entity;
}

ContentZoneMain::~ContentZoneMain() {
	//void
}
