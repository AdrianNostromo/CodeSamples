package base.fx.util;

public class ActivePropertyAnimation {

    public PropertyAnimationConfig config;

    public float lastCompletionPercent = 0;
    // Used only for offset method.
    public float lastSyncValue = 0;

    public ActivePropertyAnimation(PropertyAnimationConfig config) {
        super();

        this.config = config;
    }

}
