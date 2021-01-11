#include "VCC_MovieClip.h"
#include <base/exceptions/LogicException.h>
#include <base/util/StringUtil.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

int VCC_MovieClip::TYPE = super::NewType();

VCC_MovieClip::VCC_MovieClip(
    ManagedAssetPath* managedAtlasPath,
    std::string regionIDPrefix, std::string regionIDSuffix, int regionIndexStart, int regionIndexEnd, int indexZeroPaddedStringLen,
    float frameDuration, PlayMode playMode, bool isPlaying
)
        : super(TYPE),
        managedAtlasPath(managedAtlasPath),
        regionIDPrefix(regionIDPrefix), regionIDSuffix(regionIDSuffix), regionIndexStart(regionIndexStart), regionIndexEnd(regionIndexEnd), indexZeroPaddedStringLen(indexZeroPaddedStringLen),
        frameDuration(frameDuration), playMode(playMode), isPlaying(isPlaying)
{
    //void
}

VCC_MovieClip* VCC_MovieClip::New(
    ManagedAssetPath* managedAtlasPath,
    std::string regionIDPrefix, std::string regionIDSuffix, int regionIndexStart, int regionIndexEnd, int indexZeroPaddedStringLen,
    float frameDuration, PlayMode playMode, bool isPlaying)
{
    VCC_MovieClip* ret = newt VCC_MovieClip(
        managedAtlasPath,
        regionIDPrefix, regionIDSuffix, regionIndexStart, regionIndexEnd, indexZeroPaddedStringLen,
        frameDuration, playMode, isPlaying);

    return ret;
}

VCC_MovieClip* VCC_MovieClip::New(
    std::string atlasId,
    std::string regionIDPrefix, std::string regionIDSuffix, int regionIndexStart, int regionIndexEnd, int indexZeroPaddedStringLen,
    float frameDuration, PlayMode playMode, bool isPlaying)
{
    return New(
        ManagedAssetPath::stringPathToManagedTextureAtlasPath(atlasId),
        regionIDPrefix, regionIDSuffix, regionIndexStart, regionIndexEnd, indexZeroPaddedStringLen,
        frameDuration, playMode, isPlaying);
}

VCC_MovieClip* VCC_MovieClip::New(
    std::string atlasId,
    std::string regionIDPrefix, int regionIndexStart, int regionIndexEnd, int indexZeroPaddedStringLen,
    float frameDuration, PlayMode playMode, bool isPlaying)
{
    return New(
        atlasId,
        regionIDPrefix, std::string(), regionIndexStart, regionIndexEnd, indexZeroPaddedStringLen,
        frameDuration, playMode, isPlaying);
}

VCC_MovieClip* VCC_MovieClip::New(
    std::string atlasId,
    std::string smartRegionID,
    float frameDuration, PlayMode playMode, bool isPlaying)
{
    int smartIndexOffset = StringUtil::indexOf(smartRegionID, "#");
    if(smartIndexOffset < 0) {
        throw LogicException(LOC);
    }
    int indexZeroPaddedStringLen = (int)std::count(smartRegionID.begin(), smartRegionID.end(), '#');
    std::string regionIDPrefix = StringUtil::substringRange(smartRegionID, 0, smartIndexOffset - 1);
    std::string regionIDSuffix;
    if((int)smartRegionID.size() > smartIndexOffset + indexZeroPaddedStringLen) {
        regionIDSuffix = StringUtil::substringSegment(smartRegionID, smartIndexOffset + indexZeroPaddedStringLen, -1);
    }else {
        regionIDSuffix = std::string();
    }

    return New(
        atlasId,
        regionIDPrefix, regionIDSuffix, -1, -1, indexZeroPaddedStringLen,
        frameDuration, playMode, isPlaying);
}

VCC_MovieClip::~VCC_MovieClip() {
    //void
}
