#include "MSVPH_Connecting.h"

using namespace sh;

//asdA1;
//private static final FontConfig FC_MSVPHC_Common_Medium_AaN = new FontConfig(
//    "FC_MSVPHC_Common_Medium_AaN", FontConfig.CHARS_ID_AaN,
//    new FontSourceGenerated("font/Roboto_Medium.ttf", 14, Color.WHITE, "_dpm.width", 1f)
//);
//
//protected static final MenuItemConfig menuItemConfigs_panel_header_contentConnecting = new MenuItemConfig_ComposedVisual("contentConnecting", new MenuItemConfig[] {
//    new MenuItemConfig_Data("props", new VisualPropertiesConfig()
//        .put("width", 1f, "parent.props.width")
//        .put("height", 1f, "parent.props.height")
//    ),
//
//    new MenuItemConfig_Visual("layer", "parent.layer", VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
//        .put("x", 0f)
//        .put("y", 0f)
//        .putAnimating("alpha", new MenuVisualAnimationProperty()
//            .putState("_in_._Connecting_", 1f, EaseConfig.LINEAR)
//            .putState("_in_", 0f, EaseConfig.LINEAR)
//            .putState("_out_", 0f, EaseConfig.LINEAR)
//        )
//        .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
//            // Use float because it will be automatically casted to boolean.
//            .putState("_in_._Connecting_", 1f, EaseConfig.LINEAR)
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
//    new MenuItemConfig_Visual("flblConnecting", "layer", VisualCreateConfig.newLabel(FC_MSVPHC_Common_Medium_AaN.usageFontID, "CONNECTING"), new VisualPropertiesConfig()
//        .put("anchorX", 0.5f)
//        .put("anchorY", 0.5f)
//        .put("x", 0.5f, "props.width")
//        .put("y", 56f / 2f, "_dpm.width")
//        .put("tintColor", Color.valueOf("#FFFFFFFF"))
//    )
//});

MSVPH_Connecting::MSVPH_Connecting(
    IApp* app,
    ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
    worldGame3D::IAppGame* appGame, IAppUtils* appUtils)
    : super(app, viewItemConfigs, viewEaseDurationsSList, appGame, appUtils)
{
    //void
}

MSVPH_Connecting::~MSVPH_Connecting() {
    //void
}
