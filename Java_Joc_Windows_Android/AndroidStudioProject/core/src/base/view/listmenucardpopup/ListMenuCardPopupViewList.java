package base.view.listmenucardpopup;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Color;

import java.util.Map;

import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_SmartItem;
import base.menu.util.StateChangeDurations;
import base.util.AppStyles;
import base.visual.IDrawableNode;
import base.visual.List;
import base.visual.ListEntry;
import base.visual.util.ListEventListener;
import base.visual.util.VisualPropertiesConfig;
import local.app.AppLocal;
import local.audio.util.SoundConfig;

public class ListMenuCardPopupViewList extends ListMenuCardPopupViewBase {

    private List optionsList = null;

    private ListEventListener listEventListener = new ListEventListener() {
        @Override
        public void entryActivated(List list, ListEntry listItem) {
            super.entryActivated(list, listItem);

            if(!isActive()) {
                return ;
            }

            if(list == optionsList) {
                helperCallListEntryActivated(list, listItem);
            }else {
                Gdx.app.debug("Error", "Touch event received for a unknown target.");

                return ;
            }
        }
    };

    /*
    // NOTE - Example code only. Must be instantiated in "local" when used.
    public static final MenuItemConfig[] DEFAULT_listEntryConfig = new MenuItemConfig[] {
            new MenuItemConfig_Data("rendering3DSize", new VisualPropertiesConfig()
                    .put("width",
                            1f, new String[] {"_dataMap"}, "entryWidthDip",
                            VisualValueConfig.OPERATION_MULTIPLY,
                            1f, new String[] {"_dpm"}, "width"
                    )
                    .put("height", 38f, new String[] {"_dpm"}, "width")
            ),

            new MenuItemConfig_Visual("layerBase", null, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
            ),
            new MenuItemConfig_Visual("bg", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.0f, null, null)
                    .put("anchorY", 0.0f, null, null)
                    .put("x", 0f, null, null)
                    .put("y", 0f, null, null)
                    // Use a alpha of 0 because there is already a white background. If alpha is not 0, there will be fade artefacts on menu show,hide.
                    .put("alpha", 0.0f)
            ),
            new MenuItemConfig_Visual("touchArea", new String[] {"_local", "layerBase"}, VisualCreateConfig.newTouchAreaAudible(null, null), new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.0f)
                    .put("x", 0f)
                    .put("y", 0f)
            ),
            new MenuItemConfig_Visual("lblText", new String[] {"_local", "layerBase"}, VisualCreateConfig.newLabel(FontInstances.Roboto_Regular_White_AaN_14_DPM.usageFontID, "TYPE"), new VisualPropertiesConfig()
                    .put("anchorX", 0.5f, null, null)
                    .put("anchorY", 0.5f, null, null)
                    .put("x", 0.5f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 0.5f, new String[] {"_local", "rendering3DSize"}, "height")
            )
    };
    public static final StateChangeDurations[] DEFAULT_listEntryAnimationDurations = new StateChangeDurations[] {
            new StateChangeDurations("_in_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_out_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_in_", "_out_", AppStyles.ANIMATION_DURATION_S_DEFAULT)
    };
    */

    protected static final MenuItemConfig menuItemConfigs_contentArea_optionsList = new MenuItemConfig_SmartItem("optionsList", new String[] {"_local", "layerBase"}, "_in_", AppStyles.makeSmartItemConstructor(List.class), new VisualPropertiesConfig()
            .put("x", 0f, new String[] {"_dpm"}, "width")
            .put("y", 0f, new String[] {"_dpm"}, "width")

            .put("listOrientation", List.VERTICAL_LIST)
            .put("entryActivateSoundConfig", SoundConfig.button_down)
            // Manually set.
            //.put("entryWidth", 0f, new String[] {"_dpm"}, "width")
            .put("entryHeight", 38f, new String[] {"_dpm"}, "width")
            .put("entriesSpacingX", 0f)
            .put("entriesSpacingY", 0f)
            .put("listPaddingTop", 15f, new String[] {"_dpm"}, "width")
            .put("listPaddingBottom", 15f, new String[] {"_dpm"}, "width")
            .put("listPaddingLeft", 0f, new String[] {"_dpm"}, "width")
            .put("listPaddingRight", 0f, new String[] {"_dpm"}, "width")
            //.put("rowsCount", 1)
            //.put("columnsCount", 1)
            .put("scaleSizeToContent", true)
            //.put("listWidthOverride", 144f, new String[] {"_dpm"}, "width")
            //.put("listHeightOverride", 144f, new String[] {"_dpm"}, "width")
    );

    public ListMenuCardPopupViewList(AppLocal app, MenuItemConfig[] menuItemConfigs, StateChangeDurations[] easeDurationsSList) {
        super(app, menuItemConfigs, easeDurationsSList);

        //void
    }

    private void helperCallListEntryActivated(List list, ListEntry listEntry) {
        if(viewEventListener != null) {
            viewEventListener.listEntryActivated(this, list, listEntry);
        }
    }

    @Override
    protected void createVisuals(Map<String, Object> createConfigMap) {
        super.createVisuals(createConfigMap);

        optionsList = (List)getItem(new String[]{"_root", "contentArea", "optionsList"});
        optionsList.setEventListener(listEventListener);
    }

    public ListEntry createNewListEntry(MenuItemConfig[] itemInstanceConfigOverride, StateChangeDurations[] easeDurationsSList, Map<String, Object> dataMapOverride) {
        ListEntry entry = optionsList.createNewEntry(itemInstanceConfigOverride, easeDurationsSList, dataMapOverride);

        return entry;
    }

    @Override
    protected void syncVisuals(boolean syncAbsoluteValues, boolean syncAnimatingValues, String startStateID, String endStateID, float stateCompletionPercent) {
        super.syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

        if(viewConfig != null) {
            IDrawableNode optionsList_bg = (IDrawableNode) getItem(new String[]{"_root", "contentArea", "optionsList", "bg"});

            optionsList_bg.setTintColor(new Color(viewConfig.bgColor));

            optionsList.setPropertyValue("entryWidth", viewConfig.widthDip * app.screenDensityMultiplier.x);
        }
    }

    @Override
    protected void activeStateChangedEvent() {
        super.activeStateChangedEvent();

        if(!isActive()) {
            //void
        }else {
            //void
        }
    }

}
