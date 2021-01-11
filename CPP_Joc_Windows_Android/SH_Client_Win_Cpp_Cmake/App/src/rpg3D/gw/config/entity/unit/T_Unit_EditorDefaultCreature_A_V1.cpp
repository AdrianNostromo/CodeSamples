#include "T_Unit_EditorDefaultCreature_A_V1.h"
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
#include <base/statics/StaticsInit.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigCapsule.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigCylinder.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>
#include <rpg3D/gw/entity/template/physics3D/pActorController/CfgCharacterPActorController.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigPhysicsPush.h>

using namespace rpg3D;

std::string T_Unit_EditorDefaultCreature_A_V1::DATA_KEY_voxelGridZXY = "_voxelGridZXY_";
std::string T_Unit_EditorDefaultCreature_A_V1::DATA_KEY_materials = "_materials_";

float T_Unit_EditorDefaultCreature_A_V1::default_voxelSize = 0.03f;
float T_Unit_EditorDefaultCreature_A_V1::default_stackable_haulOffsetZM = 0.33;// The height from ground to head (excluding the crest);
Vector3 T_Unit_EditorDefaultCreature_A_V1::default_voxelGridOffset{ -12.0f / 2.0f + 0.5f, -10.0f / 2.0f + 0.5f, 0.5f };
Vector3 T_Unit_EditorDefaultCreature_A_V1::default_fixedSizeAABBToushShape_min{
	-0.1f,
	-0.1f,
	-0.0f
};
Vector3 T_Unit_EditorDefaultCreature_A_V1::default_fixedSizeAABBToushShape_max{
	0.1f,
	0.1f,
	0.33f
};

Array3D<signed char>* T_Unit_EditorDefaultCreature_A_V1::chicken_voxelGridZXY = base::StaticsInit::AddCbGeneral<Array3D<signed char>*>(1, []() {
	const int gSizeX = 12;
	const int gSizeY = 10;
	const int gSizeZ = 12;

	// Empty
	const signed char O = -1;
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

	chicken_voxelGridZXY = new Array3D<signed char>(
		gSizeX, gSizeY, gSizeZ,
		false, true, true,
		new signed char[gSizeX * gSizeY * gSizeZ]{
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

			// 6
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

			//11
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
});

ArrayList<Visual3DModelMaterial*>* T_Unit_EditorDefaultCreature_A_V1::chicken_materialsList = base::StaticsInit::AddCbGeneral<ArrayList<Visual3DModelMaterial*>*>(1, []() {
	chicken_materialsList = new ArrayList<Visual3DModelMaterial*>();

	chicken_materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#E7E7E7FF")
	));
	chicken_materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#E7D719FF")
	));
	chicken_materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#E7D719FF")
	));
	chicken_materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#E7392BFF")
	));
	chicken_materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#000000FF")
	));
	chicken_materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#E7392BFF")
	));
});

Array3D<signed char>* T_Unit_EditorDefaultCreature_A_V1::flatPad_voxelGridZXY = base::StaticsInit::AddCbGeneral<Array3D<signed char>*>(1, []() {
	Vector3Int gSize{ 7, 7, 4 };

	// Empty
	const signed char O = -1;
	// Base
	const signed char A = 0;
	// Green
	const signed char R = 1;
	// Green
	const signed char G = 2;
	// Blue
	const signed char B = 3;

	flatPad_voxelGridZXY = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z,
		false, true, true,
		new signed char[gSize.x * gSize.y * gSize.z]{
			O,O,O,O,O,O,O,
			O,O,O,O,O,O,O,
			O,O,O,O,O,O,O,
			O,O,O,B,O,O,O,
			O,O,O,O,O,O,O,
			O,O,O,O,O,O,O,
			O,O,O,O,O,O,O,

			O,O,O,O,O,O,O,
			O,O,O,O,O,O,O,
			O,O,O,O,O,O,O,
			O,O,O,B,O,O,O,
			O,O,O,O,O,O,O,
			O,O,O,O,O,O,O,
			O,O,O,O,O,O,O,

			O,O,O,O,O,O,O,
			O,O,O,O,O,O,O,
			O,O,O,O,O,O,O,
			O,O,O,B,O,O,O,
			O,O,O,O,O,O,O,
			O,O,O,O,O,O,O,
			O,O,O,O,O,O,O,

			A,A,A,G,A,A,A,
			A,A,A,G,A,A,A,
			A,A,A,G,A,A,A,
			A,A,A,A,R,R,R,
			A,A,A,A,A,A,A,
			A,A,A,A,A,A,A,
			A,A,A,A,A,A,A
		}
	);
});

ArrayList<Visual3DModelMaterial*>* T_Unit_EditorDefaultCreature_A_V1::flatPad_materialsList = base::StaticsInit::AddCbGeneral<ArrayList<Visual3DModelMaterial*>*>(1, []() {
	flatPad_materialsList = new ArrayList<Visual3DModelMaterial*>();

	flatPad_materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#E7E7E7FF")
		));
	flatPad_materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#880009FF")
		));
	flatPad_materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#008800FF")
		));
	flatPad_materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#000088FF")
	));
});

int T_Unit_EditorDefaultCreature_A_V1::templateVariationIdCounter = 0;
EntityTemplate* T_Unit_EditorDefaultCreature_A_V1::getNew(
	float voxelSize, Vector3& voxelGridOffset,
	Vector3& fixedSizeAABBToushShape_min, Vector3& fixedSizeAABBToushShape_max,
	float stackable_haulOffsetZM)
{
    EntityTemplate* t = EntityTemplatesList::newEntry("Unit_EditorDefaultCreature_A_V1_" + std::to_string(++templateVariationIdCounter));

    t->tagsList.appendReference(EntityTags::UNIT);
    t->tagsList.appendReference(EntityTags::CREATURE);
    t->tagsList.appendReference(EntityTags::EDITABLE_CREATURE);

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
                    ->appendDirect_chain(new Drawable3DCreateConfig_VoxelGrid(
                        "_VoxelGridGroup_",//id
                        new std::string("_ROOT_"),//parentId

                        nullptr,// pos
                        // Note rot is dynamic.
                        nullptr,//rot
                        nullptr,// scale

						voxelSize,
						new Vector3(voxelGridOffset),

                        nullptr, &DATA_KEY_voxelGridZXY,
                        nullptr, &DATA_KEY_materials
                    ))
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
		0.6f/*linearDamping*/, 0.0f/*angularDamping*/,
		nullptr/*customGravity*/
	);

	t->setModuleTemplate(new PlatformInteractorTemplate());

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

	//asdA19;// Make this dynamic for this template.
	t->setModuleTemplate(new rpg3D::StackableTemplate(
		true/*canHaul*/, true,/*canBeCarried*/
		stackable_haulOffsetZM,
		true/*allowPlaceOnThis*/
	));

	t->setModuleTemplate(new Touchable3DTemplate(
		Touchable3DTemplate::ShapeType::FixedSizeAABB,
		Vector3{ fixedSizeAABBToushShape_min },
		Vector3{ fixedSizeAABBToushShape_max }
	));

	t->setModuleTemplate(new TeamTemplate(
		//void
	));

	t->setModuleTemplate(new rpg3D::EditableVoxelisedTemplate(
		true/*isEditorStructureEnabled*/
	));

    return t;
}
