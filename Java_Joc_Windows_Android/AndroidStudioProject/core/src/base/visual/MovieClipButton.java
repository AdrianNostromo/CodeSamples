package base.visual;

import com.badlogic.gdx.graphics.g2d.Animation;
import com.badlogic.gdx.graphics.g2d.TextureRegion;

public class MovieClipButton extends MovieClip implements base.interaction.ITouchListener {

    public base.audio.util.SoundItem touchDownSound;
    public base.audio.util.SoundItem touchUpSound;

    public MovieClipButton(Animation<TextureRegion> animation, base.audio.util.SoundItem touchDownSound, base.audio.util.SoundItem touchUpSound) {
        super(animation);

        setIsInputInteractive(true);

        this.touchDownSound = touchDownSound;
        this.touchUpSound = touchUpSound;
    }

    @Override
    public boolean touchDown(IInteractiveDrawable target, float worldX, float worldY) {
        if(touchDownSound != null) {
            touchDownSound.play();
        }

        return false;
    }

    @Override
    public void touchUp(IInteractiveDrawable target, float worldX, float worldY) {
        if (touchUpSound != null) {
            touchUpSound.play();
        }
    }

    @Override
    public void touchUpOutside(IInteractiveDrawable target, float worldX, float worldY) {
        if (touchUpSound != null) {
            touchUpSound.play();
        }
    }

    @Override
    public void touchTap(IInteractiveDrawable target, float worldX, float worldY) {
        //void
    }

    @Override
    public void touchMove(IInteractiveDrawable target, float worldX, float worldY) {
        //void
    }

}
