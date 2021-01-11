package base.visual;

import java.util.Map;

import base.audio.util.DynamicSoundItem;
import local.audio.util.SoundConfig;
import base.interaction.ToggleControlEventListener;
import base.interaction.TouchListener;
import base.menu.AnimatedComposedStiledVisual;
import base.menu.util.MenuItemConfig;
import base.menu.util.StateChangeDurations;
import local.app.AppLocal;

/**
 * Created by Adrin on 6/4/2016.
 */
public abstract class ToggleControl extends AnimatedComposedStiledVisual implements IDynamicPropertiesObject {

    private DynamicSoundItem toggleOffDSound = null;
    private DynamicSoundItem toggleOnDSound = null;

    private ToggleControlEventListener eventListener = null;

    private IInteractiveDrawable touchAreaBack = null;

    private boolean isOn = false;

    private boolean isEnabled = true;

    private TouchListener touchListener = new TouchListener() {
        @Override
        public boolean touchDown(IInteractiveDrawable target, float worldX, float worldY) {
            if(!isEnabled) {
                return false;
            }

            if(target == touchAreaBack) {
                setIsOn(!getIsOn(), true, true, true, true);
            }else {
                app.errorHandler.handleAppCriticalError("ServerHandler.1.", "ServerHandler.1.");

                return false;
            }

            return false;
        }
    };

    public ToggleControl(AppLocal app, MenuItemConfig[] menuItemConfigs, StateChangeDurations[] easeDurationsSList) {
        super(app, menuItemConfigs, easeDurationsSList);

        //void
    }

    public void setEventListener(ToggleControlEventListener eventListener) {
        this.eventListener = eventListener;
    }

    @Override
    public void create(Map<String, Object> createConfigMap) {
        super.create(createConfigMap);

        toggleOffDSound = new DynamicSoundItem(app);
        toggleOnDSound = new DynamicSoundItem(app);
    }

    @Override
    protected void createVisuals(Map<String, Object> createConfigMap) {
        super.createVisuals(createConfigMap);

        touchAreaBack = (IInteractiveDrawable)getItem(new String[] {"_root", "touchAreaBack"});

        touchAreaBack.setTouchListener(touchListener);
    }

    @Override
    public Object getPropertyValue(String property) {
        if(property.equals("x")) {
            return visualsHolder.getX();
        }else if(property.equals("y")) {
            return visualsHolder.getY();
        }

        else if(property.equals("toggleOffSoundConfig")) {
            return toggleOffDSound.getSoundConfig();
        }else if(property.equals("toggleOnSoundConfig")) {
            return toggleOnDSound.getSoundConfig();
        }

        throw new Error("PathFollowerNodeConfig_Pause.Error.1.");
    }

    @Override
    public void setPropertyValue(String property, Object value) {
        if(property.equals("x")) {
            visualsHolder.setX((Float)value);
        }else if(property.equals("y")) {
            visualsHolder.setY((Float)value);
        }

        else if(property.equals("toggleOffSoundConfig")) {
            toggleOffDSound.putSoundConfig((SoundConfig) value);
        }else if(property.equals("toggleOnSoundConfig")) {
            toggleOnDSound.putSoundConfig((SoundConfig) value);
        }

        else {
            throw new Error("PathFollowerNodeConfig_Pause.Error.1.");
        }
    }

    public boolean getIsOn() {
        return isOn;
    }

    @Override
    protected String computeStateFlags(String baseState) {
        String state = super.computeStateFlags(baseState);

        if(isEnabled) {
            state += "enabled_";
        }else {
            state += "disabled_";
        }

        if(isOn) {
            state += "on_";
        }else {
            state += "off_";
        }

        return state;
    }

    public void setIsOn(boolean isOn, boolean doUpdate, boolean doAnimation, boolean playSound, boolean dispatchEvent) {
        if(this.isOn == isOn) {
            return ;
        }

        this.isOn = isOn;

        if(doUpdate) {
            updateShowStateIfChanged(doAnimation, null);
        }

        if(dispatchEvent) {
            if(eventListener != null) {
                eventListener.selectedStateChanged(this);
            }
        }

        if(playSound) {
            if(!isOn) {
                // Play off sound.

                toggleOffDSound.play();
            }else {
                // Play on sound.

                toggleOnDSound.play();
            }
        }
    }

    public void setIsEnabled(boolean isEnabled, boolean doUpdate, boolean doAnimation) {
        if(this.isEnabled == isEnabled) {
            return ;
        }

        this.isEnabled = isEnabled;

        if(doUpdate) {
            updateShowStateIfChanged(doAnimation, null);
        }
    }

    @Override
    public void dispose() {
        //void

        super.dispose();
    }

}
