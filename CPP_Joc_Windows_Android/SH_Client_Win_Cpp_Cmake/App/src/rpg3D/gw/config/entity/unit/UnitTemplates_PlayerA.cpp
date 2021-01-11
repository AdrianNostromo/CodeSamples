#include "UnitTemplates_PlayerA.h"
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigPhysicsRollingSphere.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigDirect.h>
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
#include <rpg3D/gw/entity/template/voxelizedEditor/VoxelizedEditorTemplate.h>
#include <rpg3D/gw/entity/template/zoneEditor/ZoneEditorTemplate.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigSphere.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigCapsule.h>
#include <rpg3D/gw/entity/template/physics3D/constraint/PhysicsConstraintConfigGeneric6Dof.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>
#include <rpg3D/gw/entity/template/physics3D/pActorController/CfgCharacterPActorController.h>
#include <base/physics3D/config/body/PBodyConfig.h>
#include <rpg3D/gw/entity/template/grid2DContent/Grid2DContentTemplate.h>
#include <rpg3D/gw/entity/template/inventory/InventoryTemplate.h>
#include <rpg3D/gw/entity/template/worldItemsCollector/WorldItemsCollectorTemplate.h>

using namespace rpg3D;

ArrayList<Visual3DModelMaterial*>* UnitTemplates_PlayerA::defaultMaterialsList = new ArrayList<Visual3DModelMaterial*>();

