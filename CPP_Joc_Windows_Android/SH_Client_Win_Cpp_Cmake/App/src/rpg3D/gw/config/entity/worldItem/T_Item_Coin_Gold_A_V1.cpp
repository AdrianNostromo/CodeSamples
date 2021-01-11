#include "T_Item_Coin_Gold_A_V1.h"
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <rpg3D/gw/entity/template/worldItem/WorldItemTemplate.h>
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
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/statics/StaticsInit.h>
#include <base/list/ArrayList.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigBox.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>
#include <rpg3D/gw/entity/template/physics3D/pActorController/CfgCharacterPActorController.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>

using namespace rpg3D;

std::string T_Item_Coin_Gold_A_V1::ID = "Item_Coin_Gold_A_V1";

const float T_Item_Coin_Gold_A_V1::GROUND_PLACEMENT_OFFSET_Z = 0.1f;

EntityTemplate* T_Item_Coin_Gold_A_V1::TEMPLATE = base::StaticsInit::AddCbGeneral<EntityTemplate*>(1, []() {
	EntityTemplate* t = TEMPLATE = EntityTemplatesList::newEntry(ID);

	t->tagsList.appendReference(rpg3D::EntityTags::COIN);

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
            -(1.0f / 2.0f) * 0.05f,
            -(4.0f / 2.0f) * 0.05f,
            0.0f
        ),
        Vector3(
            1.0f / 2.0f * 0.05f,
            (4.0f / 2.0f) * 0.05f,
            4.0f * 0.05f
        )
    ));

    t->setModuleTemplate(FrustumCullable3DTemplate::newOfsetedZSphere(
        0.1f/*offsetZ*/,
        Math::dist(
            0.0f, 0.0f, 
            0.1f, 0.1f
        )/*radiusM*/
    ));

    rpg3D::Physics3DTemplate* physics3D;
    t->setModuleTemplate(physics3D = new rpg3D::Physics3DTemplate(
        rpg3D::PhysicsCollissionConfig::Unit_Projectile,
		new rpg3D::CfgCharacterPActorController()
	));
	physics3D->pBodiesConfigsList.append_emplace_chain(
        rpg3D::PBodyConfig::UssageBitGroups::Movement | rpg3D::PBodyConfig::UssageBitGroups::CardinalRotation | rpg3D::PBodyConfig::UssageBitGroups::VisualsUpdate,
        rpg3D::PBodyConfig::DynamicsType::Dynamic/*dynamicsType*/,
        new Vector3{ 0.0f, 0.0f, 0.0f }/*physicsBodyOffsetPos*/, nullptr/*physicsBodyOffsetRot*/,
		new physics3D::PhysicsShapeConfigBox(
            rpg3D::PhysicsUtil::SMALL_COLLISSION_MARGIN/*collisionMargin*/,
            Vector3{0.025f, 0.1f, 0.1f}/*halfExtents*/
		),
        0.5f/*mass*/, 0.5f/*friction*/, 0.0f/*rollingFriction*/,
        Vector3{ 1, 1, 1 }/*enabledRotationAxes*/,
        0.0f/*linearDamping*/, 0.0f/*angularDamping*/,
        nullptr/*customGravity*/
	);

    t->setModuleTemplate(new rpg3D::WorldItemTemplate(
        &rpg3D::SoundConfigsList::Projectile_Arrow_Basic_Impact_A_V1->managedPath->path, 0.25f
    ));

    return t;
});

Drawable3DCreateConfig_VoxelGrid* T_Item_Coin_Gold_A_V1::buildBase() {
	Vector3Int gSize{ 1, 4, 4 };
	
	const signed char O = -1;// Unused
    // Body
    const signed char B = 0;
	
	Array3D<signed char>* grid = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z,
		false, true, true,
		new signed char[gSize.x * gSize.y * gSize.z]{
            O,
            B,
            B,
            O,
            
            B,
            B,
            B,
            B,
            
            B,
            B,
            B,
            B,
            
            O,
            B,
            B,
            O
        }
    );

    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#F6E503FF")
    ));

    return new Drawable3DCreateConfig_VoxelGrid(
        "_VoxelGridGroup_",//id
        new std::string("_ROOT_"),//parentId

        nullptr,// pos
        nullptr,//rot
        nullptr,// scale

        0.05f,
        new Vector3(-gSize.x / 2.0f + 0.5f, -gSize.y / 2.0f + 0.5f, -gSize.z / 2.0f + 0.5f),

        grid, nullptr,
        materialsList, nullptr
    );
}
