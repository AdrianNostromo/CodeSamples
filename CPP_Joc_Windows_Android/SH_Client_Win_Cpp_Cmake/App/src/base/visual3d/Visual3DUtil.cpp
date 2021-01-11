#include "Visual3DUtil.h"
#include <graphics/visual3d/drawable/IDrawableNode3D.h>
#include <base/exceptions/LogicException.h>
#include <graphics/visual3d/drawable/Container3D.h>
#include <graphics/visual3d/drawable/IDrawableNode3D.h>
#include <base/visual3d/util/Drawable3DCreateConfigBase.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <graphics/visual3d/drawable/Container3D.h>
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>
#include <base/visual3d/util/Visual3DCreateConfig.h>
#include <base/list/Array3D.h>
#include <base/map/ExtraData.h>
#include <base/list/ArrayList.h>
#include <base/map/Map1D.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation.h>
#include <base/object/IWrappedValue.h>

IDrawableNode3D* Visual3DUtil::createConfiguredVisual3D(
    Visual3DCreateConfig* visualCreateConfig,
    ArrayList<IDrawableNode3D*>* drawablesList, Map1D<std::string, IDrawableNode3D*>* drawablesMap,
    IContainer3D* visualParent, std::shared_ptr<ExtraData> extraData,
    bool doReservedCreateVisuals)
{
    if (visualCreateConfig == nullptr) {
        throw LogicException(LOC);
    }

    IDrawableNode3D* rootDrawable = nullptr;

    for(int i=0;i<visualCreateConfig->drawableCreateConfigs->size();i++) {
        ArrayList<Drawable3DCreateConfigBase*>* drawableConfigsGroupList = *visualCreateConfig->drawableCreateConfigs->getPointer(i);
        if(drawableConfigsGroupList != nullptr) {
            for(int j=0;j<drawableConfigsGroupList->size();j++) {
                Drawable3DCreateConfigBase* drawableConfig = *drawableConfigsGroupList->getPointer(j);
                if(drawableConfig != nullptr) {
                    // Create single visual2D.
                    IDrawableNode3D* drawable = Visual3DUtil::newDrawable(
                        drawableConfig,

                        extraData
                    );

                    drawablesList->appendReference(drawable);

                    if(drawablesMap->containsKey(drawableConfig->id) && drawablesMap->getPointer(drawableConfig->id) != nullptr) {
                        throw LogicException(LOC);
                    }

                    drawablesMap->putDirect(drawableConfig->id, drawable);

                    if(drawableConfig->id == "_ROOT_") {
                        if(rootDrawable != nullptr) {
                            throw LogicException(LOC);
                        }

                        if(drawableConfig->parentId != nullptr || drawableConfig->pos != nullptr || drawableConfig->rot != nullptr || drawableConfig->scale != nullptr) {
                            throw LogicException(LOC);
                        }

                        rootDrawable = drawable;
                    }

                    if(drawableConfig->parentId != nullptr) {
                        if(!drawablesMap->containsKey(*drawableConfig->parentId)) {
                            throw LogicException(LOC);
                        }
                        IDrawableNode3D* parentGroup = *drawablesMap->getPointer(*drawableConfig->parentId);
                        if(parentGroup == nullptr) {
                            throw LogicException(LOC);
                        }

                        IContainer3D* lParentGroup = dynamic_cast<IContainer3D*>(parentGroup);
                        if(!lParentGroup) {
                            throw LogicException(LOC);
                        }

                        lParentGroup->addChild(drawable);
                    }
                }
            }
        }
    }

    if(rootDrawable == nullptr) {
        throw LogicException(LOC);
    }

    if(visualParent != nullptr) {
        visualParent->addChild(rootDrawable);
    }

    if(doReservedCreateVisuals) {
        for (int i = drawablesList->size() - 1; i >= 0; i--) {
            IDrawableNode3D *drawable = *drawablesList->getPointer(i);
            if (drawable != nullptr) {
                IManagedDynamicVisual *lDrawable = dynamic_cast<IManagedDynamicVisual *>(drawable);
                if (lDrawable != nullptr) {
                    lDrawable->reservedCreate();
                }
            }
        }
    }

    return rootDrawable;
}

IDrawableNode3D* Visual3DUtil::newDrawable(
        Drawable3DCreateConfigBase* drawableCreateConfig,
        std::shared_ptr<ExtraData> extraData)
{
    IDrawableNode3D* drawable;

    Drawable3DCreateConfig_VoxelGrid* lCfg_voxelGrid;
    Drawable3DCreateConfig_Group* lCfg_group;
    if((lCfg_voxelGrid = dynamic_cast<Drawable3DCreateConfig_VoxelGrid*>(drawableCreateConfig))) {
        drawable = newVoxelGrid(lCfg_voxelGrid, extraData);
    }else if((lCfg_group = dynamic_cast<Drawable3DCreateConfig_Group*>(drawableCreateConfig))) {
        drawable = newGroup(lCfg_group, extraData);
    }else {
        throw LogicException(LOC);
    }
    if(drawable == nullptr) {
        throw LogicException(LOC);
    }

    if(drawableCreateConfig->pos != nullptr) {
        drawable->setPosition(*drawableCreateConfig->pos);
    }

    if(drawableCreateConfig->rot != nullptr) {
        drawable->setRotation(*drawableCreateConfig->rot);
    }

    if(drawableCreateConfig->scale != nullptr) {
        drawable->setScale(*drawableCreateConfig->scale);
    }

    return drawable;
}

