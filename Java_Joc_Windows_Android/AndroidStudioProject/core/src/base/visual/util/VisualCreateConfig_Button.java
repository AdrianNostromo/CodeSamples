package base.visual.util;

import local.audio.util.SoundConfig;

/**
 * Created by Adrin on 7/30/2016.
 */
public class VisualCreateConfig_Button extends VisualCreateConfig {

    public ManagedTextureAtlasPath managedAtlasPath;
    public String buttonAssetsPrefix;

    public SoundConfig touchDownSoundConfig;
    public SoundConfig touchUpSoundConfig;
    public SoundConfig disabledTouchDownSoundConfig;

    public VisualCreateConfig_Button(ManagedTextureAtlasPath managedAtlasPath, String buttonAssetsPrefix, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig, SoundConfig disabledTouchDownSoundConfig) {
        super(TYPE_BUTTON);

        this.managedAtlasPath = managedAtlasPath;
        this.buttonAssetsPrefix = buttonAssetsPrefix;

        this.touchDownSoundConfig = touchDownSoundConfig;
        this.touchUpSoundConfig = touchUpSoundConfig;
        this.disabledTouchDownSoundConfig = disabledTouchDownSoundConfig;
    }

}
