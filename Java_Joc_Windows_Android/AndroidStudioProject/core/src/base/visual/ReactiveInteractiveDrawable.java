package base.visual;

import base.audio.util.SoundItem;
import base.interaction.ITouchListener;

/**
 * Created by Adrin on 6/23/2016.
 */
public class ReactiveInteractiveDrawable extends InteractiveDrawable implements ITouchListener {

    private SoundItem touchDownSound;
    private SoundItem touchUpSound;

    private boolean isSoundDisabledUntilNextTouchDown = false;
    private boolean isSoundDisabledUntilNextTouchDownPost = false;

    public ReactiveInteractiveDrawable(SoundItem touchDownSound, SoundItem touchUpSound) {
        super();

        this.touchDownSound = touchDownSound;
        this.touchUpSound = touchUpSound;
    }

    public void disableSoundsUntilNextTouchDown() {
        isSoundDisabledUntilNextTouchDown = true;
        isSoundDisabledUntilNextTouchDownPost = true;
    }

    @Override
    public boolean touchDown(base.visual.IInteractiveDrawable target, float worldX, float worldY) {
        if(isSoundDisabledUntilNextTouchDown) {
            isSoundDisabledUntilNextTouchDown = false;

            return false;
        }

        if(isSoundDisabledUntilNextTouchDownPost) {
            isSoundDisabledUntilNextTouchDownPost = false;
        }

        playTouchDownSound();

        return false;
    }

    @Override
    public void touchUp(base.visual.IInteractiveDrawable target, float worldX, float worldY) {
        if(isSoundDisabledUntilNextTouchDownPost) {
            return;
        }

        playTouchUpSound();
    }

    @Override
    public void touchUpOutside(base.visual.IInteractiveDrawable target, float worldX, float worldY) {
        if(isSoundDisabledUntilNextTouchDownPost) {
            return;
        }

        playTouchUpSound();
    }

    @Override
    public void touchTap(base.visual.IInteractiveDrawable target, float worldX, float worldY) {
        //void
    }

    @Override
    public void touchMove(base.visual.IInteractiveDrawable target, float worldX, float worldY) {
        //void
    }

    protected void playTouchDownSound() {
        if(touchDownSound != null) {
            touchDownSound.play();
        }
    }

    protected void playTouchUpSound() {
        if (touchUpSound != null) {
            touchUpSound.play();
        }
    }

}
