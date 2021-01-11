package base.ease;

/**
 * Created by Adrin on 6/23/2016.
 */
public class EaseFunctionCubicBezier {

    public static float easeCubicBezier (float startX, float startY, float endX, float endY, float time) {
        float cY = 3 * startY;
        float bY = 3 * (endY - startY) - cY;
        float aY = 1 - cY - bY;
        return time * (cY + time * (bY + time * aY));
    }

}
