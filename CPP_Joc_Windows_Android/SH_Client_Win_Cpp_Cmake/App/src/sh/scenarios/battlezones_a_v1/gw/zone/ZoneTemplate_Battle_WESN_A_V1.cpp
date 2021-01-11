#include "ZoneTemplate_Battle_WESN_A_V1.h"
#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityTemplatesList.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityIds.h>

using namespace battlezones_a_v1;

std::string ZoneTemplate_Battle_WESN_A_V1::FLAG_isNextZoneNexus = "isNextZoneNexus";
std::string ZoneTemplate_Battle_WESN_A_V1::FLAG_isPreviousZoneNexus = "isPreviousZoneNexus";
std::string ZoneTemplate_Battle_WESN_A_V1::FLAG_isZoneCleared = "isZoneCleared";

ZoneTemplate* ZoneTemplate_Battle_WESN_A_V1::getNew() {
	std::string zoneName = "Battle_WESN_A_V1";
    ZoneTemplate* t = new ZoneTemplate(zoneName);

    int id = 0;

    int v_empty = 0;
    int v_grass = ++id;
    int v_water = ++id;
    int v_tree = ++id;

    int v_zoneTeleport_previous = ++id;
    int v_zoneTeleport_next = ++id;

    int O = v_empty;

    int W = v_water;
    (void)W;
    int G = v_grass;

    int T = v_tree;

    int c = v_zoneTeleport_previous;
    int d = v_zoneTeleport_next;

    Array2D<int> elementsGrid(
        20, 20,
        false, true,
        new int[20 * 20]{
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,//0
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,T,T,T,T,T,T,T,O,O,O,O,O,O,O,
            O,O,O,O,T,T,T,T,T,T,T,T,T,T,T,O,O,O,O,O,//5
            O,O,O,O,T,G,G,G,G,G,G,G,G,d,T,O,O,O,O,O,
            O,O,O,T,T,G,G,G,G,G,G,G,G,G,T,T,O,O,O,O,
            O,O,O,T,T,G,G,G,G,G,G,G,G,G,T,T,O,O,O,O,
            O,O,O,T,T,G,G,G,G,G,G,G,G,G,T,T,O,O,O,O,
            O,O,O,T,T,G,G,G,G,G,G,G,G,G,T,T,O,O,O,O,//10
            O,O,O,T,T,G,G,G,G,G,G,G,G,G,T,T,O,O,O,O,
            O,O,O,T,T,G,G,G,G,G,G,G,G,G,T,T,O,O,O,O,
            O,O,O,T,T,G,G,G,G,G,G,G,G,G,T,T,O,O,O,O,
            O,O,O,O,T,c,G,G,G,G,G,G,G,G,T,O,O,O,O,O,
            O,O,O,O,T,T,T,T,T,T,T,T,T,T,T,O,O,O,O,O,//15
            O,O,O,O,O,O,T,T,T,T,T,T,T,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O//20
        }
    );

    for(int gx=0;gx<elementsGrid.getLengthA();gx++) {
        for(int gy=0;gy<elementsGrid.getLengthB();gy++) {
            int val = elementsGrid.getDirect(gx, gy);

            Array3D<signed char>* voxelsGrid = rpg3D::TestVoxelStructureComponents::groundPatchGrid;
            ArrayList<Visual3DModelMaterial*>* materialsList;
            Vector3 pos{
                -0.5f + gx,
                -0.5f + gy,
                0.0f,
            };

	        Quaternion rot{Vector3::Z, 0.0f};
            if(val == v_water) {
	            pos.z = -0.1f;

                materialsList = rpg3D::TestVoxelStructureComponents::waterAMaterialsList;
            }else if(val != v_empty) {
                pos.z = 0.0f;

                if(((gx + gy) % 2) == 0) {
                    materialsList = rpg3D::TestVoxelStructureComponents::grassAMaterialsList;
                }else {
                    materialsList = rpg3D::TestVoxelStructureComponents::grassBMaterialsList;
                }
            }else {
                continue;
            }

            t->groundPatchesList.append_emplace(
                EntityTemplatesList::Ground_BasicPatchA_1m->nSpace,
                EntityTemplatesList::Ground_BasicPatchA_1m->id,
                pos,
                rot,

                voxelsGrid,
                materialsList
            );

            std::shared_ptr<std::string> instanceId = nullptr;
            pos.set(gx, gy, 0.0f);
            std::string entityTemplateNamespace;
            std::string entityTemplateId;
	        rot.set(Vector3::Z, 0.0f);
            if(val == v_tree) {
                entityTemplateNamespace = EntityTemplatesList::Flora_TreeA->nSpace;
                entityTemplateId = EntityTemplatesList::Flora_TreeA->id;
            }else if(val == v_zoneTeleport_previous) {
				// Add states:
				// disabled_teleporter_previous_zone for flags {FLAG_isPreviousZoneNexus:false, FLAG_isZoneCleared:false}.
				entityTemplateNamespace = EntityTemplatesList::Doodad_BrokenZoneTeleporter_Stone_A_V1->nSpace;
				entityTemplateId = EntityTemplatesList::Doodad_BrokenZoneTeleporter_Stone_A_V1->id;
				instanceId = EntityIds::platform_zoneTeleporter_previous;
				rot.set(Vector3::Z, -135.0f);

				ZoneTemplate::EntitySpawnInfo* entitySpawnInfo = t->environmentEntitiesList.append_emplace(
					entityTemplateNamespace,
					entityTemplateId,
					pos,
					rot,
					instanceId
				);
				entitySpawnInfo->spawnTestFlags.setFlag(FLAG_isPreviousZoneNexus/*name*/, false/*value*/, true/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);
				entitySpawnInfo->spawnTestFlags.setFlag(FLAG_isZoneCleared/*name*/, false/*value*/, true/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);

				// teleporter_previous_zone for flags {FLAG_isPreviousZoneNexus:false, FLAG_isZoneCleared:true}.
				entityTemplateNamespace = EntityTemplatesList::Platform_ZoneTeleporter_Wood->nSpace;
				entityTemplateId = EntityTemplatesList::Platform_ZoneTeleporter_Wood->id;
				instanceId = EntityIds::platform_zoneTeleporter_previous;
				rot.set(Vector3::Z, -135.0f);

				entitySpawnInfo = t->environmentEntitiesList.append_emplace(
					entityTemplateNamespace,
					entityTemplateId,
					pos,
					rot,
					instanceId
				);
				entitySpawnInfo->spawnTestFlags.setFlag(FLAG_isPreviousZoneNexus/*name*/, false/*value*/, true/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);
				entitySpawnInfo->spawnTestFlags.setFlag(FLAG_isZoneCleared/*name*/, true/*value*/, false/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);

				// nexus_zone teleporter pad for flags {FLAG_isPreviousZoneNexus:true, FLAG_isZoneCleared:false}.
				entityTemplateNamespace = EntityTemplatesList::Doodad_BrokenNexusTeleporter_Stone_A_V1->nSpace;
				entityTemplateId = EntityTemplatesList::Doodad_BrokenNexusTeleporter_Stone_A_V1->id;
				instanceId = EntityIds::platform_zoneTeleporter_previous;
				rot.set(Vector3::Z, 45.0f);

				entitySpawnInfo = t->environmentEntitiesList.append_emplace(
					entityTemplateNamespace,
					entityTemplateId,
					pos,
					rot,
					instanceId
				);
				entitySpawnInfo->spawnTestFlags.setFlag(FLAG_isPreviousZoneNexus/*name*/, true/*value*/, false/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);
				entitySpawnInfo->spawnTestFlags.setFlag(FLAG_isZoneCleared/*name*/, false/*value*/, true/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);

				// nexus_zone teleporter pad for flags {FLAG_isPreviousZoneNexus:true, FLAG_isZoneCleared:true}.
				entityTemplateNamespace = EntityTemplatesList::Platform_NexusTeleporter_Wood_A_V1->nSpace;
				entityTemplateId = EntityTemplatesList::Platform_NexusTeleporter_Wood_A_V1->id;
				instanceId = EntityIds::platform_zoneTeleporter_previous;
				rot.set(Vector3::Z, 45.0f);

				entitySpawnInfo = t->environmentEntitiesList.append_emplace(
					entityTemplateNamespace,
					entityTemplateId,
					pos,
					rot,
					instanceId
				);
				entitySpawnInfo->spawnTestFlags.setFlag(FLAG_isPreviousZoneNexus/*name*/, true/*value*/, false/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);
				entitySpawnInfo->spawnTestFlags.setFlag(FLAG_isZoneCleared/*name*/, true/*value*/, false/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);

				continue;
            }else if(val == v_zoneTeleport_next) {
                // Add states:
                // disabled_teleporter_next_zone for flags {FLAG_isNextZoneNexus:false, FLAG_isZoneCleared:false}.
	            entityTemplateNamespace = EntityTemplatesList::Doodad_BrokenZoneTeleporter_Stone_A_V1->nSpace;
	            entityTemplateId = EntityTemplatesList::Doodad_BrokenZoneTeleporter_Stone_A_V1->id;
	            instanceId = EntityIds::platform_zoneTeleporter_next;
	            rot.set(Vector3::Z, 45.0f);

	            ZoneTemplate::EntitySpawnInfo* entitySpawnInfo = t->environmentEntitiesList.append_emplace(
		            entityTemplateNamespace,
		            entityTemplateId,
		            pos,
		            rot,
		            instanceId
	            );
	            entitySpawnInfo->spawnTestFlags.setFlag(FLAG_isNextZoneNexus/*name*/, false/*value*/, true/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);
	            entitySpawnInfo->spawnTestFlags.setFlag(FLAG_isZoneCleared/*name*/, false/*value*/, true/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);

                // teleporter_next_zone for flags {FLAG_isNextZoneNexus:false, FLAG_isZoneCleared:true}.
	            entityTemplateNamespace = EntityTemplatesList::Platform_ZoneTeleporter_Wood->nSpace;
	            entityTemplateId = EntityTemplatesList::Platform_ZoneTeleporter_Wood->id;
	            instanceId = EntityIds::platform_zoneTeleporter_next;
	            rot.set(Vector3::Z, 45.0f);

	            entitySpawnInfo = t->environmentEntitiesList.append_emplace(
		            entityTemplateNamespace,
		            entityTemplateId,
		            pos,
		            rot,
		            instanceId
	            );
	            entitySpawnInfo->spawnTestFlags.setFlag(FLAG_isNextZoneNexus/*name*/, false/*value*/, true/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);
	            entitySpawnInfo->spawnTestFlags.setFlag(FLAG_isZoneCleared/*name*/, true/*value*/, false/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);

                // nexus_zone teleporter pad for flags {FLAG_isNextZoneNexus:true, FLAG_isZoneCleared:false}.
	            entityTemplateNamespace = EntityTemplatesList::Doodad_BrokenNexusTeleporter_Stone_A_V1->nSpace;
	            entityTemplateId = EntityTemplatesList::Doodad_BrokenNexusTeleporter_Stone_A_V1->id;
	            instanceId = EntityIds::platform_zoneTeleporter_next;
	            rot.set(Vector3::Z, 45.0f);

	            entitySpawnInfo = t->environmentEntitiesList.append_emplace(
		            entityTemplateNamespace,
		            entityTemplateId,
		            pos,
		            rot,
		            instanceId
	            );
	            entitySpawnInfo->spawnTestFlags.setFlag(FLAG_isNextZoneNexus/*name*/, true/*value*/, false/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);
	            entitySpawnInfo->spawnTestFlags.setFlag(FLAG_isZoneCleared/*name*/, false/*value*/, true/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);

                // nexus_zone teleporter pad for flags {FLAG_isNextZoneNexus:true, FLAG_isZoneCleared:true}.
	            entityTemplateNamespace = EntityTemplatesList::Platform_NexusTeleporter_Wood_A_V1->nSpace;
	            entityTemplateId = EntityTemplatesList::Platform_NexusTeleporter_Wood_A_V1->id;
	            instanceId = EntityIds::platform_zoneTeleporter_next;
	            rot.set(Vector3::Z, 45.0f);

	            entitySpawnInfo = t->environmentEntitiesList.append_emplace(
		            entityTemplateNamespace,
		            entityTemplateId,
		            pos,
		            rot,
		            instanceId
	            );
	            entitySpawnInfo->spawnTestFlags.setFlag(FLAG_isNextZoneNexus/*name*/, true/*value*/, false/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);
	            entitySpawnInfo->spawnTestFlags.setFlag(FLAG_isZoneCleared/*name*/, true/*value*/, false/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);

	            continue;
            }else {
                continue;
            }

            t->environmentEntitiesList.append_emplace(
                entityTemplateNamespace,
                entityTemplateId,
                pos,
                rot,
                instanceId
            );
        }
    }

    return t;
}
