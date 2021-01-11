#include "MSVPH_LoggedIn.h"

using namespace sh;

//asdA1;
//private static final FontConfig FC_MSVPHLI_Common_Regular_AaN = new FontConfig(
//    "FC_MSVPHLI_Common_Regular_AaN", FontConfig.CHARS_ID_AaN,
//    new FontSourceGenerated("font/Roboto_Regular.ttf", 14, Color.WHITE, "_dpm.width", 1f)
//);
//private static final FontConfig FC_MSVPHLI_Common_Medium_AaN = new FontConfig(
//    "FC_MSVPHLI_Common_Medium_AaN", FontConfig.CHARS_ID_AaN,
//    new FontSourceGenerated("font/Roboto_Medium.ttf", 14, Color.WHITE, "_dpm.width", 1f)
//);
//
//protected static final MenuItemConfig menuItemConfigs_panel_header_contentLoggedIn = new MenuItemConfig_ComposedVisual("contentLoggedIn", new MenuItemConfig[] {
//    new MenuItemConfig_Data("props", new VisualPropertiesConfig()
//        .put("width", 1f, "parent.props.width")
//        .put("height", 142f, "_dpm.width")
//    ),
//
//    new MenuItemConfig_Visual("layer", "parent.layer", VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
//        .put("x", 0f)
//        .put("y", 0f)
//        .putAnimating("alpha", new MenuVisualAnimationProperty()
//            .putState("_in_._LoggedIn_", 1f, EaseConfig.LINEAR)
//            .putState("_in_", 0f, EaseConfig.LINEAR)
//            .putState("_out_", 0f, EaseConfig.LINEAR)
//        )
//        .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
//            // Use float because it will be automatically casted to boolean.
//            .putState("_in_._LoggedIn_", 1f, EaseConfig.LINEAR)
//            .putState("_in_", 0f, EaseConfig.LINEAR)
//            .putState("_out_", 0f, EaseConfig.LINEAR)
//        )
//    ),
//    new MenuItemConfig_Visual("avatarBg", "layer", VisualCreateConfig.newSprite("visual2D/taGeneral.atlas", "mainSidebar/contentLoggedIn/avatarBg"), new VisualPropertiesConfig()
//        .put("width", 64f, "_dpm.width")
//        .put("height", 64f, "_dpm.width")
//        .put("anchorX", 0.5f)
//        .put("anchorY", 0.5f)
//        .put("x", 48f, "_dpm.width")
//        .put("y", 92f, "_dpm.width")
//    ),
//    new MenuItemConfig_Data("avatarSize", new VisualPropertiesConfig()
//        .put("width", 64f, "_dpm.width")
//        .put("height", 64f, "_dpm.height")
//    ),
//    new MenuItemConfig_Visual("avatarParent", "layer", VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
//        .put("x", 48f, "_dpm.width")
//        .put("y", 92f, "_dpm.width")
//    ),
//    /*new MenuItemConfig_Visual("avatarOutline", "layer", VisualCreateConfig.newSprite("visual2D/taGeneral.atlas", "mainSidebar/contentLoggedIn/avatarOutline"), new VisualPropertiesConfig()
//        .put("width", 64f, "_dpm.width")
//        .put("height", 64f, "_dpm.width")
//        .put("anchorX", 0.5f)
//        .put("anchorY", 0.5f)
//        .put("x", 48f, "_dpm.width")
//        .put("y", 92f, "_dpm.width")
//    ),*/
//    new MenuItemConfig_Visual("avatarLoader", "layer", VisualCreateConfig.newMovieClip("visual2D/taGeneral.atlas", "general/loader/indeterminate_round/mc_", 1, 49, 4, 1f / 32f, Animation.PlayMode.LOOP, false), new VisualPropertiesConfig()
//        .put("width", 27f, "_dpm.width")
//        .put("height", 27f, "_dpm.width")
//        .put("anchorX", 0.5f)
//        .put("anchorY", 0.5f)
//        .put("x", 48f, "_dpm.width")
//        .put("y", 92f, "_dpm.width")
//    ),
//
//    new MenuItemConfig_Visual("lblDisplayName", "layer", VisualCreateConfig.newLabel(FC_MSVPHLI_Common_Medium_AaN.usageFontID, "Display Name"), new VisualPropertiesConfig()
//        .put("anchorX", 0.0f)
//        .put("anchorY", 0.5f)
//        .put("x", 16f, "_dpm.width")
//        .put("y", 38f, "_dpm.width")
//        .put("tintColor", Color.valueOf("#FFFFFFFF"))
//    ),
//    new MenuItemConfig_Visual("lblPlayerTitle", "layer", VisualCreateConfig.newLabel(FC_MSVPHLI_Common_Regular_AaN.usageFontID, "Level 15 Warlock"), new VisualPropertiesConfig()
//        .put("anchorX", 0.0f)
//        .put("anchorY", 0.5f)
//        .put("x", 16f, "_dpm.width")
//        .put("y", 18f, "_dpm.width")
//        .put("tintColor", Color.valueOf("#FFFFFFFF"))
//    ),
//    new MenuItemConfig_Visual("btnAccountSettings", "layer", VisualCreateConfig.newButton("visual2D/taGeneral.atlas", "mainSidebar/contentLoggedIn/btnAccountSettings_", SoundConfig.button_down, null), new VisualPropertiesConfig()
//        .put("width", 24f, "_dpm.width")
//        .put("height", 24f, "_dpm.width")
//        .put("anchorX", 0.5f)
//        .put("anchorY", 0.5f)
//        .put("x",
//            1f, "props.width",
//            VisualValueConfig.OPERATION_SUBTRACT,
//            28f, "_dpm.width"
//        )
//        .put("y", 18f, "_dpm.width")
//        .putAnimating("rotationD", new MenuVisualAnimationProperty()
//            .putState("_in_._AccSettingsOpen_", 180f, EaseConfig.LINEAR)
//            .putState("_in_", 0f, EaseConfig.LINEAR)
//            .putState("_out_", 0f, EaseConfig.LINEAR)
//        )
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
//        if(target == btnAccountSettings) {
//            onActivateAccountSettings();
//        }else {
//            throw new LogicError("Err.1.");
//        }
//
//        return false;
//    }
//};
//
//private IInteractiveDrawable btnAccountSettings;
//private BitmapLabel lblDisplayName;
//private BitmapLabel lblPlayerTitle;
//private ISizedDrawable2D avatarBg;
//private Container2D avatarParent;
//
//private Sprite2D loggedInPlayerAvatar = null;
//
//private MovieClip avatarLoader = null;
//
//private boolean isDirtyStateData = true;
//private boolean isDirtyPlayerAvatar = true;

