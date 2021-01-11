#pragma once

#include "VCC_Touchable.h"
#include <string>
#include <base/assets/util/ManagedAssetPath.h>
#include <graphics/visual2d/util/PlayMode.h>

class VCC_MovieClip : public VCC_Touchable {priv typedef VCC_Touchable super;pub dCtor(VCC_MovieClip);
	pub static int TYPE;

    pub ManagedAssetPath* managedAtlasPath;

    pub std::string regionIDPrefix;
    pub std::string regionIDSuffix;
    // -1: Will be deduced automatically to 0 or 1 on MovieClip creation.
    // >=0: Manual value.
    pub int regionIndexStart;
    // -1: Will be deduced automatically to a valid value.
    // >=0: Manual value.
    pub int regionIndexEnd;
    pub int indexZeroPaddedStringLen;

    pub float frameDuration;
    pub PlayMode playMode;
    pub bool isPlaying;

    pub explicit VCC_MovieClip(
        ManagedAssetPath* managedAtlasPath,
        std::string regionIDPrefix, std::string regionIDSuffix, int regionIndexStart, int regionIndexEnd, int indexZeroPaddedStringLen,
        float frameDuration, PlayMode playMode, bool isPlaying);

    pub static VCC_MovieClip* New(
        ManagedAssetPath* managedAtlasPath,
        std::string regionIDPrefix, std::string regionIDSuffix, int regionIndexStart, int regionIndexEnd, int indexZeroPaddedStringLen,
        float frameDuration, PlayMode playMode, bool isPlaying);
    // smartRegionID: Contains the index as # (eg: 0001, 0002 will be "####"; 000, 001 will be "###");
    pub static VCC_MovieClip* New(
        std::string atlasId,
        std::string regionIDPrefix, std::string regionIDSuffix, int regionIndexStart, int regionIndexEnd, int indexZeroPaddedStringLen,
        float frameDuration, PlayMode playMode, bool isPlaying);
    pub static VCC_MovieClip* New(
        std::string atlasId,
        std::string regionIDPrefix, int regionIndexStart, int regionIndexEnd, int indexZeroPaddedStringLen,
        float frameDuration, PlayMode playMode, bool isPlaying);
    pub static VCC_MovieClip* New(
        std::string atlasId,
        std::string smartRegionID,
        float frameDuration, PlayMode playMode, bool isPlaying);

    pub ~VCC_MovieClip() override;
};
