#include "GroundTemplates_BasicPatchA_1m.h"
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

using namespace rpg3D;

EntityTemplate* GroundTemplates_BasicPatchA_1m::getNew() {
	EntityTemplate* t = EntityTemplatesList::newEntry("Ground_BasicPatchA_1m");

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
				->appendDirect_chain(new Drawable3DCreateConfig_Group(
					"_voxelisedConstructLayer_",
					new std::string("_ROOT_"),
					nullptr,
					nullptr,
					nullptr
				))
				->appendDirect_chain(new Drawable3DCreateConfig_Group(
					"_unitsLayer_",
					new std::string("_ROOT_"),
					nullptr,
					nullptr,
					nullptr
				))
			)
		),
		false,//requiresHalfGridOffsetX
		false//requiresHalfGridOffsetY
	));

	t->setModuleTemplate(FrustumCullable3DTemplate::newOfsetedZSphere(
		0.5f,
		Math::dist(0.0f, 0.0f, 0.5f, 0.5f)
	));

	t->setModuleTemplate(new VoxelisedStructureTemplate(
		"_voxelisedConstructLayer_"
	));

	const float CM = rpg3D::PhysicsUtil::DEFAULT_COLLISSION_MARGIN;
	rpg3D::Physics3DTemplate* physics3D;
	t->setModuleTemplate(physics3D = new rpg3D::Physics3DTemplate(
		rpg3D::PhysicsCollissionConfig::Environment,
		nullptr/*pActorControllerCfg*/
	));
	physics3D->pBodiesConfigsList.append_emplace_chain(
		rpg3D::PBodyConfig::UssageBitGroups::Movement | rpg3D::PBodyConfig::UssageBitGroups::CardinalRotation | rpg3D::PBodyConfig::UssageBitGroups::VisualsUpdate,
		rpg3D::PBodyConfig::DynamicsType::Static/*dynamicsType*/,
		new Vector3{ 0.5f, 0.5f, -0.05f }/*physicsBodyOffsetPos*/, nullptr/*physicsBodyOffsetRot*/,
		new physics3D::PhysicsShapeConfigBox(
			CM/*collisionMargin*/,
			Vector3{ 0.5f, 0.5f, 0.05f }/*halfExtents*/
		),
		0.0f/*mass*/, 0.8f/*friction*/, 0.0f/*rollingFriction*/,
		Vector3{ 0, 0, 0 }/*enabledRotationAxes*/,
		0.6f/*linearDamping*/, 0.0f/*angularDamping*/,
		nullptr/*customGravity*/
	);

	return t;
}
