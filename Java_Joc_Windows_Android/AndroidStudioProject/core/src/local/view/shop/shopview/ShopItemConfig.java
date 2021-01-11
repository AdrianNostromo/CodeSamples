package local.view.shop.shopview;

import local.gameworld.util.PlatformStyleConfig;
import local.gameworld.util.BallStyleConfig;

public class ShopItemConfig {

    public PlatformStyleConfig platformStyle;
    public BallStyleConfig ballStyle;

    public ShopItemConfig(PlatformStyleConfig platformStyle, BallStyleConfig ballStyle) {
        super();

        this.platformStyle = platformStyle;
        this.ballStyle = ballStyle;
    }

}
