#include "T_Floor_SubFloorDirt_TiledX6dmY6dmDeep6m.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityTemplatesList.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>
#include <base/statics/StaticsInit.h>
#include "../../../../Namespace.h"
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigTiledFloorGridXY.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>
#include <base/util/StringUtil.h>
#include <base/exceptions/LogicException.h>

using namespace rpg3D;

std::string T_Floor_SubFloorDirt_TiledX6dmY6dmDeep6m ::ID = "Floor_SubFloorDirt_TiledX6dmY6dmDeep6m";

std::string T_Floor_SubFloorDirt_TiledX6dmY6dmDeep6m ::DATA_KEY_materials = "_materials_";
Vector3Int T_Floor_SubFloorDirt_TiledX6dmY6dmDeep6m ::TileVoxelsCount{};

Array3D<signed char>* T_Floor_SubFloorDirt_TiledX6dmY6dmDeep6m ::gridDirtA = nullptr;
Array3D<signed char>* T_Floor_SubFloorDirt_TiledX6dmY6dmDeep6m ::gridDirtB = nullptr;
Array3D<signed char>* T_Floor_SubFloorDirt_TiledX6dmY6dmDeep6m ::gridStoneA = nullptr;
Array3D<signed char>* T_Floor_SubFloorDirt_TiledX6dmY6dmDeep6m ::gridStoneB = nullptr;
ArrayList<Visual3DModelMaterial*>* T_Floor_SubFloorDirt_TiledX6dmY6dmDeep6m ::materialsList = nullptr;

EntityTemplate* T_Floor_SubFloorDirt_TiledX6dmY6dmDeep6m ::TEMPLATE = base::StaticsInit::AddCbGeneral<EntityTemplate*>(1, []() {
	Vector3Int gSize{ 1, 1, 1 };
	TileVoxelsCount.set(gSize);

	gridDirtA = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z
	);
	for (int gX = 0; gX < gSize.x; gX++) {
		for (int gY = 0; gY < gSize.y; gY++) {
			for (int gZ = 0; gZ < gSize.z; gZ++) {
				gridDirtA->setDirect(gX, gY, gZ, 0);
			}
		}
	}
	
	gridDirtB = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z
	);
	for (int gX = 0; gX < gSize.x; gX++) {
		for (int gY = 0; gY < gSize.y; gY++) {
			for (int gZ = 0; gZ < gSize.z; gZ++) {
				gridDirtB->setDirect(gX, gY, gZ, 1);
			}
		}
	}
	
	gridStoneA = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z
	);
	for (int gX = 0; gX < gSize.x; gX++) {
		for (int gY = 0; gY < gSize.y; gY++) {
			for (int gZ = 0; gZ < gSize.z; gZ++) {
				gridStoneA->setDirect(gX, gY, gZ, 2);
			}
		}
	}
	
	gridStoneB = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z
	);
	for (int gX = 0; gX < gSize.x; gX++) {
		for (int gY = 0; gY < gSize.y; gY++) {
			for (int gZ = 0; gZ < gSize.z; gZ++) {
				gridStoneB->setDirect(gX, gY, gZ, 3);
			}
		}
	}
	
	materialsList = new ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#AF7C45FF")));
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#926639FF")));
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#B8B7BCFF")));
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#A0A0A5FF")));

	EntityTemplate* t = TEMPLATE = EntityTemplate::newEntry(Namespace::NAMESPACE, ID);

	t->tagsList.appendReference(rpg3D::EntityTags::PLATFORM);
	t->tagsList.appendReference(rpg3D::EntityTags::STRUCTURE);

    t->setModuleTemplate(new Visual3DTemplate(
        new Visual3DCreateConfig(
            (new ArrayList<ArrayList<Drawable3DCreateConfigBase*>*>())
                ->appendDirect_chain((new ArrayList<Drawable3DCreateConfigBase*>())
                   ->appendDirect_chain(new Drawable3DCreateConfig_Group(
                       "_ROOT_",
                       nullptr,
                       nullptr,
                       nullptr,
                       nullptr
                   ))
                   ->appendDirect_chain(buildBase())
                )
        ),
        false,//requiresHalfGridOffsetX
        false//requiresHalfGridOffsetY
    ));

    return t;
});

Drawable3DCreateConfig_VoxelGrid* T_Floor_SubFloorDirt_TiledX6dmY6dmDeep6m ::buildBase() {
    return new Drawable3DCreateConfig_VoxelGrid(
        "_Base_",//id
        new std::string("_ROOT_"),//parentId

        nullptr,// pos
        nullptr,//rot
        new Vector3(1.0f, 1.0f, 6.0f),// scale

        0.6f,
		new Vector3(0.5f, 0.5f, -0.5f),

		nullptr, nullptr,
		nullptr, & DATA_KEY_materials
    );
}
