package base.view;

import java.util.Vector;

import base.IBaseAppComponent;
import local.app.AppLocal;
import base.menu.AnimatedComposedStiledVisual;
import base.menu.util.IMenuAnimationListener;
import base.menu.util.MenuItemConfig;
import base.menu.util.StateChangeDurations;

/**
 * Created by Adrin on 9/13/2016.
 */
public abstract class AppView extends AnimatedComposedStiledVisual implements IAppView {
    
    protected static final String VFLAG_in = "_in_";
    protected static final String VFLAG_out = "_out_";
    
    protected Vector<IBaseAppComponent> childComponentsList = new Vector<IBaseAppComponent>();

    // This variable is used to make sure the app loop is called. This is used to avoid some programming errors.
    private boolean isAnyAppLoopCalled = false;
    
    public AppView(AppLocal app, MenuItemConfig[] menuItemConfigs, StateChangeDurations[] easeDurationsSList) {
        super(app, menuItemConfigs, easeDurationsSList);

        //void
    }

    asdqq;// all shows have a ViewShowDataBase viewData that can be extended as needed;
    protected void show(boolean doAnimation, IMenuAnimationListener menuAnimationListener) {
        if(isActive()) {
            throw new Error("Error. 1.");
        }

        String stateID = computeInState();
        if(!doAnimation || !endStateID.equals(stateID)) {
            setShowState(computeStateFlags("_out_toIn_"), stateID, doAnimation, menuAnimationListener, false);
        }
    }
    
    public void hide(boolean doAnimation, IMenuAnimationListener menuAnimationListener) {
        if(!isActive()) {
            throw new Error("Error. 1.");
        }

        gotoShowState(computeStateFlags("_out_"), doAnimation, menuAnimationListener, false);
    
        if(!isAnyAppLoopCalled) {
            throw new Error("Error. 1.");
        }
    }

    public void pushChildComponent(IBaseAppComponent appComponent) {
        if(childComponentsList.indexOf(appComponent) >= 0) {
            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        childComponentsList.add(appComponent);
    }

    public void disposeChildComponent(IBaseAppComponent appComponent) {
        if(!childComponentsList.remove(appComponent)) {
            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        appComponent.dispose();
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);
    
        isAnyAppLoopCalled = true;
        
        if(childComponentsList != null) {
            for (int i = childComponentsList.size() - 1; i >= 0; i--) {
                IBaseAppComponent entry = childComponentsList.get(i);
                if (entry != null) {
                    entry.appLoop(deltaS, deltaMS);
                }
            }
        }
    }

    @Override
    public void dispose() {
        if(childComponentsList != null) {
            for (int i = childComponentsList.size() - 1; i >= 0; i--) {
                IBaseAppComponent entry = childComponentsList.get(i);
                if (entry != null) {
                    entry.dispose();
                }
            }

            childComponentsList = null;
        }

        super.dispose();
    }
    
    @Override
    protected void activeStateChangedEvent() {
        super.activeStateChangedEvent();
        
        if(!isActive()) {
            //void
        }else {
            syncData(false);
        }
    }
    
    protected void syncData(boolean doAnimation) {
        //void
    }
    
}
