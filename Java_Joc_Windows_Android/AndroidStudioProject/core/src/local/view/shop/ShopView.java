package local.view.shop;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Camera;
import com.badlogic.gdx.graphics.Color;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.PerspectiveCamera;
import com.badlogic.gdx.graphics.g3d.Environment;
import com.badlogic.gdx.graphics.g3d.ModelBatch;
import com.badlogic.gdx.graphics.g3d.attributes.BlendingAttribute;
import com.badlogic.gdx.graphics.g3d.attributes.ColorAttribute;
import com.badlogic.gdx.graphics.g3d.attributes.PointLightsAttribute;
import com.badlogic.gdx.graphics.g3d.attributes.TextureAttribute;
import com.badlogic.gdx.graphics.g3d.environment.PointLight;
import com.badlogic.gdx.math.Vector3;

import java.util.Map;
import java.util.Vector;

import base.IBaseAppComponent;
import base.app.util.IRenderable3D;
import base.audio.util.SoundItem;
import base.ease.EaseFunctions;
import base.interaction.TouchListener;
import base.menu.util.EaseConfig;
import base.menu.util.IMenuAnimationListener;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_Data;
import base.menu.util.MenuItemConfig_Visual;
import base.menu.util.MenuVisualAnimationProperty;
import base.menu.util.StateChangeDurations;
import base.profile.util.PlayerProfileDataEventListener;
import base.view.AppView;
import base.view.IAppView;
import base.visual.IContainer2D;
import base.visual.IInteractiveDrawable;
import base.visual.Sprite2D;
import base.visual.SpriteButton;
import base.visual.VisualUtil;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;
import base.visual3d.ModelInstance3D;
import base.visual3d.Stage3D;
import local.app.AppLocal;
import local.audio.util.SoundConfig;
import local.gameworld.util.BallConfig;
import local.gameworld.util.BallStyleConfig;
import local.gameworld.util.PlatformConfig;
import local.gameworld.util.PlatformStyleConfig;
import local.profile.PlayerProfileDataLocal;
import local.view.shop.shopview.ShopItem;
import local.view.shop.shopview.ShopItemConfig;

// This is the game title. A separate menu is used because it is animated in and out in some cases.
public class ShopView extends AppView implements IBaseAppComponent, IRenderable3D {

    private ViewShowDataBase callerViewShowData = null;
    private IAppView callerView = null;

    private SpriteButton tbtnFloors = null;
    private SpriteButton tbtnBalls = null;

    private ModelBatch batch3D;
    private Stage3D stage3D;
    private Camera camera3D;
    public Environment environment3D;

    public PointLightsAttribute categorySunAttribute;
    public PointLight categorySunLight3D;

    public PointLightsAttribute sunLight3DAttributeLeft;
    public PointLight sunLight3DLeft;

    public PointLightsAttribute sunLight3DAttributeRight;
    public PointLight sunLight3DRight;

    private Vector3 initialCameraPos = null;
    private Vector3 menuCameraPos = null;

    private Vector3 initialCameraLookAtTargetOffset = null;
    private Vector3 menuCameraLookAtTargetOffset = null;

    private Vector3 shopItems3DOrigin = new Vector3(0, 0, 0);

    private boolean listShowState = false;
    private Vector<ShopItem> itemsList = null;

    private ModelInstance3D unitCategoryModelInstance;
    private ModelInstance3D unitCategoryOutlineModelInstance;

    private ModelInstance3D platformCategoryModelInstance;
    private ModelInstance3D platformCategoryOutlineModelInstance;

    private float listScrollPercent = 0f;
    private float list2DTemplateHeight = 0;
    private float list3DHeight = 0;
    private Vector3 listPosOffset = new Vector3(0, 0, 0);

    private boolean isTouchDownActive = false;
    private boolean isDragThresholdPassed = false;
    private float dragPosY;

    private int touchDownIndex = -1;

    private float scrollDeltaYToProcess = 0f;
    private float listScrollInertia = 0f;

    private SoundItem sound_shopItem_unlockFail = null;
    private SoundItem sound_shopItem_unlock = null;
    private SoundItem sound_shopItem_activate = null;

    private static final float GRID_CELL_2D_TEMPLATE_WIDTH = 180f;
    private static final float GRID_CELL_2D_TEMPLATE_HEIGHT = 110f;

    private static final float LIST_TEMPLATE_POS_Y = 350f;

    private static final Vector3 category3DModelPosOffset_in = new Vector3(
            0f,
            10f,
            4.5f
    );
    private static final Vector3 category3DModelPosOffset_out = new Vector3(
            0f - 9f,
            10f,
            4.5f - 3f
    );

    private static final Vector3 listAnimationOffset_in = new Vector3(
            0f,
            0f,
            0f
    );
    private static final Vector3 listAnimationOffset_out = new Vector3(
            0f,
            0f,
            0f - 5f
    );


    private int activeItemsGroup = 0;

    private static final int itemUnlockCost = 10;

    private static final int ITEMS_GROUP_UNITS = 1;
    private static final int ITEMS_GROUP_PLATFORMS = 2;

    private TouchListener touchListener = new base.interaction.TouchListener() {
        @Override
        public boolean touchDown(IInteractiveDrawable target, float worldX, float worldY) {
            // Touch events are ignored while the menu is animating.
            if(!isActive()) {
                return false;
            }

            if(target == getItem(new String[] {"_root", "tbtnBalls"})) {
                setActiveItemsGroup(ITEMS_GROUP_UNITS);
            }else if(target == getItem(new String[] {"_root", "tbtnFloors"})) {
                setActiveItemsGroup(ITEMS_GROUP_PLATFORMS);
            }else if(target == getItem(new String[] {"_root", "btnBack"})) {
                hide(true, null);

                if(callerView != null) {
                    callerView.show(true, null);
                }
            }else if(target == getItem(new String[] {"_root", "imgBgOverlayMask"})) {
                //void
            }
            else {
                Gdx.app.debug("Error", "Touch event received for a unknown target.");

                return false;
            }

            return false;
        }
    };

