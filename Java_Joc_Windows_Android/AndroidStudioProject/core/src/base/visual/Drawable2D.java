package base.visual;

import com.badlogic.gdx.math.Rectangle;

// Sprites are used to render images on the screen.
public class Drawable2D extends DrawableNode implements IDrawable2D {

	private float localWidth = 0f;
	private float localHeight = 0f;

	private float globalWidth = 0f;
	private float globalHeight = 0f;

	private float anchorX = 0f;
	private float anchorY = 0f;

	private boolean isDirty_width = true;
	private boolean isDirty_height = true;

	// This contains the global position and rendering3DSize combined with the anchor.
	private Rectangle drawBounds;

	public Drawable2D() {
        super();

		drawBounds = new Rectangle(0, 0, 0, 0);
    }

	@Override
	public void setParent(IContainer2D parent) {
		if(parent != null) {
			isDirty_width = true;
			isDirty_height = true;
		}

		super.setParent(parent);
	}

	@Override
	public final void syncDrawSpace(boolean syncX, boolean syncY, boolean syncScale, boolean syncAlpha, boolean syncTintColor) {
		if(getParent() != null) {
			syncDrawSpace(syncX, syncY, syncScale, syncAlpha, syncTintColor, isDirty_width, isDirty_height);
		}
	}

	@Override
	public void syncDrawSpace(boolean syncX, boolean syncY, boolean syncScale, boolean syncAlpha, boolean syncTintColor, boolean syncWidth, boolean syncHeight) {
		super.syncDrawSpace(syncX, syncY, syncScale, syncAlpha, syncTintColor);

		if(getParent() != null) {
			if (syncWidth || syncScale) {
				globalWidth = computeGlobalWidth();
			}
			if (syncHeight || syncScale) {
				globalHeight = computeGlobalHeight();
			}
			if (syncX || syncY || syncWidth || syncHeight || syncScale || isDirty_width || isDirty_height) {
				computeDrawBounds(drawBounds, getGlobalX(), getGlobalY(), globalWidth, globalHeight, anchorX, anchorY);
			}
		}

		isDirty_width = false;
		isDirty_height = false;
	}

    protected float computeGlobalWidth() {
        return localWidth * getGlobalScaleX();
    }

    protected float computeGlobalHeight() {
        return localHeight * getGlobalScaleY();
    }

	protected void computeDrawBounds(Rectangle drawBounds, float globalX, float globalY, float globalWidth, float globalHeight, float anchorX, float anchorY) {
		drawBounds.width = globalWidth;
		drawBounds.height = globalHeight;

		drawBounds.x = globalX - globalWidth * anchorX;
		drawBounds.y = globalY - globalHeight * anchorY;
	}

	@Override
	public void setSize(float width, float height) {
		localWidth = width;
		localHeight = height;

		syncDrawSpace(false, false, false, false, false, true, true);
	}

	@Override
	public float getWidth() {
		return localWidth;
	}

	@Override
	public void setWidth(float width) {
		localWidth = width;

		syncDrawSpace(false, false, false, false, false, true, false);
	}

	@Override
	public float getHeight() {
		return localHeight;
	}

	@Override
	public void setHeight(float height) {
		localHeight = height;

		syncDrawSpace(false, false, false, false, false, false, true);
	}

	@Override
	public void setBounds(float x, float y, float width, float height) {
		localWidth = width;
		localHeight = height;

		isDirty_width = true;
		isDirty_height = true;

		// The rendering3DSize will be synced automatically.
		//syncDrawSpace(true, true, false, false, true, true);

		setPosition(x, y);
	}

	@Override
	public float getAnchorX() {
		return anchorX;
	}

	@Override
	public void setAnchorX(float anchorX) {
		this.anchorX = anchorX;

		computeDrawBounds(drawBounds, getGlobalX(), getGlobalY(), globalWidth, globalHeight, anchorX, anchorY);
	}

	@Override
	public float getAnchorY() {
		return anchorY;
	}

	@Override
	public void setAnchorY(float anchorY) {
		this.anchorY = anchorY;

		computeDrawBounds(drawBounds, getGlobalX(), getGlobalY(), globalWidth, globalHeight, anchorX, anchorY);
	}

	@Override
	public void setAnchor(float anchorX, float anchorY) {
		this.anchorX = anchorX;
		this.anchorY = anchorY;

		computeDrawBounds(drawBounds, getGlobalX(), getGlobalY(), globalWidth, globalHeight, anchorX, anchorY);
	}

	@Override
	public Rectangle getDrawBounds() {
		return drawBounds;
	}

    @Override
    public Object getPropertyValue(String property) {
        if(property.equals("width")) {
            return getWidth();
        }else if(property.equals("height")) {
            return getHeight();
        }else if(property.equals("anchorX")) {
            return getAnchorX();
        }else if(property.equals("anchorY")) {
            return getAnchorY();
        }

        return super.getPropertyValue(property);
    }

    @Override
    public void setPropertyValue(String property, Object value) {
        if(property.equals("width")) {
            setWidth((Float)value);
        }else if(property.equals("height")) {
            setHeight((Float)value);
        }else if(property.equals("anchorX")) {
            setAnchorX((Float)value);
        }else if(property.equals("anchorY")) {
            setAnchorY((Float)value);
        }else {
            super.setPropertyValue(property, value);
        }
    }

}
