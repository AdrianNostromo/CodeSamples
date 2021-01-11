package base.visual.util;

/**
 * Created by Adrin on 7/30/2016.
 */
public class VisualCreateConfig_Label extends VisualCreateConfig {

    // String - Font id.
    // FontInstances - Font config.
    public Object fontData;
    public String defaultText;

    public VisualCreateConfig_Label(Object fontData, String defaultText) {
        super(TYPE_LABEL);

        this.fontData = fontData;
        this.defaultText = defaultText;
    }

}
