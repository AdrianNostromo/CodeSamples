package base.menu;

import com.badlogic.gdx.Gdx;

import java.util.Map;

import base.menu.util.IMenuAnimationListener;
import base.menu.util.IMenuItemConfig_DynamicProperties;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_SmartItem;
import base.menu.util.StateChangeDurations;
import local.profile.config.ProfileDataIDs;
import local.app.AppLocal;
import base.visual.IContainer2D;
import base.visual.IDynamicPropertiesObject;

// Each menu or panel extends this class it.
public abstract class AnimatedComposedStiledVisual extends ComposedStiledVisual {

    private StateChangeDurations[] easeDurationsSList;

	protected String startStateID = null;
    protected String endStateID = null;
    private float animationDurationS = 0f;
    private float stateCompletionPercent = 1f;

    protected IMenuAnimationListener menuAnimationListener;

    public AnimatedComposedStiledVisual(AppLocal app, MenuItemConfig[] menuItemConfigs, StateChangeDurations[] easeDurationsSList) {
        super(app, menuItemConfigs);

        this.easeDurationsSList = easeDurationsSList;
    }

    public boolean isActive() {
        if(endStateID.contains("_in_")) {
            return true;
        }

        return false;
    }

	public String getEndStateID() {
		return endStateID;
	}

    protected final float getStateCompletionPercent() {
        return stateCompletionPercent;
    }

	public boolean isAnimating() {
		if(stateCompletionPercent < 1) {
			return true;
		}

		return false;
	}

    public boolean isActiveOrAnimating() {
        return isActive() || isAnimating();
    }

    protected void activeStateChangedEvent() {
        //void
    }

    protected void gotoShowState(String endStateID, boolean doAnimation, IMenuAnimationListener menuAnimationListener, boolean restartAnimation) {
		setShowState(this.endStateID, endStateID, doAnimation, menuAnimationListener, restartAnimation);
	}

    private StateChangeDurations getFirstValidEaseDurationConfig(String startStateID, String endStateID) {
        for(int i=0;i<easeDurationsSList.length;i++) {
            StateChangeDurations valueEntry = easeDurationsSList[i];

            boolean isStartStateCheckPassed = checkIsStatePassed(startStateID, valueEntry.startComposedStateID);
            boolean isEndStateCheckPassed = checkIsStatePassed(endStateID, valueEntry.endComposedStateID);

            if(isStartStateCheckPassed && isEndStateCheckPassed) {
                return valueEntry;
            }
        }

        return null;
    }

    protected void setShowState(String startStateID, String endStateID, boolean doAnimation, IMenuAnimationListener menuAnimationListener, boolean restartAnimation) {
		this.menuAnimationListener = menuAnimationListener;

		if(!restartAnimation && (this.startStateID.equals(startStateID) && this.endStateID.equals(endStateID))) {
			return ;
		}

        boolean oldActiveState = isActive();

		this.startStateID = startStateID;
        this.endStateID = endStateID;

        StateChangeDurations easeDurationConfig = getFirstValidEaseDurationConfig(startStateID, endStateID);
        if(easeDurationConfig == null) {
            app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

            throw new Error("Error. 1.");
        }
        this.animationDurationS = easeDurationConfig.durationS;

		if(doAnimation) {
			stateCompletionPercent = 0;
		}else {
			stateCompletionPercent = 1;
		}

        if(oldActiveState != isActive()) {
            activeStateChangedEvent();
        }

        // Also update absolute values because some absolute values may use other absolute values that are manually modified when the menu state change is requested.
		updateStateData(true, true);

        if(stateCompletionPercent >= 1) {
            if(menuAnimationListener != null) {
                menuAnimationListener.animationFinished(this);

                menuAnimationListener = null;

                animationFinishedEvent();
            }
        }
	}

    protected boolean isVisible() {
        return visualsHolder.getVisible();
    }

    protected void isOnDisplayListChanged() {
        //void
    }

    @Override
    public void doFullVisualsSync() {
        //super.doFullVisualsSync();

        updateStateData(true, true);
    }

    @Override
    public void create(Map<String, Object> createConfigMap) {
        startStateID = computeStateFlags("_out_toIn_");
        endStateID = computeStateFlags("_out_toIn_");

        super.create(createConfigMap);
    }

