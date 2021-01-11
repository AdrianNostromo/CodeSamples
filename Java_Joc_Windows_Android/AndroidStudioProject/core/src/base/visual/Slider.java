package base.visual;

import java.util.Map;

import base.interaction.SliderEventListener;
import base.interaction.TouchListener;
import base.menu.AnimatedComposedStiledVisual;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_Visual;
import base.menu.util.StateChangeDurations;
import local.app.AppLocal;
import base.util.AppStyles;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;
import base.visual.util.VisualValueConfig;

/**
 * Created by Adrin on 6/4/2016.
 */
public class Slider extends AnimatedComposedStiledVisual implements IDynamicPropertiesObject {

    protected SliderEventListener eventListener = null;

    protected float barWidth = 0f;
    protected float movementSpeedMultiplierS = 10f;
    protected float knobDiameter_idle = 0f;
    protected float knobDiameter_active = 0f;

    protected float activeStateChangeAnimationDurationS = 0.1f;

    protected IDrawableNode layerBase = null;
    protected IInteractiveDrawable bg_bar_fill = null;
    protected IInteractiveDrawable knob = null;
    protected IInteractiveDrawable touchAreaBack = null;

    private float value = 0f;
    private float visualUpdateValue = 0f;

    private boolean isInitialSync = false;

    private boolean isTouchSlideActive = false;
    // Animates from iddle(0f) to touch_down(1f) so the knob grows in rendering3DSize when touch slide is in progress.
    private float slideFocusStatePercent = 0f;

    private TouchListener touchListener = new TouchListener() {
        @Override
        public boolean touchDown(IInteractiveDrawable target, float worldX, float worldY) {
            if(target == touchAreaBack) {
                isTouchSlideActive = true;

                // Compute and set baseValue.
                float newValue = computeSliderValueAtTouchWorldPos(worldX, worldY);

                if(newValue != value) {
                    setValue(newValue, false, true);
                }

                return true;
            }else {
                app.errorHandler.handleAppCriticalError("ServerHandler.1.", "ServerHandler.1.");

                return false;
            }
        }

        @Override
        public void touchMove(IInteractiveDrawable target, float worldX, float worldY) {
            super.touchMove(target, worldX, worldY);

            if(target == touchAreaBack && isTouchSlideActive) {
                // Compute and set baseValue.
                float newValue = computeSliderValueAtTouchWorldPos(worldX, worldY);

                if(newValue != value) {
                    setValue(newValue, false, true);
                }
            }
        }

        @Override
        public void touchUp(IInteractiveDrawable target, float worldX, float worldY) {
            super.touchUp(target, worldX, worldY);

            if(target == touchAreaBack && isTouchSlideActive) {
                isTouchSlideActive = false;
            }
        }

        @Override
        public void touchUpOutside(IInteractiveDrawable target, float worldX, float worldY) {
            super.touchUpOutside(target, worldX, worldY);

            if(target == touchAreaBack && isTouchSlideActive) {
                isTouchSlideActive = false;
            }
        }
    };

    private static final MenuItemConfig[] itemConfigs = new MenuItemConfig[] {
            new MenuItemConfig_Visual("layerBase", new String[] {"_local", "parent", "layerBase"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    .put("x", 0f)
                    .put("y", 0f)
            ),

            new MenuItemConfig_Visual("touchAreaBack", new String[] {"_local", "layerBase"}, VisualCreateConfig.newTouchAreaAudible(null, null), new VisualPropertiesConfig()
                    .put("width",
                            1f, new String[] {"_local"}, "barWidth",
                            VisualValueConfig.OPERATION_ADD,
                            10f, new String[] {"_dpm"}, "width"
                    )
                    .put("height", 46f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.5f)
                    .put("x", -5f, new String[] {"_dpm"}, "width")
                    .put("y", 0f)
            ),
            new MenuItemConfig_Visual("bg_bar_base", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/simple_grey_B9B9B9"), new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local"}, "barWidth")
                    .put("height", 2f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.5f)
                    .put("x", 0f)
                    .put("y", 0f)
            ),
            new MenuItemConfig_Visual("bg_bar_fill", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/simple_blue_2196F3"), new VisualPropertiesConfig()
                    // width is dynamicly handled.
                    //.put("width", 1f, new String[] {"_local"}, "barWidth")
                    .put("height", 2f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.5f)
                    .put("x", 0f)
                    .put("y", 0f)
            ),

