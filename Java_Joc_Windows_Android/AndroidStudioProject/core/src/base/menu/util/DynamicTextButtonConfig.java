package base.menu.util;

/**
 * Created by Adrin on 8/20/2016.
 */
public class DynamicTextButtonConfig {

    public int id;

    public String text;
    public float rawTextWidthDPM;
    public int tintColor;
    public float alpha;

    public DynamicTextButtonConfig(int id, String text, float rawTextWidthDPM, int tintColor, float alpha) {
        super();

        this.id = id;

        this.text = text;
        this.rawTextWidthDPM = rawTextWidthDPM;
        this.tintColor = tintColor;
        this.alpha = alpha;
    }

}
