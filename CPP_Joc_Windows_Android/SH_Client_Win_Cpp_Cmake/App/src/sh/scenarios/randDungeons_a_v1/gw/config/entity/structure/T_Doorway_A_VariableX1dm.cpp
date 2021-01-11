#include "T_Doorway_A_VariableX1dm.h"
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
#include <sh/scenarios/randDungeons_a_v1/Namespace.h>
#include <base/util/StringUtil.h>
#include <base/exceptions/LogicException.h>

using namespace randDungeons_a_v1;

std::string T_Doorway_A_VariableX1dm::ID_WITH_PLACEHOLDERS = "Doorway_A_VariableX1dm_#size#dm";

EntityTemplate* T_Doorway_A_VariableX1dm::getOrCreate(int size_dm) {
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

    return t;
}

Drawable3DCreateConfig_VoxelGrid* T_Doorway_A_VariableX1dm::buildBase(int size_dm) {
    Vector3Int gSize{ size_dm, 1, 10 };

	static const signed char O = -1;
    static const signed char W = 0;

    static Array3D<signed char> sidesCopySource{
        5, 1, 10,
        false, true, true,
        new signed char[5 * 1 * 10]{
            O,O,O,O,W,
            O,O,W,W,O,
            O,W,W,O,O,
            W,O,O,O,O,
            W,O,O,O,O,
            W,O,O,O,O,
            W,O,O,O,O,
            W,O,O,O,O,
            W,O,O,O,O,
            W,O,O,O,O
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
