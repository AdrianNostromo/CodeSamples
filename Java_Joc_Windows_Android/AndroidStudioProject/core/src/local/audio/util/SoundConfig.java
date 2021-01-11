package local.audio.util;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

public class SoundConfig {

	public String id;

	public String fileName;

	public float playVolume;

	public float playDelayS;

	public float fadeInDurationS;
	public float fadeOutDurationS;

	public static final Vector<SoundConfig> entriesList = new Vector<SoundConfig>();
	public static final Map<String, SoundConfig> entriesMap = new HashMap<String, SoundConfig>();

	public static final SoundConfig button_down = new SoundConfig("button_down", "audio/button_down.wav", 1f, 0f, 0f, 0f);
    public static final SoundConfig specialButton_disabled_down = new SoundConfig("specialButton_disabled_down", "audio/specialButton_disabled_down.wav", 1f, 0f, 0f, 0f);

    public static final SoundConfig level_start = new SoundConfig("level_start", "audio/level_start.wav", 1f, 0f, 0f, 0f);
    public static final SoundConfig level_end = new SoundConfig("level_end", "audio/level_end.wav", 1f, 0f, 0f, 0f);

    public static final SoundConfig soundPointCollected = new SoundConfig("soundPointCollected", "audio/soundPointCollected.wav", 1f, 0f, 0f, 0f);
    public static final SoundConfig point_collected = new SoundConfig("point_collected", "audio/point_collected.wav", 1f, 0f, 0f, 0f);

    public static final SoundConfig shopItem_activate = new SoundConfig("shopItem_activate", "audio/shopItem_activate.wav", 1f, 0f, 0f, 0f);
    public static final SoundConfig shopItem_unlock = new SoundConfig("shopItem_unlock", "audio/shopItem_unlock.wav", 1f, 0f, 0f, 0f);
	public static final SoundConfig shopItem_unlockFail = new SoundConfig("shopItem_unlockFail", "audio/shopItem_unlockFail.wav", 1f, 0f, 0f, 0f);

	public static final SoundConfig ball_jump = new SoundConfig("ball_jump", "audio/ball_jump.wav", 1f, 0f, 0f, 0f);

	public SoundConfig(String id, String fileName, float playVolume, float playDelayS, float fadeInDurationS, float fadeOutDurationS) {
		super();

		this.id = id;

		this.fileName = fileName;

		this.playVolume = playVolume;

		this.playDelayS = playDelayS;

		this.fadeInDurationS = fadeInDurationS;
		this.fadeOutDurationS = fadeOutDurationS;

		entriesList.add(this);
		entriesMap.put(id, this);
	}
}
