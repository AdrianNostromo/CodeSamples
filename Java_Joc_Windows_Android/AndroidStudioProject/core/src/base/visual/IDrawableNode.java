package base.visual;

import com.badlogic.gdx.graphics.Color;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.g2d.Batch;

import base.util.Point2D;

// Interface for visual elements that are drawn on the screen.
public interface IDrawableNode extends IDynamicPropertiesObject {

    void batchDraw(Batch batch, OrthographicCamera camera, float deltaS);

	boolean getVisible();
	void setVisible(boolean visible);

    IContainer2D getParent();
    void setParent(IContainer2D parent);

	void setPosition(float x, float y);
	float getX();
	void setX(float x);
	float getY();
	void setY(float y);

	Point2D getScale();
	void setScale(float scaleX, float scaleY);

    float getScaleX();
    void setScaleX(float scaleX);

    float getScaleY();
    void setScaleY(float scaleY);

	float getAlpha();
	void setAlpha(float alpha);

	Color getTintColor();
	void setTintColor(Color tintColor);

	float getGlobalX();
	float getGlobalY();

	float getGlobalScaleX();
	float getGlobalScaleY();

	float getGlobalAlpha();

	Color getGlobalTintColor();

	void syncDrawSpace(boolean syncX, boolean syncY, boolean syncScale, boolean syncAlpha, boolean syncTintColor);

	void dispose();

}
