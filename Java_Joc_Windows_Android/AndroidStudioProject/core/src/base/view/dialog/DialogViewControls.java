package base.view.dialog;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Color;

import java.util.Map;

import base.font.config.FontConfig;
import base.font.util.FontSourceGenerated;
import base.interaction.TouchListener;
import base.menu.util.DynamicTextButtonConfig;
import base.menu.util.EaseConfig;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_ComposedVisual;
import base.menu.util.MenuItemConfig_Data;
import base.menu.util.MenuItemConfig_Visual;
import base.menu.util.MenuVisualAnimationProperty;
import base.menu.util.StateChangeDurations;
import base.visual.BitmapLabel;
import base.visual.IInteractiveDrawable;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;
import local.app.AppLocal;
import local.audio.util.SoundConfig;

/**
 * Created by Adrin on 8/25/2016.
 */
public class DialogViewControls extends DialogViewBody {
    
    public static final FontConfig FC_DialogViewControls_LblButton = new FontConfig(
            "FC_DialogViewControls_LblButton", FontConfig.CHARS_ID_AaN,
            new FontSourceGenerated("font/Roboto_Medium.ttf", 14, Color.WHITE, "_dpm_width", 1f)
    );

    private IInteractiveDrawable[] cuntrolButtonTouchAreas = null;
    private BitmapLabel[] cuntrolButtonLbls = null;

    private TouchListener touchListener = new TouchListener() {
        @Override
        public boolean touchDown(IInteractiveDrawable target, float worldX, float worldY) {
            // Touch events are ignored while the menu is animating.
            if(!isActive()) {
                return false;
            }

            if(target == getItem(new String[] {"_root", "contentArea", "controlsArea", "taBtnA"})
                    || target == getItem(new String[] {"_root", "contentArea", "controlsArea", "taBtnB"})
                    || target == getItem(new String[] {"_root", "contentArea", "controlsArea", "taBtnC"})) {
                DynamicTextButtonConfig dynamicButtonConfig = (DynamicTextButtonConfig)target.getUserDataMap().get("dynamicButtonConfig");
                dynamicButtonActivated(dynamicButtonConfig);
            }else {
                Gdx.app.debug("Error", "Touch event received for a unknown target.");

                return false;
            }

            return false;
        }
    };

    protected static final MenuItemConfig menuItemConfigs_contentArea_controlsArea = new MenuItemConfig_ComposedVisual("controlsArea", new MenuItemConfig[] {
            new MenuItemConfig_Data("template", new VisualPropertiesConfig()
                    .put("paddingTop", 8f, new String[] {"_dpm"}, "width")
                    .put("paddingBottom", 8f, new String[] {"_dpm"}, "width")
                    .put("controlsRowHeight", 36f, new String[] {"_dpm"}, "width")

                    .put("paddingRight", 8f, new String[] {"_dpm"}, "width")
                    .put("interItemsHorizontalSpacing", 8f, new String[] {"_dpm"}, "width")
                    .put("labelTouchMargin", 8f, new String[] {"_dpm"}, "width")
            ),

            new MenuItemConfig_Data("rendering3DSize", new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "parent", "controlsAreaSize"}, "width")
                    .put("height", 1f, new String[] {"_local", "parent", "controlsAreaSize"}, "height")
            ),

