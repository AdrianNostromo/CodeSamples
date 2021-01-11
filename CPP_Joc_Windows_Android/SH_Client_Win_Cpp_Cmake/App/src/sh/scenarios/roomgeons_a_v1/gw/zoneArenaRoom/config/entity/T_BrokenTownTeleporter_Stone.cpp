#include "T_BrokenTownTeleporter_Stone.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include "../../../../Namespace.h"

using namespace roomgeons_a_v1;
using namespace roomgeons_a_v1::ArenaRoom;

EntityTemplate* T_BrokenTownTeleporter_Stone::getOrCreateTemplate() {
    std::string templateBakedId = "T_BrokenTownTeleporter_Stone";

    EntityTemplate* t = EntityTemplate::fromId(Namespace::NAMESPACE, templateBakedId);
    if (t != nullptr) {
        return t;
    }

    t = EntityTemplate::newEntry(Namespace::NAMESPACE, templateBakedId);

    t->tagsList.appendReference(rpg3D::EntityTags::DOODAD);
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
            0.05f
        )
    ));

    t->setModuleTemplate(FrustumCullable3DTemplate::newOfsetedZSphere(
        0.4f,
        Math::dist(0.0f, 0.0f, 0.4f, 0.4f)
    ));

    return t;
}

Drawable3DCreateConfig_VoxelGrid* T_BrokenTownTeleporter_Stone::buildBase() {
    const signed char O = -1;
    const signed char W = 0; // Stone

    Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
        8, 8, 1,
        false, true, true,
        new signed char[8 * 8 * 1] {
            O,O,W,W,W,O,O,O,
            O,W,O,O,O,O,O,O,
            W,O,O,W,W,O,O,O,
            W,O,W,W,W,W,O,W,
            W,O,W,W,W,W,O,W,
            W,O,O,W,W,O,O,W,
            O,W,O,O,O,O,W,O,
            O,O,W,W,W,W,O,O
        }
    );

    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#B1B1B1FF")
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
