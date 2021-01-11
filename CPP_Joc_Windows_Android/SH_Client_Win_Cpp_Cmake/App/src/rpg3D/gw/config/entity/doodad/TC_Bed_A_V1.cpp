#include "TC_Bed_A_V1.h"
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
#include <base/physics3D/config/shape/PhysicsShapeConfigSphere.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>
#include <rpg3D/gw/entity/template/physics3D/pActorController/CfgCharacterPActorController.h>
#include <rpg3D/gw/entity/template/grid2DContent/Grid2DContentTemplate.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <base/util/StringUtil.h>
#include "../../../../Namespace.h"

using namespace rpg3D;

bool TC_Bed_A_V1::IsStaticsInit = base::StaticsInit::AddCbGeneral<bool>(1, []() {
    EntityTemplate::customTemplatesFactoriesMap.putDirect(Namespace::NAMESPACE, BaseId, TC_Bed_A_V1::GetOrCreateTemplateFromConfigParams);
});

std::string TC_Bed_A_V1::BaseId = "TC_Bed_A_V1_";

EntityTemplate* TC_Bed_A_V1::GetOrCreateTemplate(
    unsigned int voxelSizeCM)
{
    sp<Array1D<std::shared_ptr<base::IWrappedValue>>> configValues = new Array1D<std::shared_ptr<base::IWrappedValue>>(1);
    configValues->setDirect(0, base::IWrappedValue::new_uint(voxelSizeCM));

    return GetOrCreateTemplateFromConfigParams(configValues);
}

EntityTemplate* TC_Bed_A_V1::GetOrCreateTemplateFromConfigParams(sp<Array1D<std::shared_ptr<base::IWrappedValue>>>& configValues) {
    unsigned int voxelSizeCM = configValues->getReference(0)->getDirectAs_uint();

    std::string smartId = BaseId + "#voxelSizeCM#";

    StringUtil::replace(smartId, "#voxelSizeCM#", std::to_string(voxelSizeCM));
    EntityTemplate* t = EntityTemplate::fromId(Namespace::NAMESPACE, smartId);
    if (t != nullptr) {
        return t;
    }

    t = EntityTemplate::newEntry(Namespace::NAMESPACE, smartId, BaseId, configValues);

	t->tagsList.appendReference(rpg3D::EntityTags::COIN);
	//t->tagsList.appendReference(rpg3D::EntityTags::ZONE_EDITOR_NOT_DELETABLE);

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
                    ->appendDirect_chain(BuildBase(voxelSizeCM))
                )
        ),
        false,//requiresHalfGridOffsetX
        false//requiresHalfGridOffsetY
    ));

    t->setModuleTemplate(new Touchable3DTemplate(
        Touchable3DTemplate::ShapeType::AABB,
        Vector3(
            -0.3f,
            -0.2f,
            -0.05f
        ),
        Vector3(
            0.3f,
            0.2f,
            0.05f
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
        new Vector3{ 0.0f, 0.0f, 0.15f }/*physicsBodyOffsetPos*/, nullptr/*physicsBodyOffsetRot*/,
        new physics3D::PhysicsShapeConfigSphere(
            0.15f/*radius*/
        ),
        2.0f/*mass*/, 1.0f/*friction*/, 0.0f/*rollingFriction*/,
        Vector3{ 0, 0, 1 }/*enabledRotationAxes*/,
        0.0f/*linearDamping*/, 0.0f/*angularDamping*/,
        nullptr/*customGravity*/
    );

    t->setModuleTemplate(new rpg3D::StackableTemplate(
        false/*canHaul*/, true,/*canBeCarried*/
        0.2f/*haulOffsetZM*/,
        true/*allowPlaceOnThis*/
    ));

    return t;
}

Drawable3DCreateConfig_VoxelGrid* TC_Bed_A_V1::BuildBase(unsigned int voxelSizeCM) {
    Vector3Int gSize{ 8, 4, 3 };

    float voxelSizeM = voxelSizeCM / 100.0f;

    // Empty
    const signed char O = -1;
    // Wood
    const signed char W = 0;
    // Sheet
    const signed char S = 1;
    // Pillow
    const signed char P = 2;

	Array3D<signed char>* grid = new Array3D<signed char>(
		gSize.x, gSize.y, gSize.z,
		false, true, true,
		new signed char[gSize.x * gSize.y * gSize.z]{
            O,O,O,O,O,O,P,P,
            O,O,O,O,O,O,P,P,
            O,O,O,O,O,O,P,P,
            O,O,O,O,O,O,P,P,

            S,S,S,S,S,S,S,S,
            S,S,S,S,S,S,S,S,
            S,S,S,S,S,S,S,S,
            S,S,S,S,S,S,S,S,

            W,O,O,O,O,O,O,W,
            O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,
            W,O,O,O,O,O,O,W
        }
    );

    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#b06f41FF")));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#d9b9a3FF")));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#f7f7f7FF")));

    return new Drawable3DCreateConfig_VoxelGrid(
        "_VoxelGridGroup_",//id
        new std::string("_ROOT_"),//parentId

        nullptr,// pos
        nullptr,//rot
        nullptr,// scale

        voxelSizeM,
        new Vector3(-gSize.x / 2.0f + 0.5f, -gSize.y / 2.0f + 0.5f, 0.5f),

        grid, nullptr,
        materialsList, nullptr
    );
}
