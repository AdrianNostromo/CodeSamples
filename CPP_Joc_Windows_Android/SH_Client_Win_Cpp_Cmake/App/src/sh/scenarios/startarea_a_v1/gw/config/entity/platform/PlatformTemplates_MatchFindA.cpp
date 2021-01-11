#include "PlatformTemplates_MatchFindA.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <rpg3D/gw/entity/template/platform/PlatformTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/template/customLogic/CustomLogicTemplate.h>
#include <rpg3D/gw/entity/module/customLogic/CustomLogicModule_MatchFindPlatformA.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigCylinder.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace startarea_a_v1;

EntityTemplate* PlatformTemplates_MatchFindA::GetNew() {
    EntityTemplate* t = EntityTemplatesList::newEntry("Platform_MatchFindA");

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

                    ->appendDirect_chain(new Drawable3DCreateConfig_Group(
                        "_Spinner_",
                        new std::string("_ROOT_"),
                        new Vector3(
                            0.0f,
                            0.0f,
                            0.05f
                        ),
                        nullptr,
                        nullptr
                    ))

                    ->appendDirect_chain(new Drawable3DCreateConfig_Group(
                        "_SpinnerSymbol_",
                        new std::string("_Spinner_"),
                        nullptr,
                        nullptr,
                        new Vector3(0.23f)
                    ))

                    ->appendDirect_chain(buildBase())
                    ->appendDirect_chain(buildSpinner(
                        "_SpinnerSwordLeft_",
                        new std::string("_SpinnerSymbol_"),
                        new Quaternion(Vector3::Z, 135.0f)
                    ))
                    ->appendDirect_chain(
                        buildSpinner("_SpinnerSwordRight_",
                        new std::string("_SpinnerSymbol_"),
                        new Quaternion(Vector3::Z, 45.0f)
                    ))
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
            0.05f
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

	t->setModuleTemplate(new base::CustomLogicTemplate(
		"customLogic_MatchFindPlatformA", rpg3D::ICustomLogicModule_MatchFindPlatformA::TYPE,
		[](IWorldEntity* entity, base::CustomLogicTemplate* t) -> base::CustomLogicModule * { 
			return new rpg3D::CustomLogicModule_MatchFindPlatformA(
				entity, t,
				"_Spinner_", "_SpinnerSymbol_"
			);
		}
	));

    t->setModuleTemplate(FrustumCullable3DTemplate::newOfsetedZSphere(
        0.4f,
        Math::dist(0.0f, 0.0f, 0.4f, 0.4f)
    ));

    return t;
}

Drawable3DCreateConfig_VoxelGrid* PlatformTemplates_MatchFindA::buildBase() {
    const signed char O = -1;
    const signed char W = 0; // Wood

    Array3D<signed char>* padVoxelGrid = newt Array3D<signed char>(
        8, 8, 1,
        false, true, true,
        new signed char[8 * 8 * 1] {
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

    //asdB;// the filler has a transparency of 0.4f or 0.6f, test it. The java version may have used opacity instead of alpha;
    ArrayList<Visual3DModelMaterial*>* materialsList = newt ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#684D1EFF")
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

Drawable3DCreateConfig_VoxelGrid* PlatformTemplates_MatchFindA::buildSpinner(std::string id, std::string* parentId, Quaternion* rot) {
    //asdB;// the filler has a transparency of 0.4f or 0.6f, test it. The java version may have used opacity instead of alpha;
    ArrayList<Visual3DModelMaterial*>* materialsList = newt ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#082520FF")// 0
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#1F8A78FF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#156355FF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#2AC7ACFF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#33EACBFF")//4
    ));

    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#0D3F36FF")//5
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#684D1EFF")//6
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#493615FF")//7
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#291E0CFF")//8
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#896727FF")//9
    ));

    const signed char O = -1;
    const signed char A = 0;
    const signed char B = 1;
    const signed char C = 2;
    const signed char D = 3;
    const signed char E = 4;
    const signed char F = 5;
    const signed char G = 6;
    const signed char H = 7;
    const signed char I = 8;
    const signed char J = 9;

    Array3D<signed char>* padVoxelGrid = newt Array3D<signed char>(
        16, 16, 1,
        false, true, true,
        new signed char[16 * 16 * 1] {
            O,O,O,O,O,O,O,O,O,O,O,O,O,F,F,F,
            O,O,O,O,O,O,O,O,O,O,O,O,F,E,D,F,
            O,O,O,O,O,O,O,O,O,O,O,F,E,D,E,F,
            O,O,O,O,O,O,O,O,O,O,F,E,D,E,F,O,
            O,O,O,O,O,O,O,O,O,F,E,D,E,F,O,O,
            O,O,O,O,O,O,O,O,F,E,D,E,F,O,O,O,
            O,O,A,A,O,O,O,F,E,D,E,F,O,O,O,O,
            O,O,A,C,A,O,F,E,D,E,F,O,O,O,O,O,
            O,O,O,A,B,A,E,D,E,F,O,O,O,O,O,O,
            O,O,O,A,B,A,D,E,F,O,O,O,O,O,O,O,
            O,O,O,O,A,B,A,A,O,O,O,O,O,O,O,O,
            O,O,O,H,G,A,B,C,A,O,O,O,O,O,O,O,
            O,O,H,J,I,O,A,A,C,A,O,O,O,O,O,O,
            A,A,G,I,O,O,O,O,A,A,O,O,O,O,O,O,
            A,B,A,O,O,O,O,O,O,O,O,O,O,O,O,O,
            A,A,A,O,O,O,O,O,O,O,O,O,O,O,O,O
        }
    );

    return new Drawable3DCreateConfig_VoxelGrid(
        id,//id
        parentId,//parentId

        nullptr,// pos
        rot,//rot
        nullptr,// scale

        0.1f,
        new Vector3(-16.0f / 2.0f + 0.5f, -16.0f / 2.0f + 0.5f, -0.5f + 0.5f),

        padVoxelGrid, nullptr,
        materialsList, nullptr
    );

}
