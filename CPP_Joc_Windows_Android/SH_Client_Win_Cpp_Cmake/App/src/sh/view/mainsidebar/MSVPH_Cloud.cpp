#include "MSVPH_Cloud.h"

using namespace sh;

//asdA1;
//protected static final MenuItemConfig menuItemConfigs_panel_header_cloudContent = new MenuItemConfig_ComposedVisual("cloudContent", new MenuItemConfig[] {
//    new MenuItemConfig_Visual("layer", "parent.layer", VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
//        .put("x", 0.5f, "parent.props.width")
//        .put("y", 100f, "_dpm.width")
//        .putAnimating("alpha", new MenuVisualAnimationProperty()
//            .putState("_in_._Offline_", 1f, EaseConfig.LINEAR)
//            .putState("_in_._Connecting_", 1f, EaseConfig.LINEAR)
//            .putState("_in_", 0f, EaseConfig.LINEAR)
//            .putState("_out_", 0f, EaseConfig.LINEAR)
//        )
//        .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
//            // Use float because it will be automatically casted to boolean.
//            .putState("_in_._Offline_", 1f, EaseConfig.LINEAR)
//            .putState("_in_._Connecting_", 1f, EaseConfig.LINEAR)
//            .putState("_in_", 0f, EaseConfig.LINEAR)
//            .putState("_out_", 0f, EaseConfig.LINEAR)
//        )
//    ),
//    new MenuItemConfig_Visual("imgCloud", "layer", VisualCreateConfig.newSprite("visual2D/taGeneral.atlas", "mainSidebar/imgCloud"), new VisualPropertiesConfig()
//        .put("width", 84f, "_dpm.width")
//        .put("height", 60f, "_dpm.width")
//        .put("anchorX", 0.5f)
//        .put("anchorY", 0.5f)
//        .put("x", 0f)
//        .put("y", 0f)
//    ),
//    new MenuItemConfig_Visual("imgCloudMouth", "layer", VisualCreateConfig.newSprite("visual2D/taGeneral.atlas", "mainSidebar/imgCloudMouth"), new VisualPropertiesConfig()
//        .put("width", 16f, "_dpm.width")
//        .put("height", 8f, "_dpm.width")
//        .put("anchorX", 0.5f)
//        .put("anchorY", 0.5f)
//        .put("x", 0f, "_dpm.width")
//        .put("y", -14f, "_dpm.width")
//        /*Manual property.
//        .putAnimating("rotationD", new MenuVisualAnimationProperty()
//            .putState("_Connecting_", 0f, EaseConfig.LINEAR)
//            .putState("_LoggedIn_", 0f, EaseConfig.LINEAR)
//            .putState("_", 180f, EaseConfig.LINEAR)
//        )*/
//    )
//});
//
//private ISizedDrawable2D imgCloudMouth = null;

MSVPH_Cloud::MSVPH_Cloud(
    IApp* app,
    ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
    worldGame3D::IAppGame* appGame, IAppUtils* appUtils)
    : super(app, viewItemConfigs, viewEaseDurationsSList, appGame, appUtils)
{
    //void
}

//asdA1;
//@Override
//protected void createVisuals(Map<String, Object> createConfigMap) {
//    super.createVisuals(createConfigMap);
//}
//
//@Override
//protected void create_content() {
//    super.create_content();
//
//    imgCloudMouth = getItem("panel.header.cloudContent.imgCloudMouth");
//}
//
//private boolean isInitialMouthAngleSet = false;
//@Override
//protected void onPlayGamesStatusChanged() {
//    super.onPlayGamesStatusChanged();
//
//    if(!isInitialMouthAngleSet) {
//        isInitialMouthAngleSet = true;
//
//        float angD;
//        if(peekPlayGamesStatus() == PlayGamesStatus.OFFLINE) {
//            // Sad face.
//
//            angD = 180f;
//        }else {
//            // Happy face;
//
//            angD = 0f;
//        }
//
//        imgCloudMouth.setRotationD(angD);
//    }
//}
//
//private final float mouthTurnSpeedDegS = 480f;
//@Override
//public void visualLoop(float deltaS, int deltaMS) {
//    super.visualLoop(deltaS, deltaMS);
//
//    PlayGamesStatus playGamesStatus = peekPlayGamesStatus();
//
//    // 0 deg: happy.
//    // 180 deg: sad.
//
//    final float angD = MathUtil.normalizeAngleDeg(imgCloudMouth.getRotationD());
//
//    float newAngD;
//    if (playGamesStatus == PlayGamesStatus.LOGGED_IN) {
//        // Happy face (0 deg).
//
//        if (angD != 0f) {
//            newAngD = MathUtil.normalizeAngleDeg(angD - mouthTurnSpeedDegS * deltaS);
//            if(angD > 0f && newAngD < 0f) {
//                // End reached.
//
//                newAngD = 0f;
//            }
//        } else {
//            newAngD = angD;
//        }
//    } else if (playGamesStatus == PlayGamesStatus.CONNECTING) {
//        // Spinning.
//
//        newAngD = angD - mouthTurnSpeedDegS * deltaS;
//    } else {
//        // Sad face (180 deg).
//
//        if (angD != 180f) {
//            newAngD = MathUtil.normalizeAngleDeg(angD - mouthTurnSpeedDegS * deltaS);
//            if(angD < 0f && newAngD > 0f) {
//                // End reached.
//
//                newAngD = 180f;
//            }
//        } else {
//            newAngD = angD;
//        }
//    }
//
//    if (newAngD != angD) {
//        imgCloudMouth.setRotationD(newAngD);
//    }
//}

MSVPH_Cloud::~MSVPH_Cloud() {
    //void
}
