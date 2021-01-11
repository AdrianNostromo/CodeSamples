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
public class DialogViewBody extends DialogViewTitle {
    
    public static final FontConfig FC_DialogViewBody_DialogBody = new FontConfig(
            "FC_DialogViewBody_DialogBody", FontConfig.CHARS_ID_AaN,
            new FontSourceGenerated("font/Roboto_Regular.ttf", 16, Color.WHITE, "_dpm_width", 1f)
    );

    protected static final MenuItemConfig menuItemConfigs_contentArea_bodyArea = new MenuItemConfig_ComposedVisual("bodyArea", new MenuItemConfig[] {
            new MenuItemConfig_Data("template", new VisualPropertiesConfig()
                    .put("paddingBottom", 20f, new String[] {"_dpm"}, "width")
                    .put("firstLineHeight", 13f, new String[] {"_dpm"}, "width")
                    .put("additionalLineHeight", 18f, new String[] {"_dpm"}, "width")
            ),

            new MenuItemConfig_Data("rendering3DSize", new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "parent", "bodyAreaSize"}, "width")
                    .put("height", 1f, new String[] {"_local", "parent", "bodyAreaSize"}, "height")
            ),

            new MenuItemConfig_Visual("layerBase", new String[] {"_local", "parent", "layerBase"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    .put("x", 0f)
                    .put("y", 1f, new String[] {"_local", "parent", "controlsAreaSize"}, "height")
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_isBodyExists_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_"}, 0f, null, null, EaseConfig.DEFAULT)
                    )
                    .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_isBodyExists_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_"}, 0f, null, null, EaseConfig.DEFAULT)
                    )
            ),

            new MenuItemConfig_Visual("lblText", new String[] {"_local", "layerBase"}, VisualCreateConfig.newLabel(FC_DialogViewBody_DialogBody.usageFontID, "Dialog description."), new VisualPropertiesConfig()
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
                            0f, new String[] {"_dpm"}, "width"
                    )
                    .put("tintColor", Color.valueOf("#000000FF"))
                    .put("alpha", AppStyles.DARK_PRIMARY_TEXT_ALPHA)
                    .put("wrap", true)
            )
    });

    public DialogViewBody(AppLocal app, MenuItemConfig[] menuItemConfigs, StateChangeDurations[] menuAnimationDurations) {
        super(app, menuItemConfigs, menuAnimationDurations);

        //void
    }

    @Override
    protected void initializeDialogAreas() {
        super.initializeDialogAreas();

        float bodyAreaHeight = 0f;

        if(this.dialogConfig.dynamicBodyTextItem != null) {
            putIsBodyExists(true, false);

            if (this.dialogConfig.dynamicBodyTextItem.linesCount < 1) {
                app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }

            float firstLineHeight = (Float) getItemPropertyValue(new String[]{"_root", "contentArea", "bodyArea", "template"}, "firstLineHeight", null);
            float additionalLineHeight = (Float) getItemPropertyValue(new String[]{"_root", "contentArea", "bodyArea", "template"}, "additionalLineHeight", null);
            float paddingBottom = (Float) getItemPropertyValue(new String[]{"_root", "contentArea", "bodyArea", "template"}, "paddingBottom", null);

            bodyAreaHeight = firstLineHeight;
            if (this.dialogConfig.dynamicBodyTextItem.linesCount > 1) {
                bodyAreaHeight += (this.dialogConfig.dynamicBodyTextItem.linesCount - 1) * additionalLineHeight;
            }

            bodyAreaHeight += paddingBottom;

            BitmapLabel lblText = (BitmapLabel) getItem(new String[]{"_root", "contentArea", "bodyArea", "lblText"});

            lblText.setText(this.dialogConfig.dynamicBodyTextItem.text);
        }

        getDataMap().put("bodyAreaHeight", bodyAreaHeight);
    }

}
