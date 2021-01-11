package base.visual;

import java.util.Map;

import base.interaction.ITouchListener;

// Interface for visuals that can interact with touch events.
public interface IInteractiveDrawable extends IDrawable2D {

	boolean getIsInputInteractive();
    void setIsInputInteractive(boolean isInputInteractive);
    boolean getIsTouchFallthroughEnabled();
    void setIsTouchFallthroughEnabled(boolean isTouchFallthroughEnabled);
	boolean getIsInputFrozen();

	ITouchListener getTouchListener();
	void setTouchListener(ITouchListener touchListener);

    void setPressed(boolean pressed);

    Map<String, Object> getUserDataMap();
    void putUserData(String key, Object value);
    Object getUserData(String key);

}
