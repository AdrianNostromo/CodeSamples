package base.audio;

import local.app.AppLocal;
import local.audio.util.SoundConfig;

public class SoundManager extends SoundController {

	public SoundManager(AppLocal app) {
		super(app);

        //void
	}

	public base.audio.util.SoundItem hookNewSoundItem(SoundConfig soundConfig) {
		base.audio.util.SoundItem sound = new base.audio.util.SoundItem(this, app.embeddedAssetsManager.getSound(soundConfig.fileName), soundConfig);

        addSound(sound);

		return sound;
	}

}
