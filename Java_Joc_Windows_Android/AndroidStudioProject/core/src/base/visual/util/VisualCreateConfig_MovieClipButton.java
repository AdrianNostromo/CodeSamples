package base.visual.util;

import com.badlogic.gdx.graphics.g2d.Animation;

import local.audio.util.SoundConfig;

/**
 * Created by Adrin on 7/30/2016.
 */
public class VisualCreateConfig_MovieClipButton extends VisualCreateConfig {

    public ManagedTextureAtlasPath managedAtlasPath;

    public String regionIDPrefix;
    public int regionIndexStart;
    public int regionIndexEnd;
    public int indexZeroPadding;

    public float frameDuration;
    public Animation.PlayMode playMode;
    public boolean isPlaying;

    public SoundConfig touchDownSoundConfig;
    public SoundConfig touchUpSoundConfig;

    public VisualCreateConfig_MovieClipButton(ManagedTextureAtlasPath managedAtlasPath, String regionIDPrefix, int regionIndexStart, int regionIndexEnd, int indexZeroPadding, float frameDuration, Animation.PlayMode playMode, boolean isPlaying, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig) {
        super(TYPE_MOVIE_CLIP_BUTTON);

        this.managedAtlasPath = managedAtlasPath;

        this.regionIDPrefix = regionIDPrefix;
        this.regionIndexStart = regionIndexStart;
        this.regionIndexEnd = regionIndexEnd;
        this.indexZeroPadding = indexZeroPadding;

        this.frameDuration = frameDuration;
        this.playMode = playMode;
        this.isPlaying = isPlaying;

        this.touchDownSoundConfig = touchDownSoundConfig;
        this.touchUpSoundConfig = touchUpSoundConfig;
    }

}