VoxelGridDrawable3D* Visual3DUtil::newVoxelGrid(
        Drawable3DCreateConfig_VoxelGrid* drawableCreateConfig,
        std::shared_ptr<ExtraData> extraData)
{
    Array3D<signed char>* voxelGrid = nullptr;
    std::shared_ptr<graphics::TilingInformation> tilingInformation = nullptr;
    if(drawableCreateConfig->voxelGrid) {
        voxelGrid = drawableCreateConfig->voxelGrid;
    }else if(drawableCreateConfig->voxelGridExtraDataIndicator != nullptr) {
        std::shared_ptr<base::IWrappedValue> wVal = extraData->getDataOptional(*drawableCreateConfig->voxelGridExtraDataIndicator);
        if (wVal == nullptr) {
            throw LogicException(LOC);
        }

        voxelGrid = wVal->getDirectAs_Custom<Array3D<signed char>*>(-1);
    }
    if (voxelGrid == nullptr) {
        // Handle tiling.
        std::shared_ptr<base::IWrappedValue> wVal = extraData->getDataOptional("graphics::TilingInformation");
        if (wVal == nullptr) {
            throw LogicException(LOC);
        }

        tilingInformation = wVal->getDirectAs_CustomB<std::shared_ptr<graphics::TilingInformation>>("graphics::TilingInformation");
    }

    ArrayList<Visual3DModelMaterial*>* materialsList = nullptr;
    if (drawableCreateConfig->voxelGrid) {
        materialsList = drawableCreateConfig->materialsList;
    } else if(drawableCreateConfig->materialsListExtraDataIndicator != nullptr) {
        std::shared_ptr<base::IWrappedValue> wVal = extraData->getDataOptional(*drawableCreateConfig->materialsListExtraDataIndicator);
        if (wVal == nullptr) {
            throw LogicException(LOC);
        }
        
        materialsList = wVal->getDirectAs_Custom<ArrayList<Visual3DModelMaterial*>*>(-1);
        if (materialsList == nullptr) {
            throw LogicException(LOC);
        }
    }
    if (materialsList == nullptr) {
        throw LogicException(LOC);
    }

    VoxelGridDrawable3D* drawable = newVoxelGrid(
        drawableCreateConfig->voxelSize,
        drawableCreateConfig->voxelGridOffset,
        voxelGrid,
        materialsList,
        tilingInformation
    );

    return drawable;
}

VoxelGridDrawable3D* Visual3DUtil::newVoxelGrid(
    float voxelSize,
    Vector3* voxelGridOffset,
    Array3D<signed char>* voxelGrid,
    ArrayList<Visual3DModelMaterial*>* materialsList,
    std::shared_ptr<graphics::TilingInformation> tilingInformation)
{
    VoxelGridDrawable3D* drawable = new VoxelGridDrawable3D();

    if (drawable->getVoxelsGrid()->setVoxelSize(voxelSize) != 0) {
        throw LogicException(LOC);
    }
	drawable->getVoxelsGrid()->setVoxelGridOffset(*voxelGridOffset);

    // Push the materials.
    drawable->getVoxelsGrid()->setMaterialsListCloneMode(materialsList);
    if (voxelGrid != nullptr) {
        if (tilingInformation != nullptr) {
            throw LogicException(LOC);
        }

        drawable->getVoxelsGrid()->setVoxelsGrid(voxelGrid);
    } else if (tilingInformation != nullptr) {
        if (voxelGrid != nullptr) {
            throw LogicException(LOC);
        }
        
        drawable->getVoxelsGrid()->setVoxelsGrid(tilingInformation.get());
    } else {
        throw LogicException(LOC);
    }

    return drawable;
}

Container3D* Visual3DUtil::newGroup(
        Drawable3DCreateConfig_Group* drawableCreateConfig,
        std::shared_ptr<ExtraData> extraData)
{
    Container3D* drawable = new Container3D();

    return drawable;
}

Container3D* Visual3DUtil::newGroup(
        IContainer3D* parent)
{
    Container3D* drawable = newGroup(nullptr, nullptr);

    if (parent != nullptr) {
        parent->addChild(drawable);
    }

    return drawable;
}

void Visual3DUtil::disposeDrawables(ArrayList<IDrawableNode3D*>* drawablesList) {
    if(drawablesList == nullptr) {
        return;
    }

    for(int i=drawablesList->size()-1;i>=0;i--) {
        IDrawableNode3D* drawable = drawablesList->getDirect(i);

        if(drawable->getParent() != nullptr) {
            drawable->getParent()->removeChild(drawable);
        }

        drawablesList->remove(i);

        drawable->reservedDisposeMain();
        delete drawable;
    }
}
