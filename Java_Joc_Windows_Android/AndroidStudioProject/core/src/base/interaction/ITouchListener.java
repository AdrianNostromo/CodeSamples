package base.interaction;

import base.visual.IInteractiveDrawable;

public interface ITouchListener {

    // return true : it will receive the next touch up event anywhere it occurs.
    boolean touchDown(IInteractiveDrawable target, float worldX, float worldY);

    void touchUp(IInteractiveDrawable target, float worldX, float worldY);

    void touchUpOutside(IInteractiveDrawable target, float worldX, float worldY);

    void touchTap(IInteractiveDrawable target, float worldX, float worldY);

    void touchMove(IInteractiveDrawable target, float worldX, float worldY);

}
