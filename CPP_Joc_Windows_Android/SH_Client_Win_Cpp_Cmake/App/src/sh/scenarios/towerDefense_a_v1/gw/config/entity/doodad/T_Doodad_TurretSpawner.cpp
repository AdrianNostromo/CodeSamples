#include "T_Doodad_TurretSpawner.h"
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
#include <sh/scenarios/towerDefense_a_v1/Namespace.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <worldGame3D/gw/entity/template/actions/ActionsTemplate.h>

using namespace towerDefense_a_v1;

std::string T_Doodad_TurretSpawner::ID = "Doodad_TurretSpawner";

EntityTemplate* T_Doodad_TurretSpawner::TEMPLATE = base::StaticsInit::AddCbGeneral<EntityTemplate*>(1, []() {
	EntityTemplate* t = TEMPLATE = EntityTemplate::newEntry(Namespace::NAMESPACE, ID);

    t->tagsList.appendReference(rpg3D::EntityTags::DOODAD);

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

	// This has no touchable so stackables can't be placed on them.
	t->setModuleTemplate(new Touchable3DTemplate(
		Touchable3DTemplate::ShapeType::AABB,
		Vector3(
			-0.25f,
			-0.25f,
			0.0f
		),
		Vector3(
			0.25f,
			0.25f,
			0.05f
		)
	));

    t->setModuleTemplate(FrustumCullable3DTemplate::newOfsetedZSphere(
        0.4f,
        Math::dist(0.0f, 0.0f, 0.4f, 0.4f)
    ));

	t->setModuleTemplate(new worldGame3D::ActionsTemplate(
		//void
	));

	t->setModuleTemplate(new rpg3D::StackableTemplate(
		true/*canHaul*/, false,/*canBeCarried*/
		0.0f/*haulOffsetZM*/,
		false/*allowPlaceOnThis*/
	));

    return t;
});

Drawable3DCreateConfig_VoxelGrid* T_Doodad_TurretSpawner::buildBase() {
	Vector3Int gSize{ 8, 8, 1 };

    const signed char O = -1;
    const signed char W = 0;

    Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z,
        false, true, true,
        new signed char[gSize.x * gSize.y * gSize.z] {
			O,O,W,W,W,W,O,O,
			O,W,O,O,O,O,W,O,
			W,O,O,O,O,O,O,W,
			W,O,O,O,O,O,O,W,
			W,O,O,O,O,O,O,W,
			W,O,O,O,O,O,O,W,
			O,W,O,O,O,O,W,O,
			O,O,W,W,W,W,O,O
        }
    );

    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#684D1EFF")));

    return new Drawable3DCreateConfig_VoxelGrid(
        "_Base_",//id
        new std::string("_ROOT_"),//parentId

        nullptr,// pos
        nullptr,//rot
        nullptr,// scale

        0.06f,
        new Vector3(-gSize.x / 2.0f + 0.5f, -gSize.y / 2.0f + 0.5f, -0.5f + 0.5f),

        padVoxelGrid, nullptr,
        materialsList, nullptr
    );
}
