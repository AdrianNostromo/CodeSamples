#include "Template_BlockerPillar_A_V1.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <sh/scenarios/towerDefense_a_v1/Namespace.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <base/statics/StaticsInit.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>
#include <sh/scenarios/towerDefense_a_v1/gw/entity/template/customCubesCannonTurret/CustomCubesCannonTurretTemplate.h>
#include <worldGame3D/gw/entity/template/actions/ActionsTemplate.h>
#include <rpg3D/gw/entity/template/TeamTemplate.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/sounds/SoundConfigsList.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>

using namespace towerDefense_a_v1;

std::string Template_BlockerPillar_A_V1::ID = "BlockerPillar_A_V1";

EntityTemplate* Template_BlockerPillar_A_V1::TEMPLATE = base::StaticsInit::AddCbGeneral<EntityTemplate*>(1, []() {
	EntityTemplate* t = TEMPLATE = EntityTemplate::newEntry(Namespace::NAMESPACE, ID);

	t->tagsList.appendReference(rpg3D::EntityTags::TURRET);

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
		0.5f,
		Math::dist(0.0f, 0.0f, 0.5f, 0.5f)
	));

	t->setModuleTemplate(new worldGame3D::ActionsTemplate(
		//void
	));

	t->setModuleTemplate(new rpg3D::TeamTemplate(
		//void
	));

	t->setModuleTemplate(new Touchable3DTemplate(
		Touchable3DTemplate::ShapeType::AABB,
		Vector3(
			-0.2f,
			-0.2f,
			0.0f
		),
		Vector3(
			0.2f,
			0.2f,
			0.4f
		)
	));

	t->setModuleTemplate(new rpg3D::StackableTemplate(
		false/*canHaul*/, true,/*canBeCarried*/
		0.4f/*haulOffsetZM*/,
		true/*allowPlaceOnThis*/
	));
});

Drawable3DCreateConfig_VoxelGrid* Template_BlockerPillar_A_V1::buildBase() {
	Vector3Int gSize{ 2, 2, 1 };

	const signed char O = -1;
	const signed char M = 0; // Metal

	Array3D<signed char>* voxelGrid = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z,
		false, true, true,
		new signed char[gSize.x * gSize.y * gSize.z]{
			M,M,
			M,M
		}
	);
	
	ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#E7E7E7FF")));

	return new Drawable3DCreateConfig_VoxelGrid(
		"_Base_",//id
		new std::string("_ROOT_"),//parentId

		nullptr,// pos
		nullptr,//rot
		nullptr,// scale

		0.2f/*voxelSize*/,
		// The z pos is manually set so the entity sits 0.5f under the ground.
		new Vector3(-gSize.x / 2.0f + 0.5f, -gSize.y / 2.0f + 0.5f, 0.5f),

		voxelGrid, nullptr,
		materialsList, nullptr
	);
}
