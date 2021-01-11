#include "T_GridPathingIndicatorFloor_A_TiledX6dmY6dm.h"
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

std::string T_GridPathingIndicatorFloor_A_TiledX6dmY6dm::ID = "GridPathingIndicatorFloor_A_TiledX6dmY6dm";

std::string T_GridPathingIndicatorFloor_A_TiledX6dmY6dm::DATA_KEY_materials = "_materials_";
Vector3Int T_GridPathingIndicatorFloor_A_TiledX6dmY6dm::TileVoxelsCount{};

Array3D<signed char>* T_GridPathingIndicatorFloor_A_TiledX6dmY6dm::gridPathableTiles = nullptr;
Array3D<signed char>* T_GridPathingIndicatorFloor_A_TiledX6dmY6dm::gridSelfTile = nullptr;
ArrayList<Visual3DModelMaterial*>* T_GridPathingIndicatorFloor_A_TiledX6dmY6dm::materialsList = nullptr;

EntityTemplate* T_GridPathingIndicatorFloor_A_TiledX6dmY6dm::TEMPLATE = base::StaticsInit::AddCbGeneral<EntityTemplate*>(1, []() {
	Vector3Int gSize{ 6, 6, 1 };
	TileVoxelsCount.set(gSize);

	gridPathableTiles = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z
	);
	for (int gX = 0; gX < gSize.x; gX++) {
		for (int gY = 0; gY < gSize.y; gY++) {
			for (int gZ = 0; gZ < gSize.z; gZ++) {
				gridPathableTiles->setDirect(gX, gY, gZ, 0);
			}
		}
	}
	
	gridSelfTile = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z
	);
	for (int gX = 0; gX < gSize.x; gX++) {
		for (int gY = 0; gY < gSize.y; gY++) {
			for (int gZ = 0; gZ < gSize.z; gZ++) {
				gridSelfTile->setDirect(gX, gY, gZ, 1);
			}
		}
	}

	materialsList = new ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#00AA00FF")));
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#AA0000FF")));

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

    t->setModuleTemplate(FrustumCullable3DTemplate::newOfsetedZSphere(
        0.4f,
        Math::dist(0.0f, 0.0f, 0.4f, 0.4f)
    ));

    return t;
});

Drawable3DCreateConfig_VoxelGrid* T_GridPathingIndicatorFloor_A_TiledX6dmY6dm::buildBase() {
    return new Drawable3DCreateConfig_VoxelGrid(
        "_Base_",//id
        new std::string("_ROOT_"),//parentId

        nullptr,// pos
        nullptr,//rot
        nullptr,// scale

        0.1f,
		new Vector3(0.5f, 0.5f, -0.5f + 0.1f),// Add a small z offset so the visual appears over the ground.

		nullptr, nullptr,
		nullptr, & DATA_KEY_materials
    );
}
