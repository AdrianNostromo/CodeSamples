package base.visual;

import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.Batch;
import com.badlogic.gdx.graphics.g2d.TextureAtlas;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.math.Rectangle;

// Sprites are used to render images on the screen.
public class Sprite2D extends InteractiveDrawable implements IRotatableDrawable {

	private TextureAtlas.AtlasRegion atlasRegion = null;
	private Texture texture = null;
	private TextureRegion textureRegion = null;

    private float rotationD = 0f;

	public Sprite2D(TextureAtlas.AtlasRegion atlasRegion) {
        super();

        this.atlasRegion = atlasRegion;
    }

	public Sprite2D() {
        super();

        //void
    }

	public Sprite2D(Texture texture) {
        super();

        this.texture = texture;
    }

	public Sprite2D(TextureRegion textureRegion) {
        super();

        this.textureRegion = textureRegion;
    }

    public void overrideTexture(Texture texture) {
        this.texture = texture;
    }

    public void overrideTextureRegion(TextureRegion textureRegion) {
        this.textureRegion = textureRegion;
    }

    public void overrideAtlasRegion(TextureAtlas.AtlasRegion atlasRegion) {
        this.atlasRegion = atlasRegion;
    }

    public float getRotationD() {
        return rotationD;
    }

    public void setRotationD(float rotationD) {
        this.rotationD = rotationD;
    }

	@Override
	protected void drawVisual(Batch batch, OrthographicCamera camera, float deltaS) {
		super.drawVisual(batch, camera, deltaS);

		Rectangle drawBounds = getDrawBounds();

        if(drawBounds.width == 0 || drawBounds.height == 0) {
            // The sprite has a zero width or height, don't draw it.
            return ;
        }

        if(atlasRegion != null) {
            if(rotationD != 0f) {
                // asd_00x;// the rendering3DSize multiplications with the anchors may be a bug, needs checking and testing.
                batch.draw(atlasRegion, drawBounds.x, drawBounds.y, drawBounds.width * getAnchorX(), drawBounds.height * getAnchorY(), drawBounds.width, drawBounds.height, 1, 1, rotationD);
            }else {
                batch.draw(atlasRegion, drawBounds.x, drawBounds.y, drawBounds.width, drawBounds.height);
            }
        }else if(texture != null){
            if(rotationD != 0f) {
                // asd_00x;// the rendering3DSize multiplications with the anchors may be a bug, needs checking and testing.
                batch.draw(texture, drawBounds.x, drawBounds.y, drawBounds.width * getAnchorX(), drawBounds.height * getAnchorY(), drawBounds.width, drawBounds.height, 1, 1, rotationD, 0, 0, texture.getWidth(), texture.getHeight(), false, false);
            }else {
                batch.draw(texture, drawBounds.x, drawBounds.y, drawBounds.width, drawBounds.height);
            }
        }else if(textureRegion != null){
            if(rotationD != 0f) {
                // asd_00x;// the rendering3DSize multiplications with the anchors may be a bug, needs checking and testing.
                batch.draw(textureRegion, drawBounds.x, drawBounds.y, drawBounds.width * getAnchorX(), drawBounds.height * getAnchorY(), drawBounds.width, drawBounds.height, 1, 1, rotationD);
            }else {
                batch.draw(textureRegion, drawBounds.x, drawBounds.y, drawBounds.width, drawBounds.height);
            }
        }else {
            // No drawing occurs.

            //void
        }
	}

    @Override
    public Object getPropertyValue(String property) {
        if(property.equals("rotationD")) {
            return getRotationD();
        }

        return super.getPropertyValue(property);
    }

    @Override
    public void setPropertyValue(String property, Object value) {
        if(property.equals("rotationD")) {
            setRotationD((Float)value);
        }else {
            super.setPropertyValue(property, value);
        }
    }

}
