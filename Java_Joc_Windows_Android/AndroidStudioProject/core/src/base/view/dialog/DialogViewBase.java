package base.view.dialog;

import java.util.HashMap;
import java.util.Map;

import base.IBaseAppComponent;
import base.menu.AnimatedComposedStiledVisual;
import base.menu.util.IMenuAnimationListener;
import base.menu.util.MenuItemConfig;
import base.menu.util.StateChangeDurations;
import local.app.AppLocal;
import base.view.dialog.util.DialogConfig;
import base.view.dialog.util.IDialogEventListener;

public class DialogViewBase extends AnimatedComposedStiledVisual implements IBaseAppComponent {

    private boolean isTitleExists = false;
    private boolean isBodyExists = false;
    private boolean isControlsExists = false;

    protected DialogConfig dialogConfig = null;
    protected IDialogEventListener dialogEventListener = null;

    // dataMap entries:
    // dynamic: int width
    // dynamic: int titleAreaHeight
    // dynamic: int bodyAreaHeight
    // dynamic: int controlsAreaHeight

    private Map<String, Object> userDataMap = null;

    public DialogViewBase(AppLocal app, MenuItemConfig[] menuItemConfigs, StateChangeDurations[] menuAnimationDurations) {
        super(app, menuItemConfigs, menuAnimationDurations);

        //void
    }

    public Map<String, Object> getUserDataMap() {
        if(userDataMap == null) {
            userDataMap = new HashMap<String, Object>();
        }

        return userDataMap;
    }

    public void show(boolean doAnimation, IMenuAnimationListener menuAnimationListener, DialogConfig dialogConfig, IDialogEventListener dialogEventListener) {
        if(isActive()) {
            // View is already active.

            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        this.dialogConfig = dialogConfig;
        this.dialogEventListener = dialogEventListener;

        initializeDialogAreas();

        String stateID = computeInState();
        if(!doAnimation || !endStateID.equals(stateID)) {
            setShowState(computeStateFlags("_out_toIn_"), stateID, doAnimation, menuAnimationListener, false);
        }
    }

    protected void initializeDialogAreas() {
        getDataMap().put("width", this.dialogConfig.widthDPM * app.screenDensityMultiplier.x);
    }

    @Override
    protected void createVisuals(Map<String, Object> createConfigMap) {
        getDataMap().put("width", 0f);
        getDataMap().put("titleAreaHeight", 0f);
        getDataMap().put("bodyAreaHeight", 0f);
        getDataMap().put("controlsAreaHeight", 0f);

        super.createVisuals(createConfigMap);
    }

    @Override
    protected String computeStateFlags(String baseState) {
        String state = super.computeStateFlags(baseState);

        if(this.getIsTitleExists()) {
            state += "isTitleExists_";
        }

        if(this.getIsBodyExists()) {
            state += "isBodyExists_";
        }

        if(this.getIsControlsExists()) {
            state += "isControlsExists_";
        }

        return state;
    }

    public void hide(boolean doAnimation, IMenuAnimationListener menuAnimationListener) {
        gotoShowState(computeStateFlags("_out_"), doAnimation, menuAnimationListener, false);
    }

    protected final boolean getIsTitleExists() {
        return isTitleExists;
    }

    protected final void putIsTitleExists(boolean isTitleExists, boolean updateShowState) {
        if(this.isTitleExists == isTitleExists) {
            return ;
        }

        this.isTitleExists = isTitleExists;

        eventChangeIsTitleExists();

        if(isActive() && updateShowState) {
            updateShowStateIfChanged(true, null);
        }
    }

    protected void eventChangeIsTitleExists() {
        //void
    }

    protected final boolean getIsBodyExists() {
        return isBodyExists;
    }

    protected final void putIsBodyExists(boolean isBodyExists, boolean updateShowState) {
        if(this.isBodyExists == isBodyExists) {
            return ;
        }

        this.isBodyExists = isBodyExists;

        eventChangeIsBodyExists();

        if(isActive() && updateShowState) {
            updateShowStateIfChanged(true, null);
        }
    }

    protected void eventChangeIsBodyExists() {
        //void
    }

    protected final boolean getIsControlsExists() {
        return isControlsExists;
    }

    protected final void putIsControlsExists(boolean isControlsExists, boolean updateShowState) {
        if(this.isControlsExists == isControlsExists) {
            return ;
        }

        this.isControlsExists = isControlsExists;

        eventChangeIsControlsExists();

        if(isActive() && updateShowState) {
            updateShowStateIfChanged(true, null);
        }
    }

    protected void eventChangeIsControlsExists() {
        //void
    }

}
