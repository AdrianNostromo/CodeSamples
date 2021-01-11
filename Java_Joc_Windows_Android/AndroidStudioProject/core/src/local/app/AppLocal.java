package local.app;

import com.badlogic.gdx.Gdx;

import base.fx.FXManager;
import base.launcher.IAppLauncher;
import base.launcher.ILocalDataHandler;
import base.launcher.IWebManager;
import base.util.Point2D;
import base.visual.Container2D;
import base.visual.Sprite2D;
import base.visual.VisualUtil;
import local.ads.config.RewardVideoAdsConfig;
import local.ads.launcher.IAdsManager;
import local.ads.util.AdsEventListener;
import local.gameworld.GameWorld;
import local.playgames.app.AppPlayGames;
import local.playgames.launcher.IPlayGamesManager;
import local.view.levelend.LevelEndView;
import local.view.main.MainView;
import local.view.playercurrency.PlayerCurrencyView;
import local.view.shop.ShopView;
import local.view.splashscreen.SplashScreenView;

// Container class for general app components.
public class AppLocal extends AppPlayGames {

    public IAdsManager adsManager;

    private Sprite2D imgAppBg = null;

    private SplashScreenView splashScreenView = null;
    private MainView mainView = null;
    private PlayerCurrencyView playerCurrencyView = null;
    private ShopView shopView = null;
    private LevelEndView levelEndView = null;

    private Container2D layerGameWorld;
    private Container2D layerMenus;
    private Container2D layerMainView;
    private Container2D layerPlayerCurrencyView;
    private Container2D layerSidebar;
    private Container2D layerSplashScreenView;

    private GameWorld gameWorld;

    private AdsEventListener adsEventListener = new AdsEventListener() {
        @Override
        public void rewardVideoComplete(int rewardedVideoID) {
            super.rewardVideoComplete(rewardedVideoID);

            if(RewardVideoAdsConfig.REWARD_VIDEO_GEMS == rewardedVideoID) {
                //asd_002;
                //playerProfile.addPlayerCurrencyA(200);
                Gdx.app.debug("Error", "Error. Not Implemented. 2.");
            }
        }
    };

    public AppLocal(IAppLauncher appLauncher, ILocalDataHandler localDataHandler, IWebManager webManager, IPlayGamesManager playGamesManager, IAdsManager adsManager) {
        super(appLauncher, localDataHandler, webManager, playGamesManager);

        this.adsManager = adsManager;
    }

    @Override
    public void create() {
        super.create();

        if(adsManager != null) {
            adsManager.addEventsListener(adsEventListener);
        }
    }

    @Override
    protected void createLayers() {
        layerGameWorld = VisualUtil.newGroup(stage);
        layerMenus = VisualUtil.newGroup(stage);
        layerMainView = VisualUtil.newGroup(stage);
        layerPlayerCurrencyView = VisualUtil.newGroup(stage);
        layerSidebar = VisualUtil.newGroup(stage);

        super.createLayers();

        layerSplashScreenView = VisualUtil.newGroup(stage);
    }

    @Override
    public void assetsPreloadFinished() {
        super.assetsPreloadFinished();

        //asd_tst_001;
        /*
        imgAppBg = VisualUtil.newSprite(this, "visual/taGame.atlas", "general/imgBg", null);
        imgAppBg.setSize(screenSize.width, screenSize.height);
        imgAppBg.setPosition(0f, 0f);
        imgAppBg.setAnchor(0f, 0f);
        backLayerBackground.addChild(imgAppBg);*/
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        if(gameWorld != null) {
            gameWorld.appLoop(deltaS, deltaMS);
        }
    }

    @Override
    protected Point2D computeScreenDensityMultiplier() {
        Point2D ret = super.computeScreenDensityMultiplier();

        // Make the interface larger than the standard.
        ret.x /= 0.6f;
        ret.y /= 0.6f;

        return ret;
    }

    public boolean isSplashScreenView() {
        if(splashScreenView != null) {
            return true;
        }

        return false;
    }

    public SplashScreenView getSplashScreenView() {
        if(splashScreenView == null) {
            appComponentsList.add(splashScreenView = new SplashScreenView(this));
            splashScreenView.create();
            layerSplashScreenView.addChild(splashScreenView.getVisualsHolder());
        }

        return splashScreenView;
    }

    public MainView getMainView() {
        if(mainView == null) {
            appComponentsList.add(mainView = new MainView(this));
            mainView.create();
            layerMainView.addChild(mainView.getVisualsHolder());
        }

        return mainView;
    }

    public PlayerCurrencyView getPlayerCurrencyView() {
        if(playerCurrencyView == null) {
            appComponentsList.add(playerCurrencyView = new PlayerCurrencyView(this));
            playerCurrencyView.create();
            layerPlayerCurrencyView.addChild(playerCurrencyView.getVisualsHolder());
        }

        return playerCurrencyView;
    }

    public ShopView getShopView() {
        if(shopView == null) {
            appComponentsList.add(shopView = new ShopView(this));
            shopView.create();
            layerMenus.addChild(shopView.getVisualsHolder());
        }

        return shopView;
    }

    public LevelEndView getLevelEndView() {
        if(levelEndView == null) {
            appComponentsList.add(levelEndView = new LevelEndView(this));
            levelEndView.create();
            layerMenus.addChild(levelEndView.getVisualsHolder());
        }

        return levelEndView;
    }

    public GameWorld getCurrentGameWorld() {
        return gameWorld;
    }

    public GameWorld getNewGameWorld() {
        if(gameWorld != null) {
            disposeGameWorld();
        }

        gameWorld = new GameWorld(this);
        gameWorld.create();
        layerGameWorld.addChild(gameWorld.getGameWorldContainer2D());

        return gameWorld;
    }

    public void disposeGameWorld() {
        if(gameWorld != null) {
            fxManager.removeVFXWithGroupID(FXManager.GROUP_IN_GAME);

            gameWorld.dispose();

            gameWorld = null;
        }
    }

    @Override
    public void dispose() {
        if(gameWorld != null) {
            gameWorld.dispose();

            gameWorld = null;
        }

        super.dispose();
    }
}
