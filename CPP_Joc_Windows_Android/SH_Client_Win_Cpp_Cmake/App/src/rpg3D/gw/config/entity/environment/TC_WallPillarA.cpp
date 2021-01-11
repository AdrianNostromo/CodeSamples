#include "TC_WallPillarA.h"
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <rpg3D/gw/entity/template/platformInteractor/PlatformInteractorTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>
#include <worldGame3D/gw/entity/template/referenceFrame/ReferenceFrameTemplate.h>
#include <worldGame3D/gw/entity/template/voxelisedStructure/VoxelisedStructureTemplate.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigBox.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>
#include <base/util/StringUtil.h>
#include <base/exceptions/LogicException.h>
#include "../../../../Namespace.h"

using namespace rpg3D;

EntityTemplate* TC_WallPillarA::getOrCreateTemplate(unsigned int pillarSizeXYCM, unsigned int pillarSizeZCM) {
	std::string templateBakedId = "WallPillarA_#pillarSizeXYCM#_#pillarSizeZCM#";

	StringUtil::replace(templateBakedId, "#pillarSizeXYCM#", std::to_string(pillarSizeXYCM));
	StringUtil::replace(templateBakedId, "#pillarSizeZCM#", std::to_string(pillarSizeZCM));
	EntityTemplate* t = EntityTemplate::fromId(Namespace::NAMESPACE, templateBakedId);
	if (t != nullptr) {
		return t;
	}

	t = EntityTemplatesList::newEntry(templateBakedId);

	t->tagsList.appendReference(EntityTags::GROUND_PATCH);

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
				->appendDirect_chain(buildBase(pillarSizeXYCM, pillarSizeZCM))
			)
		),
		false,//requiresHalfGridOffsetX
		false//requiresHalfGridOffsetY
	));

	float voxelAndTileSizeM = pillarSizeXYCM / 100.0f;
	float pillarHeightM = pillarSizeZCM / 100.0f;

	const float CM = rpg3D::PhysicsUtil::DEFAULT_COLLISSION_MARGIN;
	rpg3D::Physics3DTemplate* physics3D;
	t->setModuleTemplate(physics3D = new rpg3D::Physics3DTemplate(
		rpg3D::PhysicsCollissionConfig::Environment,
		nullptr/*pActorControllerCfg*/
	));
	physics3D->pBodiesConfigsList.append_emplace_chain(
		rpg3D::PBodyConfig::UssageBitGroups::Movement | rpg3D::PBodyConfig::UssageBitGroups::CardinalRotation | rpg3D::PBodyConfig::UssageBitGroups::VisualsUpdate,
		rpg3D::PBodyConfig::DynamicsType::Static/*dynamicsType*/,
		new Vector3{ 0.0f, 0.0f, pillarHeightM / 2.0f }/*physicsBodyOffsetPos*/, nullptr/*physicsBodyOffsetRot*/,
		new physics3D::PhysicsShapeConfigBox(
			CM/*collisionMargin*/,
			Vector3{ voxelAndTileSizeM / 2.0f, voxelAndTileSizeM / 2.0f, pillarHeightM / 2.0f }/*halfExtents*/
		),
		0.0f/*mass*/, 0.8f/*friction*/, 0.0f/*rollingFriction*/,
		Vector3{ 0, 0, 0 }/*enabledRotationAxes*/,
		0.6f/*linearDamping*/, 0.0f/*angularDamping*/,
		nullptr/*customGravity*/
	);

	return t;
}

Drawable3DCreateConfig_VoxelGrid* TC_WallPillarA::buildBase(unsigned int pillarSizeXYCM, unsigned int pillarSizeZCM) {
	const signed char O = -1;
	const signed char S = 0; // Stone

	Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
		1, 1, 1,
		false, true, true,
		new signed char[1 * 1 * 1]{
			S
		}
	);

	ArrayList<Visual3DModelMaterial*>* materialsListB = new ArrayList<Visual3DModelMaterial*>();
	materialsListB->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#B1B1B1FF")
	));

	// This is used for better visuals.
	float scaleFactorXY = 0.85f;

	return new Drawable3DCreateConfig_VoxelGrid(
		"_Base_",//id
		new std::string("_ROOT_"),//parentId

		nullptr,// pos
		nullptr,//rot
		new Vector3((pillarSizeXYCM / 100.0f) * scaleFactorXY, (pillarSizeXYCM / 100.0f) * scaleFactorXY, pillarSizeZCM / 100.0f),// scale

		1.0f/*voxelSizeM*/,
		new Vector3(0.5f - padVoxelGrid->getLengthA() / 2.0f, 0.5f - padVoxelGrid->getLengthB() / 2.0f, 0.5f),

		padVoxelGrid, nullptr,
		materialsListB, nullptr
	);
}
