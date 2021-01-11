#include "T_Npc_Shopkeep_A_V1.h"
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <rpg3D/gw/entity/template/platformInteractor/PlatformInteractorTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <worldGame3D/gw/entity/template/lookAt/LookAtTemplate.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <rpg3D/gw/entity/template/TeamTemplate.h>
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <worldGame3D/gw/entity/template/actions/ActionsTemplate.h>
#include <rpg3D/gw/entity/template/userControlled/UserControlledTemplate.h>
#include <rpg3D/gw/entity/template/payloadReceiverTarget/PayloadReceiverTargetTemplate.h>
#include <rpg3D/gw/entity/template/LivingEntityTemplate.h>
#include <rpg3D/gw/entity/template/controller/ControllerTemplate.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <rpg3D/gw/entity/template/editableVoxelised/EditableVoxelisedTemplate.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigCapsule.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigSphere.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>
#include <rpg3D/gw/entity/template/physics3D/pActorController/CfgCharacterPActorController.h>
#include <base/statics/StaticsInit.h>
#include <sh/scenarios/randDungeons_a_v1/Namespace.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigPhysicsPush.h>

using namespace randDungeons_a_v1;

std::string T_Npc_Shopkeep_A_V1::ID = "Npc_Shopkeep_A_V1";

EntityTemplate* T_Npc_Shopkeep_A_V1::TEMPLATE = base::StaticsInit::AddCbGeneral<EntityTemplate*>(1, []() {
	EntityTemplate* t = TEMPLATE = EntityTemplate::newEntry(Namespace::NAMESPACE, ID);

    t->tagsList.appendReference(rpg3D::EntityTags::UNIT);

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

	t->setModuleTemplate(new worldGame3D::ActionsTemplate(
		//void
	));

	const float CM = rpg3D::PhysicsUtil::DEFAULT_COLLISSION_MARGIN;
	rpg3D::Physics3DTemplate* physics3D;
	t->setModuleTemplate(physics3D = new rpg3D::Physics3DTemplate(
		rpg3D::PhysicsCollissionConfig::Unit,
		new rpg3D::CfgCharacterPActorController()
	));
	physics3D->pBodiesConfigsList.append_emplace_chain(
		rpg3D::PBodyConfig::UssageBitGroups::Movement | rpg3D::PBodyConfig::UssageBitGroups::CardinalRotation | rpg3D::PBodyConfig::UssageBitGroups::VisualsUpdate,
        rpg3D::PBodyConfig::DynamicsType::Dynamic/*dynamicsType*/,
        new Vector3{ 0.0f, 0.0f, 0.2f }/*physicsBodyOffsetPos*/, nullptr/*physicsBodyOffsetRot*/,
		new physics3D::PhysicsShapeConfigCapsule(
			Vector3::Z/*mainAxis*/,
			0.2f/*endDomesRadius*/, 0.0f/*cylinderPartHeight*/
		),
		10.0f/*mass*/, 0.5f/*friction*/, 0.0f/*rollingFriction*/,
		Vector3{ 0, 0, 0 }/*enabledRotationAxes*/,
		0.0f/*linearDamping*/, 0.0f/*angularDamping*/,
		nullptr/*customGravity*/
	);

	t->setModuleTemplate(new LookAtTemplate(
		//void
	));

    rpg3D::MovingEntityTemplate* movingEntity;
	t->setModuleTemplate(movingEntity = new rpg3D::MovingEntityTemplate(
		// Turn is in degrees per second.
		360.0f * 2.0f, 360.0f * 4.0f
	));
    movingEntity->pushMovementConfig(new rpg3D::MovementConfigPhysicsPush(
		60.0f/*moveForce*/, 0.9995/*moveDamping*/,
		0.9999/*moveStopDamping*/,
		false/*applyForceToMovementPBodyOnly*/
    ));

	t->setModuleTemplate(new rpg3D::StackableTemplate(
		true/*canHaul*/, true,/*canBeCarried*/
        0.52f/*haulOffsetZM*/,
		true/*allowPlaceOnThis*/
	));

	t->setModuleTemplate(new Touchable3DTemplate(
		Touchable3DTemplate::ShapeType::FixedSizeAABB,
		Vector3(
            -0.15f,
            -0.15f,
            -0.0f
        ),
        Vector3(
            0.15f,
            0.15f,
            0.5f
        )
	));

	t->setModuleTemplate(new rpg3D::EditableVoxelisedTemplate(
		true/*isEditorStructureEnabled*/
	));

    return t;
});

Drawable3DCreateConfig_VoxelGrid* T_Npc_Shopkeep_A_V1::buildBase() {
	Vector3Int gSize{ 7, 8, 13 };

    const signed char O = -1;
    const signed char A = 0;
    const signed char B = 1;
    const signed char C = 2;

	Array3D<signed char>* grid = new Array3D<signed char>(
        gSize.x, gSize.y, gSize.z,
        false, true, true,
        new signed char[gSize.x * gSize.y * gSize.z]{
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,

            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,

            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,C,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,C,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,

            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,C,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,C,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,

            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,

            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,C,
            A,A,A,A,A,A,C,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,

            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,
            A,A,A,A,A,A,A,

            O,O,O,A,O,O,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,O,O,A,O,O,O,

            O,O,O,A,O,O,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,O,O,A,O,O,O,

            O,O,O,A,O,O,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,O,O,A,O,O,O,

            O,O,O,B,O,O,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,A,A,A,A,A,O,
            O,O,O,B,O,O,O,

            O,O,O,O,O,O,O,
            O,O,A,A,A,O,O,
            O,O,A,A,A,O,O,
            O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,
            O,O,A,A,A,O,O,
            O,O,A,A,A,O,O,
            O,O,O,O,O,O,O,

            O,O,O,O,O,O,O,
            O,O,O,A,O,O,O,
            O,O,O,A,O,O,O,
            O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,
            O,O,O,A,O,O,O,
            O,O,O,A,O,O,O,
            O,O,O,O,O,O,O
        }
	);

	ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#E7E7E7FF")));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#E7BC8DFF")));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#000000FF")));

	return new Drawable3DCreateConfig_VoxelGrid(
		"_Base_",//id
		new std::string("_ROOT_"),//parentId

		nullptr,// pos
		nullptr,//rot
		nullptr,// scale

		0.04f,
		new Vector3(-gSize.x / 2.0f + 0.5f, -gSize.y / 2.0f + 0.5f, -0.5f + 0.5f),

		grid, nullptr,
		materialsList, nullptr
	);
}
