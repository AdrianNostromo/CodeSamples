package base.visual;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Color;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.g2d.Batch;
import com.badlogic.gdx.math.Rectangle;

import java.util.HashMap;
import java.util.Map;

import base.util.Point2D;

// Sprites are used to render images on the screen.
public class DrawableNode implements IDrawableNode {

	private IContainer2D parent = null;

	// Variables that affect the drawing of the child visuals.
	private boolean visible = true;

	// Local draw offsets.
	private float localX = 0f;
	private float localY = 0f;
	private float localScaleX = 1f;
	private float localScaleY = 1f;
	private float localAlpha = 1f;
    private Color localTintColor = null;

	// These variables contain the combined the parent and local values.
	private float globalX = 0f;
	private float globalY = 0f;
	private float globalScaleX = 1f;
	private float globalScaleY = 1f;
	private float globalAlpha = 1f;
    private Color globalTintColor = null;

	private Rectangle maskRectangle = null;

    private Map<String, Boolean> staticAnimatingPropertiesMap = new HashMap<String, Boolean>();

	public DrawableNode() {
        super();

		//void
    }

	@Override
	public final void batchDraw(Batch batch, OrthographicCamera camera, float deltaS) {
		if(!visible) {
			return ;
		}

		if(maskRectangle != null) {
			float scaleX = Gdx.graphics.getWidth() / camera.viewportWidth;
			float scaleY = Gdx.graphics.getHeight() / camera.viewportHeight;

			int maskX = (int)Math.ceil(maskRectangle.x * scaleX);
			int maskY = (int)Math.ceil(maskRectangle.y * scaleY);
			int maskWidth = (int)Math.floor(maskRectangle.width * scaleX);
			int maskHeight = (int)Math.floor(maskRectangle.height * scaleY);

			batch.flush();

			Gdx.gl.glEnable(GL20.GL_SCISSOR_TEST);
			Gdx.gl.glScissor(maskX, maskY, maskWidth, maskHeight);
		}

		if(globalTintColor != null) {
			batch.setColor(globalTintColor.r, globalTintColor.g, globalTintColor.b, globalTintColor.a * globalAlpha);
		}else if(globalAlpha < 1f) {
			batch.setColor(1f, 1f, 1f, 1f * globalAlpha);
		}

		drawVisual(batch, camera, deltaS);

		if(globalTintColor != null) {
			batch.setColor(1f, 1f, 1f, 1f);
		}else if(globalAlpha < 1f) {
			batch.setColor(1f, 1f, 1f, 1f);
		}

		if(maskRectangle != null) {
			batch.flush();

			Gdx.gl.glDisable(GL20.GL_SCISSOR_TEST);
		}
	}

	public Rectangle getMaskRectangle() {
		return maskRectangle;
	}

	public void setMaskRectangle(Rectangle maskRectangle) {
		this.maskRectangle = maskRectangle;
	}

	// Extendable.
	protected void drawVisual(Batch batch, OrthographicCamera camera, float deltaS) {
		//void
	}

	@Override
	public boolean getVisible() {
		return visible;
	}

	@Override
	public void setVisible(boolean visible) {
		this.visible = visible;
	}

	@Override
	public base.visual.IContainer2D getParent() {
		return parent;
	}

	@Override
	public void setParent(base.visual.IContainer2D parent) {
		this.parent = parent;

		if(this.parent != null) {
			// Compute the visual draw bound in local space.
			syncDrawSpace(true, true, true, true, true);
		}
	}

	@Override
	public void setPosition(float x, float y) {
		this.localX = x;
		this.localY = y;

		syncDrawSpace(true, true, false, false, false);
	}

	@Override
	public float getX() {
		return localX;
	}

	@Override
	public void setX(float x) {
		this.localX = x;

		syncDrawSpace(true, false, false, false, false);
	}

	@Override
	public float getY() {
		return localY;
	}

	@Override
	public void setY(float y) {
		this.localY = y;

		syncDrawSpace(false, true, false, false, false);
	}

    @Override
    public Point2D getScale() {
        return new Point2D(localScaleX, localScaleY);
    }

    @Override
    public void setScale(float scaleX, float scaleY) {
        localScaleX = scaleX;
        localScaleY = scaleY;

        syncDrawSpace(false, false, true, false, false);
    }

