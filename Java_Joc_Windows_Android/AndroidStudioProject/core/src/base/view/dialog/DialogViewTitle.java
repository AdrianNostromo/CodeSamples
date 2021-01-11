package base.view.dialog;

import com.badlogic.gdx.graphics.Color;

import base.font.config.FontConfig;
import base.font.util.FontSourceGenerated;
import base.menu.util.EaseConfig;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_ComposedVisual;
import base.menu.util.MenuItemConfig_Data;
import base.menu.util.MenuItemConfig_Visual;
import base.menu.util.MenuVisualAnimationProperty;
import base.menu.util.StateChangeDurations;
import base.util.AppStyles;
import base.visual.BitmapLabel;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;
import base.visual.util.VisualValueConfig;
import local.app.AppLocal;

/**
 * Created by Adrin on 8/25/2016.
 */
public class DialogViewTitle extends DialogViewBase {
    
    public static final FontConfig GC_VialogViewTitle_DialogTitle = new FontConfig(
            "GC_VialogViewTitle_DialogTitle", FontConfig.CHARS_ID_AaN,
            new FontSourceGenerated("font/Roboto_Medium.ttf", 20, Color.WHITE, "_dpm_width", 1f)
    );
    
    protected static final MenuItemConfig menuItemConfigs_contentArea_titleArea = new MenuItemConfig_ComposedVisual("titleArea", new MenuItemConfig[] {
            new MenuItemConfig_Data("template", new VisualPropertiesConfig()
                    .put("paddingTop", 24f, new String[] {"_dpm"}, "width")
                    .put("paddingBottom", 20f, new String[] {"_dpm"}, "width")
                    .put("firstLineHeight", 19f, new String[] {"_dpm"}, "width")
                    .put("additionalLineHeight", 20f, new String[] {"_dpm"}, "width")
            ),

            new MenuItemConfig_Data("rendering3DSize", new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "parent", "titleAreaSize"}, "width")
                    .put("height", 1f, new String[] {"_local", "parent", "titleAreaSize"}, "height")
            ),

            new MenuItemConfig_Visual("layerBase", new String[] {"_local", "parent", "layerBase"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    .put("x", 0f)
                    .put("y",
                            1f, new String[] {"_local", "parent", "controlsAreaSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bodyAreaSize"}, "height"
                    )
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_isTitleExists_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_"}, 0f, null, null, EaseConfig.DEFAULT)
                    )
                    .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_isTitleExists_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_"}, 0f, null, null, EaseConfig.DEFAULT)
                    )
            ),

            new MenuItemConfig_Visual("lblTitle", new String[] {"_local", "layerBase"}, VisualCreateConfig.newLabel(GC_VialogViewTitle_DialogTitle.usageFontID, "Dialog title"), new VisualPropertiesConfig()
                    .put("width",
                            1f, new String[] {"_local", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_SUBTRACT,
                            24f * 2f, new String[] {"_dpm"}, "width"
                    )
                    .put("anchorX", 0.0f)
                    .put("anchorY", 1.0f)
                    .put("x", 24f, new String[] {"_dpm"}, "width")
                    .put("y",
                            1f, new String[] {"_local", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_SUBTRACT,
                            24f, new String[] {"_dpm"}, "width"
                    )
                    .put("tintColor", Color.valueOf("#000000FF"))
                    .put("alpha", AppStyles.DARK_PRIMARY_TEXT_ALPHA)
                    .put("wrap", true)
            )
    });

    public DialogViewTitle(AppLocal app, MenuItemConfig[] menuItemConfigs, StateChangeDurations[] menuAnimationDurations) {
        super(app, menuItemConfigs, menuAnimationDurations);

        //void
    }

    @Override
    protected void initializeDialogAreas() {
        super.initializeDialogAreas();

        float titlePaddingTop = (Float) getItemPropertyValue(new String[]{"_root", "contentArea", "titleArea", "template"}, "paddingTop", null);

        float titleAreaHeight = titlePaddingTop;

        if(this.dialogConfig.titleText != null) {
            putIsTitleExists(true, false);

            if(this.dialogConfig.titleTotalLinesCount < 1) {
                app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }

            float firstLineHeight = (Float) getItemPropertyValue(new String[]{"_root", "contentArea", "titleArea", "template"}, "firstLineHeight", null);
            float additionalLineHeight = (Float) getItemPropertyValue(new String[]{"_root", "contentArea", "titleArea", "template"}, "additionalLineHeight", null);
            float paddingBottom = (Float) getItemPropertyValue(new String[]{"_root", "contentArea", "titleArea", "template"}, "paddingBottom", null);

            titleAreaHeight += firstLineHeight;
            if(this.dialogConfig.titleTotalLinesCount > 1) {
                titleAreaHeight += (this.dialogConfig.titleTotalLinesCount - 1f) * additionalLineHeight;
            }

            titleAreaHeight += paddingBottom;

            BitmapLabel lblTitle = (BitmapLabel)getItem(new String[]{"_root", "contentArea", "titleArea", "lblTitle"});

            lblTitle.setText(this.dialogConfig.titleText);
        }

        getDataMap().put("titleAreaHeight", titleAreaHeight);
    }

}
