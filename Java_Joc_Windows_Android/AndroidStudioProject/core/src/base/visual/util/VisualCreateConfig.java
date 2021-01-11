package base.visual.util;

import com.badlogic.gdx.graphics.g2d.Animation;

import local.audio.util.SoundConfig;
import base.util.Size2DInt;

public class VisualCreateConfig {

    public static final int TYPE_GROUP = 1;
    public static final int TYPE_SPRITE = 2;
    public static final int TYPE_BUTTON = 3;
    public static final int TYPE_MOVIE_CLIP = 4;
    public static final int TYPE_MOVIE_CLIP_BUTTON = 5;
    public static final int TYPE_LABEL = 6;
    public static final int TYPE_TOUCH_AREA_AUDIBLE = 7;
    public static final int TYPE_TOUCH_AREA_LIGHTWEIGHT = 8;
    public static final int TYPE_CUSTOM_RECTANGLE_SPRITE = 9;
    public static final int TYPE_PIXEL_SPRITE = 10;

    public int type;

	public VisualCreateConfig(int type) {
		super();

        this.type = type;
	}

    public static VisualCreateConfig_Group newGroup() {
        VisualCreateConfig_Group ret = new VisualCreateConfig_Group();

        return ret;
    }

    public static VisualCreateConfig_Sprite newSprite(ManagedTextureAtlasPath managedAtlasPath, String regionID) {
        VisualCreateConfig_Sprite ret = new VisualCreateConfig_Sprite(managedAtlasPath, regionID);

        return ret;
    }

    public static VisualCreateConfig_Sprite newSprite(String atlasId, String regionID) {
        return newSprite(new ManagedTextureAtlasPath(atlasId), regionID);
    }

    public static VisualCreateConfig_Button newButton(ManagedTextureAtlasPath managedAtlasPath, String buttonAssetsPrefix, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig, SoundConfig disabledTouchDownSoundConfig) {
        VisualCreateConfig_Button ret = new VisualCreateConfig_Button(managedAtlasPath, buttonAssetsPrefix, touchDownSoundConfig, touchUpSoundConfig, disabledTouchDownSoundConfig);

        return ret;
    }
    
    public static VisualCreateConfig_Button newButton(String atlasID, String buttonAssetsPrefix, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig, SoundConfig disabledTouchDownSoundConfig) {
        return newButton(new ManagedTextureAtlasPath(atlasID), buttonAssetsPrefix, touchDownSoundConfig, touchUpSoundConfig, disabledTouchDownSoundConfig);
    }
    
    public static VisualCreateConfig_Button newButton(ManagedTextureAtlasPath managedAtlasPath, String buttonAssetsPrefix, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig) {
        return newButton(managedAtlasPath, buttonAssetsPrefix, touchDownSoundConfig, touchUpSoundConfig, null);
    }
    
    public static VisualCreateConfig_Button newButton(String atlasID, String buttonAssetsPrefix, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig) {
        return newButton(new ManagedTextureAtlasPath(atlasID), buttonAssetsPrefix, touchDownSoundConfig, touchUpSoundConfig);
    }
    
    public static VisualCreateConfig_MovieClip newMovieClip(ManagedTextureAtlasPath managedAtlasPath, String regionIDPrefix, int regionIndexStart, int regionIndexEnd, int indexZeroPadding, float frameDuration, Animation.PlayMode playMode, boolean isPlaying) {
        VisualCreateConfig_MovieClip ret = new VisualCreateConfig_MovieClip(managedAtlasPath, regionIDPrefix, regionIndexStart, regionIndexEnd, indexZeroPadding, frameDuration, playMode, isPlaying);

        return ret;
    }
    
    public static VisualCreateConfig_MovieClip newMovieClip(String atlasID, String regionIDPrefix, int regionIndexStart, int regionIndexEnd, int indexZeroPadding, float frameDuration, Animation.PlayMode playMode, boolean isPlaying) {
        return newMovieClip(new ManagedTextureAtlasPath(atlasID), regionIDPrefix, regionIndexStart, regionIndexEnd, indexZeroPadding, frameDuration, playMode, isPlaying);
    }
    
    public static VisualCreateConfig_MovieClipButton newMovieClipButton(ManagedTextureAtlasPath managedAtlasPath, String regionIDPrefix, int regionIndexStart, int regionIndexEnd, int indexZeroPadding, float frameDuration, Animation.PlayMode playMode, boolean isPlaying, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig) {
        VisualCreateConfig_MovieClipButton ret = new VisualCreateConfig_MovieClipButton(managedAtlasPath, regionIDPrefix, regionIndexStart, regionIndexEnd, indexZeroPadding, frameDuration, playMode, isPlaying, touchDownSoundConfig, touchUpSoundConfig);

        return ret;
    }
    
    public static VisualCreateConfig_MovieClipButton newMovieClipButton(String atlasID, String regionIDPrefix, int regionIndexStart, int regionIndexEnd, int indexZeroPadding, float frameDuration, Animation.PlayMode playMode, boolean isPlaying, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig) {
        return newMovieClipButton(new ManagedTextureAtlasPath(atlasID), regionIDPrefix, regionIndexStart, regionIndexEnd, indexZeroPadding, frameDuration, playMode, isPlaying, touchDownSoundConfig, touchUpSoundConfig);
    }

    public static VisualCreateConfig_Label newLabel(Object fontData, String defaultText) {
        VisualCreateConfig_Label ret = new VisualCreateConfig_Label(fontData, defaultText);

        return ret;
    }

    public static VisualCreateConfig_TouchAreaAudible newTouchAreaAudible(SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig) {
        VisualCreateConfig_TouchAreaAudible ret = new VisualCreateConfig_TouchAreaAudible(touchDownSoundConfig, touchUpSoundConfig);

        return ret;
    }

    public static VisualCreateConfig_TouchAreaLightweight newTouchAreaLightweight() {
        VisualCreateConfig_TouchAreaLightweight ret = new VisualCreateConfig_TouchAreaLightweight();

        return ret;
    }

    public static VisualCreateConfig_CustomRectangleSprite newCustomRectangleSprite(Object color, Size2DInt textureSize) {
        VisualCreateConfig_CustomRectangleSprite ret = new VisualCreateConfig_CustomRectangleSprite(color, textureSize);

        return ret;
    }

    public static VisualCreateConfig_PixelSprite newPixelSprite(PixelVisualData pixelData) {
        VisualCreateConfig_PixelSprite ret = new VisualCreateConfig_PixelSprite(pixelData);

        return ret;
    }

}
