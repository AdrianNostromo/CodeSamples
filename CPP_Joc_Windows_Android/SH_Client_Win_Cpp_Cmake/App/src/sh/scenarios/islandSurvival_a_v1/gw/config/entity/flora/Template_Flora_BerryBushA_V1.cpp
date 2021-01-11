#include "Template_Flora_BerryBushA_V1.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <sh/scenarios/islandSurvival_a_v1/Namespace.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <base/statics/StaticsInit.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>

using namespace islandSurvival_a_v1;

std::string Template_Flora_BerryBushA_V1::ID = "Flora_BerryBushA_V1";

EntityTemplate* Template_Flora_BerryBushA_V1::TEMPLATE = base::StaticsInit::AddCbGeneral<EntityTemplate*>(1, []() {
	EntityTemplate* t = TEMPLATE = EntityTemplate::newEntry(Namespace::NAMESPACE, ID);

	t->tagsList.appendReference(rpg3D::EntityTags::FLORA);
	t->tagsList.appendReference(rpg3D::EntityTags::FLOWER);
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
		0.5f,
		Math::dist(0.0f, 0.0f, 0.5f, 0.5f)
	));

	return t;
});

Drawable3DCreateConfig_VoxelGrid* Template_Flora_BerryBushA_V1::buildBase() {
	Vector3Int gSize{ 5, 5, 5 };

	const signed char O = -1;
	const signed char G = 0; // Green
	const signed char P = 1; // Petals
	const signed char C = 2; // Center

	Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z,
		false, true, true,
		new signed char[gSize.x * gSize.y * gSize.z]{
			O,O,O,O,O,
			O,O,O,P,O,
			O,O,O,P,O,
			O,O,O,P,O,
			O,O,O,O,O,
			
			O,O,O,O,O,
			O,O,O,P,O,
			O,O,G,P,C,
			O,O,O,P,O,
			O,O,O,O,O,
			
			O,O,O,O,O,
			O,O,O,P,O,
			O,O,G,P,O,
			O,O,O,P,O,
			O,O,O,O,O,
			
			O,O,O,O,O,
			O,O,O,O,O,
			O,O,G,O,O,
			O,O,O,O,O,
			O,O,O,O,O,
			
			O,O,O,O,O,
			O,G,G,G,O,
			O,G,G,G,O,
			O,G,G,G,O,
			O,O,O,O,O
		}
	);

	ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#3FC85BFF")));
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#FFA8E4FF")));
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#f55142FF")));

	return new Drawable3DCreateConfig_VoxelGrid(
		"_Base_",//id
		new std::string("_ROOT_"),//parentId

		nullptr,// pos
		nullptr,//rot
		nullptr,// scale

		0.1f,
		// The z pos is manually set so the entity sits 0.5f under the ground.
		new Vector3(-gSize.x / 2.0f + 0.5f, -gSize.y / 2.0f + 0.5f, -0.5f + 0.5f),

		padVoxelGrid, nullptr,
		materialsList, nullptr
	);
}
