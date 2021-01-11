#include "VCC_Sprite.h"
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

int VCC_Sprite::TYPE = super::NewType();

VCC_Sprite::VCC_Sprite()
    : super(TYPE),
      managedAtlasPath(nullptr), regionID("")
{
    //void
}

VCC_Sprite::VCC_Sprite(ManagedAssetPath* managedAtlasPath, std::string regionID)
    : super(TYPE),
      managedAtlasPath(managedAtlasPath), regionID(regionID)
{
    //void
}

VCC_Sprite* VCC_Sprite::New() {
    VCC_Sprite* ret = newt VCC_Sprite();

    return ret;
}

VCC_Sprite* VCC_Sprite::New(ManagedAssetPath* managedAtlasPath, std::string regionID) {
    VCC_Sprite* ret = newt VCC_Sprite(managedAtlasPath, regionID);

    return ret;
}

VCC_Sprite* VCC_Sprite::New(std::string atlasId, std::string regionID) {
    return New(ManagedAssetPath::stringPathToManagedTextureAtlasPath(atlasId), regionID);
}

VCC_Sprite::~VCC_Sprite() {
    //void
}
