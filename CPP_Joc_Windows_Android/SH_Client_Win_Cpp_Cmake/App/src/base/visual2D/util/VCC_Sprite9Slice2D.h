#pragma once

#include <base/gh.h>
#include "VCC_Touchable.h"
#include <string>
#include <base/assets/util/ManagedAssetPath.h>
#include <base/math/Slice9SizePercents.h>

class VCC_Sprite9Slice2D : public VCC_Touchable {priv typedef VCC_Touchable super;pub dCtor(VCC_Sprite9Slice2D);
	pub static int TYPE;

	// If null, the sprite is created without a texture (eg. optional texture mode used for u.i. dynamic icons).
    pub ManagedAssetPath* managedAtlasPath;
    pub std::string regionID;

    pub bool drawCenterRegion;
    pub Slice9SizePercents sliceSizePercents;

    pub explicit VCC_Sprite9Slice2D(
        bool drawCenterRegion);
    pub explicit VCC_Sprite9Slice2D(
        ManagedAssetPath* managedAtlasPath, std::string regionID,
        bool drawCenterRegion, Slice9SizePercents sliceSizePercents);

    pub static VCC_Sprite9Slice2D* New(
        bool drawCenterRegion);
    pub static VCC_Sprite9Slice2D* New(
        ManagedAssetPath* managedAtlasPath, std::string regionID,
        bool drawCenterRegion, Slice9SizePercents sliceSizePercents);
    pub static VCC_Sprite9Slice2D* New(
        std::string atlasId, std::string regionID,
        bool drawCenterRegion, Slice9SizePercents sliceSizePercents);

    pub virtual ~VCC_Sprite9Slice2D();
};
