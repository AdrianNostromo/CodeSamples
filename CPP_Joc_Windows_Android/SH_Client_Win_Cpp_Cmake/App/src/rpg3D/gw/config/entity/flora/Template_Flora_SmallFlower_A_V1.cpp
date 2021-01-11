#include "Template_Flora_SmallFlower_A_V1.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>

using namespace rpg3D;

EntityTemplate* Template_Flora_SmallFlower_A_V1::getNew() {
	EntityTemplate* t = EntityTemplatesList::newEntry("Flora_SmallFlower_A_V1");

	t->tagsList.appendReference(EntityTags::FLORA);
	t->tagsList.appendReference(EntityTags::FLOWER);
	t->tagsList.appendReference(EntityTags::STRUCTURE);

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

	// Touchable not used currently.
	t->setModuleTemplate(new Touchable3DTemplate(
		Touchable3DTemplate::ShapeType::AABB,
		Vector3(
			-0.1f,
			-0.1f,
			-0.05f
		),
		Vector3(
			0.1f,
			0.1f,
			// This value will be overridden when choppable trees are implemented.
			0.3f
		)
	));

	t->setModuleTemplate(FrustumCullable3DTemplate::newOfsetedZSphere(
		0.5f,
		Math::dist(0.0f, 0.0f, 0.5f, 0.5f)
	));

	return t;
}

Drawable3DCreateConfig_VoxelGrid* Template_Flora_SmallFlower_A_V1::buildBase() {
	const int gSizeX = 5;
	const int gSizeY = 5;
	const int gSizeZ = 5;

	const signed char O = -1;
	const signed char G = 0; // Green
	const signed char P = 1; // Petals
	const signed char C = 2; // Center

	Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
		gSizeX, gSizeY, gSizeZ,
		false, true, true,
		new signed char[gSizeX * gSizeY * gSizeZ]{
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
	materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#3FC85BFF")
	));
	materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#FFA8E4FF")
	));
	materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#FEE25CFF")
	));

	return new Drawable3DCreateConfig_VoxelGrid(
		"_Base_",//id
		new std::string("_ROOT_"),//parentId

		nullptr,// pos
		nullptr,//rot
		nullptr,// scale

		0.1f,
		// The z pos is manually set so the entity sits 0.5f under the ground.
		new Vector3(-gSizeX / 2.0f + 0.5f, -gSizeY / 2.0f + 0.5f, -0.5f + 0.5f),

		padVoxelGrid, nullptr,
		materialsList, nullptr
	);
}
