#include "PlatformTemplates_TeleporterA.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <rpg3D/gw/entity/template/platform/PlatformTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/EntityTemplatesList.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigCylinder.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>

using namespace startarea_a_v1;

EntityTemplate* PlatformTemplates_TeleporterA::GetNew() {
    EntityTemplate* t = EntityTemplatesList::newEntry("Platform_TeleporterA");

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
                )
        ),
        false,//requiresHalfGridOffsetX
        false//requiresHalfGridOffsetY
    ));

    t->setModuleTemplate(new Touchable3DTemplate(
        Touchable3DTemplate::ShapeType::AABB,
        Vector3(
            -0.3f,
            -0.3f,
            -0.05f
        ),
        Vector3(
            0.3f,
            0.3f,
            0.65f
        )
    ));

	const float CM = rpg3D::PhysicsUtil::DEFAULT_COLLISSION_MARGIN;
    rpg3D::Physics3DTemplate* physics3D;
    t->setModuleTemplate(physics3D = new rpg3D::Physics3DTemplate(
        rpg3D::PhysicsCollissionConfig::Platform,
		nullptr/*pActorControllerCfg*/
	));
	physics3D->pBodiesConfigsList.append_emplace_chain(
        rpg3D::PBodyConfig::UssageBitGroups::Movement | rpg3D::PBodyConfig::UssageBitGroups::CardinalRotation | rpg3D::PBodyConfig::UssageBitGroups::VisualsUpdate,
        rpg3D::PBodyConfig::DynamicsType::Static/*dynamicsType*/,
        nullptr/*physicsBodyOffsetPos*/, nullptr/*physicsBodyOffsetRot*/,
        new physics3D::PhysicsShapeConfigCylinder(
            CM/*collisionMargin*/,
            Vector3::Z, Vector3{ 0.2f, 0.2f, 0.05f }
        ),
        0.0f/*mass*/, 0.5f/*friction*/, 0.0f/*rollingFriction*/,
        Vector3{ 0, 0, 0 }/*enabledRotationAxes*/,
        0.0f/*linearDamping*/, 0.0f/*angularDamping*/,
        nullptr/*customGravity*/
	);

    t->setModuleTemplate(new rpg3D::PlatformTemplate());

    t->setModuleTemplate(FrustumCullable3DTemplate::newOfsetedZSphere(
        0.4f,
        Math::dist(0.0f, 0.0f, 0.4f, 0.4f)
    ));

    return t;
}

Drawable3DCreateConfig_VoxelGrid* PlatformTemplates_TeleporterA::buildBase() {
    const signed char O = -1;
    const signed char W = 0; // Wood
    const signed char P = 1; // Portal material

    Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
        8, 8, 8,
        false, true, true,
        new signed char[8 * 8 * 8] {
            O,O,O,O,O,O,O,O,
            O,O,P,P,P,P,O,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,O,P,P,P,P,O,O,
            O,O,O,O,O,O,O,O,

            O,O,O,O,O,O,O,O,
            O,O,P,P,P,P,O,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,O,P,P,P,P,O,O,
            O,O,O,O,O,O,O,O,

            O,O,O,O,O,O,O,O,
            O,O,P,P,P,P,O,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,O,P,P,P,P,O,O,
            O,O,O,O,O,O,O,O,

            O,O,O,O,O,O,O,O,
            O,O,P,P,P,P,O,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,O,P,P,P,P,O,O,
            O,O,O,O,O,O,O,O,

            O,O,O,O,O,O,O,O,
            O,O,P,P,P,P,O,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,O,P,P,P,P,O,O,
            O,O,O,O,O,O,O,O,

            O,O,O,O,O,O,O,O,
            O,O,P,P,P,P,O,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,O,P,P,P,P,O,O,
            O,O,O,O,O,O,O,O,

            O,O,O,O,O,O,O,O,
            O,O,P,P,P,P,O,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,P,P,P,P,P,P,O,
            O,O,P,P,P,P,O,O,
            O,O,O,O,O,O,O,O,

            O,O,W,W,W,W,O,O,
            O,W,P,P,P,P,W,O,
            W,P,P,P,P,P,P,W,
            W,P,P,P,P,P,P,W,
            W,P,P,P,P,P,P,W,
            W,P,P,P,P,P,P,W,
            O,W,P,P,P,P,W,O,
            O,O,W,W,W,W,O,O
        }
    );

    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#997535FF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#4FC6E7FF")
    ));

    return new Drawable3DCreateConfig_VoxelGrid(
        "_Base_",//id
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
