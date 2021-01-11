#include "TextTemplate_TeleporterRogueZone.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>

using namespace rpg3D;

EntityTemplate* TextTemplate_TeleporterRogueZone::getNew() {
    EntityTemplate* t = EntityTemplatesList::newEntry("Text_TeleporterRogueZone");
    t->tagsList.appendReference(EntityTags::DOODAD);
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
                    ->appendDirect_chain(buildTextRogue())
                    ->appendDirect_chain(buildTextZone())
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

Drawable3DCreateConfig_VoxelGrid* TextTemplate_TeleporterRogueZone::buildTextRogue() {
    const signed char O = -1;
    const signed char W = 0; // Wood

    Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
        23, 5, 1,
        false, true, true,
        new signed char[23 * 5 * 1] {
            W,W,W,O,O,O,W,W,O,O,O,W,W,W,O,W,O,O,W,O,W,W,W,
            W,O,O,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,W,O,W,O,O,
            W,W,W,O,O,W,O,O,W,O,W,O,W,W,O,W,O,O,W,O,W,W,W,
            W,O,W,O,O,W,O,O,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,
            W,O,O,W,O,O,W,W,O,O,O,W,W,W,O,O,W,W,O,O,W,W,W
        }
    );

    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#997535FF")
    ));

    return new Drawable3DCreateConfig_VoxelGrid(
        "_TextRogue_",//id
        new std::string("_ROOT_"),//parentId

        nullptr,// pos
        nullptr,//rot
        nullptr,// scale

        0.05f,
        new Vector3(-23.0f / 2.0f + 0.5f, -5.0f / 2.0f + 0.5f + 3.0f, -0.5f + 0.5f),

        padVoxelGrid, nullptr,
        materialsList, nullptr
    );
}

Drawable3DCreateConfig_VoxelGrid* TextTemplate_TeleporterRogueZone::buildTextZone() {
    const signed char O = -1;
    const signed char W = 0; // Wood

    Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
        20, 5, 1,
        false, true, true,
        new signed char[20 * 5 * 1] {
            W,W,W,W,O,O,W,W,O,O,W,O,O,W,O,W,W,W,O,O,
            O,O,O,W,O,W,O,O,W,O,W,W,O,W,O,W,O,O,O,O,
            O,W,W,O,O,W,O,O,W,O,W,O,W,W,O,W,W,W,O,O,
            W,O,O,O,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,O,
            W,W,W,W,O,O,W,W,O,O,W,O,O,W,O,W,W,W,O,O
        }
    );

    ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#997535FF")
    ));

    return new Drawable3DCreateConfig_VoxelGrid(
        "_TextZone_",//id
        new std::string("_ROOT_"),//parentId

        nullptr,// pos
        nullptr,//rot
        nullptr,// scale

        0.05f,
        new Vector3(-20.0f / 2.0f + 0.5f, -5.0f / 2.0f + 0.5f - 3.0f, -0.5f + 0.5f),

        padVoxelGrid, nullptr,
        materialsList, nullptr
    );
}
