package base.menu.util;

import base.ease.CubicBezierConfig;

/**
 * Created by Adrin on 8/29/2016.
 */
public class EaseConfig {
    
    /**
     * @deprecated Use linear instead;
     */
    public static final EaseConfig DEFAULT = new EaseConfig(
            "linear",
            null
    );

    public static final EaseConfig LINEAR = new EaseConfig(
            "linear",
            null
    );

    public static final EaseConfig ACCELERATION = new EaseConfig(
            "cubicBezier",
            new CubicBezierConfig(0.4f, 0.0f, 1f, 1f)
    );

    public static final EaseConfig DECELERATION = new EaseConfig(
            "cubicBezier",
            new CubicBezierConfig(0.0f, 0.0f, 0.2f, 1f)
    );

    public static final EaseConfig SHARP = new EaseConfig(
            "cubicBezier",
            new CubicBezierConfig(0.4f, 0.0f, 0.6f, 1f)
    );

    public String easeFunctionID;
    public Object easeFunctionConfig;

    public float startPercentOverride;
    public float endPercentOverride;

    public EaseConfig(String easeFunctionID, Object easeFunctionConfig, float startPercentOverride, float endPercentOverride) {
        super();

        this.easeFunctionID = easeFunctionID;
        this.easeFunctionConfig = easeFunctionConfig;

        this.startPercentOverride = startPercentOverride;
        this.endPercentOverride = endPercentOverride;
    }

    public EaseConfig(String easeFunctionID, Object easeFunctionConfig) {
        this(easeFunctionID, easeFunctionConfig, 0f, 1f);
    }

}
