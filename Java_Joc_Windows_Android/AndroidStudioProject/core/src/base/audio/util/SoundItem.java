package base.audio.util;

import com.badlogic.gdx.audio.Sound;

import local.audio.util.SoundConfig;

public class SoundItem {

    public ISoundController soundController;

	public Sound soundData;

	public SoundConfig config;

	public long playId = -1;

	public boolean isPlaying = false;
    public boolean isLooping = false;

    public float playDelayS = 0;
    public float fadeInPercent = 0;
    public float fadeOutPercent = 0;

	public SoundItem(ISoundController soundController, Sound soundData, SoundConfig config) {
		super();

        this.soundController = soundController;

		this.soundData = soundData;

		this.config = config;
	}

    public void play() {
        play(false);
    }

    public void play(boolean loop) {
        soundController.soundPlay(this, loop);
	}

	public void stop() {
        soundController.soundStop(this);
	}

}
