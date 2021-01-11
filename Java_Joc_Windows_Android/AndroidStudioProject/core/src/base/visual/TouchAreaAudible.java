package base.visual;

import base.audio.util.SoundItem;

// A area used to get touch event from a 2d screen area.
public class TouchAreaAudible extends ReactiveInteractiveDrawable {

	public TouchAreaAudible(SoundItem touchDownSound, SoundItem touchUpSound) {
        super(touchDownSound, touchUpSound);

        setIsInputInteractive(true);
    }

    //asd_001;// implement a debug drawable that uses app.debugMode flags.

}
