package base.visual;

import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.g2d.Animation;
import com.badlogic.gdx.graphics.g2d.Batch;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.math.Rectangle;

// Movie clips are used to draw image animations on the screen.
public class MovieClip extends InteractiveDrawable implements IRotatableDrawable {

	private Animation<TextureRegion> animation;

	private float stateTime = 0f;
	private boolean isPlaying = true;
	public boolean pauseOnLoopRestart = false;

    private int keyFrameIndexOverride = -1;

	private float rotationD = 0f;

	public MovieClip(Animation<TextureRegion> animation) {
		super();

		this.animation = animation;
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

        if(isPlaying) {
            if(pauseOnLoopRestart && animation.getKeyFrameIndex(stateTime + deltaS) < animation.getKeyFrameIndex(stateTime)) {
                pauseOnLoopRestart = false;

                isPlaying = false;
                stateTime = 0;
            }

            stateTime += deltaS;
		}

        TextureRegion currentFrame;
        if(keyFrameIndexOverride >= 0) {
            // Redundant casting is required because the key frames are saved in a Object[], not a T[] as expected.
            // May be a bug with libgdx.
            currentFrame = (TextureRegion)((Object[])animation.getKeyFrames())[keyFrameIndexOverride];
        }else {
            currentFrame = animation.getKeyFrame(stateTime);
        }

		Rectangle drawBounds = getDrawBounds();
		if(rotationD != 0f) {
			// asd_00x;// the rendering3DSize multiplications with the anchors may be a bug, needs checking and testing.
			batch.draw(currentFrame, drawBounds.x, drawBounds.y, drawBounds.width * getAnchorX(), drawBounds.height * getAnchorY(), drawBounds.width, drawBounds.height, 1, 1, rotationD);
		}else {
			batch.draw(currentFrame, drawBounds.x, drawBounds.y, drawBounds.width, drawBounds.height);
		}

	}

	public void play(boolean restart) {
		isPlaying = true;
		if(restart) {
			stateTime = 0f;
		}

		pauseOnLoopRestart = false;
	}

	public void pause() {
		isPlaying = false;

		pauseOnLoopRestart = false;
	}

	public void stop() {
		isPlaying = false;
		stateTime = 0f;

		pauseOnLoopRestart = false;
	}

    public void setKeyFrameIndexOverride(int keyFrameIndexOverride) {
        this.keyFrameIndexOverride = keyFrameIndexOverride;
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
