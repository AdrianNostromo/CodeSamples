#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include "T_Projectile_Arrow_Basic_Grid06Range3Tiles_A_V1.h"
#include <rpg3D/gw/entity/template/TeamTemplate.h>
#include <rpg3D/gw/entity/template/projectile/ProjectileTemplate.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <rpg3D/gw/entity/template/flying/FlyingTemplate.h>
#include <rpg3D/gw/entity/template/autoDestroyDuration/AutoDestroyDurationTemplate.h>
#include <worldGame3D/gw/entity/template/actions/ActionsTemplate.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigConvexHull.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>
#include <rpg3D/gw/entity/template/physics3D/pActorController/CfgCharacterPActorController.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>
#include "../../../../Namespace.h"

using namespace rpg3D;

EntityTemplate* T_Projectile_Arrow_Basic_Grid06Range3Tiles_A_V1::getOrCreateTemplate() {
    std::string templateBakedId = "Projectile_Arrow_Basic_Grid06Range3Tiles_A_V1";

    EntityTemplate* t = EntityTemplate::fromId(Namespace::NAMESPACE, templateBakedId);
    if (t != nullptr) {
        return t;
    }

    t = EntityTemplatesList::newEntry(templateBakedId); 
    
    t->tagsList.appendReference(EntityTags::PROJECTILE);

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

	t->setModuleTemplate(new rpg3D::TeamTemplate(
        //void
    ));

	t->setModuleTemplate(new rpg3D::FlyingTemplate(
		5.0f/*flyingSpeedS*/
	));

	t->setModuleTemplate(new rpg3D::AutoDestroyDurationTemplate(
		1.2f/*lifeDistM*/, 1.2f/*lifeDurationS*/
	));

	t->setModuleTemplate(new rpg3D::ProjectileTemplate(
		1/*maxImpactsCount*/, -1/*maxEnvironmentImpactsCount*/, 
        rpg3D::ProjectileTemplate::ImpactAcknowledgeType::EachContact/*impactAcknowledgeType*/, 0.5f/*reImpactOnPersistentContactCooldownS*/,
        true/*freezeMovementOnEnvironmentImpact*/, true/*disablePayloadImpactsOnEnvironmentImpact*/,
		&SoundConfigsList::Projectile_Arrow_Basic_Impact_A_V1->managedPath->path/*impactSoundConfig_id*/, 0.25f/*impactSoundConfig_volume*/, 0.2f/*impactSoundConfig_overlapCooldownS*/,
        0.2f/*payloadPercentDecreasePerTargetImpact*/, 0.2f/*minPayloadsValuesMultiplier*/
    ));

	const float CM = rpg3D::PhysicsUtil::DEFAULT_COLLISSION_MARGIN;
    rpg3D::Physics3DTemplate* physics3D;
    t->setModuleTemplate(physics3D = new rpg3D::Physics3DTemplate(
        rpg3D::PhysicsCollissionConfig::Projectile,
		nullptr/*pActorControllerCfg*/
	));
	physics3D->pBodiesConfigsList.append_emplace_chain(
        rpg3D::PBodyConfig::UssageBitGroups::Movement | rpg3D::PBodyConfig::UssageBitGroups::CardinalRotation | rpg3D::PBodyConfig::UssageBitGroups::VisualsUpdate,
        rpg3D::PBodyConfig::DynamicsType::Dynamic/*dynamicsType*/,
        nullptr/*physicsBodyOffsetPos*/, nullptr/*physicsBodyOffsetRot*/,
        new physics3D::PhysicsShapeConfigConvexHull(
            CM/*collisionMargin*/,
            2,
            new physics3D::PhysicsShapeConfigConvexHull::Vec3[2]{
                {-0.4f * 0.03f - 0.04f + CM, 0.0f, 0.0f}, {0.4f * 0.03f + 0.04f - CM, 0.0f, 0.0f}
            }
        ),
        0.02f/*mass*/, 0.5f/*friction*/, 0.0f/*rollingFriction*/,
        Vector3{ 1, 1, 1 }/*enabledRotationAxes*/,
        0.6f/*linearDamping*/, 0.0f/*angularDamping*/,
        new Vector3(0.0f, 0.0f, 0.0f)/*customGravity*/
	);

    return t;
}

Drawable3DCreateConfig_VoxelGrid* T_Projectile_Arrow_Basic_Grid06Range3Tiles_A_V1::buildBase() {
    const signed char O = -1;
    const signed char W = 0; // Wood
    const signed char S = 1; // Stone
    const signed char F = 2; // Feathers

    Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
        8, 8, 1,
        false, true, true,
        new signed char[8 * 8 * 1] {
            O,O,O,O,O,S,S,S,
            O,O,O,O,O,O,W,S,
            O,O,O,O,O,W,O,S,
            O,O,O,O,W,O,O,O,
            O,O,O,W,O,O,O,O,
            O,O,W,O,O,O,O,O,
            F,W,O,O,O,O,O,O,
            O,F,O,O,O,O,O,O
        }
    );

    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#5E4A27FF")));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#757575FF")));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#D8D8D8FF")));

    return new Drawable3DCreateConfig_VoxelGrid(
        "_VoxelGridGroup_",//id
        new std::string("_ROOT_"),//parentId

        nullptr,// pos
        new Quaternion(Vector3::Z, -45.0f),
        nullptr,// scale

        0.03f,
        new Vector3(0.5f - padVoxelGrid->getLengthA() / 2.0f, 0.5f - padVoxelGrid->getLengthB() / 2.0f, 0.5f - padVoxelGrid->getLengthC() / 2.0f),

        padVoxelGrid, nullptr,
        materialsList, nullptr
    );
}
