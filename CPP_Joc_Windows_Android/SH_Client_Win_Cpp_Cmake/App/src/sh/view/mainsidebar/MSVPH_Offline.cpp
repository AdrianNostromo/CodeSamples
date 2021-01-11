#include "MSVPH_Offline.h"

using namespace sh;

//asdA1;
//private static final FontConfig FC_MSVPHO_Common_Medium_AaN = new FontConfig(
//    "FC_MSVPHO_Common_Medium_AaN", FontConfig.CHARS_ID_AaN,
//    new FontSourceGenerated("font/Roboto_Medium.ttf", 14, Color.WHITE, "_dpm.width", 1f)
//);
//
//protected static final MenuItemConfig menuItemConfigs_panel_header_contentOffline = new MenuItemConfig_ComposedVisual("contentOffline", new MenuItemConfig[] {
//    new MenuItemConfig_Data("props", new VisualPropertiesConfig()
//        .put("width", 1f, "parent.props.width")
//        .put("height", 1f, "parent.props.height")
//    ),
//
//    new MenuItemConfig_Visual("layer", "parent.layer", VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
//        .put("x", 0f)
//        .put("y", 0f)
//        .putAnimating("alpha", new MenuVisualAnimationProperty()
//            .putState("_in_._Offline_", 1f, EaseConfig.LINEAR)
//            .putState("_in_", 0f, EaseConfig.LINEAR)
//            .putState("_out_", 0f, EaseConfig.LINEAR)
//        )
//        .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
//            // Use float because it will be automatically casted to boolean.
//            .putState("_in_._Offline_", 1f, EaseConfig.LINEAR)
//            .putState("_in_", 0f, EaseConfig.LINEAR)
//            .putState("_out_", 0f, EaseConfig.LINEAR)
//        )
//    ),
//    new MenuItemConfig_Visual("imgLowerBg", "layer", VisualCreateConfig.newSprite("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), new VisualPropertiesConfig()
//        .put("width", 1f, "props.width")
//        .put("height", 56f, "_dpm.width")
//        .put("anchorX", 0.0f)
//        .put("anchorY", 0.0f)
//        .put("x", 0f)
//        .put("y", 0f)
//        .put("alpha", 0.5f)
//    ),
//    new MenuItemConfig_Visual("flblNotConnected", "layer", VisualCreateConfig.newLabel(FC_MSVPHO_Common_Medium_AaN.usageFontID, "NOT CONNECTED"), new VisualPropertiesConfig()
//        .put("anchorX", 0.0f)
//        .put("anchorY", 0.5f)
//        .put("x", 19f, "_dpm.width")
//        .put("y", 56f / 2f, "_dpm.width")
//        .put("tintColor", Color.valueOf("#FF3333FF"))
//    ),
//    new MenuItemConfig_Data("data_btnConnect", new VisualPropertiesConfig()
//        .put("x",
//            1f, "props.width",
//            VisualValueConfig.OPERATION_SUBTRACT,
//            64f, "_dpm.width"
//        )
//        .put("y", 56f / 2f, "_dpm.width")
//    ),
//    new MenuItemConfig_ComposedVisual("shadowHolder_btnConnect", new MenuItemConfig[] {
//        new MenuItemConfig_Data("props", new VisualPropertiesConfig()
//            .put("width", 96f, "_dpm.width")
//            .put("height", 34f, "_dpm.width")
//        ),
//        new MenuItemConfig_Data("shadowOffset", new VisualPropertiesConfig()
//            .put("x", 0f)
//            .put("y", 0f)
//        ),
//        new MenuItemConfig_Visual("layer", "parent.layer", VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
//            .put("x", 1f, "parent.data_btnConnect.x")
//            .put("y", 1f, "parent.data_btnConnect.y")
//        ),
//        AppStyles.ITEM_CONFIG_shadow_RAISED_BUTTON
//    }),
//    new MenuItemConfig_Visual("btnConnect", "layer", VisualCreateConfig.newButton("visual2D/taGeneral.atlas", "mainSidebar/contentOffline/btnConnect_", SoundConfig.button_down, null), new VisualPropertiesConfig()
//        .put("width", 106f, "_dpm.width")
//        .put("height", 44f, "_dpm.width")
//        .put("anchorX", 0.5f)
//        .put("anchorY", 0.5f)
//        .put("x", 1f, "data_btnConnect.x")
//        .put("y", 1f, "data_btnConnect.y")
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
//        if(target == btnConnect) {
//            appEcosystem.getPlayGamesManager().startConnect(true);
//        }else {
//            throw new LogicError("Err.1.");
//        }
//
//        return false;
//    }
//};
//
//private IInteractiveDrawable btnConnect;

MSVPH_Offline::MSVPH_Offline(
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
//    (btnConnect = getItem("panel.header.contentOffline.btnConnect")).addTouchListener(touchListener);
//}

MSVPH_Offline::~MSVPH_Offline() {
    //void
}
