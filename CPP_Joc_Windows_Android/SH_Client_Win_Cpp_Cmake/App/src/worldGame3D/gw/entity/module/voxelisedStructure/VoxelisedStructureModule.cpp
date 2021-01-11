#include "VoxelisedStructureModule.h"
#include <worldGame3D/gw/entity/module/referenceFrame/ReferenceFrameModule.h>
#include <worldGame3D/gw/entity/module/visual3D/Visual3DModule.h>
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/voxelisedstructure/VSVoxelType.h>
#include <base/visual3d/Visual3DUtil.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>

VoxelisedStructureModule::VoxelisedStructureModule(
    IWorldEntity* entity,
    VoxelisedStructureTemplate* t
)
    : super(t->moduleName, entity),
      t(t)
{
    structureGrid.setDirect(0, 0, 0, VSVoxelType::Empty);

    // VSVoxelType::Empty
    structureMaterialsList.appendDirect(
        nullptr
    );
}

void VoxelisedStructureModule::setStructure(
    Array3D<signed char>* srcStructureGrid,
    ArrayList<Visual3DModelMaterial*>* srcStructureMaterialsList)
{
    if(srcStructureGrid == nullptr || srcStructureMaterialsList == nullptr) {
        throw LogicException(LOC);
    }

    structureGrid.set(*srcStructureGrid);

    structureMaterialsList.clear();
    for(int i=0;i<srcStructureMaterialsList->size();i++) {
        Visual3DModelMaterial* mat = srcStructureMaterialsList->getDirect(i);

        structureMaterialsList.appendDirect(mat);
    }

    vGridVisual->getVoxelsGrid()->clearMaterials();
    for(int i=0;i<structureMaterialsList.size();i++) {
        Visual3DModelMaterial* mat = structureMaterialsList.getDirect(i);

        int matId;
        Visual3DModelMaterial_Color* lMat_color;
        if(!mat) {
			matId = vGridVisual->getVoxelsGrid()->pushNullMaterial();
        }else if((lMat_color = dynamic_cast<Visual3DModelMaterial_Color*>(mat))) {
			matId = vGridVisual->getVoxelsGrid()->pushMaterial(*lMat_color->diffuseColor);
        }else {
            throw LogicException(LOC);
        }
    }

    vGridVisual->getVoxelsGrid()->setVoxelsGrid(&structureGrid);
}

void VoxelisedStructureModule::createBMain() {
    super::createBMain();

    worldGame3D::IVisual3DModule* visual3D = entity->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);

    voxelisedConstructLayer = visual3D->getDrawableOptionalAs<IContainer3D*>(t->voxelisedConstructLayerId);

    vGridVisual = Visual3DUtil::newVoxelGrid(
        structureVoxelSize,
        &structureGridOffset,
        &structureGrid,
        &structureMaterialsList,
        nullptr/*tilingInformation*/
    );
    voxelisedConstructLayer->addChild(vGridVisual);
}

VoxelisedStructureModule::~VoxelisedStructureModule() {
    //void
}
