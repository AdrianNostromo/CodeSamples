package base.visual;

import com.badlogic.gdx.math.Rectangle;

// Interface for visuals that can interact with touch events.
public interface IDrawable2D extends IDrawableNode {

	void setSize(float width, float height);
	float getWidth();
	void setWidth(float width);
	float getHeight();
	void setHeight(float height);

	void setBounds(float x, float y, float width, float height);

	float getAnchorX();
	void setAnchorX(float anchorX);
	float getAnchorY();
	void setAnchorY(float anchorY);
	void setAnchor(float anchorX, float anchorY);

	Rectangle getDrawBounds();

	void syncDrawSpace(boolean syncX, boolean syncY, boolean syncScale, boolean syncAlpha, boolean syncTintColor, boolean syncWidth, boolean syncHeight);

}
