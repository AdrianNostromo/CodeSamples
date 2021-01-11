package base.visual;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.g2d.Batch;
import com.badlogic.gdx.graphics.g2d.TextureAtlas;
import com.badlogic.gdx.math.Rectangle;

import java.util.Map;

import base.audio.util.SoundItem;

// Sprite button.
public class SpriteButton extends ReactiveInteractiveDrawable {

    /*Valid states:
    up
    down -> up
    disabled -> up
    selected_up -> up
    selected_down -> selected_up -> down -> up
    selected_disabled -> disabled -> selected_up -> up
     */
    private Map<String, TextureAtlas.AtlasRegion> stateVisuals;

	private SoundItem disabledTouchDownSound;

	private TextureAtlas.AtlasRegion activeStateRegion = null;

	private boolean selected = false;
	private boolean enabled = true;

	private float rotationD = 0f;

    public SpriteButton(SoundItem touchDownSound, SoundItem touchUpSound, SoundItem disabledTouchDownSound, Map<String, TextureAtlas.AtlasRegion> stateVisuals) {
        super(touchDownSound, touchUpSound);

        this.stateVisuals = stateVisuals;

	    this.disabledTouchDownSound = disabledTouchDownSound;

        setIsInputInteractive(true);
    }

	public float getRotationD() {
		return rotationD;
	}

	public void setRotationD(float rotationD) {
		this.rotationD = rotationD;
	}

	@Override
	public void setPressed(boolean pressed) {
		super.setPressed(pressed);

		updateActiveRegion();
	}

	public boolean getSelected() {
		return selected;
	}

	public void setSelected(boolean selected) {
		if(this.selected == selected) {
			return ;
		}

		this.selected = selected;

		updateActiveRegion();
	}

	public boolean getEnabled() {
		return enabled;
	}

	public void setEnabled(boolean enabled) {
		if(this.enabled == enabled) {
			return ;
		}

		this.enabled = enabled;

		updateActiveRegion();
	}

	@Override
	public void setParent(base.visual.IContainer2D parent) {
		super.setParent(parent);

		if(parent != null) {
			updateActiveRegion();
		}
	}

	@Override
	protected void drawVisual(Batch batch, OrthographicCamera camera, float deltaS) {
		super.drawVisual(batch, camera, deltaS);

		if(activeStateRegion == null) {
			updateActiveRegion();
		}

		Rectangle drawBounds = getDrawBounds();
		if(rotationD != 0f) {
			// asd_00x;// the rendering3DSize multiplications with the anchors may be a bug, needs checking and testing.
			batch.draw(activeStateRegion, drawBounds.x, drawBounds.y, drawBounds.width * getAnchorX(), drawBounds.height * getAnchorY(), drawBounds.width, drawBounds.height, 1, 1, rotationD);
		}else {
			batch.draw(activeStateRegion, drawBounds.x, drawBounds.y, drawBounds.width, drawBounds.height);
		}
    }

	private void updateActiveRegion() {
		activeStateRegion = null;

		String[] targetStates;

		if(!selected) {
			if(!enabled) {
				// disabled

				targetStates = new String[] {"disabled", "up"};
			}else {
				if(!getPressed()) {
					// up

					targetStates = new String[] {"up"};
				}else {
					// down

					targetStates = new String[] {"down", "up"};
				}
			}
		}else {
			if(!enabled) {
				// selected_disabled

				targetStates = new String[] {"selected_disabled", "disabled", "selected_up", "up"};
			}else {
				if(!getPressed()) {
					// selected_up

					targetStates = new String[] {"selected_up", "up"};
				}else {
					// selected_down

					targetStates = new String[] {"selected_down", "selected_up", "down", "up"};
				}
			}
		}

		activeStateRegion = getFirstStateVisual(targetStates);
		if(activeStateRegion == null) {
			Gdx.app.debug("CDU", "Unable to find valid button state visual. StateID: " + targetStates[0]);

			return ;
		}
	}

	private TextureAtlas.AtlasRegion getFirstStateVisual(String[] targetStates) {
        for(int i=0;i<targetStates.length;i++) {
            String stateID = targetStates[i];

	        if(stateID != null) {
		        TextureAtlas.AtlasRegion region = stateVisuals.get(stateID);
		        if (region != null) {

			        return region;
		        }
	        }
        }

        return null;
    }

	@Override
	protected void playTouchDownSound() {
		if(enabled) {
			super.playTouchDownSound();
		}else {
			if(disabledTouchDownSound != null) {
				disabledTouchDownSound.play();
			}
		}
	}

	@Override
	protected void playTouchUpSound() {
		if(enabled) {
			super.playTouchUpSound();
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
