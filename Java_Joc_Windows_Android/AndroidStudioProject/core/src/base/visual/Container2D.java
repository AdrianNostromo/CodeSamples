package base.visual;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.g2d.Batch;
import com.badlogic.gdx.math.Rectangle;

import java.util.Vector;

// Visuals container. It can contain sprites and other containers. This class is used for visuals organizing.
public class Container2D extends DrawableNode implements IContainer2D {

	// Visuals group variables.
    protected Vector<IDrawableNode> visuals;

    private boolean isInputInteractive = false;

	// Touch interaction variables.
	//asd_002;// Use a vector for this variable because of IInteractiveDrawable.isTouchFallthroughEnabled can cause multiple visuals to be valid touch down targets.
	protected static IInteractiveDrawable touchDownTarget = null;
	// This object will receive move events even if they are not over it.
	//asd_002;// Use a vector for this variable because of IInteractiveDrawable.isTouchFallthroughEnabled can cause multiple visuals to be valid touch down targets.
	protected static IInteractiveDrawable touchDownHookingTarget = null;

    public Container2D() {
        super();

        visuals = new Vector<IDrawableNode>();
    }

    public void addChild(IDrawableNode visual) {
	    if(visual == this) {
		    Gdx.app.debug("Error", "Adding a visual to its self is not valid.");

		    return ;
	    }
	    if(visual.getParent() == this) {
		    Gdx.app.debug("Error", "Visual is already a child of the current group.");

		    return ;
	    }

	    if(visual.getParent() != null) {
		    visual.getParent().removeChild((visual));
	    }

	    addChildAt(visual, visuals.size());
    }

	public int getChildIndex(IDrawableNode visual) {
		return visuals.indexOf(visual);
	}

	public void addChildAt(IDrawableNode visual, int childIndex) {
		if(visual == this) {
			Gdx.app.debug("Error", "Adding a visual to its self is not valid.");

			return ;
		}
		if(visual.getParent() == this) {
			Gdx.app.debug("Error", "Visual is already a child of the current group.");

			return ;
		}

		if(visual.getParent() != null) {
			visual.getParent().removeChild((visual));
		}

		visual.setParent(this);

		visuals.add(childIndex, visual);
	}

    public void removeChild(IDrawableNode visual) {
	    if(visual.getParent() != this) {
		    Gdx.app.debug("Debug.Error", "Visual is not a child of the current group.");

		    return ;
	    }

	    clearIfTouchDownTarget(visual);

        visual.setParent(null);

        if(visuals.remove(visuals.indexOf(visual)) == null) {
            Gdx.app.debug("Debug.Error", "Visual not found on the specified parent.");

	        return ;
        }
    }

	@Override
	public void clearIfTouchDownTarget(IDrawableNode visual) {
		if(visual == touchDownHookingTarget) {
			touchDownHookingTarget = null;
		}
		if(visual == touchDownTarget) {
			touchDownTarget.setPressed(false);

			touchDownTarget = null;
		}
	}

	public boolean worldTouchEvent(int eventType, float worldX, float worldY, int pointer, int button) {
		for(int i=visuals.size() - 1;i>=0;i--) {
			Object entry = visuals.get(i);
			if(entry != null) {
				if(entry instanceof IContainer2D) {
					if(((IContainer2D)entry).getVisible() && ((IContainer2D)entry).getIsInputInteractive()) {
						if(((IContainer2D) entry).worldTouchEvent(eventType, worldX, worldY, pointer, button)) {
							return true;
						}
					}
				}
				if(entry instanceof IInteractiveDrawable) {
					if(((IInteractiveDrawable)entry).getVisible() && ((IInteractiveDrawable)entry).getIsInputInteractive()) {
						Rectangle bounds = ((IInteractiveDrawable) entry).getDrawBounds();

						if(worldX >= bounds.x && worldY >= bounds.y && worldX < bounds.x + bounds.width && worldY < bounds.y + bounds.height) {
							if (!((IInteractiveDrawable)entry).getIsInputFrozen()) {
								base.interaction.ITouchListener selfTouchListener = null;
								if(entry instanceof base.interaction.ITouchListener) {
									selfTouchListener = (base.interaction.ITouchListener)entry;
								}
								base.interaction.ITouchListener touchListener = ((IInteractiveDrawable) entry).getTouchListener();

								if(eventType == base.interaction.TouchEventType.TOUCH_DOWN) {
									if (touchListener != null && touchListener.touchDown((IInteractiveDrawable)entry, worldX, worldY)) {
										touchDownHookingTarget = (IInteractiveDrawable) entry;
									}
									if(selfTouchListener != null) {
										selfTouchListener.touchDown((IInteractiveDrawable)entry, worldX, worldY);
									}

									touchDownTarget = (IInteractiveDrawable) entry;
									touchDownTarget.setPressed(true);
								}else if(eventType == base.interaction.TouchEventType.TOUCH_UP) {
									if(touchListener != null) {
										touchListener.touchUp((IInteractiveDrawable)entry, worldX, worldY);
									}
									if(selfTouchListener != null) {
										selfTouchListener.touchUp((IInteractiveDrawable)entry, worldX, worldY);
									}

									if(touchDownTarget == entry) {
										touchDownTarget.setPressed(false);
										touchDownTarget = null;

										if(touchListener != null) {
											touchListener.touchTap((IInteractiveDrawable)entry, worldX, worldY);
										}
										if(selfTouchListener != null) {
											selfTouchListener.touchTap((IInteractiveDrawable)entry, worldX, worldY);
										}
									}
								}else if(eventType == base.interaction.TouchEventType.TOUCH_MOVE) {
									// The target object will receive move event in another place.
									if(touchListener != null && touchDownHookingTarget != entry) {
										touchListener.touchMove((IInteractiveDrawable)entry, worldX, worldY);
									}
									if(selfTouchListener != null) {
										selfTouchListener.touchMove((IInteractiveDrawable)entry, worldX, worldY);
									}
								}else {
									Gdx.app.debug("Error", "SpriteStage.touchDown. Unhandled touch event type.");

									return false;
								}

								if(!((IInteractiveDrawable)entry).getIsTouchFallthroughEnabled()) {
									return true;
								}
							}
						}
					}
				}
			}
		}

		return false;
	}

    @Override
    public boolean getIsInputInteractive() {
        return isInputInteractive;
    }

    @Override
    public void setIsInputInteractive(boolean isInputInteractive) {
        this.isInputInteractive = isInputInteractive;
    }

    @Override
	protected void drawVisual(Batch batch, OrthographicCamera camera, float deltaS) {
		super.drawVisual(batch, camera, deltaS);

		for(int i=0;i< visuals.size();i++) {
			IDrawableNode entry = visuals.get(i);

			if(entry.getVisible()) {
				entry.batchDraw(batch, camera, deltaS);
			}
		}
	}

	@Override
	public void syncDrawSpace(boolean syncX, boolean syncY, boolean syncScale, boolean syncAlpha, boolean syncTintColor) {
		super.syncDrawSpace(syncX, syncY, syncScale, syncAlpha, syncTintColor);

		if(getParent() != null) {
			for (int i=0;i<visuals.size();i++) {
				IDrawableNode entry = visuals.get(i);
				if (entry != null) {
					entry.syncDrawSpace(syncX || syncScale, syncY || syncScale, syncScale, syncAlpha, syncTintColor);
				}
			}
		}
	}

	@Override
	public void dispose() {
		//void
	}

}
