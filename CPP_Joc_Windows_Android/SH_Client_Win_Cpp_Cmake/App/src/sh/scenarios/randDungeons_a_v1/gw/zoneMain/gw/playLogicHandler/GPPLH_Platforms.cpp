#include "GPPLH_Platforms.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/module/controller/ControllerModule.h>
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <rpg3D/gw/entity/module/movingEntity/IMovingEntityModule.h>
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include "../../../zoneRDungeonA/dungeonContent/DungeonContent.h"
#include "../../../zoneRDungeonA/dungeonGenerator/DungeonGenerator.h"
#include "../../../zoneRDungeonA/dungeonGenerator/config/CfgDungeonGenerator.h"
#include "../../../zoneRDungeonA/config/ContentZoneRDungeonA.h"
#include "../../../zoneRDungeonA/config/util/ZoneStartExtraData.h"
#include "../../../zonePrefabRoomsDungeonA/dungeonGenerator/DungeonGenerator.h"
#include "../../../zonePrefabRoomsDungeonA/dungeonGenerator/config/CfgDungeonGenerator.h"
#include <rpg3D/gw/ui/GPUIMainUIView.h>
#include <worldGame3D/gw/handler/GWEventDataBase.h>
#include <worldGame3D/gw/contentZones/IGamePartContentZones.h>
#include <memory>
#include "../../../zonePrefabRoomsDungeonA/config/ContentZonePrefabRoomsDungeonA.h"
#include "../../../zonePrefabRoomsDungeonA/config/util/ZoneStartExtraData.h"
#include "../../../zonePrefabRoomsDungeonA/prefab/PrefabRoom.h"
#include <rpg3D/gw/manager/IGPManager.h>
#include "../../config/ContentZoneMain.h"
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/structure/T_Floor_A_TiledX6dmY6dm.h>
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/structure/T_Wall_A_TiledX6dm.h>
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/structure/T_Doorway_A_VariableX1dm.h>
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/platform/T_Platform_ExitStairs.h>
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/platform/T_Platform_StairsDown.h>
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/platform/T_Platform_StairsUp.h>
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/mob/Template_Mob_General_A_V1.h>

#include "../../../zoneTurnBasedRDungeonA/dungeonContent/DungeonContent.h"
#include "../../../zoneTurnBasedRDungeonA/dungeonGenerator/DungeonGenerator.h"
#include "../../../zoneTurnBasedRDungeonA/dungeonGenerator/config/CfgDungeonGenerator.h"
#include "../../../zoneTurnBasedRDungeonA/config/ContentZoneTurnBasedRDungeonA.h"
#include "../../../zoneTurnBasedRDungeonA/config/util/ZoneStartExtraData.h"

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::Main;

