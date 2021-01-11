package base.fx.util;

public class PropertyValuePair {

    public String property;

    public float value;

    public static final String POSITION_X = "x";
    public static final String POSITION_Y = "y";
    public static final String SIZE_WIDTH = "width";
    public static final String SIZE_HEIGHT = "height";
    public static final String ALPHA = "alpha";
    public static final String SCALE_X = "scaleX";
    public static final String SCALE_Y = "scaleY";

    public PropertyValuePair(String property, float value) {
        super();

        this.property = property;

        this.value = value;
    }

}