            new MenuItemConfig_Visual("layerBase", new String[] {"_local", "parent", "layerBase"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    .put("x", 0f)
                    .put("y", 0f)
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_isControlsExists_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_"}, 0f, null, null, EaseConfig.DEFAULT)
                    )
                    .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_isControlsExists_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_"}, 0f, null, null, EaseConfig.DEFAULT)
                    )
            ),

            new MenuItemConfig_Visual("lblBtnA", new String[] {"_local", "layerBase"}, VisualCreateConfig.newLabel(FC_DialogViewControls_LblButton.usageFontID, "UPDATE"), new VisualPropertiesConfig()
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    // x is dynamicly updated.
                    //.put("x",
                    //        1f, new String[] {"_local", "rendering3DSize"}, "width",
                    //        VisualValueConfig.OPERATION_SUBTRACT,
                    //        51f, new String[] {"_dpm"}, "width"
                    //)
                    //.put("y", 33f, new String[] {"_dpm"}, "width")
            ),
            new MenuItemConfig_Visual("taBtnA", new String[] {"_local", "layerBase"}, VisualCreateConfig.newTouchAreaAudible(SoundConfig.button_down, null), new VisualPropertiesConfig()
                    // x is dynamicly updated.
                    // .put("width", 74f, new String[] {"_dpm"}, "width")
                    .put("height", 36f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    // x is dynamicly updated.
                    //.put("x",
                    //        1f, new String[] {"_local", "rendering3DSize"}, "width",
                    //        VisualValueConfig.OPERATION_SUBTRACT,
                    //        51f, new String[] {"_dpm"}, "width"
                    //)
                    //.put("y", 33f, new String[] {"_dpm"}, "width")
            ),

            new MenuItemConfig_Visual("lblBtnB", new String[] {"_local", "layerBase"}, VisualCreateConfig.newLabel(FC_DialogViewControls_LblButton.usageFontID, "UPDATE"), new VisualPropertiesConfig()
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    // x is dynamicly updated.
                    //.put("x",
                    //        1f, new String[] {"_local", "rendering3DSize"}, "width",
                    //        VisualValueConfig.OPERATION_SUBTRACT,
                    //        51f, new String[] {"_dpm"}, "width"
                    //)
                    //.put("y", 33f, new String[] {"_dpm"}, "width")
            ),
            new MenuItemConfig_Visual("taBtnB", new String[] {"_local", "layerBase"}, VisualCreateConfig.newTouchAreaAudible(SoundConfig.button_down, null), new VisualPropertiesConfig()
                    // x is dynamicly updated.
                    // .put("width", 74f, new String[] {"_dpm"}, "width")
                    .put("height", 36f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    // x is dynamicly updated.
                    //.put("x",
                    //        1f, new String[] {"_local", "rendering3DSize"}, "width",
                    //        VisualValueConfig.OPERATION_SUBTRACT,
                    //        51f, new String[] {"_dpm"}, "width"
                    //)
                    //.put("y", 33f, new String[] {"_dpm"}, "width")
            ),

            new MenuItemConfig_Visual("lblBtnC", new String[] {"_local", "layerBase"}, VisualCreateConfig.newLabel(FC_DialogViewControls_LblButton.usageFontID, "UPDATE"), new VisualPropertiesConfig()
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    // x is dynamicly updated.
                    //.put("x",
                    //        1f, new String[] {"_local", "rendering3DSize"}, "width",
                    //        VisualValueConfig.OPERATION_SUBTRACT,
                    //        51f, new String[] {"_dpm"}, "width"
                    //)
                    //.put("y", 33f, new String[] {"_dpm"}, "width")
            ),
            new MenuItemConfig_Visual("taBtnC", new String[] {"_local", "layerBase"}, VisualCreateConfig.newTouchAreaAudible(SoundConfig.button_down, null), new VisualPropertiesConfig()
                    // x is dynamicly updated.
                    // .put("width", 74f, new String[] {"_dpm"}, "width")
                    .put("height", 36f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    // x is dynamicly updated.
                    //.put("x",
                    //        1f, new String[] {"_local", "rendering3DSize"}, "width",
                    //        VisualValueConfig.OPERATION_SUBTRACT,
                    //        51f, new String[] {"_dpm"}, "width"
                    //)
                    //.put("y", 33f, new String[] {"_dpm"}, "width")
            ),
    });

    public DialogViewControls(AppLocal app, MenuItemConfig[] menuItemConfigs, StateChangeDurations[] menuAnimationDurations) {
        super(app, menuItemConfigs, menuAnimationDurations);

        //void
    }

    private void dynamicButtonActivated(DynamicTextButtonConfig dynamicButtonConfig) {
        if(dialogEventListener != null) {
            dialogEventListener.dynamicButtonActivated((DialogView)this, dynamicButtonConfig);
        }
    }

    @Override
    protected void createVisuals(Map<String, Object> createConfigMap) {
        super.createVisuals(createConfigMap);

        cuntrolButtonTouchAreas = new IInteractiveDrawable[] {
                (IInteractiveDrawable) getItem(new String[]{"_root", "contentArea", "controlsArea", "taBtnA"}),
                (IInteractiveDrawable) getItem(new String[]{"_root", "contentArea", "controlsArea", "taBtnB"}),
                (IInteractiveDrawable) getItem(new String[]{"_root", "contentArea", "controlsArea", "taBtnC"})
        };
        for(int i=0;i<cuntrolButtonTouchAreas.length;i++) {
            IInteractiveDrawable entry = cuntrolButtonTouchAreas[i];

            entry.setTouchListener(touchListener);
        }

        cuntrolButtonLbls = new BitmapLabel[] {
                (BitmapLabel) getItem(new String[]{"_root", "contentArea", "controlsArea", "lblBtnA"}),
                (BitmapLabel) getItem(new String[]{"_root", "contentArea", "controlsArea", "lblBtnB"}),
                (BitmapLabel) getItem(new String[]{"_root", "contentArea", "controlsArea", "lblBtnC"})
        };
    }

    public BitmapLabel getControlLbl(int controlIndex) {
        String[] controlsList = new String[] {
                "lblBtnA",
                "lblBtnB",
                "lblBtnC"
        };

        BitmapLabel lbl = (BitmapLabel) getItem(new String[]{"_root", "contentArea", "controlsArea", controlsList[controlIndex]});

        return lbl;
    }

    @Override
    protected void initializeDialogAreas() {
        super.initializeDialogAreas();

        int controlsLinesCount = 0;
        if(this.dialogConfig.dynamicButtonConfigs != null) {
            putIsControlsExists(true, false);

            for (int i = 0; i < this.dialogConfig.dynamicButtonConfigs.length; i++) {
                DynamicTextButtonConfig dinamicTextButtonConfig = this.dialogConfig.dynamicButtonConfigs[i];

                if(this.dialogConfig.isOneButtonControlPerLine) {
                    controlsLinesCount++;
                }else {
                    controlsLinesCount = 1;
                }
            }

            if(controlsLinesCount <= 0) {
                app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }
        }

        float paddingTop = (Float) getItemPropertyValue(new String[]{"_root", "contentArea", "controlsArea", "template"}, "paddingTop", null);
        float paddingBottom = (Float) getItemPropertyValue(new String[]{"_root", "contentArea", "controlsArea", "template"}, "paddingBottom", null);
        float controlsRowHeight = (Float) getItemPropertyValue(new String[]{"_root", "contentArea", "controlsArea", "template"}, "controlsRowHeight", null);

        float controlsAreaHeight = 0f;
        if(controlsLinesCount == 1) {
            controlsAreaHeight += paddingTop;
        }
        controlsAreaHeight += controlsLinesCount * controlsRowHeight;
        controlsAreaHeight += paddingBottom;

        getDataMap().put("controlsAreaHeight", controlsAreaHeight);

        // Initialize the control buttons.
        int btnIndex = 0;
        if(dialogConfig.dynamicButtonConfigs != null) {
            for (int i = 0; i < dialogConfig.dynamicButtonConfigs.length; i++) {
                DynamicTextButtonConfig dBtnConfig = dialogConfig.dynamicButtonConfigs[i];

                BitmapLabel btnLbl = cuntrolButtonLbls[btnIndex];
                IInteractiveDrawable btnTA = cuntrolButtonTouchAreas[btnIndex];

                initDynamicBtn(dBtnConfig, btnTA, btnLbl);

                btnIndex++;
            }
        }
        for(int i=btnIndex;i<cuntrolButtonLbls.length;i++) {
            BitmapLabel lblBtn = cuntrolButtonLbls[i];
            IInteractiveDrawable taBtn = cuntrolButtonTouchAreas[i];

            lblBtn.setVisible(false);

            taBtn.setVisible(false);
            taBtn.getUserDataMap().put("dynamicButtonConfig", null);
        }
    }

    private void initDynamicBtn(DynamicTextButtonConfig dynamicButtonConfig, IInteractiveDrawable taBtn, BitmapLabel lblBtn) {
        if(dynamicButtonConfig == null) {
            app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

            throw new Error("PathFollowerNodeConfig_Pause.Error.1.");
        }

        taBtn.getUserDataMap().put("dynamicButtonConfig", dynamicButtonConfig);

        taBtn.setVisible(true);
        lblBtn.setVisible(true);

        float labelTouchMargin = (Float) getItemPropertyValue(new String[]{"_root", "contentArea", "controlsArea", "template"}, "labelTouchMargin", null);

        float touchAreaWidth = dynamicButtonConfig.rawTextWidthDPM * app.screenDensityMultiplier.x + labelTouchMargin * 2f;

        taBtn.setWidth(touchAreaWidth);

        lblBtn.setText(dynamicButtonConfig.text);
        lblBtn.setTintColor(new Color(dynamicButtonConfig.tintColor));
        lblBtn.setAlpha(dynamicButtonConfig.alpha);
    }

    @Override
    protected void syncVisuals(boolean syncAbsoluteValues, boolean syncAnimatingValues, String startStateID, String endStateID, float stateCompletionPercent) {
        super.syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

        syncDynamicButtonsPositions();
    }

    private void syncDynamicButtonsPositions() {
        float paddingRight = (Float) getItemPropertyValue(new String[]{"_root", "contentArea", "controlsArea", "template"}, "paddingRight", null);
        float paddingBottom = (Float) getItemPropertyValue(new String[]{"_root", "contentArea", "controlsArea", "template"}, "paddingBottom", null);
        float interItemsHorizontalSpacing = (Float) getItemPropertyValue(new String[]{"_root", "contentArea", "controlsArea", "template"}, "interItemsHorizontalSpacing", null);
        float controlsRowHeight = (Float) getItemPropertyValue(new String[]{"_root", "contentArea", "controlsArea", "template"}, "controlsRowHeight", null);
        float controlsAreaWidth = (Float) getItemPropertyValue(new String[]{"_root", "contentArea", "controlsAreaSize"}, "width", null);

        float rightWallPosX = controlsAreaWidth - paddingRight;
        float posY = paddingBottom + controlsRowHeight / 2f;

        for(int i=0;i<cuntrolButtonTouchAreas.length;i++) {
            IInteractiveDrawable taBtn = cuntrolButtonTouchAreas[i];
            BitmapLabel lblBtn = cuntrolButtonLbls[i];

            if(taBtn.getUserDataMap().get("dynamicButtonConfig") != null) {
                float posX = rightWallPosX - taBtn.getWidth() / 2f;

                taBtn.setPosition(posX, posY);
                lblBtn.setPosition(posX, posY);

                if(dialogConfig.isOneButtonControlPerLine) {
                    posY += controlsRowHeight;
                }else {
                    rightWallPosX = rightWallPosX - taBtn.getWidth() - interItemsHorizontalSpacing;
                }
            }
        }
    }

}
