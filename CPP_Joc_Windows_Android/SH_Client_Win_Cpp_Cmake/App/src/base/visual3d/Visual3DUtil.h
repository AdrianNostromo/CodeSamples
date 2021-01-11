#pragma once

#include <string>
#include <memory>

class Visual3DCreateConfig;
class Drawable3DCreateConfigBase;
class VoxelGridDrawable3D;
class Drawable3DCreateConfig_VoxelGrid;
class Drawable3DCreateConfig_Group;
class IContainer3D;
class Container3D;
class IDrawableNode3D;
class Vector3;
class Visual3DModelMaterial;
class ExtraData;
namespace graphics {
    class TilingInformation;
};

template <typename T>
class Array3D;

template <typename V>
class ArrayList;

template <typename K, typename V>
class Map1D;

class Visual3DUtil {
public:
    static IDrawableNode3D* createConfiguredVisual3D(
        Visual3DCreateConfig* visualCreateConfig,
        ArrayList<IDrawableNode3D*>* drawablesList, Map1D<std::string, IDrawableNode3D*>* drawablesMap,
        IContainer3D* visualParent, std::shared_ptr<ExtraData> extraData,
        bool doReservedCreateVisuals);

    static IDrawableNode3D* newDrawable(
            Drawable3DCreateConfigBase* drawableCreateConfig,
            std::shared_ptr<ExtraData> extraData
    );
    static VoxelGridDrawable3D* newVoxelGrid(
            Drawable3DCreateConfig_VoxelGrid* drawableCreateConfig,
            std::shared_ptr<ExtraData> extraData
    );
    static VoxelGridDrawable3D* newVoxelGrid(
        float voxelSize,
        Vector3* voxelGridOffset,
        Array3D<signed char>* voxelGrid,
        ArrayList<Visual3DModelMaterial*>* materialsList,
        std::shared_ptr<graphics::TilingInformation> tilingInformation
    );
    static Container3D* newGroup(
            Drawable3DCreateConfig_Group* drawableCreateConfig,
            std::shared_ptr<ExtraData> extraData
    );
    static Container3D* newGroup(
            IContainer3D* parent
    );

    static void disposeDrawables(ArrayList<IDrawableNode3D*>* drawablesList);

};