MSVPH_LoggedIn::MSVPH_LoggedIn(
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
//    (btnAccountSettings = getItem("panel.header.contentLoggedIn.btnAccountSettings")).addTouchListener(touchListener);
//
//    lblDisplayName = getItem("panel.header.contentLoggedIn.lblDisplayName");
//    lblPlayerTitle = getItem("panel.header.contentLoggedIn.lblPlayerTitle");
//
//    avatarBg = getItem("panel.header.contentLoggedIn.avatarBg");
//    avatarParent = getItem("panel.header.contentLoggedIn.avatarParent");
//}
//
//protected void onActivateAccountSettings() {
//    //void
//}
//
//@Override
//protected void createVisuals(Map<String, Object> createConfigMap) {
//    super.createVisuals(createConfigMap);
//
//    avatarLoader = getItem("panel.header.contentLoggedIn.avatarLoader");
//    avatarLoader.setAlpha(0f);
//    avatarLoader.setVisible(false);
//}
//
//@Override
//protected void onPlayGamesStatusChanged() {
//    super.onPlayGamesStatusChanged();
//
//    isDirtyStateData = true;
//    isDirtyPlayerAvatar = true;
//}
//
//@Override
//protected void onPlayerIconLoaded() {
//    super.onPlayerIconLoaded();
//
//    isDirtyPlayerAvatar = true;
//}
//
//@Override
//protected void onSelectedStateChanged() {
//    super.onSelectedStateChanged();
//
//    if(!isActive()) {
//        //void
//    }else {
//        syncStateData();
//
//        updatePlayerAvatar(true, true);
//    }
//}
//
//private void syncStateData() {
//    isDirtyStateData = false;
//
//    if(peekPlayGamesStatus() == PlayGamesStatus.LOGGED_IN) {
//        ConnectedPlayerData connectedPlayerData = appEcosystem.getPlayGamesManager().getConnectedPlayerData();
//
//        lblDisplayName.setText(connectedPlayerData.displayName);
//        String playerTitle = "Level " + Integer.toString(connectedPlayerData.levelNumber) + " " + connectedPlayerData.playerTitle;
//        lblPlayerTitle.setText(playerTitle);
//    }
//}
//
//@Override
//public void visualLoop(float deltaS, int deltaMS) {
//    super.visualLoop(deltaS, deltaMS);
//
//    if (isActiveOrAnimating()) {
//        if(isDirtyStateData) {
//            syncStateData();
//        }
//
//        if(isDirtyPlayerAvatar) {
//            updatePlayerAvatar(false, false);
//        }
//
//        if(loggedInPlayerAvatar != null && loggedInPlayerAvatar.getAlpha() < 1f) {
//            float alpha = loggedInPlayerAvatar.getAlpha();
//            alpha = Math.min(1f, alpha + deltaS / 0.2f);
//            loggedInPlayerAvatar.setAlpha(alpha);
//
//            if(alpha >= 1f) {
//                avatarBg.setVisible(false);
//            }else {
//                if(alpha < 0.75f) {
//                    avatarBg.setAlpha(1f);
//                }else {
//                    float n = 1f - (alpha - 0.75f) * 4f;
//                    avatarBg.setAlpha(n);
//                }
//            }
//        }
//
//        // If LoggedIn and avatar is null.
//        if(peekPlayGamesStatus() == PlayGamesStatus.LOGGED_IN && loggedInPlayerAvatar == null) {
//            // Main loading sequence is active.
//
//            if(!avatarLoader.getVisible()) {
//                avatarLoader.setVisible(true);
//                avatarLoader.play(true);
//            }
//
//            float alpha = avatarLoader.getAlpha();
//            if(alpha < 1f) {
//                alpha = Math.min(1f, alpha + deltaS / 0.2f);
//                avatarLoader.setAlpha(alpha);
//            }
//        }else {
//            float alpha = avatarLoader.getAlpha();
//            if(alpha > 0f) {
//                alpha = Math.max(0f, alpha - deltaS / 0.2f);
//                avatarLoader.setAlpha(alpha);
//            }
//
//            if(alpha <= 0f && avatarLoader.getVisible()) {
//                avatarLoader.stop();
//                avatarLoader.setVisible(false);
//            }
//        }
//        if(avatarLoader.getVisible()) {
//            avatarLoader.setRotationD(avatarLoader.getRotationD() - 360f * deltaS);
//        }
//    }
//}
//
//@Override
//protected void syncVisuals(boolean syncAbsoluteValues, boolean syncAnimatingValues, String startStateID, String endStateID, float stateCompletionPercent) {
//    super.syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);
//
//    if(peekPlayGamesStatus() == PlayGamesStatus.LOGGED_IN && loggedInPlayerAvatar != null) {
//        syncPlayerAvatarVisual();
//    }
//}
//
//private void syncPlayerAvatarVisual() {
//    float ww = getItemPropertyValue("panel.header.contentLoggedIn.avatarSize.width", null);
//    float hh = getItemPropertyValue("panel.header.contentLoggedIn.avatarSize.height", null);
//
//    loggedInPlayerAvatar.setSize(ww, hh);
//}
//
//private void updatePlayerAvatar(boolean forceRemovalWhenAnimating, boolean forceFinalAlphaOnCreation) {
//    if(peekPlayGamesStatus() == PlayGamesStatus.LOGGED_IN) {
//        // Show avatar if loaded.
//
//        if(loggedInPlayerAvatar == null) {
//            ConnectedPlayerData connectedPlayerData = appEcosystem.getPlayGamesManager().getConnectedPlayerData();
//
//            if(connectedPlayerData.iconImagePixelsARGB8888 != null) {
//                int finalImgSizeW = connectedPlayerData.iconImageWidth;
//                int finalImgSizeH = connectedPlayerData.iconImageHeight;
//
//                //int[] pdRGBA8888 = VisualUtil.copyBitmapDataAndConvert_ARGB8888_To_RGBA8888(connectedPlayerData.iconImagePixelsARGB8888);
//
//                int[] pdRGBA8888 = VisualUtil.copyBitmapDataAndConvert_ARGB8888_To_RGBA8888_AndUpscale2X(
//                    connectedPlayerData.iconImagePixelsARGB8888,
//                    finalImgSizeW, finalImgSizeH
//                );
//                finalImgSizeW *= 2;
//                finalImgSizeH *= 2;
//
//                VisualUtil.bitmapData_applyAntialiasedCircularMaskRGBA8888(
//                    pdRGBA8888, finalImgSizeW, finalImgSizeH, 1f
//                );
//
//                loggedInPlayerAvatar = VisualUtil.newPixelSprite(new PixelVisualData(
//                    finalImgSizeW, finalImgSizeH,
//                    pdRGBA8888
//                ), avatarParent);
//                loggedInPlayerAvatar.setAnchor(0.5f, 0.5f);
//                if(!forceFinalAlphaOnCreation) {
//                    loggedInPlayerAvatar.setAlpha(0f);
//                }else {
//                    avatarBg.setVisible(false);
//                }
//
//                syncPlayerAvatarVisual();
//            }
//        }
//    }else {
//        if(!forceRemovalWhenAnimating && isAnimating()) {
//            return;
//        }
//
//        // Remove the avatar if exists.
//        if(loggedInPlayerAvatar != null) {
//            if(loggedInPlayerAvatar.getParent() != null) {
//                loggedInPlayerAvatar.getParent().removeChild(loggedInPlayerAvatar);
//            }
//
//            loggedInPlayerAvatar = null;
//        }
//
//        avatarBg.setVisible(true);
//    }
//
//    isDirtyPlayerAvatar = false;
//}

MSVPH_LoggedIn::~MSVPH_LoggedIn() {
    //void
}
