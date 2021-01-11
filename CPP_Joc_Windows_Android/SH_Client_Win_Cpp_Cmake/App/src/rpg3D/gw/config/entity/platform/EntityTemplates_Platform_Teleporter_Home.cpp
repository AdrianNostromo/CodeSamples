#include "EntityTemplates_Platform_Teleporter_Home.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <rpg3D/gw/entity/template/platform/PlatformTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <base/physics3D/config/shape/PhysicsShapeConfigCylinder.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>

using namespace rpg3D;

EntityTemplate* EntityTemplates_Platform_Teleporter_Home::getNew() {
    EntityTemplate* t = EntityTemplatesList::newEntry("Platform_Teleporter_Home");

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
					->appendDirect_chain(buildIcon())
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
        new Vector3(0.0f, 0.0f, 0.05f)/*physicsBodyOffsetPos*/, nullptr/*physicsBodyOffsetRot*/,
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

    t->setModuleTemplate(FrustumCullable3DTemplate::newOfsetedZSphere(
        0.4f,
        Math::dist(0.0f, 0.0f, 0.4f, 0.4f)
    ));

    return t;
}

Drawable3DCreateConfig_VoxelGrid* EntityTemplates_Platform_Teleporter_Home::buildBase() {
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

    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
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

Drawable3DCreateConfig_VoxelGrid* EntityTemplates_Platform_Teleporter_Home::buildIcon() {
	const int gSizeX = 7;
	const int gSizeY = 7;
	const int gSizeZ = 1;

	const signed char O = -1;
	const signed char A = 0; // Icon material 1

	Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
		gSizeX, gSizeY, gSizeZ,
		false, true, true,
        new signed char[gSizeX * gSizeY * gSizeZ]{
			O,O,O,A,O,O,O,
			O,O,A,A,A,O,O,
			O,A,A,A,A,A,O,
			A,A,A,A,A,A,A,
			O,A,O,A,O,A,O,
			O,A,O,A,O,A,O,
			O,A,A,A,A,A,O
		}
	);

	ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(
		new Color("#684D1EFF")
	));

	return new Drawable3DCreateConfig_VoxelGrid(
		"_Icon_",//id
		new std::string("_ROOT_"),//parentId

		nullptr,// pos
		new Quaternion(Vector3::Z, -45.0f),//rot
		nullptr,// scale

		0.06f,
		// The z pos is manually set so the entity sits 0.5f under the ground.
		new Vector3(-gSizeX / 2.0f + 0.5f, -gSizeY / 2.0f + 0.5f, -0.5f + 0.5f),

		padVoxelGrid, nullptr,
		materialsList, nullptr
	);
}
