#include "Template_Text_Custom.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <worldGame3D/gw/entity/template/actions/ActionsTemplate.h>
#include <rpg3D/Namespace.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <base/exceptions/LogicException.h>
#include <base/math/Vector3Int.h>
#include <graphics/util/GraphicsUtil.h>

using namespace rpg3D;

std::string Template_Text_Custom::ID_Prefix = "Text_Custom_";

EntityTemplate* Template_Text_Custom::GetNewOrExistingTemplate(std::string text, float voxelSize) {
	std::string id = ID_Prefix + text;
	EntityTemplate* t = EntityTemplate::fromId(Namespace::NAMESPACE, id);
	if (t != nullptr) {
		return t;
	}

	t = EntityTemplate::newEntry(Namespace::NAMESPACE, id);

	t->tagsList.appendReference(rpg3D::EntityTags::DOODAD);
	t->tagsList.appendReference(rpg3D::EntityTags::TEXT);

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
				->appendDirect_chain(buildBase(text, voxelSize))
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

	return t;
};

Drawable3DCreateConfig_VoxelGrid* Template_Text_Custom::buildBase(std::string& text, float voxelSize) {
	// Enumerate each character to compute the final grid size.
	Vector3Int gridSize{};
	graphics::GraphicsUtil::GenerateTextVoxelsGrid3D(text, gridSize);

	const signed char O = -1;
	const signed char W = 0;

	Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(gridSize.x, gridSize.y, gridSize.z);
	padVoxelGrid->memSetData(-1);

	// Enumerate the characters again and generate the grid.
	graphics::GraphicsUtil::GenerateTextVoxelsGrid3D(text, gridSize, padVoxelGrid);

	ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#684D1EFF")));

	return new Drawable3DCreateConfig_VoxelGrid(
		"_Base_",//id
		new std::string("_ROOT_"),//parentId

		nullptr,// pos
		nullptr,//rot
		nullptr,// scale

		voxelSize/*voxelSize*/,
		new Vector3(-gridSize.x / 2.0f + 0.5f, -gridSize.y / 2.0f + 0.5f, -0.5f + 0.5f),

		padVoxelGrid, nullptr,
		materialsList, nullptr
	);
}
