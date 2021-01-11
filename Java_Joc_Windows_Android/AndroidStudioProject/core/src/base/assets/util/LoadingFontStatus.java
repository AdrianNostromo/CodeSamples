package base.assets.util;

import base.font.config.FontConfig;

public class LoadingFontStatus {

    public FontConfig fontConfig;

    public int bakedFontSize;
    public String bakedBufferFontID;

    public LoadingFontStatus(FontConfig fontConfig, int bakedFontSize, String bakedBufferFontID) {
        super();

        this.fontConfig = fontConfig;
        this.bakedFontSize = bakedFontSize;
        this.bakedBufferFontID = bakedBufferFontID;
    }

}
