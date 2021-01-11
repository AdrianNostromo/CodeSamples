package base.visual;

// Interface for a visuals container.
public interface IContainer2D extends IDrawableNode {

	void addChild(IDrawableNode visual);
	int getChildIndex(IDrawableNode visual);
    void removeChild(IDrawableNode visual);

	void clearIfTouchDownTarget(IDrawableNode visual);

	boolean worldTouchEvent(int eventType, float worldX, float worldY, int pointer, int button);

    boolean getIsInputInteractive();
    void setIsInputInteractive(boolean isInputInteractive);

}
