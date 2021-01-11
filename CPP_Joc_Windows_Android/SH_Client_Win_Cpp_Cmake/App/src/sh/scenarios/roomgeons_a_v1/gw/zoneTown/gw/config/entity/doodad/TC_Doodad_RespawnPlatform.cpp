#include "TC_Doodad_RespawnPlatform.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityTemplatesList.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>
#include <base/statics/StaticsInit.h>
#include "../../../../../../Namespace.h"
#include <rpg3D/gw/entity/template/platform/PlatformTemplate.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigCylinder.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>
#include <base/util/StringUtil.h>

using namespace roomgeons_a_v1;
using namespace roomgeons_a_v1::Town;

EntityTemplate* TC_Doodad_RespawnPlatform::getOrCreateTemplate(unsigned int voxelSizeCM) {
    std::string templateBakedId = "TC_Doodad_RespawnPlatform_#voxelSizeCM#";
    StringUtil::replace(templateBakedId, "#voxelSizeCM#", std::to_string(voxelSizeCM));

    EntityTemplate* t = EntityTemplate::fromId(Namespace::NAMESPACE, templateBakedId);
    if (t != nullptr) {
        return t;
    }

	t = EntityTemplate::newEntry(Namespace::NAMESPACE, templateBakedId);

	t->tagsList.appendReference(rpg3D::EntityTags::PLATFORM);
	t->tagsList.appendReference(rpg3D::EntityTags::TELEPORT);
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
                   ->appendDirect_chain(buildBase(voxelSizeCM))
                )
        ),
        false,//requiresHalfGridOffsetX
        false//requiresHalfGridOffsetY
    ));

    // ( * voxelSizeCM / 10.0f) is used to scale up and down the value. The value is exact (-0.3f) at a voxel size of 10cm;
    t->setModuleTemplate(new Touchable3DTemplate(
        Touchable3DTemplate::ShapeType::AABB,
        Vector3(
            -0.3f * voxelSizeCM / 10.0f,
            -0.3f * voxelSizeCM / 10.0f,
            -0.05f
        ),
        Vector3(
            0.3f * voxelSizeCM / 10.0f,
            0.3f * voxelSizeCM / 10.0f,
            0.05f
        )
    ));

    return t;
};

Drawable3DCreateConfig_VoxelGrid* TC_Doodad_RespawnPlatform::buildBase(unsigned int voxelSizeCM) {
	Vector3Int gSize{ 9, 9, 1 };

	const signed char O = -1;
    const signed char W = 0;

	Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z,
		false, true, true,
		new signed char[gSize.x * gSize.y * gSize.z]{
			O,W,W,W,W,W,W,W,O,
			W,O,O,O,O,O,O,O,W,
			W,O,O,O,O,O,O,O,W,
            W,O,O,O,O,O,O,O,W,
            W,O,O,O,O,O,O,O,W,
            W,O,O,O,O,O,O,O,W,
            W,O,O,O,O,O,O,O,W,
			W,O,O,O,O,O,O,O,W,
			O,W,W,W,W,W,W,W,O
		}
	);

    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#B1B1B1FF")));

    return new Drawable3DCreateConfig_VoxelGrid(
        "_Base_",//id
        new std::string("_ROOT_"),//parentId

        nullptr,// pos
        nullptr,//rot
        nullptr,// scale

        voxelSizeCM / 100.0f/*voxelSizeM*/,
		new Vector3(-gSize.x / 2.0f + 0.5f, -gSize.y / 2.0f + 0.5f, -0.5f + 0.5f),

        padVoxelGrid, nullptr,
        materialsList, nullptr
    );
}
