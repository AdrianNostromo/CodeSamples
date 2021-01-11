package base.view.listmenucardpopup.util;

/**
 * Created by Adrin on 8/30/2016.
 */
public class ListMenuCardPopupConfig {

    public float posXDip;
    public float posYDip;

    public float widthDip;

    public boolean isFullScreenDimmer;

    public int bgColor;

    public ListMenuCardPopupConfig(float posXDip, float posYDip, float widthDip, boolean isFullScreenDimmer, int bgColor) {
        super();

        this.posXDip = posXDip;
        this.posYDip = posYDip;

        this.widthDip = widthDip;

        this.isFullScreenDimmer = isFullScreenDimmer;

        this.bgColor = bgColor;
    }

}
