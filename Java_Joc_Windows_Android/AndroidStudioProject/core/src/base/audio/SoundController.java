package base.audio;

import com.badlogic.gdx.Gdx;

import java.util.Stack;
import java.util.Vector;

import base.audio.util.ISoundController;
import base.audio.util.SoundControllerEventListener;
import base.audio.util.SoundItem;
import local.app.AppLocal;

public class SoundController implements ISoundController {

    protected AppLocal app;

    private Stack<SoundItem> soundsList = new Stack<SoundItem>();

    private boolean isUnmuted = false;//asd_tst_001;

    protected Vector<SoundControllerEventListener> eventListenersList = new Vector<SoundControllerEventListener>();

    public SoundController(AppLocal app) {
        super();

        this.app = app;
    }

    public boolean getIsUnmuted() {
        return isUnmuted;
    }

    public void setIsUnmuted(boolean isUnmuted) {
        if(this.isUnmuted == isUnmuted) {
            return ;
        }

        this.isUnmuted = isUnmuted;

        if(!isUnmuted) {
            // Remove all sounds.

            while(soundsList.size() > 0) {
                SoundItem soundItem = soundsList.pop();
                if (soundItem != null) {
                    soundItem.stop();
                }
            }
        }

        for(int i=0;i<eventListenersList.size();i++) {
            SoundControllerEventListener entry = eventListenersList.get(i);
            if(entry != null) {
                entry.unmuteStatusChanged();
            }
        }
    }

    protected void addSound(SoundItem soundItem) {
        soundsList.push(soundItem);
    }

    public void removeSound(SoundItem soundItem) {
        soundItem.stop();

        int i = soundsList.indexOf(soundItem);

        soundsList.remove(i);
    }

    public void toggleIsUnmuted() {
        boolean newIsUnmuted;
        if(!isUnmuted) {
            newIsUnmuted = true;
        }else {
            newIsUnmuted = false;
        }

        setIsUnmuted(newIsUnmuted);
    }

    public void appLoop(float deltaS, int deltaMS) {
        for(int i=0;i<soundsList.size();i++) {
            SoundItem soundItem = soundsList.get(i);
            if(soundItem != null) {
                appLoop_soundItem(soundItem, deltaS, deltaMS);
            }
        }
    }

    private void appLoop_soundItem(SoundItem soundItem, float deltaS, int deltaMS) {
        if(soundItem.playDelayS > 0) {
            soundItem.playDelayS = Math.max(0, soundItem.playDelayS - deltaS);

            if(soundItem.playDelayS <= 0) {
                if(!soundItem.isLooping) {
                    soundItem.playId = soundItem.soundData.play(soundItem.config.playVolume);
                }else {
                    soundItem.playId = soundItem.soundData.loop(soundItem.config.playVolume);
                }
            }
        }
        if(soundItem.isPlaying && soundItem.playDelayS <= 0 && soundItem.config.fadeInDurationS > 0 && soundItem.fadeInPercent < 1) {
            soundItem.fadeInPercent = Math.min(1, soundItem.fadeInPercent + deltaS / soundItem.config.fadeInDurationS);
            soundItem.soundData.setVolume(soundItem.playId, soundItem.fadeInPercent);
        }
        if(!soundItem.isPlaying && soundItem.config.fadeOutDurationS > 0 && soundItem.fadeOutPercent < 1 && soundItem.playId != -1) {
            soundItem.fadeOutPercent = Math.min(1, soundItem.fadeOutPercent + deltaS / soundItem.config.fadeOutDurationS);
            soundItem.soundData.setVolume(soundItem.playId, 1 - soundItem.fadeOutPercent);

            if(soundItem.fadeOutPercent >= 1) {
                if(soundItem.playId != -1) {
                    soundItem.soundData.stop(soundItem.playId);

                    soundItem.playId = -1;
                }
            }
        }
    }

    @Override
    public void soundPlay(SoundItem soundItem, boolean loop) {
        if(!isUnmuted) {
            // Don't play sounds if muted.

            return ;
        }

        soundItem.isPlaying = true;
        soundItem.isLooping = loop;
        soundItem.fadeInPercent = 0;
        soundItem.fadeOutPercent = 0;

        soundItem.playDelayS = soundItem.config.playDelayS;
        if(soundItem.playDelayS <= 0) {
            if(!loop) {
                soundItem.playId = soundItem.soundData.play(soundItem.config.playVolume);
            }else {
                soundItem.playId = soundItem.soundData.loop(soundItem.config.playVolume);
            }
        }
    }

    @Override
    public void soundStop(SoundItem soundItem) {
        if(soundItem.config.fadeOutDurationS <= 0 && soundItem.playId != -1) {
            soundItem.soundData.stop(soundItem.playId);

            soundItem.playId = -1;
        }

        soundItem.isPlaying = false;
        soundItem.isLooping = false;
        soundItem.playDelayS = 0;
        soundItem.fadeInPercent = 0;
        soundItem.fadeOutPercent = 0;
    }

    public void addEventsListener(SoundControllerEventListener eventsListener) {
        if(eventListenersList.indexOf(eventsListener) >= 0) {
            Gdx.app.debug("Error", "Events listener add error.");

            return ;
        }

        eventListenersList.add(eventsListener);
    }

    public void removeEventsListener(SoundControllerEventListener eventsListener) {
        int i = eventListenersList.indexOf(eventsListener);
        if(i < 0) {
            Gdx.app.debug("Error", "Events listener remove error.");

            return ;
        }

        eventListenersList.remove(i);
    }

}
