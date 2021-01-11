package base.visual;

import com.badlogic.gdx.InputProcessor;
import com.badlogic.gdx.math.Vector3;

import local.app.AppLocal;

// The root visuals container. This class handles touch events.
public class SpriteStage extends Container2D implements InputProcessor {

    private AppLocal app;

	private Container2D rootParent;

    private Vector3 cache_touchDown = new Vector3();
    private Vector3 cache_touchUp = new Vector3();
    private Vector3 cache_touchDragged = new Vector3();

    private boolean synchChecker_touchDown = false;
    private boolean synchChecker_touchUp = false;
    private boolean synchChecker_touchDragged = false;

    public SpriteStage(AppLocal app) {
        super();

        this.app = app;
    }

	public void create() {
		rootParent = VisualUtil.newGroup(null);

		setParent(rootParent);
	}

    @Override
    public boolean keyDown(int keycode) {
        //void

        return false;
    }

    @Override
    public boolean keyUp(int keycode) {
        //void

        return false;
    }

    @Override
    public boolean keyTyped(char character) {
        //void

        return false;
    }

    @Override
    public boolean touchDown(int screenX, int screenY, int pointer, int button) {
        if(synchChecker_touchDown) {
            throw new Error("Error.");
        }
        synchChecker_touchDown = true;

        if(!getVisible()) {
            synchChecker_touchDown = false;
            return false;
        }

        cache_touchDown.set(screenX, screenY, 0);
        app.camera2D.unproject(cache_touchDown);

        if(worldTouchEvent(base.interaction.TouchEventType.TOUCH_DOWN, cache_touchDown.x, cache_touchDown.y, pointer, button)) {
            synchChecker_touchDown = false;
            return true;
        }

        synchChecker_touchDown = false;
        return false;
    }

    @Override
    public boolean touchUp(int screenX, int screenY, int pointer, int button) {
        if(synchChecker_touchUp) {
            throw new Error("Error.");
        }
        synchChecker_touchUp = true;

	    boolean ret = false;

        cache_touchUp.set(screenX, screenY, 0);
        app.camera2D.unproject(cache_touchUp);

        if(worldTouchEvent(base.interaction.TouchEventType.TOUCH_UP, cache_touchUp.x, cache_touchUp.y, pointer, button)) {
            ret = true;
        }

        if(touchDownTarget != null) {
            if(touchDownTarget.getTouchListener() != null) {
                touchDownTarget.getTouchListener().touchUpOutside(touchDownTarget, cache_touchUp.x, cache_touchUp.y);
            }
	        if(touchDownTarget instanceof base.interaction.ITouchListener) {
		        ((base.interaction.ITouchListener)touchDownTarget).touchUpOutside(touchDownTarget, cache_touchUp.x, cache_touchUp.y);

	        }

            touchDownTarget.setPressed(false);
            touchDownTarget = null;
        }

        touchDownHookingTarget = null;

        synchChecker_touchUp = false;
        return ret;
    }

    @Override
    public boolean touchDragged(int screenX, int screenY, int pointer) {
        if(synchChecker_touchDragged) {
            throw new Error("Error.");
        }
        synchChecker_touchDragged = true;

        if(!getVisible()) {
            synchChecker_touchDragged = false;
            return false;
        }

        cache_touchDragged.set(screenX, screenY, 0);
        app.camera2D.unproject(cache_touchDragged);
    
        boolean ret = false;
        if(worldTouchEvent(base.interaction.TouchEventType.TOUCH_MOVE, cache_touchDragged.x, cache_touchDragged.y, pointer, 0)) {
            // No need to cancel
            ret = true;
        }

        if(touchDownHookingTarget != null) {
            base.interaction.ITouchListener touchListener = touchDownHookingTarget.getTouchListener();

            if(touchListener != null) {
                touchDownHookingTarget.getTouchListener().touchMove(touchDownHookingTarget, cache_touchDragged.x, cache_touchDragged.y);
            }
	        if(touchDownHookingTarget instanceof base.interaction.ITouchListener) {
		        ((base.interaction.ITouchListener)touchDownHookingTarget).touchMove(touchDownHookingTarget, cache_touchDragged.x, cache_touchDragged.y);
	        }
        }

        synchChecker_touchDragged = false;
        return ret;
    }

    @Override
    public boolean mouseMoved(int screenX, int screenY) {
        //void

        return false;
    }

    @Override
    public boolean scrolled(int amount) {
        //void

        return false;
    }

	@Override
	public void setVisible(boolean visible) {
		// Visibility is locked for the root sprites container.

		//void
	}
}