            new MenuItemConfig_Visual("knob", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taGame.atlas", "general/slider/knob"), new VisualPropertiesConfig()
                    // Size is dinamic.
                    //.put("width", 12f, new String[] {"_dpm"}, "width")
                    //.put("height", 12f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    // x is dinamicly handled.
                    //.put("x", 0f)
                    .put("y", 0f)
            )
    };

    private static final StateChangeDurations[] animationDurations = new StateChangeDurations[] {
            new StateChangeDurations("_in_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_out_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_in_", "_out_", AppStyles.ANIMATION_DURATION_S_DEFAULT)
    };

    public Slider(AppLocal app) {
        super(app, itemConfigs, animationDurations);

        //void
    }

    public void setEventListener(SliderEventListener eventListener) {
        this.eventListener = eventListener;
    }

    @Override
    public void create(Map<String, Object> createConfigMap) {
        super.create(createConfigMap);

        //voi
    }

    @Override
    protected void createVisuals(Map<String, Object> createConfigMap) {
        super.createVisuals(createConfigMap);

        layerBase = (IDrawableNode)getItem(new String[] {"_root", "layerBase"});
        bg_bar_fill = (IInteractiveDrawable)getItem(new String[] {"_root", "bg_bar_fill"});
        knob = (base.visual.IInteractiveDrawable)getItem(new String[] {"_root", "knob"});
        touchAreaBack = (base.visual.IInteractiveDrawable)getItem(new String[] {"_root", "touchAreaBack"});

        touchAreaBack.setTouchListener(touchListener);
    }

    @Override
    protected void syncVisuals(boolean syncAbsoluteValues, boolean syncAnimatingValues, String startStateID, String endStateID, float stateCompletionPercent) {
        super.syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

        if(!isInitialSync) {
            isInitialSync = true;

            syncSliderVisuals();
        }
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        boolean doSync = false;

        if((!isTouchSlideActive && slideFocusStatePercent > 0f)
                || isTouchSlideActive && slideFocusStatePercent < 1f)
        {
            if(!isTouchSlideActive) {
                slideFocusStatePercent = Math.max(0f, slideFocusStatePercent - deltaS / activeStateChangeAnimationDurationS);
            }else {
                slideFocusStatePercent = Math.min(1f, slideFocusStatePercent + deltaS / activeStateChangeAnimationDurationS);
            }

            doSync = true;
        }

        if(visualUpdateValue != value) {
            visualUpdateValue += (value - visualUpdateValue) * (movementSpeedMultiplierS * deltaS);

            doSync = true;
        }

        if(doSync) {
            syncSliderVisuals();
        }
    }

    private void syncSliderVisuals() {
        float newX = barWidth * visualUpdateValue;

        knob.setX(newX);

        bg_bar_fill.setWidth(barWidth * visualUpdateValue);

        float newKnobDiameter = knobDiameter_idle + (knobDiameter_active - knobDiameter_idle) * slideFocusStatePercent;
        knob.setSize(newKnobDiameter, newKnobDiameter);
    }

    @Override
    public Object getPropertyValue(String property) {
        if(property.equals("x")) {
            return visualsHolder.getX();
        }else if(property.equals("y")) {
            return visualsHolder.getY();
        }

        else if(property.equals("barWidth")) {
            return barWidth;
        }else if(property.equals("movementSpeedMultiplierS")) {
            return movementSpeedMultiplierS;
        }else if(property.equals("knobDiameter_idle")) {
            return knobDiameter_idle;
        }else if(property.equals("knobDiameter_active")) {
            return knobDiameter_active;
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

        else if(property.equals("barWidth")) {
            barWidth = (Float)value;
        }else if(property.equals("movementSpeedMultiplierS")) {
            movementSpeedMultiplierS = (Float)value;
        }else if(property.equals("knobDiameter_idle")) {
            knobDiameter_idle = (Float)value;
        }else if(property.equals("knobDiameter_active")) {
            knobDiameter_active = (Float)value;
        }else {
            throw new Error("PathFollowerNodeConfig_Pause.Error.1.");
        }
    }

    public float getValue() {
        return value;
    }

    public void setValue(float value, boolean doAnimation, boolean dispatchEvent) {
        // Update even if the same baseValue because the extra parameters need to be processed.
        /*if(this.baseValue == baseValue) {
            return ;
        }*/

        this.value = value;

        if(!doAnimation) {
            visualUpdateValue = this.value;

            syncSliderVisuals();
        }

        if(dispatchEvent) {
            if(eventListener != null) {
                eventListener.valueChanged(this);
            }
        }
    }

    private float computeSliderValueAtTouchWorldPos(float worldX, float worldY) {
        float val = (worldX - layerBase.getGlobalX()) / barWidth;
        val = Math.max(0f, Math.min(1f, val));

        return val;
    }

    @Override
    public void dispose() {
        //void

        super.dispose();
    }

}
