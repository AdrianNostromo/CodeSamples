#include "PlatformTemplates_PaintPad.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <rpg3D/gw/entity/template/platform/PlatformTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/template/customLogic/CustomLogicTemplate.h>
#include <rpg3D/gw/entity/module/customLogic/CustomLogicModule_PaintPad.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigCylinder.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>

using namespace rpg3D;

EntityTemplate* PlatformTemplates_PaintPad::getNew() {
    EntityTemplate* t = EntityTemplatesList::newEntry("Platform_PaintPad");

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
                        "_BrushHolder_",
                        new std::string("_ROOT_"),
                        new Vector3(
                            -0.03f,
                            -0.07f,
                            0.05f
                        ),
                        nullptr,
                        nullptr
                    ))

                    ->appendDirect_chain(buildBase())
                    ->appendDirect_chain(buildPad())
                    ->appendDirect_chain(buildBrush())
                )
        ),
        false,//requiresHalfGridOffsetX
        false//requiresHalfGridOffsetY
    ));

    t->setModuleTemplate(new Touchable3DTemplate(
        Touchable3DTemplate::ShapeType::AABB,
        Vector3(
            -0.4f,
            -0.4f,
            0.0f
        ),
        Vector3(
            0.4f,
            0.4f,
            0.1f
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

    t->setModuleTemplate(new PlatformTemplate());

	t->setModuleTemplate(new base::CustomLogicTemplate(
		"customLogic_PaintPad", ICustomLogicModule_PaintPad::TYPE,
		[](IWorldEntity* entity, base::CustomLogicTemplate* t) -> base::CustomLogicModule * {
			return new CustomLogicModule_PaintPad(
				entity, t,
				"_Brush_", "_BrushHolder_"
			);
		}
	));

    t->setModuleTemplate(FrustumCullable3DTemplate::newOfsetedZSphere(
        0.0f,
        Math::dist(0.0f, 0.0f, 0.4f, 0.4f)
    ));

    return t;
}

Drawable3DCreateConfig_VoxelGrid* PlatformTemplates_PaintPad::buildBase() {
    const signed char O = -1;
    const signed char W = 0; // Wood

    Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
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
    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#997535FF")
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

Drawable3DCreateConfig_VoxelGrid* PlatformTemplates_PaintPad::buildPad() {
    const signed char X = -1; // Empty
    const signed char E = 0; // Edge
    const signed char W = 1; // Wood

    const signed char R = 2; // Red paint
    const signed char r = 3; // Red paint b
    const signed char B = 4; // Blue paint
    const signed char b = 5; // Blue paint b
    const signed char G = 6; // Green paint
    const signed char g = 7; // Green paint b

    Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
        35, 22, 3,
        false, true, true,
		new signed char[35 * 22 * 3]{
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,r,r,r,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,r,r,r,r,r,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,r,r,r,r,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,r,r,X,X,X,b,b,X,X,X,g,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,b,b,b,b,X,X,X,g,g,g,g,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,b,b,b,b,b,X,X,X,g,g,g,g,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,b,b,b,X,X,X,X,g,g,g,g,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,

			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,R,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,R,R,R,R,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,R,R,R,R,R,R,R,R,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,R,R,R,R,R,R,R,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,R,R,R,R,R,R,X,B,B,X,X,G,G,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,R,R,R,R,X,B,B,B,B,X,G,G,G,G,G,G,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,R,R,X,B,B,B,B,B,B,X,G,G,G,G,G,G,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,B,B,B,B,B,B,B,X,G,G,G,G,G,G,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,B,B,B,B,B,B,X,X,G,G,G,G,G,G,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,B,B,B,B,B,X,X,G,G,G,G,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,

			X,X,X,X,X,X,X,X,X,X,X,X,E,E,E,E,E,E,E,E,E,E,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,E,W,W,W,W,W,W,W,W,W,W,E,E,E,E,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,E,E,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,E,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,X,X,X,
			X,X,X,X,X,X,X,X,X,X,E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,X,X,
			X,X,X,X,X,X,X,X,X,E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,X,
			X,X,X,X,X,X,X,X,E,W,W,W,E,E,E,E,E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,X,
			X,X,E,E,E,E,E,E,W,W,W,E,X,X,X,X,X,E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,
			X,E,W,W,W,W,W,W,W,W,W,E,X,X,X,X,X,E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,
			E,W,W,W,W,W,W,W,W,W,W,E,X,X,X,X,X,E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,
			E,W,W,W,W,W,W,W,W,W,W,W,E,E,E,E,E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,
			E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,
			E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,
			E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,X,
			X,E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,X,X,
			X,X,E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,X,X,X,
			X,X,X,E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,E,X,X,X,X,
			X,X,X,X,E,E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,E,X,X,X,X,X,X,
			X,X,X,X,X,X,E,E,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,E,E,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,E,E,E,W,W,W,W,W,W,W,W,W,W,W,E,E,E,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,E,E,E,E,E,E,E,E,E,E,E,X,X,X,X,X,X,X,X,X,X,X,X,X
		}
    );

    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#242424FF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#E7B678FF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#E74638FF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#E78B76FF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#4FC6E7FF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#A5DBE7FF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#4FE77CFF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#AEE7BDFF")
    ));

    return new Drawable3DCreateConfig_VoxelGrid(
        "_Pad_",//id
        new std::string("_ROOT_"),//parentId

        nullptr,// pos
        new Quaternion(Vector3::Z,-30.0f),//rot
        new Vector3(0.16f),// scale

        0.1f,
        new Vector3(-35.0f / 2.0f + 0.5f, -22.0f / 2.0f + 0.5f, 0.0f + 0.5f),

        padVoxelGrid, nullptr,
        materialsList, nullptr
    );
}

