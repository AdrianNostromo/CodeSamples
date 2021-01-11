package base.ease;

import com.badlogic.gdx.Gdx;

import base.ease.pennereasing.EaseFunctionBack;
import base.ease.pennereasing.EaseFunctionElastic;
import base.ease.pennereasing.EaseFunctionLinear;
import base.ease.pennereasing.EaseFunctionQuart;

public class EaseFunctions {

    public static final String linear = "linear";
    public static final String easeOutCubic = "easeOutCubic";
    public static final String easeOutElastic = "easeOutElastic";
    public static final String easeInQuart = "easeInQuart";

    public static float computeEaseValue(String easeFunctionName, float completionPercent) {
        return computeEaseValue(easeFunctionName, null, completionPercent);
    }

    public static float computeEaseValue(String easeFunctionName, Object easeFunctionConfig, float completionPercent) {
        float animatedValue;

        if(easeFunctionName.equals("cubicBezier")) {
            CubicBezierConfig bezierConfig = (CubicBezierConfig)easeFunctionConfig;
            animatedValue = base.ease.EaseFunctionCubicBezier.easeCubicBezier(
                    bezierConfig.startX, bezierConfig.startY, bezierConfig.endX, bezierConfig.endY,
                    completionPercent
            );
        }else if(easeFunctionName.equals("linear")) {
            animatedValue = EaseFunctionLinear.easeNone(completionPercent, 0, 1, 1);
        }else if(easeFunctionName.equals("easeOutCubic")) {
            animatedValue = base.ease.pennereasing.EaseFunctionCubic.easeOut(completionPercent, 0, 1, 1);
        }else if(easeFunctionName.equals("easeOutElastic")) {
            animatedValue = EaseFunctionElastic.easeOut(completionPercent, 0, 1, 1);
        }else if(easeFunctionName.equals("easeInQuart")) {
            animatedValue = EaseFunctionQuart.easeIn(completionPercent, 0, 1, 1);
        }else if(easeFunctionName.equals("easeInBack")) {
            animatedValue = EaseFunctionBack.easeIn(completionPercent, 0, 1, 1);
        }else {
            Gdx.app.debug("Error", "Unhandled ease function: " + easeFunctionName);

            return 0;
        }

        return animatedValue;
    }

    public static float computeEaseValue(String easeFunctionName, Object easeFunctionConfig, float completionPercent, float startValue, float endValue) {
        float n = computeEaseValue(easeFunctionName, easeFunctionConfig, completionPercent);

        float val = startValue + (endValue - startValue) * n;

        return val;
    }

}