EntityTemplate* UnitTemplates_PlayerA::GetNew() {
    EntityTemplate* t = EntityTemplatesList::newEntry("Unit_PlayerA");

    t->tagsList.appendReference(EntityTags::UNIT);
    t->tagsList.appendReference(EntityTags::PLAYER_UNIT);

    defaultMaterialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#E7E7E7FF")
    ));
    defaultMaterialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#E7BC8DFF")
    ));
    defaultMaterialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#000000FF")
    ));

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
                        "_HeadInventory_",
                        new std::string("_ROOT_"),
                        // Position is exactly above the player.
                        new Vector3(0.0f, 0.0f, 13.0f * (0.1f * 0.4f)),
                        nullptr,
                        nullptr// scale should not be applied here.
                    ))
                    /*->appendDirect_chain(new Drawable3DCreateConfig_Group(
                            "_ToolSlotHead_",
                            "_ROOT_",
                            // Position is above the player with a 1 voxel spacing currently.
                            //asd_z;// make the z pos programatic to be above the visual2D inventory.
                            new Pos3D(3f * (0.1f * 0.4f), 0f, (13f + 1f) * (0.1f * 0.4f)),
                            nullptr,
                            nullptr// scale should not be applied here.
                    ))*/
                    ->appendDirect_chain(new Drawable3DCreateConfig_Group(
                        "_ToolSlotRightArm_"/*id*/,
                        new std::string("_ROOT_")/*parentId*/,
                        // Position is above the player with a 1 voxel spacing currently.
                        new Vector3(0.0f * 0.04f, -4.5f * 0.04f, 5.5f * 0.04f)/*pos*/,
                        nullptr/*rot*/,
                        nullptr/*scale*//*Scale should not be applied here.*/
                    ))
                    ->appendDirect_chain(new Drawable3DCreateConfig_Group(
                        "_ToolSlotLeftArm_",
                        new std::string("_ROOT_"),
                        // Position is above the player with a 1 voxel spacing currently.
                        new Vector3(0.0f * 0.04f, 4.5f * 0.04f, 5.5f * 0.04f),
                        nullptr,
                        nullptr// scale should not be applied here.
                    ))
                    ->appendDirect_chain(new Drawable3DCreateConfig_VoxelGrid(
                        "_VoxelGridGroup_",//id
                        new std::string("_ROOT_"),//parentId

                        nullptr,// pos
                        // Note rot is dynamic.
                        nullptr,//rot
                        nullptr,// scale

                        0.04f,
                        new Vector3(-7.0f / 2.0f + 0.5f, -8.0f / 2.0f + 0.5f, 0.5f),

                        nullptr, &DATA_KEY_voxelGridZXY,
                        nullptr, &DATA_KEY_materials
                    ))
                )
        ),
        false,//requiresHalfGridOffsetX
        false//requiresHalfGridOffsetY
    ));

    rpg3D::MovingEntityTemplate* movingEntity;
    t->setModuleTemplate(movingEntity = new rpg3D::MovingEntityTemplate(
        // Turn is in degrees per second.
        360.0f * 2.0f, 360.0f * 4.0f
    ));
    movingEntity->pushMovementConfig(new rpg3D::MovementConfigPhysicsRollingSphere(
        15.0f/*moveAngularVelocity*/, 0.0f/*moveAngularDamping*/,
        2.9f/*stopAngularDamping*/
    ));
    movingEntity->pushMovementConfig(new rpg3D::MovementConfigDirect(
        2.4f/*moveSpeedS*/
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

    t->setModuleTemplate(new PlatformInteractorTemplate());

    t->setModuleTemplate(FrustumCullable3DTemplate::newOfsetedZSphere(
        0.4f,
        Math::dist(0.0f, 0.0f, 0.4f, 0.4f)
    ));

    t->setModuleTemplate(new LookAtTemplate(
        //void
    ));

    t->setModuleTemplate(new ToolsHandlerTemplate(
        new Array1D<ToolSlotConfig*>(2, new ToolSlotConfig*[2] {
            new ToolSlotConfig("_ToolSlotRightArm_"),
            new ToolSlotConfig("_ToolSlotLeftArm_")
        })
    ));

    t->setModuleTemplate(new TeamTemplate(
        //void
    ));
    t->setModuleTemplate(new worldGame3D::ActionsTemplate(
        //void
    ));
    t->setModuleTemplate(new UserControlledTemplate(
        //void
    ));
    t->setModuleTemplate(new rpg3D::LivingEntityTemplate(
        4
    ));

    t->setModuleTemplate(new rpg3D::PayloadReceiverTargetTemplate());

    t->setModuleTemplate(new rpg3D::ControllerTemplate(
        //void
    ));

	t->setModuleTemplate(new rpg3D::StackableTemplate(
		true/*canHaul*/, true,/*canBeCarried*/
		0.52f/*haulOffsetZM*/,
		true/*allowPlaceOnThis*/
	));
	
	t->setModuleTemplate(new rpg3D::VoxelizedEditorTemplate(
		//void
	));
	t->setModuleTemplate(new rpg3D::ZoneEditorTemplate(
		//void
	));
	
	const float CM = rpg3D::PhysicsUtil::DEFAULT_COLLISSION_MARGIN;
	// The size must be 0.04f smaller in each direction (6 directions) because of the 0.04f collission margin.

    rpg3D::Physics3DTemplate* physics3D;
    t->setModuleTemplate(physics3D = new rpg3D::Physics3DTemplate(
        rpg3D::PhysicsCollissionConfig::Unit,
        new rpg3D::CfgCharacterPActorController()
    ));
    physics3D->pBodiesConfigsList.append_emplace_chain(
        rpg3D::PBodyConfig::UssageBitGroups::Movement,
        rpg3D::PBodyConfig::DynamicsType::Dynamic/*dynamicsType*/,
        new Vector3{ 0.0f, 0.0f, 0.2f }/*physicsBodyOffsetPos*/, nullptr/*physicsBodyOffsetRot*/,
        new physics3D::PhysicsShapeConfigSphere(
            0.2f/*radius*/
        ),
        10.0f/*mass*/, 0.95f/*friction*/, 0.0f/*rollingFriction*/,
        Vector3{ 1, 1, 1 }/*enabledRotationAxes*/,
        0.0f/*linearDamping*/, 0.0f/*angularDamping*/,
        nullptr/*customGravity*/
    );
    physics3D->pBodiesConfigsList.append_emplace_chain(
        rpg3D::PBodyConfig::UssageBitGroups::CardinalRotation | rpg3D::PBodyConfig::UssageBitGroups::VisualsUpdate,
        rpg3D::PBodyConfig::DynamicsType::Dynamic/*dynamicsType*/,
        new Vector3{ 0.0f, 0.0f, 0.6f }/*physicsBodyOffsetPos*/, nullptr/*physicsBodyOffsetRot*/,
        new physics3D::PhysicsShapeConfigSphere(
            0.2f/*radius*/
        ),
        10.0f/*mass*/, 0.0f/*friction*/, 0.0f/*rollingFriction*/,
        Vector3{ 0, 0, 0 }/*enabledRotationAxes*/,
        0.0f/*linearDamping*/, 0.0f/*angularDamping*/,
        nullptr/*customGravity*/
    );

    physics3D->pConstraintsConfigsList.appendDirect(new PhysicsConstraintConfigGeneric6Dof(
        0/*pBodyIndexA*/, 1/*pBodyIndexB*/,
        Vector3(0.0f, 0.0f, 0.0f)/*framePosInA*/, Vector3(0.0f, 0.0f, -0.4f)/*framePosInB*/,
        PhysicsConstraintConfigGeneric6Dof::Limit(0.0f, 0.0f)/*limitPosX*/, PhysicsConstraintConfigGeneric6Dof::Limit(0.0f, 0.0f)/*limitPosY*/, PhysicsConstraintConfigGeneric6Dof::Limit(0.0f, 0.0f)/*limitPosZ*/,
        PhysicsConstraintConfigGeneric6Dof::Limit(PhysicsConstraintConfigGeneric6Dof::Limit::Type::FreeSpinning)/*limitAngX*/, PhysicsConstraintConfigGeneric6Dof::Limit(PhysicsConstraintConfigGeneric6Dof::Limit::Type::FreeSpinning)/*limitAngY*/, PhysicsConstraintConfigGeneric6Dof::Limit(PhysicsConstraintConfigGeneric6Dof::Limit::Type::FreeSpinning)/*limitAngZ*/,
        true/*disableCollisionsBetweenLinkedBodies*/
    ));
    
	t->setModuleTemplate(new rpg3D::Grid2DContentTemplate(
		false/*isGridOrganisingGPMandatory*/
	));

    t->setModuleTemplate(new rpg3D::InventoryTemplate());
    t->setModuleTemplate(new rpg3D::WorldItemsCollectorTemplate());

    return t;
}

/*asdB
    .setModuleTemplate(new StaminaTemplate(
        100f,
        35f
    ))
    .setModuleTemplate(new VoxelInventoryTemplate(
        //void
    ))
    .setModuleTemplate(new VoxelInventoryRendererTemplate(
        "_HeadInventory_",

        7,// maxGridSize_x
        8,// maxGridSize_y

        0.04f,

        ArrayUtil.arrayToArrayList(new Visual3DModelMaterial[] {
            new Visual3DModelMaterial_Color(
                Color.valueOf("#FFFFFFFF"), nullptr,
                true, true,
                false, 0f,
                -1
            )
        }, new ArrayList<Visual3DModelMaterial*>())
    ))
*/

std::string UnitTemplates_PlayerA::DATA_KEY_voxelGridZXY = "_voxelGridZXY_";
std::string UnitTemplates_PlayerA::DATA_KEY_materials = "_materials_";

const signed char O = -1;
const signed char A = 0;
const signed char B = 1;
const signed char C = 2;

Array3D<signed char>* UnitTemplates_PlayerA::defaultVoxelGridZXY = new Array3D<signed char>(
    7, 8, 13,
    false, true, true,
    new signed char[7 * 8 * 13] {
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