Drawable3DCreateConfig_VoxelGrid* PlatformTemplates_PaintPad::buildBrush() {
    const signed char X = -1; // Empty
    const signed char E = 0; // Edge
    const signed char B = 1; // Brush
    const signed char H = 2; // Handle
    const signed char C = 3; // Connector

    Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
        22, 21, 5,
        false, true, true,
		new signed char[22 * 21 * 5]{
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,B,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,B,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,

			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,H,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,H,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,H,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,H,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,H,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,H,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,H,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,H,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,H,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,H,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,H,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,C,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,C,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,C,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,B,B,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,B,B,B,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,B,B,B,B,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,B,B,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,

			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,E,E,E,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,E,H,H,E,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,E,H,H,H,E,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,E,H,H,H,E,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,E,H,H,H,E,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,E,H,H,H,E,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,E,H,H,H,E,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,E,H,H,H,E,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,E,H,H,H,E,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,E,H,H,H,E,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,E,H,H,H,E,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,E,E,H,H,E,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,E,C,C,E,E,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,E,E,E,C,C,C,E,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,E,B,B,E,C,C,E,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,E,B,B,B,B,E,E,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,E,B,B,B,B,B,E,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,E,B,B,B,B,B,E,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,E,B,B,B,B,E,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,E,B,B,B,E,E,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			E,E,E,E,E,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,

			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,H,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,H,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,H,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,H,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,H,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,H,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,H,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,H,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,H,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,H,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,H,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,C,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,C,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,C,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,B,B,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,B,B,B,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,B,B,B,B,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,B,B,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,

			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,B,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,B,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
			X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X
		}
    );

    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#242424FF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#AF582BFF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#5B99E7FF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#A5DBE7FF")
    ));

    return new Drawable3DCreateConfig_VoxelGrid(
        "_Brush_",//id
        new std::string("_BrushHolder_"),//parentId

        nullptr,// pos
        // Manually handled.
        nullptr,
        new Vector3(0.16f),// scale

        0.1f,
        new Vector3(-2.0f / 2.0f + 0.5f, -0.0f / 2.0f + 0.5f, -5.0f / 2.0f + 0.5f),

        padVoxelGrid, nullptr,
        materialsList, nullptr
    );
}
