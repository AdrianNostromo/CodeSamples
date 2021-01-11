package base.view.dialog.util;

import base.menu.util.DynamicTextButtonConfig;
import base.view.DynamicTextItems;

/**
 * Created by Adrin on 6/23/2016.
 */
public class DialogConfig {

    public float widthDPM;

    public String titleText;
    public int titleTotalLinesCount;

    public DynamicTextItems dynamicBodyTextItem;

    public boolean isOneButtonControlPerLine;
    public DynamicTextButtonConfig[] dynamicButtonConfigs;

    public DialogConfig(float widthDPM, String titleText, int titleTotalLinesCount, DynamicTextItems dynamicBodyTextItem, boolean isOneButtonControlPerLine, DynamicTextButtonConfig[] dynamicButtonConfigs) {
        super();

        this.widthDPM = widthDPM;

        this.titleText = titleText;
        this.titleTotalLinesCount = titleTotalLinesCount;

        this.dynamicBodyTextItem = dynamicBodyTextItem;

        this.isOneButtonControlPerLine = isOneButtonControlPerLine;
        this.dynamicButtonConfigs = dynamicButtonConfigs;
    }

}
