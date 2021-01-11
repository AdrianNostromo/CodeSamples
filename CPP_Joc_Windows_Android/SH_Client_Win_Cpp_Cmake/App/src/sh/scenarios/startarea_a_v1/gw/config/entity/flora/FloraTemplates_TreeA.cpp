#include "FloraTemplates_TreeA.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/EntityTemplatesList.h>

using namespace startarea_a_v1;

EntityTemplate* FloraTemplates_TreeA::GetNew() {
    EntityTemplate* t = EntityTemplatesList::newEntry("Flora_TreeA");

    t->tagsList.appendReference(EntityTags::FLORA);
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

    /*asdB
    .setModuleTemplate(new LivingEntityTemplate(
        184
    ))
    .setModuleTemplate(new TreeTemplate(
        "_VoxelGridGroup_",
        W, EntityTemplatesList.Item_Wood.id,
        L, EntityTemplatesList.Item_GreenWood.id
    ))*/

    /*// Touchable not used currently.
    t->setModuleTemplate(new Touchable3DTemplate(
        Touchable3DTemplate::ShapeType::AABB,
        Vector3(
            -0.2f,
            -0.2f,
            -0.05f
        ),
        Vector3(
            0.2f,
            0.2f,
            // This value will be overridden when choppable trees are implemented.
            0.5f
        )
    ));*/

    t->setModuleTemplate(FrustumCullable3DTemplate::newOfsetedZSphere(
        0.5f,
        Math::dist(0.0f, 0.0f, 0.5f, 0.5f)
    ));

    return t;
}

Drawable3DCreateConfig_VoxelGrid* FloraTemplates_TreeA::buildBase() {
    const signed char O = -1;
    const signed char W = 0; // Wood
    const signed char L = 1; // Leafs

    Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
        6, 6, 10,
        false, true, true,
        new signed char[6 * 6 * 10] {
            O,O,O,O,O,O,
            O,L,L,L,L,O,
            O,L,L,L,L,O,
            O,L,L,L,L,O,
            O,L,L,L,L,O,
            O,O,O,O,O,O,

            O,L,L,L,L,O,
            L,L,L,L,L,L,
            L,L,L,L,L,L,
            L,L,L,L,L,L,
            L,L,L,L,L,L,
            O,L,L,L,L,O,

            O,L,L,L,L,O,
            L,L,L,L,L,L,
            L,L,W,W,L,L,
            L,L,W,W,L,L,
            L,L,L,L,L,L,
            O,L,L,L,L,O,

            O,L,L,L,L,O,
            L,L,L,L,L,L,
            L,L,W,W,L,L,
            L,L,W,W,L,L,
            L,L,L,L,L,L,
            O,L,L,L,L,O,

            O,L,L,L,L,O,
            L,L,L,L,L,L,
            L,L,W,W,L,L,
            L,L,W,W,L,L,
            L,L,L,L,L,L,
            O,L,L,L,L,O,

            O,O,O,O,O,O,
            O,L,L,L,L,O,
            O,L,W,W,L,O,
            O,L,W,W,L,O,
            O,L,L,L,L,O,
            O,O,O,O,O,O,

            O,O,O,O,O,O,
            O,O,O,O,O,O,
            O,O,W,W,O,O,
            O,O,W,W,O,O,
            O,O,O,O,O,O,
            O,O,O,O,O,O,

            O,O,O,O,O,O,
            O,O,O,O,O,O,
            O,O,W,W,O,O,
            O,O,W,W,O,O,
            O,O,O,O,O,O,
            O,O,O,O,O,O,

            O,O,O,O,O,O,
            O,O,O,O,O,O,
            O,O,W,W,O,O,
            O,O,W,W,O,O,
            O,O,O,O,O,O,
            O,O,O,O,O,O,

            O,O,O,O,O,O,
            O,O,W,W,O,O,
            O,W,W,W,W,O,
            O,W,W,W,W,O,
            O,O,W,W,O,O,
            O,O,O,O,O,O
        }
    );

    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#997535FF")
    ));
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#05E70EFF")
    ));

    return new Drawable3DCreateConfig_VoxelGrid(
        "_Base_",//id
        new std::string("_ROOT_"),//parentId

        nullptr,// pos
        nullptr,//rot
        nullptr,// scale

        0.1f,
        new Vector3(-6.0f / 2.0f + 0.5f, -6.0f / 2.0f + 0.5f, -0.5f + 0.5f),

        padVoxelGrid, nullptr,
        materialsList, nullptr
    );
}
