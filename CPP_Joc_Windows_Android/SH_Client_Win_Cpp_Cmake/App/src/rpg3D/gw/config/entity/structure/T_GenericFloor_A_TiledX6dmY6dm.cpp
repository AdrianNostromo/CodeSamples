#include "T_GenericFloor_A_TiledX6dmY6dm.h"
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

std::string T_GenericFloor_A_TiledX6dmY6dm::ID = "GenericFloor_A_TiledX6dmY6dm";

std::string T_GenericFloor_A_TiledX6dmY6dm::DATA_KEY_materials = "_materials_";
Vector3Int T_GenericFloor_A_TiledX6dmY6dm::TileVoxelsCount{};

Array3D<signed char>* T_GenericFloor_A_TiledX6dmY6dm::gridStoneA = nullptr;
Array3D<signed char>* T_GenericFloor_A_TiledX6dmY6dm::gridStoneB = nullptr;
Array3D<signed char>* T_GenericFloor_A_TiledX6dmY6dm::gridGrassA = nullptr;
Array3D<signed char>* T_GenericFloor_A_TiledX6dmY6dm::gridGrassB = nullptr;
ArrayList<Visual3DModelMaterial*>* T_GenericFloor_A_TiledX6dmY6dm::materialsList = nullptr;

EntityTemplate* T_GenericFloor_A_TiledX6dmY6dm::TEMPLATE = base::StaticsInit::AddCbGeneral<EntityTemplate*>(1, []() {
	Vector3Int gSize{ 6, 6, 1 };
	TileVoxelsCount.set(gSize);

	gridStoneA = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z
	);
	for (int gX = 0; gX < gSize.x; gX++) {
		for (int gY = 0; gY < gSize.y; gY++) {
			for (int gZ = 0; gZ < gSize.z; gZ++) {
				gridStoneA->setDirect(gX, gY, gZ, 0);
			}
		}
	}
	
	gridStoneB = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z
	);
	for (int gX = 0; gX < gSize.x; gX++) {
		for (int gY = 0; gY < gSize.y; gY++) {
			for (int gZ = 0; gZ < gSize.z; gZ++) {
				gridStoneB->setDirect(gX, gY, gZ, 1);
			}
		}
	}
	
	gridGrassA = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z
	);
	for (int gX = 0; gX < gSize.x; gX++) {
		for (int gY = 0; gY < gSize.y; gY++) {
			for (int gZ = 0; gZ < gSize.z; gZ++) {
				gridGrassA->setDirect(gX, gY, gZ, 2);
			}
		}
	}
	
	gridGrassB = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z
	);
	for (int gX = 0; gX < gSize.x; gX++) {
		for (int gY = 0; gY < gSize.y; gY++) {
			for (int gZ = 0; gZ < gSize.z; gZ++) {
				gridGrassB->setDirect(gX, gY, gZ, 3);
			}
		}
	}

	materialsList = new ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#B8B7BCFF")));
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#A0A0A5FF")));
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#78C214FF")));
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#70B80DFF")));

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

	rpg3D::Physics3DTemplate* physics3D;
	t->setModuleTemplate(physics3D = new rpg3D::Physics3DTemplate(
		rpg3D::PhysicsCollissionConfig::Environment,
		nullptr/*pActorControllerCfg*/
	));
	physics3D->pBodiesConfigsList.append_emplace_chain(
		rpg3D::PBodyConfig::UssageBitGroups::Movement | rpg3D::PBodyConfig::UssageBitGroups::CardinalRotation | rpg3D::PBodyConfig::UssageBitGroups::VisualsUpdate,
		rpg3D::PBodyConfig::DynamicsType::Static/*dynamicsType*/,
		nullptr/*physicsBodyOffsetPos*/, nullptr/*physicsBodyOffsetRot*/,
		// Keep convex hull because this may use a composed physics shape in the future.
		new physics3D::PhysicsShapeConfigTiledFloorGridXY(
			rpg3D::PhysicsUtil::DEFAULT_COLLISSION_MARGIN/*collisionMargin*/,
			Vector2(gSize.x * 0.1f, gSize.y * 0.1f)
		),
		0.0f/*mass*/, 0.8f/*friction*/, 0.0f/*rollingFriction*/,
		Vector3{ 0, 0, 0 }/*enabledRotationAxes*/,
		0.6f/*linearDamping*/, 0.0f/*angularDamping*/,
		nullptr/*customGravity*/
	);

    return t;
});

Drawable3DCreateConfig_VoxelGrid* T_GenericFloor_A_TiledX6dmY6dm::buildBase() {
    return new Drawable3DCreateConfig_VoxelGrid(
        "_Base_",//id
        new std::string("_ROOT_"),//parentId

        nullptr,// pos
        nullptr,//rot
        nullptr,// scale

        0.1f,
		new Vector3(0.5f, 0.5f, -0.5f),

		nullptr, nullptr,
		nullptr, & DATA_KEY_materials
    );
}