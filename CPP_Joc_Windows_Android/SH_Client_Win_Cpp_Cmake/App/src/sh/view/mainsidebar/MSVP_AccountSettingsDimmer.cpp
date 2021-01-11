#include "MSVP_AccountSettingsDimmer.h"

using namespace sh;

//asdA1;
//protected static final MenuItemConfig menuItemConfigs_panel_accountSettingsDimmer = new MenuItemConfig_ComposedVisual("accountSettingsDimmer", new MenuItemConfig[] {
//    new MenuItemConfig_Data("props", new VisualPropertiesConfig()
//        .put("width", 1f, "parent.props.width")
//        .put("height", 1f, "parent.props.height")
//    ),
//
//    new MenuItemConfig_Visual("layer", "parent.layer", VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
//        .put("x", 0f)
//        .put("y", 0f)
//    ),
//
//    new MenuItemConfig_Visual("taBg", "layer", VisualCreateConfig.newTouchArea(null, null), new VisualPropertiesConfig()
//        .put("width", 1f, "props.width")
//        .put("height", 1f, "props.height")
//        .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
//            // Use float because it will be automatically casted to boolean.
//            .putState("_in_._AccSettingsOpen_", 1f, EaseConfig.LINEAR)
//            .putState("_", 0f, EaseConfig.LINEAR)
//        )
//    ),
//
//    new MenuItemConfig_Visual("panelDimmer", "layer", VisualCreateConfig.newSprite("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), new VisualPropertiesConfig()
//        .put("width", 1f, "props.width")
//        .put("height", 1f, "props.height")
//        .put("anchorX", 0.0f)
//        .put("anchorY", 0.0f)
//        .put("x", 0f)
//        .put("y", 0f)
//        .putAnimating("alpha", new MenuVisualAnimationProperty()
//            .putState("_in_._AccSettingsOpen_", AppStyles.SCREEN_DIMMER_ALPHA, EaseConfig.LINEAR)
//            .putState("_", 0f, EaseConfig.LINEAR)
//        )
//        .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
//            // Use float because it will be automatically casted to boolean.
//            .putState("_in_._AccSettingsOpen_", 1f, EaseConfig.LINEAR)
//            .putState("_", 0f, EaseConfig.LINEAR)
//        )
//        .put("tintColor", Color.valueOf("#000000FF"))
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
//            onAccountSettingsParentPanelTaTouchDown();
//        }
//
//        else {
//            throw new LogicError("Unhandled touch target.");
//        }
//
//        return false;
//    }
//};
//
//private IInteractiveDrawable taBg;

MSVP_AccountSettingsDimmer::MSVP_AccountSettingsDimmer(
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
//    (taBg = getItem("panel.accountSettingsDimmer.taBg")).addTouchListener(touchListener);
//}

MSVP_AccountSettingsDimmer::~MSVP_AccountSettingsDimmer() {
    //void
}
