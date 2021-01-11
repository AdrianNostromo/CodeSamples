#include "EntityTemplates_Platform_CreatureCreatorPad.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <rpg3D/gw/entity/template/platform/PlatformTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/template/customLogic/CustomLogicTemplate.h>
#include <rpg3D/gw/entity/module/customLogic/CustomLogicModule_EntityEditorPlatform.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigCylinder.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace rpg3D;

std::string EntityTemplates_Platform_CreatureCreatorPad::V_ID_Base = "_Base_";
std::string EntityTemplates_Platform_CreatureCreatorPad::V_ID_Base_Disabled = "_Base_Disabled_";
std::string EntityTemplates_Platform_CreatureCreatorPad::V_ID_Icon = "_Icon_";

EntityTemplate* EntityTemplates_Platform_CreatureCreatorPad::getNew() {
	EntityTemplate* t = EntityTemplatesList::newEntry("Platform_CreatureCreatorPad");

	t->tagsList.appendReference(EntityTags::PLATFORM);
	t->tagsList.appendReference(EntityTags::TELEPORT);
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
				->appendDirect_chain(buildBase_disabled())
				->appendDirect_chain(buildIcon())
			)
		),
		false,//requiresHalfGridOffsetX
		false//requiresHalfGridOffsetY
	));

	t->setModuleTemplate(new Touchable3DTemplate(
		Touchable3DTemplate::ShapeType::AABB,
		Vector3(
			-0.4f,
			-0.4f,
			0.0f
		),
		Vector3(
			0.4f,
			0.4f,
			0.1f
		)
	));

	const float CM = rpg3D::PhysicsUtil::DEFAULT_COLLISSION_MARGIN;
	rpg3D::Physics3DTemplate* physics3D;
	t->setModuleTemplate(physics3D = newt rpg3D::Physics3DTemplate(
		rpg3D::PhysicsCollissionConfig::Platform,
		nullptr/*pActorControllerCfg*/
	));
	physics3D->pBodiesConfigsList.append_emplace_chain(
		rpg3D::PBodyConfig::UssageBitGroups::Movement | rpg3D::PBodyConfig::UssageBitGroups::CardinalRotation | rpg3D::PBodyConfig::UssageBitGroups::VisualsUpdate,
		rpg3D::PBodyConfig::DynamicsType::Static/*dynamicsType*/,
		new Vector3(0.0f, 0.0f, 0.05f)/*physicsBodyOffsetPos*/, nullptr/*physicsBodyOffsetRot*/,
		new physics3D::PhysicsShapeConfigCylinder(
			CM/*collisionMargin*/,
			Vector3::Z, Vector3{ 0.2f, 0.2f, 0.05f }
		),
		0.0f/*mass*/, 0.5f/*friction*/, 0.0f/*rollingFriction*/,
		Vector3{ 0, 0, 0 }/*enabledRotationAxes*/,
		0.0f/*linearDamping*/, 0.0f/*angularDamping*/,
		nullptr/*customGravity*/
	);

	t->setModuleTemplate(new PlatformTemplate());

	t->setModuleTemplate(new base::CustomLogicTemplate(
		"customLogic_EntityEditorPlatform", ICustomLogicModule_EntityEditorPlatform::TYPE,
		[](IWorldEntity* entity, base::CustomLogicTemplate* t) -> base::CustomLogicModule * { 
			return new CustomLogicModule_EntityEditorPlatform(entity, t);
		}
	));

	t->setModuleTemplate(FrustumCullable3DTemplate::newOfsetedZSphere(
		0.0f,
		Math::dist(0.0f, 0.0f, 0.4f, 0.4f)
	));

	return t;
}

Drawable3DCreateConfig_VoxelGrid* EntityTemplates_Platform_CreatureCreatorPad::buildBase() {
	const signed char O = -1;
	const signed char W = 0; // Wood

	Array3D<signed char>* padVoxelGrid = newt Array3D<signed char>(
		8, 8, 1,
		false, true, true,
		new signed char[8 * 8 * 1]{
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

	ArrayList<Visual3DModelMaterial*>* materialsList = newt ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#997535FF")
	));

	return new Drawable3DCreateConfig_VoxelGrid(
		V_ID_Base,//id
		new std::string("_ROOT_"),//parentId

		nullptr,// pos
		nullptr,//rot
		nullptr,// scale

		0.1f,
		new Vector3(-8.0f / 2.0f + 0.5f, -8.0f / 2.0f + 0.5f, -0.5f + 0.5f),

		padVoxelGrid, nullptr,
		materialsList, nullptr
	);
}

Drawable3DCreateConfig_VoxelGrid* EntityTemplates_Platform_CreatureCreatorPad::buildBase_disabled() {
	const signed char O = -1;
	const signed char W = 0; // Wood

	Array3D<signed char>* padVoxelGrid = newt Array3D<signed char>(
		8, 8, 1,
		false, true, true,
		new signed char[8 * 8 * 1]{
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

	ArrayList<Visual3DModelMaterial*>* materialsList = newt ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#B1B1B1FF")
	));

	return new Drawable3DCreateConfig_VoxelGrid(
		V_ID_Base_Disabled,//id
		new std::string("_ROOT_"),//parentId

		nullptr,// pos
		nullptr,//rot
		nullptr,// scale

		0.1f,
		new Vector3(-8.0f / 2.0f + 0.5f, -8.0f / 2.0f + 0.5f, -0.5f + 0.5f),

		padVoxelGrid, nullptr,
		materialsList, nullptr
	);
}

Drawable3DCreateConfig_VoxelGrid* EntityTemplates_Platform_CreatureCreatorPad::buildIcon() {
	const int gSizeX = 4;
	const int gSizeY = 3;
	const int gSizeZ = 1;

	const signed char O = -1;
	const signed char A = 0; // Icon material 1
	const signed char B = 1; // Icon material 2

	Array3D<signed char>* padVoxelGrid = newt Array3D<signed char>(
		gSizeX, gSizeY, gSizeZ,
		false, true, true,
		new signed char[gSizeX * gSizeY * gSizeZ]{
			B,A,O,O,
			O,A,A,A,
			O,A,O,A
		}
	);

	ArrayList<Visual3DModelMaterial*>* materialsList = newt ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#5A4A4AFF")
	));
	materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#EEA452FF")
	));

	return new Drawable3DCreateConfig_VoxelGrid(
		V_ID_Icon,//id
		new std::string("_ROOT_"),//parentId

		nullptr,// pos
		new Quaternion(Vector3::Z, -45.0f),//rot
		nullptr,// scale

		0.1f,
		// The z pos is manually set so the entity sits 0.5f under the ground.
		new Vector3(-gSizeX / 2.0f + 0.5f, -gSizeY / 2.0f + 0.5f, -0.5f + 0.5f),

		padVoxelGrid, nullptr,
		materialsList, nullptr
	);
}
