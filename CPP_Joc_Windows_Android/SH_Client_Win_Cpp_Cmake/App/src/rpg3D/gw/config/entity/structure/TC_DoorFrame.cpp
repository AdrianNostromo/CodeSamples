#include "TC_DoorFrame.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityTemplatesList.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>
#include <base/statics/StaticsInit.h>
#include <rpg3D/Namespace.h>
#include <base/util/StringUtil.h>
#include <base/exceptions/LogicException.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <base/physics3D/config/shape/PSC_BvhTriangleMeshShape.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/physics3D/util/PhysicsUtil.h>

using namespace rpg3D;

std::string TC_DoorFrame::ID_WITH_PLACEHOLDERS = "TC_DoorFrame#size#dm";

EntityTemplate* TC_DoorFrame::getOrCreate(int size_dm) {
    if (size_dm <= 0) {
        throw LogicException(LOC);
    }

    std::string id = ID_WITH_PLACEHOLDERS;
    StringUtil::replace(id, "#size#", std::to_string(size_dm));

    EntityTemplate* t = EntityTemplate::fromId(Namespace::NAMESPACE, id);
    if (t != nullptr) {
        return t;
    }

    t = EntityTemplate::newEntry(Namespace::NAMESPACE, id);

    t->tagsList.appendReference(rpg3D::EntityTags::PLATFORM);
    t->tagsList.appendReference(rpg3D::EntityTags::TELEPORT);
    t->tagsList.appendReference(rpg3D::EntityTags::STRUCTURE);

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
                ->appendDirect_chain(buildBase(size_dm))
            )
        ),
        false,//requiresHalfGridOffsetX
        false//requiresHalfGridOffsetY
    ));

    t->setModuleTemplate(FrustumCullable3DTemplate::newOfsetedZSphere(
        0.4f,
        Math::dist(0.0f, 0.0f, 0.4f, 0.4f)
    ));

    const float CM = rpg3D::PhysicsUtil::DEFAULT_COLLISSION_MARGIN;
    rpg3D::Physics3DTemplate* physics3D;
    t->setModuleTemplate(physics3D = new rpg3D::Physics3DTemplate(
        rpg3D::PhysicsCollissionConfig::Environment,
        nullptr/*pActorControllerCfg*/
    ));

    typedef physics3D::PSC_BvhTriangleMeshShape::Vec3 Vec3;
    int pointsCount = 3/*points_per_triangle*/ * 2/*triangles_per_quad*/ * 10/*total_quads_count*/;
    Vec3* pointsArray = new Vec3[pointsCount]{};
    int pointsIndex = 0;
    physics3D::PSC_BvhTriangleMeshShape::SetQuadTriangles(Vec3{ 0.0f + CM, 0.0f + CM, 0.0f + CM }, Vec3{0.6f - CM, 0.0f + CM, 0.0f + CM },  Vec3{ 0.6f - CM, 0.0f + CM, 0.6f - CM }, Vec3{ 0.0f + CM, 0.0f + CM, 0.6f - CM }, pointsArray, pointsIndex/*pointsIndexRef*/);
    physics3D::PSC_BvhTriangleMeshShape::SetQuadTriangles(Vec3{ 0.6f - CM, 0.0f + CM, 0.0f + CM }, Vec3{ 0.6f - CM, 0.4f, 0.0f + CM },      Vec3{ 0.6f - CM, 0.4f, 0.6f - CM },      Vec3{ 0.6f - CM, 0.0f + CM, 0.6f - CM }, pointsArray, pointsIndex/*pointsIndexRef*/);
    physics3D::PSC_BvhTriangleMeshShape::SetQuadTriangles(Vec3{ 0.6f - CM, 0.4f, 0.0f + CM },      Vec3{ 0.4f, 0.6f - CM, 0.0f + CM },      Vec3{ 0.4f, 0.6f - CM, 0.6f - CM },      Vec3{ 0.6f - CM, 0.4f, 0.6f - CM },      pointsArray, pointsIndex/*pointsIndexRef*/);
    physics3D::PSC_BvhTriangleMeshShape::SetQuadTriangles(Vec3{ 0.4f, 0.6f - CM, 0.0f + CM },      Vec3{ 0.0f + CM, 0.6f - CM, 0.0f + CM }, Vec3{ 0.0f + CM, 0.6f - CM, 0.6f - CM }, Vec3{ 0.4f, 0.6f - CM, 0.6f - CM },      pointsArray, pointsIndex/*pointsIndexRef*/);
    physics3D::PSC_BvhTriangleMeshShape::SetQuadTriangles(Vec3{ 0.0f + CM, 0.6f - CM, 0.0f + CM }, Vec3{ 0.0f + CM, 0.0f + CM, 0.0f + CM }, Vec3{ 0.0f + CM, 0.0f + CM, 0.6f - CM }, Vec3{ 0.0f + CM, 0.6f - CM, 0.6f - CM }, pointsArray, pointsIndex/*pointsIndexRef*/);

    // Offset until the second pillar creation;
    float f = (size_dm / 10.0f) - 0.6f;
    physics3D::PSC_BvhTriangleMeshShape::SetQuadTriangles(Vec3{ f + 0.0f + CM, 0.0f + CM, 0.0f + CM }, Vec3{ f + 0.6f - CM, 0.0f + CM, 0.0f + CM }, Vec3{ f + 0.6f - CM, 0.0f + CM, 0.6f - CM }, Vec3{ f + 0.0f + CM, 0.0f + CM, 0.6f - CM }, pointsArray, pointsIndex/*pointsIndexRef*/);
    physics3D::PSC_BvhTriangleMeshShape::SetQuadTriangles(Vec3{ f + 0.6f - CM, 0.0f + CM, 0.0f + CM }, Vec3{ f + 0.6f - CM, 0.6f - CM, 0.0f + CM }, Vec3{ f + 0.6f - CM, 0.6f - CM, 0.6f - CM }, Vec3{ f + 0.6f - CM, 0.0f + CM, 0.6f - CM }, pointsArray, pointsIndex/*pointsIndexRef*/);
    physics3D::PSC_BvhTriangleMeshShape::SetQuadTriangles(Vec3{ f + 0.6f - CM, 0.6f - CM, 0.0f + CM }, Vec3{ f + 0.2f, 0.6f - CM, 0.0f + CM },      Vec3{ f + 0.2f, 0.6f - CM, 0.6f - CM },      Vec3{ f + 0.6f - CM, 0.6f - CM, 0.6f - CM }, pointsArray, pointsIndex/*pointsIndexRef*/);
    physics3D::PSC_BvhTriangleMeshShape::SetQuadTriangles(Vec3{ f + 0.2f, 0.6f - CM, 0.0f + CM },      Vec3{ f + 0.0f + CM, 0.4f, 0.0f + CM },      Vec3{ f + 0.0f + CM, 0.4f, 0.6f - CM },      Vec3{ f + 0.2f, 0.6f - CM, 0.6f - CM },      pointsArray, pointsIndex/*pointsIndexRef*/);
    physics3D::PSC_BvhTriangleMeshShape::SetQuadTriangles(Vec3{ f + 0.0f + CM, 0.4f, 0.0f + CM },      Vec3{ f + 0.0f + CM, 0.0f + CM, 0.0f + CM }, Vec3{ f + 0.0f + CM, 0.0f + CM, 0.6f - CM }, Vec3{ f + 0.0f + CM, 0.4f, 0.6f - CM },      pointsArray, pointsIndex/*pointsIndexRef*/);

    physics3D->pBodiesConfigsList.append_emplace_chain(
        rpg3D::PBodyConfig::UssageBitGroups::Movement | rpg3D::PBodyConfig::UssageBitGroups::CardinalRotation | rpg3D::PBodyConfig::UssageBitGroups::VisualsUpdate,
        rpg3D::PBodyConfig::DynamicsType::Static/*dynamicsType*/,
        nullptr/*physicsBodyOffsetPos*/, nullptr/*physicsBodyOffsetRot*/,
        new physics3D::PSC_BvhTriangleMeshShape(
            CM/*collisionMargin*/,
            pointsCount/*pointsCount*/,
            pointsArray
        ),
        0.0f/*mass*/, 0.5f/*friction*/, 0.0f/*rollingFriction*/,
        Vector3{ 0, 0, 0 }/*enabledRotationAxes*/,
        0.0f/*linearDamping*/, 0.0f/*angularDamping*/,
        nullptr/*customGravity*/
    );

    return t;
}

