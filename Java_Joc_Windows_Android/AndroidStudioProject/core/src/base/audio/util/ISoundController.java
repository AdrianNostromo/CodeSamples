package base.audio.util;

public interface ISoundController {

    void soundPlay(SoundItem soundItem, boolean loop);
    void soundStop(SoundItem soundItem);

}