GPPLH_Platforms::GPPLH_Platforms(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		rpg3D::IGPManager::GWEvent_onPlayerActivatedPlatformNonSpawn->type,
		std::bind(&GPPLH_Platforms::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPPLH_Platforms::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if (event.type == rpg3D::IGPManager::GWEvent_onPlayerActivatedPlatformNonSpawn->type) {
		if (event.getEntityData()->getInstanceId() == EntityIds::enterRandomDungeonA) {
			// Generate and activate a new dungeon.
			std::shared_ptr<RDungeonA::CfgDungeonGenerator> configDungeonGenerator = std::make_shared<RDungeonA::CfgDungeonGenerator>();
			configDungeonGenerator->floor_tileableTemplate_gridXY = T_Floor_A_TiledX6dmY6dm::TEMPLATE;
			configDungeonGenerator->floor_tileVoxelsCount.set(T_Floor_A_TiledX6dmY6dm::TileVoxelsCount);
			configDungeonGenerator->floor_materialsList = T_Floor_A_TiledX6dmY6dm::materialsList;
			configDungeonGenerator->floor_extraDataId_materialsList = T_Floor_A_TiledX6dmY6dm::DATA_KEY_materials;
			configDungeonGenerator->floor_tilesConfigList = std::make_shared<ArrayList<graphics::TilingInformation::TileConfig>>();
			configDungeonGenerator->floor_tilesConfigList->append_emplace(T_Floor_A_TiledX6dmY6dm::gridA);
			configDungeonGenerator->floor_tilesConfigList->append_emplace(T_Floor_A_TiledX6dmY6dm::gridB);
			
			configDungeonGenerator->wall_tileableTemplate_linearX = T_Wall_A_TiledX6dm::TEMPLATE;
			configDungeonGenerator->wall_tileVoxelsCount.set(T_Wall_A_TiledX6dm::TileVoxelsCount);
			configDungeonGenerator->wall_materialsList = T_Wall_A_TiledX6dm::materialsList;
			configDungeonGenerator->wall_extraDataId_materialsList = T_Wall_A_TiledX6dm::DATA_KEY_materials;
			configDungeonGenerator->wall_tileConfig = std::make_shared<graphics::TilingInformation::TileConfig>(
				T_Wall_A_TiledX6dm::grid
			);

			configDungeonGenerator->templateFactory_door = T_Doorway_A_VariableX1dm::getOrCreate;
			
			configDungeonGenerator->templateIdentifier_teleporter_exitStairs.set(T_Platform_ExitStairs::TEMPLATE->nSpace, T_Platform_ExitStairs::TEMPLATE->id);
			configDungeonGenerator->templateIdentifier_teleporter_stairsDown.set(T_Platform_StairsDown::TEMPLATE->nSpace, T_Platform_StairsDown::TEMPLATE->id);
			configDungeonGenerator->templateIdentifier_teleporter_stairsUp.set(T_Platform_StairsUp::TEMPLATE->nSpace, T_Platform_StairsUp::TEMPLATE->id);
			
			configDungeonGenerator->templateIdentifier_mob.set(Template_Mob_General_A_V1::TEMPLATE->nSpace, Template_Mob_General_A_V1::TEMPLATE->id);

			RDungeonA::DungeonGenerator dungeonGenerator{};
			dungeonGenerator.generate(configDungeonGenerator, Math::Rand::nextInt());
			std::shared_ptr< RDungeonA::DungeonContent> dungeonContent = dungeonGenerator.getDungeonContent();
			if (dungeonContent->entryPointLevelZonIndex < 0 || dungeonContent->entryPointPosEntityInstanceId == nullptr) {
				throw LogicException(LOC);
			}

			std::shared_ptr<RDungeonA::ZoneStartExtraData> zoneStartExtraData(new RDungeonA::ZoneStartExtraData(
				dungeonContent->entryPointPosEntityInstanceId, nullptr/*teleportTargetOffset*/,
				dungeonContent, dungeonContent->entryPointLevelZonIndex/*levelZoneToActivate*/,
				std::make_shared<std::string>(ContentZoneMain::NAME)/*exitZoneName*/, EntityIds::enterRandomDungeonA/*exitZonePlayerPosEntityId*/,
				std::make_shared<std::string>(ContentZoneMain::NAME)/*zeroHpZoneName*/, EntityIds::enterRandomDungeonA/*zeroHpZonePlayerPosEntityId*/
			));
			
			worldGame3D::IGamePartContentZones* gpContentZones = gw->getComponentAs<worldGame3D::IGamePartContentZones*>(true/*mustExist*/);
			gpContentZones->setActiveZone(
				RDungeonA::ContentZoneRDungeonA::NAME, zoneStartExtraData, true/*restartZoneIfActive*/,
				true/*useDelayedOnLoopActivation*/
			);
		}else if (event.getEntityData()->getInstanceId() == EntityIds::enterTurnBasedRandomDungeonA) {
			// Generate and activate a new dungeon.
			std::shared_ptr<TurnBasedRDungeonA::CfgDungeonGenerator> configDungeonGenerator = std::make_shared<TurnBasedRDungeonA::CfgDungeonGenerator>();
			configDungeonGenerator->floor_tileableTemplate_gridXY = T_Floor_A_TiledX6dmY6dm::TEMPLATE;
			configDungeonGenerator->floor_tileVoxelsCount.set(T_Floor_A_TiledX6dmY6dm::TileVoxelsCount);
			configDungeonGenerator->floor_materialsList = T_Floor_A_TiledX6dmY6dm::materialsList;
			configDungeonGenerator->floor_extraDataId_materialsList = T_Floor_A_TiledX6dmY6dm::DATA_KEY_materials;
			configDungeonGenerator->floor_tilesConfigList = std::make_shared<ArrayList<graphics::TilingInformation::TileConfig>>();
			configDungeonGenerator->floor_tilesConfigList->append_emplace(T_Floor_A_TiledX6dmY6dm::gridA);
			configDungeonGenerator->floor_tilesConfigList->append_emplace(T_Floor_A_TiledX6dmY6dm::gridB);
			
			configDungeonGenerator->wall_tileableTemplate_linearX = T_Wall_A_TiledX6dm::TEMPLATE;
			configDungeonGenerator->wall_tileVoxelsCount.set(T_Wall_A_TiledX6dm::TileVoxelsCount);
			configDungeonGenerator->wall_materialsList = T_Wall_A_TiledX6dm::materialsList;
			configDungeonGenerator->wall_extraDataId_materialsList = T_Wall_A_TiledX6dm::DATA_KEY_materials;
			configDungeonGenerator->wall_tileConfig = std::make_shared<graphics::TilingInformation::TileConfig>(
				T_Wall_A_TiledX6dm::grid
			);

			configDungeonGenerator->templateFactory_door = T_Doorway_A_VariableX1dm::getOrCreate;
			
			configDungeonGenerator->templateIdentifier_teleporter_exitStairs.set(T_Platform_ExitStairs::TEMPLATE->nSpace, T_Platform_ExitStairs::TEMPLATE->id);
			configDungeonGenerator->templateIdentifier_teleporter_stairsDown.set(T_Platform_StairsDown::TEMPLATE->nSpace, T_Platform_StairsDown::TEMPLATE->id);
			configDungeonGenerator->templateIdentifier_teleporter_stairsUp.set(T_Platform_StairsUp::TEMPLATE->nSpace, T_Platform_StairsUp::TEMPLATE->id);
			
			configDungeonGenerator->templateIdentifier_mob.set(Template_Mob_General_A_V1::TEMPLATE->nSpace, Template_Mob_General_A_V1::TEMPLATE->id);

			TurnBasedRDungeonA::DungeonGenerator dungeonGenerator{};
			dungeonGenerator.generate(configDungeonGenerator, Math::Rand::nextInt());
			std::shared_ptr< TurnBasedRDungeonA::DungeonContent> dungeonContent = dungeonGenerator.getDungeonContent();
			if (dungeonContent->entryPointLevelZonIndex < 0 || dungeonContent->entryPointPosEntityInstanceId == nullptr) {
				throw LogicException(LOC);
			}

			std::shared_ptr<TurnBasedRDungeonA::ZoneStartExtraData> zoneStartExtraData(new TurnBasedRDungeonA::ZoneStartExtraData(
				dungeonContent->entryPointPosEntityInstanceId, nullptr/*teleportTargetOffset*/,
				dungeonContent, dungeonContent->entryPointLevelZonIndex/*levelZoneToActivate*/,
				std::make_shared<std::string>(ContentZoneMain::NAME)/*exitZoneName*/, EntityIds::enterTurnBasedRandomDungeonA/*exitZonePlayerPosEntityId*/,
				std::make_shared<std::string>(ContentZoneMain::NAME)/*zeroHpZoneName*/, EntityIds::enterTurnBasedRandomDungeonA/*zeroHpZonePlayerPosEntityId*/
			));
			
			worldGame3D::IGamePartContentZones* gpContentZones = gw->getComponentAs<worldGame3D::IGamePartContentZones*>(true/*mustExist*/);
			gpContentZones->setActiveZone(
				TurnBasedRDungeonA::ContentZoneTurnBasedRDungeonA::NAME, zoneStartExtraData, true/*restartZoneIfActive*/,
				true/*useDelayedOnLoopActivation*/
			);
		}else if (event.getEntityData()->getInstanceId() == EntityIds::enterPrefabRoomsDungeonA) {
			static ArrayList<PrefabRoomsDungeonA::PrefabRoom>* TestPrefabRooms = nullptr;
			if (TestPrefabRooms == nullptr) {
				TestPrefabRooms = new ArrayList<PrefabRoomsDungeonA::PrefabRoom>{};
				{
					int roomGSizeX = 7;
					int roomGSizeY = 7;

					static const signed char O = -1;
					static const signed char A = 0;
					static const signed char B = 1;
					PrefabRoomsDungeonA::PrefabRoom* roomPrefab = TestPrefabRooms->append_emplace();
					roomPrefab->floorTilesGrid = std::make_shared<Array2D<signed char>>(roomGSizeX, roomGSizeY, false/*invertA*/, true/*invertB*/, new signed char[roomGSizeX * roomGSizeY]{
						A,B,O,O,O,B,A,
						B,A,B,A,B,A,B,
						O,B,A,B,A,B,O,
						O,A,B,A,B,A,O,
						O,B,A,B,A,B,O,
						B,A,B,A,B,A,B,
						A,B,O,O,O,B,A
						});

					roomPrefab->tileSize_cm = 60;

					roomPrefab->teleporterGPosList.append_emplace(3.5f, 3.5f);

					roomPrefab->floor_tileableTemplate_gridXY = T_Floor_A_TiledX6dmY6dm::TEMPLATE;
					roomPrefab->floor_tileVoxelsCount.set(T_Floor_A_TiledX6dmY6dm::TileVoxelsCount);
					roomPrefab->floor_materialsList = T_Floor_A_TiledX6dmY6dm::materialsList;
					roomPrefab->floor_extraDataId_materialsList = T_Floor_A_TiledX6dmY6dm::DATA_KEY_materials;
					roomPrefab->floor_tilesConfigList = std::make_shared<ArrayList<graphics::TilingInformation::TileConfig>>();
					roomPrefab->floor_tilesConfigList->append_emplace(T_Floor_A_TiledX6dmY6dm::gridA);
					roomPrefab->floor_tilesConfigList->append_emplace(T_Floor_A_TiledX6dmY6dm::gridB);
				}
				{
					int roomGSizeX = 7;
					int roomGSizeY = 7;

					static const signed char O = -1;
					static const signed char A = 0;
					static const signed char B = 1;
					PrefabRoomsDungeonA::PrefabRoom* roomPrefab = TestPrefabRooms->append_emplace();
					roomPrefab->floorTilesGrid = std::make_shared<Array2D<signed char>>(roomGSizeX, roomGSizeY, false/*invertA*/, true/*invertB*/, new signed char[roomGSizeX * roomGSizeY]{
						O,O,A,B,A,O,O,
						O,A,B,A,B,A,O,
						A,B,A,B,A,B,A,
						B,A,B,A,B,A,B,
						A,B,A,B,A,B,A,
						O,A,B,A,B,A,O,
						O,O,A,B,A,O,O
					});

					roomPrefab->tileSize_cm = 60;

					roomPrefab->floor_tileableTemplate_gridXY = T_Floor_A_TiledX6dmY6dm::TEMPLATE;
					roomPrefab->floor_tileVoxelsCount.set(T_Floor_A_TiledX6dmY6dm::TileVoxelsCount);
					roomPrefab->floor_materialsList = T_Floor_A_TiledX6dmY6dm::materialsList;
					roomPrefab->floor_extraDataId_materialsList = T_Floor_A_TiledX6dmY6dm::DATA_KEY_materials;
					roomPrefab->floor_tilesConfigList = std::make_shared<ArrayList<graphics::TilingInformation::TileConfig>>();
					roomPrefab->floor_tilesConfigList->append_emplace(T_Floor_A_TiledX6dmY6dm::gridA);
					roomPrefab->floor_tilesConfigList->append_emplace(T_Floor_A_TiledX6dmY6dm::gridB);
				}
			}

			// Generate and activate a new dungeon.
			std::shared_ptr<PrefabRoomsDungeonA::CfgDungeonGenerator> configDungeonGenerator = std::make_shared<PrefabRoomsDungeonA::CfgDungeonGenerator>();
			configDungeonGenerator->zoneFloorsCount = 2;
			configDungeonGenerator->prefabRoomsList = TestPrefabRooms;

			configDungeonGenerator->templateIdentifier_teleporter_exitStairs.set(T_Platform_ExitStairs::TEMPLATE->nSpace, T_Platform_ExitStairs::TEMPLATE->id);
			configDungeonGenerator->templateIdentifier_teleporter_stairsDown.set(T_Platform_StairsDown::TEMPLATE->nSpace, T_Platform_StairsDown::TEMPLATE->id);
			configDungeonGenerator->templateIdentifier_teleporter_stairsUp.set(T_Platform_StairsUp::TEMPLATE->nSpace, T_Platform_StairsUp::TEMPLATE->id);

			configDungeonGenerator->templateIdentifier_mob.set(Template_Mob_General_A_V1::TEMPLATE->nSpace, Template_Mob_General_A_V1::TEMPLATE->id);

			PrefabRoomsDungeonA::DungeonGenerator dungeonGenerator{};
			dungeonGenerator.generate(configDungeonGenerator, Math::Rand::nextInt());
			std::shared_ptr< PrefabRoomsDungeonA::DungeonContent> dungeonContent = dungeonGenerator.getDungeonContent();
			if (dungeonContent->entryPointLevelZonIndex < 0 || dungeonContent->entryPointPosEntityInstanceId == nullptr) {
				throw LogicException(LOC);
			}

			std::shared_ptr<PrefabRoomsDungeonA::ZoneStartExtraData> zoneStartExtraData(new PrefabRoomsDungeonA::ZoneStartExtraData(
				dungeonContent->entryPointPosEntityInstanceId, nullptr/*teleportTargetOffset*/,
				dungeonContent, dungeonContent->entryPointLevelZonIndex/*levelZoneToActivate*/,
				std::make_shared<std::string>(ContentZoneMain::NAME)/*exitZoneName*/, EntityIds::enterPrefabRoomsDungeonA/*exitZonePlayerPosEntityId*/,
				std::make_shared<std::string>(ContentZoneMain::NAME)/*zeroHpZoneName*/, EntityIds::enterPrefabRoomsDungeonA/*zeroHpZonePlayerPosEntityId*/
			));

			worldGame3D::IGamePartContentZones* gpContentZones = gw->getComponentAs<worldGame3D::IGamePartContentZones*>(true/*mustExist*/);
			gpContentZones->setActiveZone(
				PrefabRoomsDungeonA::ContentZonePrefabRoomsDungeonA::NAME, zoneStartExtraData, true/*restartZoneIfActive*/,
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

GPPLH_Platforms::~GPPLH_Platforms() {
	//void
}
