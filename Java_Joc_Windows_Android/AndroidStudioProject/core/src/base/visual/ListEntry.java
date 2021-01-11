package base.visual;

import java.util.HashMap;
import java.util.Map;

import base.menu.AnimatedComposedStiledVisual;
import base.menu.util.IMenuAnimationListener;
import base.menu.util.MenuItemConfig;
import base.menu.util.StateChangeDurations;
import local.app.AppLocal;

/**
 * Created by Adrin on 6/4/2016.
 */
public class ListEntry extends AnimatedComposedStiledVisual {

    public List list;

    private Map<String, Object> userDataMap = null;

    public ListEntry(AppLocal app, MenuItemConfig[] menuItemConfigs, StateChangeDurations[] easeDurationsSList, List list) {
        super(app, menuItemConfigs, easeDurationsSList);

        this.list = list;
    }

    public Map<String, Object> getUserDataMap() {
        if(userDataMap == null) {
            userDataMap = new HashMap<String, Object>();
        }

        return userDataMap;
    }

    @Override
    public void setShowState(String startStateID, String endStateID, boolean doAnimation, IMenuAnimationListener menuAnimationListener, boolean restartAnimation) {
        super.setShowState(startStateID, endStateID, doAnimation, menuAnimationListener, restartAnimation);

        //void
    }

    @Override
    public void gotoShowState(String endStateID, boolean doAnimation, IMenuAnimationListener menuAnimationListener, boolean restartAnimation) {
        super.gotoShowState(endStateID, doAnimation, menuAnimationListener, restartAnimation);

        //void
    }

    @Override
    public String computeStateFlags(String baseState) {
        return super.computeStateFlags(baseState);
    }
}
