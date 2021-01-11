package base.visual;

import java.util.HashMap;
import java.util.Map;

// Sprite button.
public class InteractiveDrawable extends Drawable2D implements IInteractiveDrawable {

	private base.interaction.ITouchListener touchListener;

    private boolean isInputInteractive = false;
    private boolean isTouchFallthroughEnabled = false;
    private boolean isInputFrozen = false;
    private boolean pressed = false;

    private Map<String, Object> userDataMap = null;

    public InteractiveDrawable() {
        super();

        //void
    }

    @Override
    public boolean getIsInputInteractive() {
        return isInputInteractive;
    }

    @Override
    public void setIsInputInteractive(boolean isInputInteractive) {
        this.isInputInteractive = isInputInteractive;
    }

	@Override
	public boolean getIsTouchFallthroughEnabled() {
		return isTouchFallthroughEnabled;
	}

	@Override
	public void setIsTouchFallthroughEnabled(boolean isTouchFallthroughEnabled) {
		this.isTouchFallthroughEnabled = isTouchFallthroughEnabled;
	}

	@Override
	public boolean getIsInputFrozen() {
		return isInputFrozen;
	}

	@Override
	public base.interaction.ITouchListener getTouchListener() {
		return touchListener;
	}

	@Override
	public void setTouchListener(base.interaction.ITouchListener touchListener) {
        if(this.touchListener != null && touchListener != null) {
            // Not allowed to change the touch listener directly. This is needed to avoid bugs.

            throw new Error("Error. 1.");
        }

		this.touchListener = touchListener;

		if(this.getParent() != null) {
			this.getParent().clearIfTouchDownTarget(this);
		}
	}

	@Override
	public void setVisible(boolean visible) {
		super.setVisible(visible);

		if(!visible) {
			if(this.getParent() != null) {
				this.getParent().clearIfTouchDownTarget(this);
			}
		}
	}

	public boolean getPressed() {
		return pressed;
	}

	@Override
    public void setPressed(boolean pressed) {
        this.pressed = pressed;
    }

	@Override
	public Map<String, Object> getUserDataMap() {
		if(userDataMap == null) {
			userDataMap = new HashMap<String, Object>();
		}

		return userDataMap;
	}

    public void putUserData(String key, Object value) {
        getUserDataMap().put(key, value);
    }

    public Object getUserData(String key) {
        return getUserDataMap().get(key);
    }

    @Override
    public Object getPropertyValue(String property) {
        if(property.equals("isTouchFallthroughEnabled")) {
            return getIsTouchFallthroughEnabled();
        }

        return super.getPropertyValue(property);
    }

    @Override
    public void setPropertyValue(String property, Object value) {
        if(property.equals("isTouchFallthroughEnabled")) {
            setIsTouchFallthroughEnabled((Boolean)value);
        }else {
            super.setPropertyValue(property, value);
        }
    }

}
