package base.fx.util;

import base.visual.util.VisualValueConfig;

public class PropertyAnimationConfig {

    public String propertyID;

    // If start composed baseValue is null, offset mode is used.
    public VisualValueConfig startComposedValue;
    public VisualValueConfig endComposedValue;

    public float animationStartDelayS;

    public float animationDurationS;
    public String easeFunctionID;
    public Object easeFunctionConfig;

    public PropertyAnimationConfig(String propertyID, VisualValueConfig startComposedValue, VisualValueConfig endComposedValue, float animationStartDelayS, float animationDurationS, String easeFunctionID, Object easeFunctionConfig) {
        super();

        this.propertyID = propertyID;

        this.startComposedValue = startComposedValue;
        this.endComposedValue = endComposedValue;

        this.animationStartDelayS = animationStartDelayS;

        this.animationDurationS = animationDurationS;
        this.easeFunctionID = easeFunctionID;
        this.easeFunctionConfig = easeFunctionConfig;
    }

}
