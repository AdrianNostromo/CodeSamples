#include "MSVP_AccountSettingsPanel.h"

using namespace sh;

//asdA1;
//protected static final MenuItemConfig menuItemConfigs_panel_accountSettingsPanel = new MenuItemConfig_ComposedVisual("accountSettingsPanel", new MenuItemConfig[] {
//    new MenuItemConfig_Data("props", new VisualPropertiesConfig()
//        .put("width", 242f, "_dpm.width")
//        .put("height", 68f, "_dpm.width")
//    ),
//
//    new MenuItemConfig_Visual("layer", "parent.layer", VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
//        .put("x", 0.5f, "parent.props.width")
//        .put("y",
//            1f, "parent.props.height",
//            VisualValueConfig.OPERATION_SUBTRACT,
//            1f, "parent.header.props.height"
//        )
//        .putAnimating("alpha", new MenuVisualAnimationProperty()
//            .putState("_in_._AccSettingsOpen_", 1f, EaseConfig.LINEAR)
//            .putState("_in_", 0f, EaseConfig.LINEAR)
//            .putState("_out_", 0f, EaseConfig.LINEAR)
//        )
//        .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
//            // Use float because it will be automatically casted to boolean.
//            .putState("_in_._AccSettingsOpen_", 1f, EaseConfig.LINEAR)
//            .putState("_in_", 0f, EaseConfig.LINEAR)
//            .putState("_out_", 0f, EaseConfig.LINEAR)
//        )
//    ),
//
//    new MenuItemConfig_Data("shadowOffset", new VisualPropertiesConfig()
//        .put("x", 0f)
//        .put("y", -0.5f, "props.height")
//    ),
//    AppStyles.ITEM_CONFIG_shadow_CARD,
//
//    new MenuItemConfig_Data("bgOffset", new VisualPropertiesConfig()
//        .put("x", 0.0f, "props.width")
//        .put("y", -0.5f, "props.height")
//    ),
//    AppStyles.ITEM_CONFIG_bg_CARD,
//
//    new MenuItemConfig_Visual("taBg", "layer", VisualCreateConfig.newTouchArea(null, null), new VisualPropertiesConfig()
//        .put("width", 1f, "props.width")
//        .put("height", 1f, "props.height")
//        .put("anchorX", 0.5f)
//        .put("anchorY", 0.5f)
//        .put("x", 1f, "bgOffset.x")
//        .put("y", 1f, "bgOffset.y")
//    ),
//
//    new MenuItemConfig_Data("data_btnDisconnect", new VisualPropertiesConfig()
//        .put("x", 0f)
//        .put("y", -0.5f, "props.height")
//    ),
//    new MenuItemConfig_ComposedVisual("shadowHolder_btnDisconnect", new MenuItemConfig[] {
//        new MenuItemConfig_Data("props", new VisualPropertiesConfig()
//            .put("width", 116f, "_dpm.width")
//            .put("height", 34f, "_dpm.width")
//        ),
//        new MenuItemConfig_Data("shadowOffset", new VisualPropertiesConfig()
//            .put("x", 0f)
//            .put("y", 0f)
//        ),
//        new MenuItemConfig_Visual("layer", "parent.layer", VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
//            .put("x", 1f, "parent.data_btnDisconnect.x")
//            .put("y", 1f, "parent.data_btnDisconnect.y")
//        ),
//        AppStyles.ITEM_CONFIG_shadow_RAISED_BUTTON
//    }),
//    new MenuItemConfig_Visual("btnDisconnect", "layer", VisualCreateConfig.newButton("visual2D/taGeneral.atlas", "mainSidebar/panelAccountSettings/btnDisconnect_", SoundConfig.button_down, null), new VisualPropertiesConfig()
//        .put("width", 126f, "_dpm.width")
//        .put("height", 44f, "_dpm.width")
//        .put("anchorX", 0.5f)
//        .put("anchorY", 0.5f)
//        .put("x", 1f, "data_btnDisconnect.x")
//        .put("y", 1f, "data_btnDisconnect.y")
//    )
//});
//
//private TouchListener touchListener = new TouchListener() {
//    @Override
//    public boolean touchDown(IInteractiveDrawable target, Vector2& screenPos, Vector2& localPos, int cursorIndex, base::Touch::ButtonCode* buttonId) {
//        super.touchDown(target, screenPos, localPos, cursorIndex, buttonId);
//
//        // Touch events are ignored while the menu is animating.
//        if(!isActive() || isAnimating()) {
//            return false;
//        }
//
//        if(target == taBg) {
//            //void
//        }else if(target == btnDisconnect) {
//            isAccountSettingOpen = false;
//
//            appEcosystem.getPlayGamesManager().startDisconnect(true);
//        }
//
//        else {
//            throw new LogicError("Err.1.");
//        }
//
//        return false;
//    }
//};
//
//private IInteractiveDrawable taBg;
//private IInteractiveDrawable btnDisconnect;
//
//private boolean isAccountSettingOpen = false;

MSVP_AccountSettingsPanel::MSVP_AccountSettingsPanel(
    IApp* app,
    ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
    worldGame3D::IAppGame* appGame, IAppUtils* appUtils)
    : super(app, viewItemConfigs, viewEaseDurationsSList, appGame, appUtils)
{

    //void
}

//asdA1;
//@Override
//protected void create_content() {
//    super.create_content();
//
//    (taBg = getItem("panel.accountSettingsPanel.taBg")).addTouchListener(touchListener);
//    (btnDisconnect = getItem("panel.accountSettingsPanel.btnDisconnect")).addTouchListener(touchListener);
//}

void MSVP_AccountSettingsPanel::onScreenTaTouchDown() {
//    asdA1;
//    if(isAccountSettingOpen) {
//        isAccountSettingOpen = false;
//
//        updateShowStateIfChanged(true, null);
//    }
}

//asdA1;
//protected final void onAccountSettingsParentPanelTaTouchDown() {
//    if(isAccountSettingOpen) {
//        isAccountSettingOpen = false;
//
//        updateShowStateIfChanged(true, null);
//    }
//}
//
//@Override
//protected void onPlayGamesStatusChanged() {
//    super.onPlayGamesStatusChanged();
//
//    if(peekPlayGamesStatus() != PlayGamesStatus.LOGGED_IN) {
//        isAccountSettingOpen = false;
//    }
//}
//
//@Override
//protected void onActivateAccountSettings() {
//    super.onActivateAccountSettings();
//
//    if(!isAccountSettingOpen) {
//        isAccountSettingOpen = true;
//
//        updateShowStateIfChanged(true, null);
//    }
//}
//
//@Override
//protected String computeStateFlags(String baseState) {
//    String state = super.computeStateFlags(baseState);
//
//    if(isAccountSettingOpen) {
//        state += "AccSettingsOpen_";
//    }
//
//    return state;
//}

MSVP_AccountSettingsPanel::~MSVP_AccountSettingsPanel() {
    //void
}
