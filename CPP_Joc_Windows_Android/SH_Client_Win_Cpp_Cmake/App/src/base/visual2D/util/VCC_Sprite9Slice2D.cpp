#include "VCC_Sprite9Slice2D.h"
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

int VCC_Sprite9Slice2D::TYPE = super::NewType();

VCC_Sprite9Slice2D::VCC_Sprite9Slice2D(bool drawCenterRegion)
    : super(TYPE),
    managedAtlasPath(nullptr), regionID(""),
    drawCenterRegion(drawCenterRegion), sliceSizePercents()
{
    //void
}

VCC_Sprite9Slice2D::VCC_Sprite9Slice2D(
    ManagedAssetPath* managedAtlasPath, std::string regionID,
    bool drawCenterRegion, Slice9SizePercents sliceSizePercents)
    : super(TYPE),
    managedAtlasPath(managedAtlasPath), regionID(regionID),
    drawCenterRegion(drawCenterRegion), sliceSizePercents(sliceSizePercents)
{
    //void
}

VCC_Sprite9Slice2D* VCC_Sprite9Slice2D::New(
    bool drawCenterRegion)
{
    VCC_Sprite9Slice2D* ret = newt VCC_Sprite9Slice2D(drawCenterRegion);

    return ret;
}

VCC_Sprite9Slice2D* VCC_Sprite9Slice2D::New(
    ManagedAssetPath* managedAtlasPath, std::string regionID,
    bool drawCenterRegion, Slice9SizePercents sliceSizePercents)
{
    VCC_Sprite9Slice2D* ret = newt VCC_Sprite9Slice2D(
        managedAtlasPath, regionID,
        drawCenterRegion, std::move(sliceSizePercents)
    );

    return ret;
}

VCC_Sprite9Slice2D* VCC_Sprite9Slice2D::New(
    std::string atlasId, std::string regionID,
    bool drawCenterRegion, Slice9SizePercents sliceSizePercents)
{
    return New(
        ManagedAssetPath::stringPathToManagedTextureAtlasPath(atlasId), regionID,
        drawCenterRegion, std::move(sliceSizePercents)
    );
}

VCC_Sprite9Slice2D::~VCC_Sprite9Slice2D() {
    //void
}
