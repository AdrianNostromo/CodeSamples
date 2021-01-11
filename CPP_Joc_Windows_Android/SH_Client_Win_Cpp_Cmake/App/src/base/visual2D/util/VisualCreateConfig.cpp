#include "VisualCreateConfig.h"
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

int VisualCreateConfig::NewType() {
    static int COUNTER = 0;

    COUNTER++;

    return COUNTER;
}

VisualCreateConfig::VisualCreateConfig(int type)
    : type(type)
{
    //void
}

VisualCreateConfig::~VisualCreateConfig() {
    //void
}

/*asdAA;// implement as needed;
public class VisualCreateConfig {

	public VisualCreateConfig(final int type) {
		super();

        this.type = type;
	}

    public static VisualCreateConfig_ConeTrailSprite2D newConeTrailSprite2D(String atlasPath, String regionID) {
        VisualCreateConfig_ConeTrailSprite2D ret = newt VisualCreateConfig_ConeTrailSprite2D(atlasPath, regionID);

        return ret;
    }

    public static VisualCreateConfig_Button newButton(ManagedFilePath managedAtlasPath, String buttonAssetsPrefix, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig, SoundConfig disabledTouchDownSoundConfig) {
        VisualCreateConfig_Button ret = newt VisualCreateConfig_Button(
            managedAtlasPath, buttonAssetsPrefix,
            touchDownSoundConfig, touchUpSoundConfig, disabledTouchDownSoundConfig
        );

        return ret;
    }

    public static VisualCreateConfig_Button newButton(String atlasID, String buttonAssetsPrefix, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig, SoundConfig disabledTouchDownSoundConfig) {
        return newButton(ManagedFilePath.stringPathToManagedTextureAtlasPath(atlasID), buttonAssetsPrefix, touchDownSoundConfig, touchUpSoundConfig, disabledTouchDownSoundConfig);
    }

    public static VisualCreateConfig_Button newButton(ManagedFilePath managedAtlasPath, String buttonAssetsPrefix, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig) {
        return newButton(managedAtlasPath, buttonAssetsPrefix, touchDownSoundConfig, touchUpSoundConfig, null);
    }

    public static VisualCreateConfig_Button newButton(String atlasID, String buttonAssetsPrefix, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig) {
        return newButton(ManagedFilePath.stringPathToManagedTextureAtlasPath(atlasID), buttonAssetsPrefix, touchDownSoundConfig, touchUpSoundConfig);
    }

    public static VCC_MovieClip newMovieClip(ManagedFilePath managedAtlasPath, String regionIDPrefix, int regionIndexStart, int regionIndexEnd, int indexZeroPaddedStringLen, float frameDuration, Animation.PlayMode playMode, boolean isPlaying) {
        VCC_MovieClip ret = newt VCC_MovieClip(managedAtlasPath, regionIDPrefix, regionIndexStart, regionIndexEnd, indexZeroPaddedStringLen, frameDuration, playMode, isPlaying);

        return ret;
    }

    public static VCC_MovieClip newMovieClip(String atlasID, String regionIDPrefix, int regionIndexStart, int regionIndexEnd, int indexZeroPaddedStringLen, float frameDuration, Animation.PlayMode playMode, boolean isPlaying) {
        return newMovieClip(ManagedFilePath.stringPathToManagedTextureAtlasPath(atlasID), regionIDPrefix, regionIndexStart, regionIndexEnd, indexZeroPaddedStringLen, frameDuration, playMode, isPlaying);
    }

    public static VisualCreateConfig_MovieClipButton newMovieClipButton(ManagedFilePath managedAtlasPath, String regionIDPrefix, int regionIndexStart, int regionIndexEnd, int indexZeroPaddedStringLen, float frameDuration, Animation.PlayMode playMode, boolean isPlaying, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig) {
        VisualCreateConfig_MovieClipButton ret = newt VisualCreateConfig_MovieClipButton(managedAtlasPath, regionIDPrefix, regionIndexStart, regionIndexEnd, indexZeroPaddedStringLen, frameDuration, playMode, isPlaying, touchDownSoundConfig, touchUpSoundConfig);

        return ret;
    }

    public static VisualCreateConfig_MovieClipButton newMovieClipButton(String atlasID, String regionIDPrefix, int regionIndexStart, int regionIndexEnd, int indexZeroPaddedStringLen, float frameDuration, Animation.PlayMode playMode, boolean isPlaying, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig) {
        return newMovieClipButton(ManagedFilePath.stringPathToManagedTextureAtlasPath(atlasID), regionIDPrefix, regionIndexStart, regionIndexEnd, indexZeroPaddedStringLen, frameDuration, playMode, isPlaying, touchDownSoundConfig, touchUpSoundConfig);
    }

    public static VCC_BitmapLabel newLabel(Object fontData, String defaultText) {
        VCC_BitmapLabel ret = newt VCC_BitmapLabel(fontData, defaultText);

        return ret;
    }

    public static VCC_BitmapLabelIndexedFonts newBitmapLabelIndexedFonts(Object[] fontDataList, String defaultText) {
        VCC_BitmapLabelIndexedFonts ret = newt VCC_BitmapLabelIndexedFonts(fontDataList, defaultText);

        return ret;
    }

    public static VCC_TouchArea newTouchArea(SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig) {
        VCC_TouchArea ret = newt VCC_TouchArea(touchDownSoundConfig, touchUpSoundConfig);

        return ret;
    }

    public static VCC_TouchArea newTouchArea() {
	    return newTouchArea(null, null);
    }

    public static VCC_TouchArea newTouchArea(SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig, final int touchShape) {
        VCC_TouchArea ret = newt VCC_TouchArea(touchDownSoundConfig, touchUpSoundConfig, touchShape);

        return ret;
    }

    public static VCC_TouchArea newTouchArea(final int touchShape) {
	    return newTouchArea(null, null, touchShape);
    }

    public static VisualCreateConfig_CustomRectangleSprite newCustomRectangleSprite(Object color, Size2DInt textureSize) {
        VisualCreateConfig_CustomRectangleSprite ret = newt VisualCreateConfig_CustomRectangleSprite(color, textureSize);

        return ret;
    }

    public static VisualCreateConfig_PixelSprite newPixelSprite(PixelVisualData pixelData) {
        VisualCreateConfig_PixelSprite ret = newt VisualCreateConfig_PixelSprite(pixelData);

        return ret;
    }

}*/
