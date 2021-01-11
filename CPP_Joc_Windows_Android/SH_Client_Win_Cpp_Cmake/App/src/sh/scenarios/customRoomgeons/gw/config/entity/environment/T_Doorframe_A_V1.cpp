#include "T_Doorframe_A_V1.h"
#include <rpg3D/gw/config/entity/EntityTags.h>
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
#include <rpg3D/gw/entity/template/grid2DContent/Grid2DContentTemplate.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include "../../../../Namespace.h"

using namespace customRoomgeons;

std::string T_Doorframe_A_V1::ID = "T_Doorframe_A_V1";

EntityTemplate* T_Doorframe_A_V1::TEMPLATE = base::StaticsInit::AddCbGeneral<EntityTemplate*>(1, []() {
	EntityTemplate* t = TEMPLATE = EntityTemplate::newEntry(Namespace::NAMESPACE, ID);

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

    return t;
});

Drawable3DCreateConfig_VoxelGrid* T_Doorframe_A_V1::buildBase() {
	Vector3Int gSize{ 16, 4, 11 };
	
	const signed char O = -1;// Unused
    const signed char B = 0;
	
	Array3D<signed char>* grid = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z,
		false, true, true,
		new signed char[gSize.x * gSize.y * gSize.z]{
            B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,
            B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,
            B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,
            B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,
            
            B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,
            B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,
            B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,
            B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,B,

            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            O,B,B,O,O,O,O,O,O,O,O,O,O,B,B,O,
            O,B,B,O,O,O,O,O,O,O,O,O,O,B,B,O,
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            O,B,B,O,O,O,O,O,O,O,O,O,O,B,B,O,
            O,B,B,O,O,O,O,O,O,O,O,O,O,B,B,O,
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            O,B,B,O,O,O,O,O,O,O,O,O,O,B,B,O,
            O,B,B,O,O,O,O,O,O,O,O,O,O,B,B,O,
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            O,B,B,O,O,O,O,O,O,O,O,O,O,B,B,O,
            O,B,B,O,O,O,O,O,O,O,O,O,O,B,B,O,
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            O,B,B,O,O,O,O,O,O,O,O,O,O,B,B,O,
            O,B,B,O,O,O,O,O,O,O,O,O,O,B,B,O,
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            O,B,B,O,O,O,O,O,O,O,O,O,O,B,B,O,
            O,B,B,O,O,O,O,O,O,O,O,O,O,B,B,O,
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            O,B,B,O,O,O,O,O,O,O,O,O,O,B,B,O,
            O,B,B,O,O,O,O,O,O,O,O,O,O,B,B,O,
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
            O,B,B,O,O,O,O,O,O,O,O,O,O,B,B,O,
            O,B,B,O,O,O,O,O,O,O,O,O,O,B,B,O,
            O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,

            B,B,B,B,O,O,O,O,O,O,O,O,B,B,B,B,
            B,B,B,B,O,O,O,O,O,O,O,O,B,B,B,B,
            B,B,B,B,O,O,O,O,O,O,O,O,B,B,B,B,
            B,B,B,B,O,O,O,O,O,O,O,O,B,B,B,B
        }
    );

    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#AAAAAAFF")
    ));

    return new Drawable3DCreateConfig_VoxelGrid(
        "_VoxelGridGroup_",//id
        new std::string("_ROOT_"),//parentId

        nullptr,// pos
        nullptr,//rot
        nullptr,// scale

        0.1f,
        new Vector3(-gSize.x / 2.0f + 0.5f, -gSize.y / 2.0f + 0.5f, 0.5f),

        grid, nullptr,
        materialsList, nullptr
    );
}

