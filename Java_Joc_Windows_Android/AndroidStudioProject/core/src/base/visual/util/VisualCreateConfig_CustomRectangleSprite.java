package base.visual.util;

import base.util.Size2DInt;

/**
 * Created by Adrin on 7/30/2016.
 */
public class VisualCreateConfig_CustomRectangleSprite extends VisualCreateConfig {

    public Object color;
    public Size2DInt textureSize;

    public VisualCreateConfig_CustomRectangleSprite(Object color, Size2DInt textureSize) {
        super(TYPE_CUSTOM_RECTANGLE_SPRITE);

        this.color = color;
        this.textureSize = textureSize;
    }

}
