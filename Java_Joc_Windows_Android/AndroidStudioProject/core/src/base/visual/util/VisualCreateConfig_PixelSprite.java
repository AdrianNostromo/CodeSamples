package base.visual.util;

/**
 * Created by Adrin on 7/30/2016.
 */
public class VisualCreateConfig_PixelSprite extends VisualCreateConfig {

    public PixelVisualData pixelData;

    public VisualCreateConfig_PixelSprite(PixelVisualData pixelData) {
        super(TYPE_PIXEL_SPRITE);

        this.pixelData = pixelData;
    }

}
