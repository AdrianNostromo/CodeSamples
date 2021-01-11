#include "ZoneTemplate_Nexus_A_V1.h"
#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityTemplatesList.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityIds.h>

using namespace battlezones_a_v1;

ZoneTemplate* ZoneTemplate_Nexus_A_V1::getNew() {
	std::string zoneName = "Nexus_A_V1";
    ZoneTemplate* t = new ZoneTemplate(zoneName);

    int id = 0;

    int v_empty = 0;
    int v_grass = ++id;
    int v_water = ++id;
    int v_tree = ++id;

    int v_zoneTeleport_next = ++id;
    int v_zoneTeleport_center = ++id;
    int v_zoneTeleport_home = ++id;

    int O = v_empty;

    int W = v_water;
    (void)W;
    int G = v_grass;

    int T = v_tree;

    int d = v_zoneTeleport_next;
    int e = v_zoneTeleport_center;
    int h = v_zoneTeleport_home;

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
            O,O,O,T,T,G,G,G,G,e,G,G,G,G,T,T,O,O,O,O,//10
            O,O,O,T,T,G,G,G,G,G,G,G,G,G,T,T,O,O,O,O,
            O,O,O,T,T,G,G,G,G,G,G,G,G,G,T,T,O,O,O,O,
            O,O,O,T,T,G,G,G,G,G,G,G,G,G,T,T,O,O,O,O,
            O,O,O,O,T,h,G,G,G,G,G,G,G,G,T,O,O,O,O,O,
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
            }else if(val == v_zoneTeleport_next) {
                entityTemplateNamespace = EntityTemplatesList::Platform_ZoneTeleporter_Wood->nSpace;
                entityTemplateId = EntityTemplatesList::Platform_ZoneTeleporter_Wood->id;
                instanceId = EntityIds::platform_zonesGroupStart;
	            rot.set(Vector3::Z, 45.0f);
            }else if(val == v_zoneTeleport_center) {
                entityTemplateNamespace = EntityTemplatesList::Doodad_BrokenTeleporter_Stone_A_V1->nSpace;
                entityTemplateId = EntityTemplatesList::Doodad_BrokenTeleporter_Stone_A_V1->id;
                instanceId = EntityIds::defaultSpawnLocation;
	            rot.set(Vector3::Z, 0.0f);
            }else if(val == v_zoneTeleport_home) {
                entityTemplateNamespace = EntityTemplatesList::Platform_Teleporter_Home->nSpace;
                entityTemplateId = EntityTemplatesList::Platform_Teleporter_Home->id;
                instanceId = EntityIds::platform_Teleporter_Home;
	            rot.set(Vector3::Z, 0.0f);
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
