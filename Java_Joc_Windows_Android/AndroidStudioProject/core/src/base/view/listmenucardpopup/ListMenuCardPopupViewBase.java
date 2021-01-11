package base.view.listmenucardpopup;

import com.badlogic.gdx.Gdx;

import java.util.Map;

import base.IBaseAppComponent;
import base.menu.AnimatedComposedStiledVisual;
import base.menu.util.IMenuAnimationListener;
import base.menu.util.MenuItemConfig;
import base.menu.util.StateChangeDurations;
import local.app.AppLocal;
import base.view.listmenucardpopup.util.IListMenuCardPopupEventListener;
import base.view.listmenucardpopup.util.ListMenuCardPopupConfig;

public class ListMenuCardPopupViewBase extends AnimatedComposedStiledVisual implements IBaseAppComponent, IListMenuCardPopupView {

    protected ListMenuCardPopupConfig viewConfig = null;
    protected IListMenuCardPopupEventListener viewEventListener = null;

    private static int COMMAND_NONE = 0;
    private static int COMMAND_DISPOSE = 1;

    private int outCommandType = COMMAND_NONE;

    private IMenuAnimationListener localAnimationListener = new IMenuAnimationListener() {
        @Override
        public void animationFinished(Object target) {
            // Ignore show animation complete events.
            if (!isActive()) {
                if (outCommandType <= COMMAND_NONE) {
                    //void
                } else if (outCommandType <= COMMAND_DISPOSE) {
                    helperDisposeRequest();
                } else {
                    Gdx.app.debug("Error", "Unhandled command type.");
                }

                outCommandType = COMMAND_NONE;
            } else {
                //void
            }
        }
    };

    public ListMenuCardPopupViewBase(AppLocal app, MenuItemConfig[] menuItemConfigs, StateChangeDurations[] easeDurationsSList) {
        super(app, menuItemConfigs, easeDurationsSList);

        //void
    }

    @Override
    public void create(Map<String, Object> createConfigMap) {
        super.create(createConfigMap);

        //void
    }

    private void helperDisposeRequest() {
        app.disposeListMenuCardPopupView(this);
    }

    public void show(boolean doAnimation, IMenuAnimationListener menuAnimationListener, ListMenuCardPopupConfig viewConfig, IListMenuCardPopupEventListener viewEventListener) {
        if(viewEventListener == null) {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }
        if(endStateID.contains("_in_")) {
            // View is already active.

            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        this.viewConfig = viewConfig;
        this.viewEventListener = viewEventListener;

        String stateID = computeInState();
        if(!doAnimation || !endStateID.equals(stateID)) {
            setShowState(computeStateFlags("_out_toIn_"), stateID, doAnimation, menuAnimationListener, false);
        }
    }

    // hideAndDispose is publicly available only.
    private void hide(boolean doAnimation, IMenuAnimationListener menuAnimationListener) {
        gotoShowState(computeStateFlags("_out_"), doAnimation, menuAnimationListener, false);
    }

    @Override
    public void hideAndDispose(boolean doAnimation) {
        if(!isActive()) {
            Gdx.app.debug("Error", "AnimatedComposedStiledVisual.Error.7.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        outCommandType = COMMAND_DISPOSE;
        hide(true, localAnimationListener);
    }

    @Override
    protected void activeStateChangedEvent() {
        super.activeStateChangedEvent();

        if(!isActive()) {
            //void
        }else {
            //void
        }
    }

    @Override
    protected String computeStateFlags(String baseState) {
        String state = super.computeStateFlags(baseState);

        if(viewConfig != null && viewConfig.isFullScreenDimmer) {
            state += "isFullScreenDimmer_";
        }

        return state;
    }

    @Override
    public void dispose() {
        //void

        super.dispose();
    }

}