Drawable3DCreateConfig_VoxelGrid* TC_DoorFrame::buildBase(int size_dm) {
    Vector3Int gSize{ size_dm, 6, 11 };

	static const signed char O = -1;
    static const signed char W = 0;

    static Array3D<signed char> sidesCopySource{
        12, 6, 11,
        false, true, true,
        new signed char[12 * 6 * 11] {
            O,O,O,O,O,O, O,O,O,O,O,O,
            O,O,O,O,O,O, O,O,O,O,O,O,
            O,O,O,O,O,O, W,W,W,W,W,W,
            O,O,O,O,O,O, W,W,W,W,W,W,
            O,O,O,O,O,O, O,O,O,O,O,O,
            O,O,O,O,O,O, O,O,O,O,O,O,

            O,O,O,O,O,O, O,O,O,O,O,O,
            O,O,O,O,W,W, W,W,W,W,W,W,
            O,O,O,O,W,W, W,W,W,W,W,W,
            O,O,O,O,W,W, W,W,W,W,W,W,
            O,O,O,O,W,W, W,W,W,W,W,W,
            O,O,O,O,O,O, O,O,O,O,O,O,

            O,O,W,W,O,O, O,O,O,O,O,O,
            O,O,W,W,W,W, W,W,O,O,O,O,
            O,O,W,W,W,W, W,W,O,O,O,O,
            O,O,W,W,W,W, W,W,O,O,O,O,
            O,O,W,W,W,W, W,W,O,O,O,O,
            O,O,W,W,W,W, W,O,O,O,O,O,

            O,W,W,W,O,O, O,O,O,O,O,O,
            O,W,W,W,W,O, W,O,O,O,O,O,
            O,W,W,W,W,W, W,O,O,O,O,O,
            O,W,W,W,W,W, W,O,O,O,O,O,
            O,W,W,W,W,W, W,O,O,O,O,O,
            O,W,W,W,W,W, W,O,O,O,O,O,

            W,W,W,W,O,O, O,O,O,O,O,O,
            W,W,W,W,W,O, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            
            W,W,W,W,O,O, O,O,O,O,O,O,
            W,W,W,W,W,O, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,

            W,W,W,W,O,O, O,O,O,O,O,O,
            W,W,W,W,W,O, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,

            W,W,W,W,O,O, O,O,O,O,O,O,
            W,W,W,W,W,O, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,

            W,W,W,W,O,O, O,O,O,O,O,O,
            W,W,W,W,W,O, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,

            W,W,W,W,O,O, O,O,O,O,O,O,
            W,W,W,W,W,O, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,

            W,W,W,W,O,O, O,O,O,O,O,O,
            W,W,W,W,W,O, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O,
            W,W,W,W,W,W, O,O,O,O,O,O
        }
    };

    Array3D<signed char>* grid = new Array3D<signed char>(
        gSize.x, gSize.y, gSize.z
    );
    grid->memSetData(-1);
    // Copy the sides from sidesCopySource to grid.
    int copyColumnsCount = (int)Math::ceil(size_dm / 2.0f);
    int copyOffsetXMin = sidesCopySource.getLengthA() - 1;
    // Copy left and mirror coppy the right side.
    for (int gX = 0; gX < copyColumnsCount; gX++) {
        int copySourceX = Math::min(copyOffsetXMin, gX);
        for (int gY = 0; gY < gSize.y; gY++) {
            for (int gZ = 0; gZ < gSize.z; gZ++) {
                signed char val = sidesCopySource.getDirect(copySourceX, gY, gZ);

                grid->setDirect(gX, gY, gZ, val);
                if(gX != (gSize.x - 1) - gX) {
                    grid->setDirect((gSize.x - 1) - gX, gY, gZ, val);
                }
            }
        }
    }

    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#666666FF")));

    return new Drawable3DCreateConfig_VoxelGrid(
        "_Base_",//id
        new std::string("_ROOT_"),//parentId

        nullptr,// pos
        nullptr,//rot
        nullptr,// scale

        0.1f,
		new Vector3(0.5f, 0.5f, 0.5f),

		grid, nullptr,
        materialsList, nullptr
    );
}