    @Override
    public float getScaleX() {
        return localScaleX;
    }

    @Override
    public void setScaleX(float scaleX) {
        localScaleX = scaleX;

        syncDrawSpace(false, false, true, false, false);
    }

    @Override
    public float getScaleY() {
        return localScaleY;
    }

    @Override
    public void setScaleY(float scaleY) {
        localScaleY = scaleY;

        syncDrawSpace(false, false, true, false, false);
    }

	@Override
	public float getAlpha() {
		return localAlpha;
	}

	@Override
	public void setAlpha(float alpha) {
		this.localAlpha = alpha;

		syncDrawSpace(false, false, false, true, false);
	}

	public Color getTintColor() {
		return localTintColor;
	}

	public void setTintColor(Color tintColor) {
		this.localTintColor = tintColor;

        syncDrawSpace(false, false, false, false, true);
	}

    @Override
    public Object getPropertyValue(String property) {
        if(property.equals("x")) {
            return getX();
        }else if(property.equals("y")) {
            return getY();
        }else if(property.equals("scaleX")) {
            return getScaleX();
        }else if(property.equals("scaleY")) {
            return getScaleY();
        }else if(property.equals("alpha")) {
            return getAlpha();
        }else if(property.equals("visible")) {
            return getVisible();
        }else if(property.equals("tintColor")) {
			return getTintColor();
		}

        Gdx.app.debug("Error", "Unknown visual property to get with fxId: " + property);

        return 0;
    }

    @Override
    public void setPropertyValue(String property, Object value) {
        if(property.equals("x")) {
            setX((Float)value);
        }else if(property.equals("y")) {
            setY((Float)value);
        }else if(property.equals("scaleX")) {
            setScaleX((Float)value);
        }else if(property.equals("scaleY")) {
            setScaleY((Float)value);
        }else if(property.equals("alpha")) {
            setAlpha((Float)value);
        }else if(property.equals("visible")) {
            setVisible((Boolean)value);
        }else if(property.equals("_visibleFloat")) {
			if((Float)value == 0) {
				setVisible(false);
			}else {
				setVisible(true);
			}
        }else if(property.equals("tintColor")) {
            if(!(value instanceof Color)) {
                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }

			setTintColor((Color)value);
		}else {
            Gdx.app.debug("Error", "Unknown visual property to get with fxId: " + property);
        }
    }

    @Override
	public float getGlobalX() {
		return globalX;
	}

	@Override
	public float getGlobalY() {
		return globalY;
	}

	@Override
	public float getGlobalScaleX() {
		return globalScaleX;
	}

	@Override
	public float getGlobalScaleY() {
		return globalScaleY;
	}

	@Override
	public float getGlobalAlpha() {
		return globalAlpha;
	}

    @Override
    public Color getGlobalTintColor() {
        return globalTintColor;
    }

    @Override
	public void syncDrawSpace(boolean syncX, boolean syncY, boolean syncScale, boolean syncAlpha, boolean syncTintColor) {
		if(parent != null) {
			if (syncScale) {
				globalScaleX = parent.getGlobalScaleX() * localScaleX;
				globalScaleY = parent.getGlobalScaleY() * localScaleY;
			}
			if (syncX) {
				globalX = parent.getGlobalX() + localX * parent.getGlobalScaleX();
			}
			if (syncY) {
				globalY = parent.getGlobalY() + localY * parent.getGlobalScaleY();
			}
			if (syncAlpha) {
				globalAlpha = parent.getGlobalAlpha() * localAlpha;
			}
			if (syncTintColor) {
                if(parent.getGlobalTintColor() != null && localTintColor != null) {
                    // Currently tint colors cannot be combined.

                    throw new Error("Error occurred.");
                }

                if(parent.getGlobalTintColor() != null) {
                    globalTintColor = parent.getGlobalTintColor();
                }else {
                    globalTintColor = localTintColor;
                }
			}
		}
	}

    public Boolean getStaticAnimatingPropertyStatus(String property) {
        return staticAnimatingPropertiesMap.get(property);
    }

    public void putStaticAnimatingPropertyStatus(String property, Boolean status) {
        staticAnimatingPropertiesMap.put(property, status);
    }

	@Override
	public void dispose() {
		//void
	}

}