    @Override
    protected void createVisuals(Map<String, Object> createConfigMap) {
        super.createVisuals(createConfigMap);

        visualsHolder = (IContainer2D)getItem(new String[]{"_root", "visualsHolder"});

        visualsHolder.setVisible(false);
    }

    private void updateStateData(boolean syncAbsoluteValues, boolean syncAnimatingValues) {
        if (isActive() && !visualsHolder.getVisible()) {
            visualsHolder.setVisible(true);

            isOnDisplayListChanged();
        } else if (!isActive() && stateCompletionPercent >= 1 && visualsHolder.getVisible()) {
            visualsHolder.setVisible(false);

            isOnDisplayListChanged();
        }

	    if (!visualsHolder.getVisible()) {
		    return;
	    }

	    syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);
    }

    public boolean isOnDisplayList() {
        if(!visualsHolder.getVisible()) {
            return false;
        }

        return true;
    }

	public String getBaseStateFromVariation(String stateID) {
		if(stateID != null) {
			if (stateID.contains("_in_")) {
				return "_in_";
			} else if (stateID.contains("_out_")) {
				return "_out_";
			}
		}

		Gdx.app.debug("Error", "Unhandled animated menu state fxId.");

		return null;
	}

    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        if(stateCompletionPercent < 1) {
            stateCompletionPercent = Math.min(1f, stateCompletionPercent + deltaS * (1f / animationDurationS));

            updateStateData(false, true);

            if(stateCompletionPercent >= 1) {
                if(menuAnimationListener != null) {
                    menuAnimationListener.animationFinished(this);

                    menuAnimationListener = null;
                }

                animationFinishedEvent();
            }
        }
    }

    protected void animationFinishedEvent() {
        if (isActive()) {
            String inState = computeInState();
            if(!getEndStateID().equals(inState)) {
                gotoShowState(inState, true, null, false);
            }
        }
    }

    protected final String computeInState() {
        return computeStateFlags("_in_");
    }

    // baseState: "_in_" / "_out_" / "_out_toIn_";
    protected String computeStateFlags(String baseState) {
        String state = baseState;

        if(!app.profileData.getDataWithID_boolean(ProfileDataIDs.IS_ADS_DISABLED)) {
            state = state + "adsOn_";
        }else {
            state = state + "adsOff_";
        }

        return state;
    }

    protected final void updateShowStateIfChanged(boolean doAnimation, IMenuAnimationListener menuAnimationListener) {
        if(!isActive() || isAnimating()) {
            return ;
        }

        String stateID = computeInState();
        if(!doAnimation || !endStateID.equals(stateID)) {
            gotoShowState(stateID, doAnimation, menuAnimationListener, false);
        }
    }

    @Override
    protected void visualItemCreated(Object item, MenuItemConfig itemConfig, IComposedVisual itemsMap, IComposedVisual itemsMapLayer) {
        // Local code include code from super method.
        // super.visualItemCreated(item, itemConfig, itemsMap, itemsMapLayer);

        if(item instanceof IDynamicPropertiesObject && itemConfig instanceof IMenuItemConfig_DynamicProperties) {
            syncVisualItem(
                    (IDynamicPropertiesObject)item, ((IMenuItemConfig_DynamicProperties) itemConfig).getItemProperties(),
                    true, true,
                    startStateID, endStateID,
                    0f,
                    app, itemsMap, itemsMapLayer,
                    getDataMap()
            );
        }

        if(item instanceof ComposedStiledVisual && item instanceof IDynamicPropertiesObject) {
            ((ComposedStiledVisual) item).createVisuals(createConfigMap);
        }

        if(item instanceof ComposedStiledVisual) {
            ((ComposedStiledVisual)item).doFullVisualsSync();
        }

        if(item instanceof AnimatedComposedStiledVisual && itemConfig instanceof MenuItemConfig_SmartItem) {
            MenuItemConfig_SmartItem cItemConfig = (MenuItemConfig_SmartItem)itemConfig;

            if(cItemConfig.initialBaseStateId != null) {
                String newState = ((AnimatedComposedStiledVisual)item).computeStateFlags(cItemConfig.initialBaseStateId);
                ((AnimatedComposedStiledVisual)item).gotoShowState(newState, false, null, false);
            }
        }
    }
}
