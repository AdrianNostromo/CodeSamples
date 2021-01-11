#include "Template_Mob_ChickenA_V1.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <rpg3D/gw/entity/template/LivingEntityTemplate.h>
#include <rpg3D/gw/entity/template/TeamTemplate.h>
#include <rpg3D/gw/entity/template/payloadReceiverTarget/PayloadReceiverTargetTemplate.h>
#include <rpg3D/gw/entity/template/projectile/ProjectileTemplate.h>
#include <rpg3D/gw/entity/template/controller/ControllerTemplate.h>
#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <worldGame3D/gw/entity/template/actions/ActionsTemplate.h>
#include <worldGame3D/gw/entity/template/lookAt/LookAtTemplate.h>
#include <rpg3D/gw/entity/template/userControlled/UserControlledTemplate.h>
#include <rpg3D/gw/entity/template/statusEffects/StatusEffectsTemplate.h>
#include <sh/scenarios/islandSurvival_a_v1/Namespace.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <base/statics/StaticsInit.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigCapsule.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>
#include <rpg3D/gw/entity/template/physics3D/pActorController/CfgCharacterPActorController.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigPhysicsPush.h>

using namespace islandSurvival_a_v1;

std::string Template_Mob_ChickenA_V1::ID = "Mob_ChickenA_V1";