    private TouchListener itemsTouchListener = new TouchListener() {
        @Override
        public boolean touchDown(IInteractiveDrawable target, float worldX, float worldY) {
            super.touchDown(target, worldX, worldY);

            if(!isActive()) {
                return false;
            }

            int listIndex = (Integer)target.getUserData("listIndex");

            if(isTouchDownActive) {
                isTouchDownActive = false;
            }

            touchDownIndex = listIndex;

            isTouchDownActive = true;
            isDragThresholdPassed = false;
            dragPosY = worldY;

            return true;
        }

        @Override
        public void touchMove(IInteractiveDrawable target, float worldX, float worldY) {
            super.touchMove(target, worldX, worldY);

            int listIndex = (Integer)target.getUserData("listIndex");

            if(isTouchDownActive && listIndex == touchDownIndex) {
                float moveDeltaY = worldY - dragPosY;

                if(moveDeltaY != 0) {
                    if (!isDragThresholdPassed) {
                        if (Math.abs(moveDeltaY) > (15f / 360f) * app.screenSize.width) {
                            isDragThresholdPassed = true;
                            dragPosY = worldY;
                        }
                    } else {
                        // Move the list.dd
                        float scrollDelta = (moveDeltaY * 360f / app.screenSize.width) / (list2DTemplateHeight - LIST_TEMPLATE_POS_Y);

                        setListScrollPercent(Math.max(0, Math.min(1, listScrollPercent + scrollDelta)));

                        scrollDeltaYToProcess += moveDeltaY;

                        dragPosY = worldY;
                    }
                }
            }
        }

        @Override
        public void touchUp(IInteractiveDrawable target, float worldX, float worldY) {
            super.touchUp(target,
                    worldX, worldY);

            int listIndex = (Integer)target.getUserData("listIndex");

            if(listIndex == touchDownIndex){
                if (!isTouchDownActive || !isDragThresholdPassed) {
                    // Activate or try to unlock item.

                    ShopItem shopItem = itemsList.get(listIndex);

                    // Check if item unlock is required.
                    String itemID;
                    boolean isUnlocked;
                    if(activeItemsGroup == ITEMS_GROUP_UNITS) {
                        itemID = shopItem.config.ballStyle.styleID;
                        isUnlocked = app.profileData.getIsBallStyleUnlocked(itemID);
                    }else if(activeItemsGroup == ITEMS_GROUP_PLATFORMS) {
                        itemID = shopItem.config.platformStyle.styleID;
                        isUnlocked = app.profileData.getIsPlatformStyleUnlocked(itemID);
                    }else {
                        Gdx.app.debug("Error", "Unhandled item group type.");

                        return ;
                    }

                    if(!isUnlocked) {
                        if(app.profileData.getPlayerGems() < itemUnlockCost) {
                            // Not enough points to unlock the item.

                            if(sound_shopItem_unlockFail != null) {
                                sound_shopItem_unlockFail.play();
                            }

                            return ;
                        }

                        // Unlock the item.
                        app.profileData.addPlayerGems(-itemUnlockCost);
                        if(activeItemsGroup == ITEMS_GROUP_UNITS) {
                            app.profileData.putIsBallStyleUnlocked(itemID, true);
                        }else if(activeItemsGroup == ITEMS_GROUP_PLATFORMS) {
                            app.profileData.putIsPlatformStyleUnlocked(itemID, true);
                        }else {
                            Gdx.app.debug("Error", "Unhandled item group type.");

                            return ;
                        }

                        if(sound_shopItem_unlock != null) {
                            sound_shopItem_unlock.play();
                        }
                    }else {
                        if(sound_shopItem_activate != null) {
                            sound_shopItem_activate.play();
                        }
                    }

                    // Activate the item.
                    if(activeItemsGroup == ITEMS_GROUP_UNITS) {
                        app.profileData.putActiveBallStyleId(shopItem.config.ballStyle.styleID);
                    }else if(activeItemsGroup == ITEMS_GROUP_PLATFORMS) {
                        app.profileData.putActivePlatformStyleId(shopItem.config.platformStyle.styleID);
                    }else {
                        Gdx.app.debug("Error", "Unhandled item group type.");

                        return ;
                    }
                }

                if(isTouchDownActive) {
                    isTouchDownActive = false;
                }

                touchDownIndex = -1;
            }
        }

        @Override
        public void touchUpOutside(IInteractiveDrawable target, float worldX, float worldY) {
            super.touchUpOutside(target, worldX, worldY);

            if(isTouchDownActive) {
                isTouchDownActive = false;
            }

            touchDownIndex = -1;
        }
    };

    private PlayerProfileDataEventListener playerProfileDataEventListener = new PlayerProfileDataEventListener() {
        @Override
        public void profileDataChanged(String dataID) {
            super.profileDataChanged(dataID);

            if(dataID == PlayerProfileDataLocal.ACTIVE_PLATFORM_STYLE_ID){
                if(activeItemsGroup == ITEMS_GROUP_PLATFORMS) {
                    updateCategoryModelStyle();
                }
            }

            if(dataID == PlayerProfileDataLocal.ACTIVE_BALL_STYLE_ID){
                if(activeItemsGroup == ITEMS_GROUP_UNITS) {
                    updateCategoryModelStyle();
                }
            }

            if(dataID.indexOf(PlayerProfileDataLocal.IS_UNLOCKED_PLATFORM_ID_) == 0) {
                updateItemsBuyStatus();
            }

            if(dataID.indexOf(PlayerProfileDataLocal.IS_UNLOCKED_BALL_ID_) == 0) {
                updateItemsBuyStatus();
            }
        }
    };

