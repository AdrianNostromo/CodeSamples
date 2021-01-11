package base.visual;

import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.g2d.Batch;
import com.badlogic.gdx.graphics.g2d.BitmapFont;
import com.badlogic.gdx.graphics.g2d.GlyphLayout;
import com.badlogic.gdx.math.Matrix4;
import com.badlogic.gdx.math.Rectangle;
import com.badlogic.gdx.math.Vector3;
import com.badlogic.gdx.utils.Align;

// Labels are used to render bitmap font text on the screen.
public class BitmapLabel extends Drawable2D implements IRotatableDrawable {

    private BitmapFont font;

    private String text = "";

    GlyphLayout glyphLayout;

	private boolean wrap = false;
	private String truncate = null;

	private float rotationD = 0f;

	private Matrix4 oldBatchTransformMatrix = new Matrix4();
	private Matrix4 cachedTransformMatrix = null;
	private float cachedTransformMatrix_rotationD;
	private float cachedTransformMatrix_globalX;
	private float cachedTransformMatrix_globalY;
	private float cachedTransformMatrix_globalScaleX;
	private float cachedTransformMatrix_globalScaleY;

    public BitmapLabel(BitmapFont font) {
        super();

        this.font = font;

        glyphLayout = new GlyphLayout();
	    glyphLayout.setText(font, text);
    }

	public float getRotationD() {
		return rotationD;
	}

	public void setRotationD(float rotationD) {
		this.rotationD = rotationD;
	}

	@Override
	public void setScale(float scaleX, float scaleY) {
		// Disable scale for text labels.

		//void
	}

    @Override
    public void setScaleX(float scaleX) {
        // Disable scale for text labels.

        //void
    }

    @Override
    public void setScaleY(float scaleY) {
        // Disable scale for text labels.

        //void
    }

	public boolean getWrap() {
		return wrap;
	}

	public void setWrap(boolean wrap) {
		this.wrap = wrap;
	}

	public String getTruncate() {
		return truncate;
	}

	public void setTruncate(String truncate) {
		this.truncate = truncate;
	}

    public String getText() {
        return text;
    }

	public void setFont(BitmapFont font) {
		this.font = font;

		glyphLayout.setText(font, text);

		textSizeChanged();
	}

    public void setText(String text) {
        this.text = text;

        glyphLayout.setText(font, text);

	    textSizeChanged();
    }

	private void textSizeChanged() {
		setSize(glyphLayout.width, glyphLayout.height);
	}

    //asd_001001aa;// scale doesn't modify the bounds, it edits the draw transform only.

	@Override
	protected void drawVisual(Batch batch, OrthographicCamera camera, float deltaS) {
		super.drawVisual(batch, camera, deltaS);

		Rectangle drawBounds = getDrawBounds();

		float oldAlpha = font.getColor().a;
		if(getGlobalAlpha() != 1) {
			font.getColor().a = getGlobalAlpha();
		}

        boolean isTransformMatrixUsed = false;
		if(rotationD != 0f || getGlobalScaleX() != 1f || getGlobalScaleY() != 1f) {
			oldBatchTransformMatrix.set(batch.getTransformMatrix());

			boolean updateMatrix = false;
			if(cachedTransformMatrix == null) {
                cachedTransformMatrix = new Matrix4();

				updateMatrix = true;
			}
			if(!updateMatrix && (
					cachedTransformMatrix_rotationD != getRotationD()
					|| cachedTransformMatrix_globalX != getGlobalX()
					|| cachedTransformMatrix_globalY != getGlobalY()
					|| cachedTransformMatrix_globalScaleX != getGlobalScaleX()
					|| cachedTransformMatrix_globalScaleY != getGlobalScaleY()
                ))
			{
				updateMatrix = true;
			}

			if(updateMatrix) {
                cachedTransformMatrix.idt();

                cachedTransformMatrix.translate(getGlobalX(), getGlobalY(), 0f);
                cachedTransformMatrix.scale(getGlobalScaleX(), getGlobalScaleY(), 1f);
                cachedTransformMatrix.rotate(Vector3.Z, getRotationD());
                cachedTransformMatrix.translate(-getGlobalX(), -getGlobalY(), 0f);

                cachedTransformMatrix_rotationD = getRotationD();
                cachedTransformMatrix_globalX = getGlobalX();
                cachedTransformMatrix_globalY = getGlobalY();

                cachedTransformMatrix_globalScaleX = getGlobalScaleX();
                cachedTransformMatrix_globalScaleY = getGlobalScaleY();
			}

            isTransformMatrixUsed = true;
			batch.setTransformMatrix(cachedTransformMatrix);
		}

        if(getGlobalTintColor() != null) {
            font.setColor(getGlobalTintColor().r, getGlobalTintColor().g, getGlobalTintColor().b, getGlobalTintColor().a * getGlobalAlpha());
        }else if(getGlobalAlpha() < 1f) {
            font.setColor(1f, 1f, 1f, 1f * getGlobalAlpha());
        }

		if (wrap) {
			// Try to truncate with ellipsis.
			font.draw(batch, text, drawBounds.x, drawBounds.y, 0, text.length(), drawBounds.width, Align.left, getWrap(), null);
		} else if (getTruncate() != null) {
			font.draw(batch, text, drawBounds.x, drawBounds.y, 0, text.length(), drawBounds.width, Align.left, false, getTruncate());
		} else {
			font.draw(batch, text, drawBounds.x, drawBounds.y);
		}

        if(getGlobalTintColor() != null) {
            font.setColor(1f, 1f, 1f, 1f);
        }else if(getGlobalAlpha() < 1f) {
            font.setColor(1f, 1f, 1f, 1f);
        }

		if(isTransformMatrixUsed) {
			batch.setTransformMatrix(oldBatchTransformMatrix);
		}

		if(getGlobalAlpha() != 1) {
			font.getColor().a = oldAlpha;
		}
    }

    // Use custom logic because the bitmap text has it's origin on the top left and the libgdx renderer uses a bottom left origin.
	@Override
	protected void computeDrawBounds(Rectangle drawBounds, float globalX, float globalY, float globalWidth, float globalHeight, float anchorX, float anchorY) {
		drawBounds.width = globalWidth;
		drawBounds.height = globalHeight;

		drawBounds.x = globalX - globalWidth * anchorX;
		drawBounds.y = globalY + globalHeight - globalHeight * anchorY;
	}

    @Override
    protected float computeGlobalWidth() {
        //return super.computeGlobalWidth();

        // Bitmap labels don't scale the bounds. Scaling is a batch operation.
        return getWidth();
    }

    @Override
    protected float computeGlobalHeight() {
        //return super.computeGlobalHeight();

        // Bitmap labels don't scale the bounds. Scaling is a batch operation.
        return getHeight();
    }

    @Override
	public Object getPropertyValue(String property) {
		if(property.equals("wrap")) {
			return getWrap();
		}else if(property.equals("truncate")) {
			return getTruncate();
		}else if(property.equals("rotationD")) {
			return getRotationD();
		}

		return super.getPropertyValue(property);
	}

	@Override
	public void setPropertyValue(String property, Object value) {
		if(property.equals("wrap")) {
			setWrap((Boolean)value);
		}else if(property.equals("truncate")) {
			setTruncate((String) value);
		}else if(property.equals("rotationD")) {
			setRotationD((Float)value);
		}else {
			super.setPropertyValue(property, value);
		}
	}

}
