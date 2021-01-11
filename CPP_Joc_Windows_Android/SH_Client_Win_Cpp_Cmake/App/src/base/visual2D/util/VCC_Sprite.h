#pragma once

#include <base/gh.h>
#include "VCC_Touchable.h"
#include <string>
#include <base/assets/util/ManagedAssetPath.h>

class VCC_Sprite : public VCC_Touchable {priv typedef VCC_Touchable super;pub dCtor(VCC_Sprite);
	pub static int TYPE;

	// If null, the sprite is created without a texture (eg. optional texture mode used for u.i. dynamic icons).
    pub ManagedAssetPath* managedAtlasPath;
    pub std::string regionID;

    pub explicit VCC_Sprite();
    pub explicit VCC_Sprite(ManagedAssetPath* managedAtlasPath, std::string regionID);

    pub static VCC_Sprite* New();
    pub static VCC_Sprite* New(
        ManagedAssetPath* managedAtlasPath, std::string regionID);
    pub static VCC_Sprite* New(
        std::string atlasId, std::string regionID);

    pub virtual ~VCC_Sprite();
};
