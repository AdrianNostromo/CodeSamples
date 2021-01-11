package base.visual.util;

import local.audio.util.SoundConfig;

/**
 * Created by Adrin on 7/30/2016.
 */
public class VisualCreateConfig_TouchAreaAudible extends VisualCreateConfig {

    public SoundConfig touchDownSoundConfig;
    public SoundConfig touchUpSoundConfig;

    public VisualCreateConfig_TouchAreaAudible(SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig) {
        super(TYPE_TOUCH_AREA_AUDIBLE);

        this.touchDownSoundConfig = touchDownSoundConfig;
        this.touchUpSoundConfig = touchUpSoundConfig;
    }

}
