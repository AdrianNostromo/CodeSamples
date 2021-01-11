package base.interaction;

import base.visual.IInteractiveDrawable;

// Listeners for touch event extend this class.
public abstract class TouchListener implements ITouchListener {

    public TouchListener() {
        super();

        //void
    }

    // return true : it will receive the next touch up event anywhere it occurs.
    public boolean touchDown(IInteractiveDrawable target, float worldX, float worldY) {
	    //void

        return false;
    }

    public void touchUp(IInteractiveDrawable target, float worldX, float worldY) {
	    //void
    }

    public void touchUpOutside(IInteractiveDrawable target, float worldX, float worldY) {
	    //void
    }

    public void touchTap(IInteractiveDrawable target, float worldX, float worldY) {
        //void
    }

    public void touchMove(IInteractiveDrawable target, float worldX, float worldY) {
        //void
    }
}