EntityTemplate* Template_Mob_ChickenA_V1::TEMPLATE = base::StaticsInit::AddCbGeneral<EntityTemplate*>(1, []() {
	EntityTemplate* t = TEMPLATE = EntityTemplate::newEntry(Namespace::NAMESPACE, ID);

	t->tagsList.appendReference(rpg3D::EntityTags::UNIT);
	t->tagsList.appendReference(rpg3D::EntityTags::MOB);

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

	t->setModuleTemplate(new Touchable3DTemplate(
		Touchable3DTemplate::ShapeType::AABB,
		Vector3(
			-(13.0f / 2.0f) * 0.03f,
			-(10.0f / 2.0f) * 0.03f,
			0.0f
		),
		Vector3(
			11.0f / 2.0f * 0.03f,
			(10.0f / 2.0f) * 0.03f,
			12.0f * 0.03f
		)
	));

	t->setModuleTemplate(FrustumCullable3DTemplate::newOfsetedZSphere(
		0.4f,
		Math::dist(0.0f, 0.0f, 0.4f, 0.4f)
	));

	t->setModuleTemplate(new worldGame3D::ActionsTemplate(
		//void
	));

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

	t->setModuleTemplate(new rpg3D::LivingEntityTemplate(
		2
	));
	t->setModuleTemplate(new rpg3D::TeamTemplate(
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
        0.6f/*linearDamping*/, 0.0f/*angularDamping*/,
        nullptr/*customGravity*/
	);

	t->setModuleTemplate(new rpg3D::PayloadReceiverTargetTemplate());

	t->setModuleTemplate(new rpg3D::ControllerTemplate(
		//void
	));

	t->setModuleTemplate(new rpg3D::StatusEffectsTemplate(
		//void
	));
});

Drawable3DCreateConfig_VoxelGrid* Template_Mob_ChickenA_V1::buildBase() {
	Vector3Int gSize{ 12, 10, 12 };
	
	const signed char O = -1;// Unused
    // Body
    const signed char B = 0;
    // Feet
	const signed char F = 1;
    // Nose
	const signed char N = 2;
    // Mouth
	const signed char M = 3;
    // Eye
	const signed char E = 4;
    // Crest
	const signed char C = 5;

    Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z,
		false, true, true,
		new signed char[gSize.x * gSize.y * gSize.z]{
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,C,C,C,C,C,O,O,O,
            O,O,O,O,C,C,C,C,C,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,

            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,B,B,B,B,B,O,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,O,B,B,B,B,B,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,

            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,

            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,B,B,B,B,B,B,E,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,B,B,B,B,B,B,E,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,

            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,B,B,B,B,B,B,E,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,B,B,B,B,B,B,E,O,O,
            O,O,O,B,B,B,B,B,B,B,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,

            O,O,O,B,B,B,B,O,O,O,O,O,
            O,O,O,B,B,B,B,B,O,O,O,O,
            B,B,B,B,B,B,B,B,B,B,O,O,
            B,B,B,B,B,B,B,B,B,B,O,O,
            B,B,B,B,B,B,B,B,B,B,N,N,
            B,B,B,B,B,B,B,B,B,B,N,N,
            B,B,B,B,B,B,B,B,B,B,O,O,
            B,B,B,B,B,B,B,B,B,B,O,O,
            O,O,O,B,B,B,B,B,O,O,O,O,
            O,O,O,B,B,B,B,O,O,O,O,O,

            O,O,B,B,B,B,B,O,O,O,O,O,
            O,O,O,O,B,B,B,B,O,O,O,O,
            B,B,B,B,B,B,B,B,B,B,O,O,
            B,B,B,B,B,B,B,B,B,B,O,O,
            B,B,B,B,B,B,B,B,B,B,N,N,
            B,B,B,B,B,B,B,B,B,B,N,N,
            B,B,B,B,B,B,B,B,B,B,O,O,
            B,B,B,B,B,B,B,B,B,B,O,O,
            O,O,O,O,B,B,B,B,O,O,O,O,
            O,O,B,B,B,B,B,O,O,O,O,O,

            O,O,O,B,B,B,B,O,O,O,O,O,
            O,O,O,O,B,B,B,B,O,O,O,O,
            B,B,B,B,B,B,B,B,B,B,O,O,
            B,B,B,B,B,B,B,B,B,B,O,O,
            B,B,B,B,B,B,B,B,B,B,M,O,
            B,B,B,B,B,B,B,B,B,B,M,O,
            B,B,B,B,B,B,B,B,B,B,O,O,
            B,B,B,B,B,B,B,B,B,B,O,O,
            O,O,O,O,B,B,B,B,O,O,O,O,
            O,O,O,B,B,B,B,O,O,O,O,O,

            O,O,O,B,B,B,B,O,O,O,O,O,
            O,O,O,B,B,B,B,B,O,O,O,O,
            B,B,B,B,B,B,B,B,B,B,O,O,
            B,B,B,B,B,B,B,B,B,B,O,O,
            B,B,B,B,B,B,B,B,B,B,O,O,
            B,B,B,B,B,B,B,B,B,B,O,O,
            B,B,B,B,B,B,B,B,B,B,O,O,
            B,B,B,B,B,B,B,B,B,B,O,O,
            O,O,O,B,B,B,B,B,O,O,O,O,
            O,O,O,B,B,B,B,O,O,O,O,O,

            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,B,O,O,O,O,O,
            O,O,O,O,O,B,B,B,O,O,O,O,
            O,O,O,O,O,O,B,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,B,O,O,O,O,O,
            O,O,O,O,O,B,B,B,O,O,O,O,
            O,O,O,O,O,O,B,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,

            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,F,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,F,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,

            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,F,F,F,F,F,O,O,
            O,O,O,O,O,F,F,F,F,O,O,O,
            O,O,O,O,O,F,F,F,F,F,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,F,F,F,F,F,O,O,
            O,O,O,O,O,F,F,F,F,O,O,O,
            O,O,O,O,O,F,F,F,F,F,O,O,
            O,O,O,O,O,O,O,O,O,O,O,O
        }
    );

    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#E7E7E7FF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#E7D719FF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#E7D719FF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#E7392BFF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#000000FF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#E7392BFF")
    ));

    return new Drawable3DCreateConfig_VoxelGrid(
        "_VoxelGridGroup_",//id
        new std::string("_ROOT_"),//parentId

        nullptr,// pos
        nullptr,//rot
        nullptr,// scale

        0.03f,
		new Vector3(-gSize.x / 2.0f + 0.5f, -gSize.y / 2.0f + 0.5f, -0.5f + 0.5f),

        padVoxelGrid, nullptr,
        materialsList, nullptr
    );
}