    private static final MenuItemConfig[] menuItemConfigs = new MenuItemConfig[] {
            new MenuItemConfig_Data("rendering3DSize", new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_appSize"}, "width")
                    .put("height", 1f, new String[] {"_appSize"}, "height")
            ),

            new MenuItemConfig_Visual("layerBase", new String[] {"_root", "visualsHolder"}, base.visual.util.VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    .putAnimating("x", new MenuVisualAnimationProperty()
                            .putState("in", 0f, null, null,
                                    new EaseConfig(EaseFunctions.easeOutCubic, null)
                            )
                            .putState("out", -1f, new String[] {"_local", "rendering3DSize"}, "width",
                                    new EaseConfig(EaseFunctions.easeOutCubic, null)
                            )
                    )
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            .putState("in", 1f, null, null, EaseConfig.DEFAULT)
                            .putState("out", 0f, null, null, EaseConfig.DEFAULT)
                    )
            ),

            new MenuItemConfig_Visual("screenDimmer", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.0f)
                    .put("x", 0f)
                    .put("y", 0f)
                    .put("alpha", 0.0f)
                    .put("tintColor", Color.valueOf("#000000FF"))
            ),

            new MenuItemConfig_Visual("touchAreaBg", new String[] {"_local", "layerBase"}, VisualCreateConfig.newTouchAreaLightweight(), new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.0f)
                    .put("x", 0f)
                    .put("y", 0f)
            ),

            new MenuItemConfig_Visual("itemsContainer", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    //void
            ),

            new MenuItemConfig_Visual("imgBgOverlayMask", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taGame.atlas", "shopView/manualMask_top"), new VisualPropertiesConfig()
                    .put("width", 360f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 140f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 1.0f)
                    .put("x", 0f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 570f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
            ),

            new MenuItemConfig_Visual("imgTitle", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taGame.atlas", "shopView/imgTitle"), new VisualPropertiesConfig()
                    .put("width", 116f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 32f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 180f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 520f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
            ),

            new MenuItemConfig_Visual("btnBack", new String[] {"_local", "layerBase"}, VisualCreateConfig.newButton("visual/taGame.atlas", "shopView/btnBack_", SoundConfig.button_down, null), new VisualPropertiesConfig()
                    .put("width", 24f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 24f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 28f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 542f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
            ),

            new MenuItemConfig_Visual("tbtnFloors", new String[] {"_local", "layerBase"}, VisualCreateConfig.newButton("visual/taGame.atlas", "shopView/tbtnFloors_", SoundConfig.button_down, null), new VisualPropertiesConfig()
                    .put("width", 124f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 36f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 84f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 450f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
            ),
            new MenuItemConfig_Visual("tbtnBalls", new String[] {"_local", "layerBase"}, VisualCreateConfig.newButton("visual/taGame.atlas", "shopView/tbtnBalls_", SoundConfig.button_down, null), new VisualPropertiesConfig()
                    .put("width", 123f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 36f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 276f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 450f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
            )
    };

    private static final StateChangeDurations[] menuAnimationDurations = new StateChangeDurations[] {
            new StateChangeDurations("_out_", "_in_", 0.5f),
            new StateChangeDurations("_in_", "_out_", 0.5f)
    };

    public ShopView(AppLocal app) {
        super(app, menuItemConfigs, menuAnimationDurations);

        //void
    }

    @Override
    protected void createVisuals(Map<String, Object> createConfigMap) {
        super.createVisuals(createConfigMap);

        tbtnFloors = (SpriteButton) getItem(new String[]{"_root", "tbtnFloors"});
        tbtnBalls = (SpriteButton) getItem(new String[]{"_root", "tbtnBalls"});

        ((IInteractiveDrawable) getItem(new String[]{"_root", "touchAreaBg"})).setTouchListener(touchListener);

        ((IInteractiveDrawable) getItem(new String[]{"_root", "btnBack"})).setTouchListener(touchListener);
        ((IInteractiveDrawable) getItem(new String[]{"_root", "tbtnFloors"})).setTouchListener(touchListener);
        ((IInteractiveDrawable) getItem(new String[]{"_root", "tbtnBalls"})).setTouchListener(touchListener);

        ((IInteractiveDrawable) getItem(new String[]{"_root", "imgBgOverlayMask"})).setIsInputInteractive(true);
        ((IInteractiveDrawable) getItem(new String[]{"_root", "imgBgOverlayMask"})).setTouchListener(touchListener);

        categorySunAttribute = new PointLightsAttribute();
        categorySunLight3D = new PointLight();
        categorySunLight3D.set(Color.WHITE, 0f, 0f, 0f, 3f);
        categorySunAttribute.lights.add(categorySunLight3D);

        sunLight3DAttributeLeft = new PointLightsAttribute();
        sunLight3DLeft = new PointLight();
        sunLight3DLeft.set(Color.WHITE, 0f, 0f, 0f, 3f);
        sunLight3DAttributeLeft.lights.add(sunLight3DLeft);

        sunLight3DAttributeRight = new PointLightsAttribute();
        sunLight3DRight = new PointLight();
        sunLight3DRight.set(Color.WHITE, 0f, 0f, 0f, 3f);
        sunLight3DAttributeRight.lights.add(sunLight3DRight);

        PlatformConfig platformConfig = PlatformConfig.Platform_ShopPreview;
        BallConfig ballConfig = BallConfig.PlayerSphereA;

        unitCategoryModelInstance = app.embeddedAssetsManager.getModelInstance(ballConfig.modelID);
        unitCategoryOutlineModelInstance = app.embeddedAssetsManager.getModelInstance(ballConfig.outlineModelID);

        platformCategoryModelInstance = app.embeddedAssetsManager.getModelInstance(platformConfig.modelID);
        platformCategoryOutlineModelInstance = app.embeddedAssetsManager.getModelInstance(platformConfig.outlineModelID);

        create_shopItems();

        if(SoundConfig.shopItem_unlockFail != null) {
            sound_shopItem_unlockFail = app.soundManager.hookNewSoundItem(SoundConfig.shopItem_unlockFail);
        }
        if(SoundConfig.shopItem_unlock != null) {
            sound_shopItem_unlock = app.soundManager.hookNewSoundItem(SoundConfig.shopItem_unlock);
        }
        if(SoundConfig.shopItem_activate != null) {
            sound_shopItem_activate = app.soundManager.hookNewSoundItem(SoundConfig.shopItem_activate);
        }

        // Prepare 3d world.
        batch3D = new ModelBatch();

        stage3D = new Stage3D();
        stage3D.create();

        // Perspective camera.
        camera3D = new PerspectiveCamera(75, Gdx.graphics.getWidth(), Gdx.graphics.getHeight());

        camera3D.near = 0.01f;
        camera3D.far = 300.0f;

        environment3D = new Environment();
    }

    @Override
    protected void activeStateChangedEvent() {
        super.activeStateChangedEvent();

        if(!isActive()) {
            app.getPlayerCurrencyView().hideIncrement();
        }else {
            app.getPlayerCurrencyView().showIncrement();

            app.addRenderable3D(this);

            if(activeItemsGroup == 0) {
                setActiveItemsGroup(ITEMS_GROUP_PLATFORMS);
            }

            app.getCurrentGameWorld().hookCinematicCamera();
            initialCameraPos = new Vector3(app.getCurrentGameWorld().getCamera3DPosition());
            menuCameraPos = new Vector3(initialCameraPos.x - 16f, initialCameraPos.y, initialCameraPos.z);

            Vector3 camera3DLookAtTarget = new Vector3(app.getCurrentGameWorld().getCamera3DLookAtTarget());

            initialCameraLookAtTargetOffset = new Vector3(
                    camera3DLookAtTarget.x - initialCameraPos.x,
                    camera3DLookAtTarget.y - initialCameraPos.y,
                    camera3DLookAtTarget.z - initialCameraPos.z);
            menuCameraLookAtTargetOffset = new Vector3(
                    0,
                    camera3DLookAtTarget.y - initialCameraPos.y,
                    0);

            // Compute the offset for the 3d models.
            shopItems3DOrigin.set(
                    menuCameraPos.x,
                    menuCameraPos.y + 8f,
                    menuCameraPos.z
            );

            categorySunLight3D.setPosition(
                    menuCameraPos.x + category3DModelPosOffset_in.x,
                    menuCameraPos.y + category3DModelPosOffset_in.y - 1f,
                    menuCameraPos.z + category3DModelPosOffset_in.z + 3f
            );

            sunLight3DLeft.setPosition(
                    menuCameraPos.x - 3,
                    menuCameraPos.y + 15f,
                    menuCameraPos.z
            );

            sunLight3DRight.setPosition(
                    menuCameraPos.x + 3,
                    menuCameraPos.y + 15f,
                    menuCameraPos.z
            );


            setShop3DModelsShowState(true);
            setListScrollPercent(0f);
            syncListVisuals();

            updateCategoryModelStyle();
        }
    }

    @Override
    public void render_pre() {
        //void
    }

    @Override
    public void handle3DRendering() {
        stage3D.batchDraw(batch3D, camera3D, environment3D);
    }

    private void setActiveItemsGroup(int activeItemsGroup) {
        if(this.activeItemsGroup == activeItemsGroup) {
            return ;
        }

        if(this.activeItemsGroup == ITEMS_GROUP_UNITS) {
            tbtnBalls.setSelected(false);
        }else if(this.activeItemsGroup == ITEMS_GROUP_PLATFORMS) {
            tbtnFloors.setSelected(false);
        }

        if(listShowState) {
            if(this.activeItemsGroup == ITEMS_GROUP_UNITS) {
                stage3D.removeChild(unitCategoryModelInstance);
                stage3D.removeChild(unitCategoryOutlineModelInstance);
            }else if(this.activeItemsGroup == ITEMS_GROUP_PLATFORMS) {
                stage3D.removeChild(platformCategoryModelInstance);
                stage3D.removeChild(platformCategoryOutlineModelInstance);
            }
        }

        this.activeItemsGroup = activeItemsGroup;

        if(this.activeItemsGroup == ITEMS_GROUP_UNITS) {
            tbtnBalls.setSelected(true);
        }else if(this.activeItemsGroup == ITEMS_GROUP_PLATFORMS) {
            tbtnFloors.setSelected(true);
        }

        if(listShowState) {
            if(this.activeItemsGroup == ITEMS_GROUP_UNITS) {
                stage3D.addChild(unitCategoryModelInstance);
                stage3D.addChild(unitCategoryOutlineModelInstance);
            }else if(this.activeItemsGroup == ITEMS_GROUP_PLATFORMS) {
                stage3D.addChild(platformCategoryModelInstance);
                stage3D.addChild(platformCategoryOutlineModelInstance);
            }
        }

        updateItemStyles();
        updateItemsBuyStatus();
        setListScrollPercent(0f);

        updateCategoryModelStyle();
    }

    @Override
    protected void isOnDisplayListChanged() {
        super.isOnDisplayListChanged();

        if(!isOnDisplayList()) {
            app.profileData.removeEventsListener(playerProfileDataEventListener);

            app.getCurrentGameWorld().releaseCinematicCamera();

            setShop3DModelsShowState(false);

            app.removeRenderable3D(this);
        } else {
            app.profileData.addEventsListener(playerProfileDataEventListener);
        }
    }

    private void setShop3DModelsShowState(boolean listShowState) {
        if(this.listShowState == listShowState) {
            return ;
        }

        this.listShowState = listShowState;
        if(itemsList != null) {
            for(int i=0;i<itemsList.size();i++) {
                ShopItem shopItem = itemsList.get(i);

                if(!listShowState) {
                    stage3D.removeChild(shopItem.getPlatformModelInstance());
                    stage3D.removeChild(shopItem.getPlatformOutlineModelInstance());

                    stage3D.removeChild(shopItem.getUnitModelInstance());
                    stage3D.removeChild(shopItem.getUnitOutlineModelInstance());
                    stage3D.removeChild(shopItem.getUnitShadowModelInstance());
                }else {
                    stage3D.addChild(shopItem.getPlatformModelInstance());
                    stage3D.addChild(shopItem.getPlatformOutlineModelInstance());

                    stage3D.addChild(shopItem.getUnitModelInstance());
                    stage3D.addChild(shopItem.getUnitOutlineModelInstance());
                    stage3D.addChild(shopItem.getUnitShadowModelInstance());
                }
            }
        }

        if(!listShowState) {
            if(this.activeItemsGroup == ITEMS_GROUP_UNITS) {
                stage3D.removeChild(unitCategoryModelInstance);
                stage3D.removeChild(unitCategoryOutlineModelInstance);
            }else if(this.activeItemsGroup == ITEMS_GROUP_PLATFORMS) {
                stage3D.removeChild(platformCategoryModelInstance);
                stage3D.removeChild(platformCategoryOutlineModelInstance);
            }
        }else {
            if(this.activeItemsGroup == ITEMS_GROUP_UNITS) {
                stage3D.addChild(unitCategoryModelInstance);
                stage3D.addChild(unitCategoryOutlineModelInstance);
            }else if(this.activeItemsGroup == ITEMS_GROUP_PLATFORMS) {
                stage3D.addChild(platformCategoryModelInstance);
                stage3D.addChild(platformCategoryOutlineModelInstance);
            }
        }

        if(listShowState) {
            updateItemsBuyStatus();
        }
    }

    private void updateItemStyles() {
        BallStyleConfig activeBallStyle = BallStyleConfig.entriesMap.get(app.profileData.getActiveBallStyleId());
        PlatformStyleConfig activePlatformStyle = PlatformStyleConfig.entriesMap.get(app.profileData.getActivePlatformStyleId());

        for(int i=0;i<itemsList.size();i++) {
            ShopItem shopItem = itemsList.get(i);

            PointLightsAttribute rowSunAttribute;
            if((i % 2) == 0) {
                rowSunAttribute = sunLight3DAttributeLeft;
            }else {
                rowSunAttribute = sunLight3DAttributeRight;
            }

            if(this.activeItemsGroup == ITEMS_GROUP_UNITS) {
                shopItem.getUnitModelInstance().materials.get(0).set(TextureAttribute.createDiffuse(app.embeddedAssetsManager.getTextureAtlasRegion(shopItem.config.ballStyle.textureAtlasID, shopItem.config.ballStyle.textureRegionID)));
                shopItem.getUnitModelInstance().materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 1.0f, 1.0f, 1.0f, 1.0f));
                shopItem.getUnitModelInstance().materials.get(0).set(rowSunAttribute);

                shopItem.getUnitOutlineModelInstance().materials.get(0).set(ColorAttribute.createDiffuse(shopItem.config.ballStyle.outlineDiffuseColor));
                shopItem.getUnitOutlineModelInstance().materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));
                shopItem.getUnitOutlineModelInstance().materials.get(0).set(rowSunAttribute);

                shopItem.getUnitShadowModelInstance().materials.get(0).set(TextureAttribute.createDiffuse(app.embeddedAssetsManager.getTextureAtlasRegion("visual/taTextures.atlas", "modelTextures/balls/ballShadow")));
                shopItem.getUnitShadowModelInstance().materials.get(0).set(new BlendingAttribute(GL20.GL_SRC_ALPHA, GL20.GL_ONE_MINUS_SRC_ALPHA));
                shopItem.getUnitShadowModelInstance().materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));
                shopItem.getUnitShadowModelInstance().materials.get(0).set(rowSunAttribute);

                // Set the active platform style.
                shopItem.getPlatformModelInstance().materials.get(0).set(ColorAttribute.createDiffuse(activePlatformStyle.platformDiffuseColor));
                shopItem.getPlatformModelInstance().materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));
                shopItem.getPlatformModelInstance().materials.get(0).set(rowSunAttribute);

                shopItem.getPlatformOutlineModelInstance().materials.get(0).set(ColorAttribute.createDiffuse(activePlatformStyle.outlineDiffuseColor));
                shopItem.getPlatformOutlineModelInstance().materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));
                shopItem.getPlatformOutlineModelInstance().materials.get(0).set(rowSunAttribute);
            }else if(this.activeItemsGroup == ITEMS_GROUP_PLATFORMS) {
                // Set the active unit style.
                shopItem.getUnitModelInstance().materials.get(0).set(TextureAttribute.createDiffuse(app.embeddedAssetsManager.getTextureAtlasRegion(activeBallStyle.textureAtlasID, activeBallStyle.textureRegionID)));
                shopItem.getUnitModelInstance().materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));
                shopItem.getUnitModelInstance().materials.get(0).set(rowSunAttribute);

                shopItem.getUnitOutlineModelInstance().materials.get(0).set(ColorAttribute.createDiffuse(activeBallStyle.outlineDiffuseColor));
                shopItem.getUnitOutlineModelInstance().materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));
                shopItem.getUnitOutlineModelInstance().materials.get(0).set(rowSunAttribute);

                shopItem.getUnitShadowModelInstance().materials.get(0).set(TextureAttribute.createDiffuse(app.embeddedAssetsManager.getTextureAtlasRegion("visual/taTextures.atlas", "modelTextures/balls/ballShadow")));
                shopItem.getUnitShadowModelInstance().materials.get(0).set(new BlendingAttribute(GL20.GL_SRC_ALPHA, GL20.GL_ONE_MINUS_SRC_ALPHA));
                shopItem.getUnitShadowModelInstance().materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));
                shopItem.getUnitShadowModelInstance().materials.get(0).set(rowSunAttribute);

                shopItem.getPlatformModelInstance().materials.get(0).set(ColorAttribute.createDiffuse(shopItem.config.platformStyle.platformDiffuseColor));
                shopItem.getPlatformModelInstance().materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));
                shopItem.getPlatformModelInstance().materials.get(0).set(rowSunAttribute);

                shopItem.getPlatformOutlineModelInstance().materials.get(0).set(ColorAttribute.createDiffuse(shopItem.config.platformStyle.outlineDiffuseColor));
                shopItem.getPlatformOutlineModelInstance().materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));
                shopItem.getPlatformOutlineModelInstance().materials.get(0).set(rowSunAttribute);
            }
        }
    }

    private void updateItemsBuyStatus() {
        int playerGems = app.profileData.getPlayerGems();

        for(int i=0;i<itemsList.size();i++) {
            ShopItem shopItem = itemsList.get(i);

            boolean isUnlocked;
            if(this.activeItemsGroup == ITEMS_GROUP_UNITS) {
                isUnlocked = app.profileData.getIsBallStyleUnlocked(shopItem.config.ballStyle.styleID);
            }else if(this.activeItemsGroup == ITEMS_GROUP_PLATFORMS) {
                isUnlocked = app.profileData.getIsPlatformStyleUnlocked(shopItem.config.platformStyle.styleID);
            }else {
                Gdx.app.debug("Error", "Unhandled item group type.");

                return ;
            }

            if(isUnlocked) {
                shopItem.imgLockTooExpensive.setVisible(false);
                shopItem.imgLockUnlockable.setVisible(false);

                shopItem.unlockCostImage.setVisible(false);
            }else if(playerGems >= itemUnlockCost) {
                shopItem.imgLockTooExpensive.setVisible(false);
                shopItem.imgLockUnlockable.setVisible(true);

                shopItem.unlockCostImage.setVisible(true);
            }else {
                shopItem.imgLockTooExpensive.setVisible(true);
                shopItem.imgLockUnlockable.setVisible(false);

                shopItem.unlockCostImage.setVisible(true);
            }
        }
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        if(scrollDeltaYToProcess != 0f) {
            // This will make the inertia work better.
            if(scrollDeltaYToProcess > 0 && listScrollInertia < 0
                    || scrollDeltaYToProcess < 0 && listScrollInertia > 0)
            {
                listScrollInertia = 0f;
            }

            if(listScrollInertia == 0f) {
                listScrollInertia = scrollDeltaYToProcess * 0.5f;
            }else {
                listScrollInertia = (listScrollInertia * 8f + scrollDeltaYToProcess * 2f) / 10f;
            }

            scrollDeltaYToProcess = 0f;
        }

        if(!isTouchDownActive && listScrollInertia != 0) {
            listScrollInertia = listScrollInertia * 0.75f;

            if(Math.abs(listScrollInertia) < 0.0001f) {
                listScrollInertia = 0;
            }

            float newListPercent = Math.max(0, Math.min(1, listScrollPercent + listScrollInertia / app.screenSize.width));
            setListScrollPercent(newListPercent);
            if(listScrollInertia > 0 && newListPercent >= 1) {
                listScrollInertia = 0;
            }else if(listScrollInertia < 0 && newListPercent <= 0) {
                listScrollInertia = 0;
            }
        }

        if(isActive() && !isAnimating() && itemsList != null) {
            for(int i=0;i<itemsList.size();i++) {
                ShopItem shopItem = itemsList.get(i);

                // Rotating the shop balls doesn't look nice.
                // shopItem.setAng3DRX(shopItem.getAng3DRX() - 1.57f * 360f * deltaS, true);
            }
        }
    }

    private void setListScrollPercent(float listScrollPercent) {
        this.listScrollPercent = listScrollPercent;

        syncListVisuals();
    }

    private void syncListVisuals() {
        IContainer2D itemsContainer = (IContainer2D)getItem(new String[] {"_root", "itemsContainer"});
        float listPosX = app.screenSize.width / 2f;
        float listPosY = ((LIST_TEMPLATE_POS_Y + (list2DTemplateHeight - LIST_TEMPLATE_POS_Y) * listScrollPercent) / 360f) * app.screenSize.width;
        itemsContainer.setPosition(listPosX, listPosY);

        for(int i=0;i<itemsList.size();i++) {
            ShopItem shopItem = itemsList.get(i);

            float posX = -4 + (i % 2) * 8;
            float posZ = (float)(3 - Math.floor(i / 2) * 5);

            shopItem.setPos3D(
                    shopItems3DOrigin.x + posX + listPosOffset.x,
                    shopItems3DOrigin.y + 10 + listPosOffset.y,
                    shopItems3DOrigin.z + posZ + listPosOffset.z + (list3DHeight - 16.0f) * listScrollPercent,
                    true
            );
        }
    }

    private void updateCategoryModelStyle() {
        if(activeItemsGroup == ITEMS_GROUP_UNITS) {
            BallStyleConfig activeBallStyle = BallStyleConfig.entriesMap.get(app.profileData.getActiveBallStyleId());

            unitCategoryModelInstance.materials.get(0).set(TextureAttribute.createDiffuse(app.embeddedAssetsManager.getTextureAtlasRegion(activeBallStyle.textureAtlasID, activeBallStyle.textureRegionID)));
            unitCategoryModelInstance.materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 1.0f, 1.0f, 1.0f, 1.0f));
            unitCategoryModelInstance.materials.get(0).set(categorySunAttribute);

            unitCategoryOutlineModelInstance.materials.get(0).set(ColorAttribute.createDiffuse(activeBallStyle.outlineDiffuseColor));
            unitCategoryOutlineModelInstance.materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));
            unitCategoryOutlineModelInstance.materials.get(0).set(categorySunAttribute);
        }else if(activeItemsGroup == ITEMS_GROUP_PLATFORMS) {
            PlatformStyleConfig activePlatformStyle = PlatformStyleConfig.entriesMap.get(app.profileData.getActivePlatformStyleId());

            platformCategoryModelInstance.materials.get(0).set(ColorAttribute.createDiffuse(activePlatformStyle.platformDiffuseColor));
            platformCategoryModelInstance.materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));
            platformCategoryModelInstance.materials.get(0).set(categorySunAttribute);

            platformCategoryOutlineModelInstance.materials.get(0).set(ColorAttribute.createDiffuse(activePlatformStyle.outlineDiffuseColor));
            platformCategoryOutlineModelInstance.materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));
            platformCategoryOutlineModelInstance.materials.get(0).set(categorySunAttribute);
        }else {
            Gdx.app.debug("Error", "Unhandled item group type.");

            return ;
        }
    }

    private void create_shopItems() {
        itemsList = new Vector<ShopItem>();

        IContainer2D itemsContainer = (IContainer2D)getItem(new String[] {"_root", "itemsContainer"});

        PlatformConfig platformConfig = PlatformConfig.Platform_ShopPreview;
        BallConfig ballConfig = BallConfig.PlayerSphereA;

        PlatformStyleConfig platformStyle = null;
        BallStyleConfig ballStyle = null;
        for(int i=0;i<PlatformStyleConfig.entriesList.size() || i<BallStyleConfig.entriesList.size();i++) {
            if(i < PlatformStyleConfig.entriesList.size()) {
                platformStyle = PlatformStyleConfig.entriesList.get(i);
            }
            if(i < BallStyleConfig.entriesList.size()) {
                ballStyle = BallStyleConfig.entriesList.get(i);
            }

            ShopItem shopItem = newShopItem(new ShopItemConfig(platformStyle, ballStyle), i, platformConfig, ballConfig, itemsContainer);

            itemsList.add(shopItem);

            if((i % 2) == 0) {
                list2DTemplateHeight += GRID_CELL_2D_TEMPLATE_HEIGHT;

                list3DHeight += 5;
            }
        }
    }

    private ShopItem newShopItem(ShopItemConfig shopItemConfig, int listIndex, PlatformConfig platformConfig, BallConfig ballConfig, IContainer2D itemsContainer) {
        float itemGridCellWidth = (GRID_CELL_2D_TEMPLATE_WIDTH / 360f) * app.screenSize.width;
        float itemGridCellHeight = (GRID_CELL_2D_TEMPLATE_HEIGHT / 360f) * app.screenSize.width;

        float visualPosX = -itemGridCellWidth / 2 + (listIndex % 2) * itemGridCellWidth;
        float visualPosY = (float)(-Math.floor(listIndex / 2) * itemGridCellHeight);

        float iconOffsetX;
        if((listIndex % 2) == 0) {
            iconOffsetX = (0f / 360f) * app.screenSize.width;
        }else {
            iconOffsetX = -(0f / 360f) * app.screenSize.width;
        }

        IContainer2D container2D = VisualUtil.newGroup(itemsContainer);
        container2D.setPosition(visualPosX, visualPosY);

        Sprite2D touchImage = VisualUtil.newSprite(
                app,
                "visual/taGame.atlas", "general/debugTouchDetector",
                0, 0,//x, y
                itemGridCellWidth, itemGridCellHeight,// width, height
                0.5f, 0.5f,// anchorX, anchorY
                container2D
        );
        touchImage.setIsInputInteractive(true);
        touchImage.setAlpha(0.0f);
        touchImage.putUserData("listIndex", new Integer(listIndex));
        touchImage.setTouchListener(itemsTouchListener);

        Sprite2D imgLockTooExpensive = VisualUtil.newSprite(
                app,
                "visual/taGame.atlas", "shopView/imgItemTooExpensive",
                iconOffsetX + (0f / 360f) * app.screenSize.width, (20f / 360f) * app.screenSize.width,//x, y
                (32f / 360f) * app.screenSize.width, (42f / 360f) * app.screenSize.width,// width, height
                0.5f, 0.5f,// anchorX, anchorY
                container2D
        );

        Sprite2D imgLockUnlockable = VisualUtil.newSprite(
                app,
                "visual/taGame.atlas", "shopView/imgItemUnlockable",
                iconOffsetX + (0f / 360f) * app.screenSize.width, (20f / 360f) * app.screenSize.width,//x, y
                (32f / 360f) * app.screenSize.width, (42f / 360f) * app.screenSize.width,// width, height
                0.5f, 0.5f,// anchorX, anchorY
                container2D
        );

        Sprite2D unlockCostImage = VisualUtil.newSprite(
                app,
                "visual/taGame.atlas", "shopView/imgItemUnlockCost",
                iconOffsetX + (0f / 360f) * app.screenSize.width, (-21f / 360f) * app.screenSize.width,//x, y
                (52f / 360f) * app.screenSize.width, (24f / 360f) * app.screenSize.width,// width, height
                0.5f, 0.5f,// anchorX, anchorY
                container2D
        );

        ModelInstance3D platformModelInstance = app.embeddedAssetsManager.getModelInstance(platformConfig.modelID);
        ModelInstance3D platformOutlineModelInstance = app.embeddedAssetsManager.getModelInstance(platformConfig.outlineModelID);

        ModelInstance3D unitModelInstance = app.embeddedAssetsManager.getModelInstance(ballConfig.modelID);
        ModelInstance3D unitOutlineModelInstance = app.embeddedAssetsManager.getModelInstance(ballConfig.outlineModelID);
        ModelInstance3D unitShadowModelInstance = app.embeddedAssetsManager.getModelInstance(ballConfig.shadowModelID);

        ShopItem shopItem = new ShopItem(
                shopItemConfig,
                platformConfig,
                ballConfig,
                container2D, touchImage,
                imgLockTooExpensive, imgLockUnlockable, unlockCostImage,
                platformModelInstance, platformOutlineModelInstance,
                unitModelInstance, unitOutlineModelInstance, unitShadowModelInstance,
                1.4f, 2f
        );

        return shopItem;
    }

    @Override
    protected void syncVisuals(boolean syncAbsoluteValues, boolean syncAnimatingValues, String startStateID, String endStateID, float stateCompletionPercent) {
        super.syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

        if(syncAnimatingValues) {
            float animationPercent = EaseFunctions.computeEaseValue(EaseFunctions.easeOutCubic, stateCompletionPercent);

            Vector3 initialPos, finalPos;
            Vector3 initialLookAtTargetPosOffset, finalLookAtTargetPosOffset;
            Vector3 initialCategory3DModelPos, finalCategory3DModelPos;
            Vector3 initialListOffset, finalListOffset;
            if (!isActive()) {
                initialPos = menuCameraPos;
                finalPos = initialCameraPos;

                initialLookAtTargetPosOffset = menuCameraLookAtTargetOffset;
                finalLookAtTargetPosOffset = initialCameraLookAtTargetOffset;

                initialCategory3DModelPos = category3DModelPosOffset_in;
                finalCategory3DModelPos = category3DModelPosOffset_out;

                initialListOffset = listAnimationOffset_in;
                finalListOffset = listAnimationOffset_out;
            } else {
                initialPos = initialCameraPos;
                finalPos = menuCameraPos;

                initialLookAtTargetPosOffset = initialCameraLookAtTargetOffset;
                finalLookAtTargetPosOffset = menuCameraLookAtTargetOffset;

                initialCategory3DModelPos = category3DModelPosOffset_out;
                finalCategory3DModelPos = category3DModelPosOffset_in;

                initialListOffset = listAnimationOffset_out;
                finalListOffset = listAnimationOffset_in;
            }


            float camX = initialPos.x + (finalPos.x - initialPos.x) * animationPercent;
            float camY = initialPos.y + (finalPos.y - initialPos.y) * animationPercent;
            float camZ = initialPos.z + (finalPos.z - initialPos.z) * animationPercent;
            camera3D.position.set(
                    camX,
                    camY,
                    camZ
            );

            float lookTargetX = camera3D.position.x + (initialLookAtTargetPosOffset.x + (finalLookAtTargetPosOffset.x - initialLookAtTargetPosOffset.x) * animationPercent);
            float lookTargetY = camera3D.position.y + (initialLookAtTargetPosOffset.y + (finalLookAtTargetPosOffset.y - initialLookAtTargetPosOffset.y) * animationPercent);
            float lookTargetZ = camera3D.position.z + (initialLookAtTargetPosOffset.z + (finalLookAtTargetPosOffset.z - initialLookAtTargetPosOffset.z) * animationPercent);
            camera3D.lookAt(
                    lookTargetX,
                    lookTargetY,
                    lookTargetZ
            );

            app.getCurrentGameWorld().setCinematicCameraPos(
                    camX,
                    camY,
                    camZ
            );

            app.getCurrentGameWorld().setCinematicCameraLookAt(
                    lookTargetX,
                    lookTargetY,
                    lookTargetZ
            );

            unitCategoryModelInstance.transform.idt().translate(
                    camera3D.position.x + (initialCategory3DModelPos.x + (finalCategory3DModelPos.x - initialCategory3DModelPos.x) * animationPercent),
                    camera3D.position.y + (initialCategory3DModelPos.y + (finalCategory3DModelPos.y - initialCategory3DModelPos.y) * animationPercent),
                    camera3D.position.z + (initialCategory3DModelPos.z + (finalCategory3DModelPos.z - initialCategory3DModelPos.z) * animationPercent)
            ).scale(
                    1.4f,
                    1.4f,
                    1.4f
            );

            unitCategoryOutlineModelInstance.transform.idt().translate(
                    camera3D.position.x + (initialCategory3DModelPos.x + (finalCategory3DModelPos.x - initialCategory3DModelPos.x) * animationPercent),
                    camera3D.position.y + (initialCategory3DModelPos.y + (finalCategory3DModelPos.y - initialCategory3DModelPos.y) * animationPercent),
                    camera3D.position.z + (initialCategory3DModelPos.z + (finalCategory3DModelPos.z - initialCategory3DModelPos.z) * animationPercent)
            ).scale(
                    1.4f * 1.2f,
                    1.4f * 1.2f,
                    1.4f * 1.2f
            );


            platformCategoryModelInstance.transform.idt().translate(
                    camera3D.position.x + (initialCategory3DModelPos.x + (finalCategory3DModelPos.x - initialCategory3DModelPos.x) * animationPercent),
                    camera3D.position.y + (initialCategory3DModelPos.y + (finalCategory3DModelPos.y - initialCategory3DModelPos.y) * animationPercent),
                    camera3D.position.z + (initialCategory3DModelPos.z + (finalCategory3DModelPos.z - initialCategory3DModelPos.z) * animationPercent)
            ).scale(
                    0.5f,
                    0.5f,
                    0.8f
            ).rotate(Vector3.X, 35f);

            platformCategoryOutlineModelInstance.transform.idt().translate(
                    camera3D.position.x + (initialCategory3DModelPos.x + (finalCategory3DModelPos.x - initialCategory3DModelPos.x) * animationPercent),
                    camera3D.position.y + (initialCategory3DModelPos.y + (finalCategory3DModelPos.y - initialCategory3DModelPos.y) * animationPercent),
                    camera3D.position.z + (initialCategory3DModelPos.z + (finalCategory3DModelPos.z - initialCategory3DModelPos.z) * animationPercent)
            ).scale(
                    0.5f * 1.2f,
                    0.5f * 1.2f,
                    0.8f * 1.2f
            ).rotate(Vector3.X, 35f);

            listPosOffset.set(
                    initialListOffset.x + (finalListOffset.x - initialListOffset.x) * animationPercent,
                    initialListOffset.y + (finalListOffset.y - initialListOffset.y) * animationPercent,
                    initialListOffset.z + (finalListOffset.z - initialListOffset.z) * animationPercent
            );
            syncListVisuals();

            camera3D.update();
        }
    }

    public void show(boolean doAnimation, IMenuAnimationListener menuAnimationListener, IAppView callerView) {
        this.callerView = callerView;

        super.show(doAnimation, menuAnimationListener);
    }

    @Override
    public void show(boolean doAnimation, IMenuAnimationListener menuAnimationListener) {
        //super.show(doAnimation, menuAnimationListener);

        show(doAnimation, menuAnimationListener, null);
    }

}
