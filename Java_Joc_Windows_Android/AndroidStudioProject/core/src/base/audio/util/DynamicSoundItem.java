package base.audio.util;

import local.app.AppLocal;
import local.audio.util.SoundConfig;

/**
 * Created by Adrin on 9/12/2016.
 */
public class DynamicSoundItem {

    protected AppLocal app;

    private SoundConfig soundConfig = null;
    private SoundItem soundItem = null;

    public DynamicSoundItem(AppLocal app) {
        super();

        this.app = app;
    }

    public SoundConfig getSoundConfig() {
        return soundConfig;
    }

    public void putSoundConfig(SoundConfig soundConfig) {
        if(this.soundConfig == soundConfig) {
            return ;
        }

        if(soundItem != null) {
            app.soundManager.removeSound(soundItem);

            soundItem = null;
        }

        this.soundConfig = soundConfig;

        if(this.soundConfig != null) {
            soundItem = app.soundManager.hookNewSoundItem(soundConfig);
        }
    }

    public void play() {
        if(soundItem != null) {
            soundItem.play();
        }
    }

}
