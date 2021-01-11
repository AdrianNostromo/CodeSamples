package local;

import java.util.Map;
import java.util.Vector;

import base.MainBase;
import base.assets.TemplatesInitializer;
import base.launcher.IAppLauncher;
import base.launcher.ILocalDataHandler;
import base.launcher.IWebManager;
import base.menu.util.IMenuAnimationListener;
import local.ads.launcher.IAdsManager;
import local.app.AppLocal;
import local.audio.util.SoundConfig;
import local.font.config.FontConfig;
import local.gameworld.GameWorld;
import local.gameworld.util.BallConfig;
import local.gameworld.util.BallStyleConfig;
import local.gameworld.util.ItemConfig;
import local.gameworld.util.PlatformConfig;
import local.gameworld.util.PlatformStyleConfig;
import local.playgames.launcher.IPlayGamesManager;
import local.playgames.util.PlayGamesEventListener;
import local.profile.config.ProfileDataIDs;
import local.view.levelend.LevelEndView;
import local.view.main.MainView;
import local.view.playercurrency.PlayerCurrencyView;
import local.view.shop.ShopView;
import local.view.splashscreen.SplashScreenView;

import static local.profile.config.ProfileDataIDs.PROFILE_DATA_VERSION;

public class MainLocal extends MainBase {

    private IAdsManager adsManager;
    private IPlayGamesManager playGamesManager;

    private boolean isSplashScreenActive = false;
    private long splashScreenActivateTimeMS = 0;
    private boolean splashScreenFinished = false;

    // Increase this to reset the profile data.
    public static final int PROFILE_VERSION_ID = 11;

    private PlayGamesEventListener playGamesEventListener = new PlayGamesEventListener() {
        @Override
        public void playerInitiatedConnect() {
            super.playerInitiatedConnect();

            app.profileData.putDataWithID_boolean(ProfileDataIDs.IS_PLAYER_PREFFERES_LOGGED_IN, true);
        }

        @Override
        public void playerInitiatedDisconnect() {
            super.playerInitiatedDisconnect();

            app.profileData.putDataWithID_boolean(ProfileDataIDs.IS_PLAYER_PREFFERES_LOGGED_IN, false);
        }

        @Override
        public void playerCanceledLoginAccountSelection() {
            super.playerCanceledLoginAccountSelection();

            app.profileData.putDataWithID_boolean(ProfileDataIDs.IS_PLAYER_PREFFERES_LOGGED_IN, false);
        }
    };

    public MainLocal(IAppLauncher appLauncher, ILocalDataHandler localDataHandler, IAdsManager adsManager, IPlayGamesManager playGamesManager, IWebManager webManager) {
        super(appLauncher, localDataHandler, webManager);

        this.adsManager = adsManager;
        this.playGamesManager = playGamesManager;

        // Required to initialize the static variables so they are available when the assets are preloaded.
        TemplatesInitializer.initializeTemplates(new Class[]{
                MainView.class,
                SplashScreenView.class,
                LevelEndView.class,
                ShopView.class,
                PlayerCurrencyView.class,

                GameWorld.class,

                PlatformConfig.class,
                BallConfig.class,
                BallStyleConfig.class,
                ItemConfig.class,

                SoundConfig.class
        });
    }

    @Override
    public void createMain() {
        super.createMain();

        // Add a event listener on the play_games_manager to detect when the player refuses to log in.
        app.playGamesWrapper.getPlayGamesManager().addEventsListener(playGamesEventListener);

        // Show the splash screen. This will block until the required assets finish loading.
        app.getSplashScreenView().show(false, null);
        isSplashScreenActive = true;
        splashScreenActivateTimeMS = System.currentTimeMillis();
    }

    @Override
    protected void createApp() {
        app = new AppLocal(appLauncher, localDataHandler, webManager, playGamesManager, adsManager);
        app.create();
    }

    @Override
    public void preloadPhaseEnded(int phaseIndex) {
        super.preloadPhaseEnded(phaseIndex);

        if(phaseIndex == 1) {
            splashScreenFinished = true;

            app.assetsPreloadFinished();

            // Initialize all menus and the game world.

            //app.getLandingView();

            if(app.adsManager != null) {
                app.adsManager.gameWorldReady();
            }
        }
    }

    @Override
    protected boolean isResetProfileDataRequired() {
        if(app.profileData.getDataWithID_int(PROFILE_DATA_VERSION) != PROFILE_VERSION_ID) {
            return true;
        }

        return false;
    }

    @Override
    protected void mainLoop(float deltaS, int deltaMS) {
        super.mainLoop(deltaS, deltaMS);

        // This will make the splash screen be active for at least 1.2 seconds.
        if(isSplashScreenActive && splashScreenFinished && System.currentTimeMillis() - splashScreenActivateTimeMS >= 1200) {
            isSplashScreenActive = false;

            app.getSplashScreenView().hide(true, null);

            app.getMainView().show(false, new IMenuAnimationListener() {
                @Override
                public void animationFinished(Object target) {
                    if(app.profileData.getDataWithID_boolean(ProfileDataIDs.IS_PLAYER_PREFFERES_LOGGED_IN)) {
                        app.playGamesWrapper.getPlayGamesManager().startConnect(false);
                    }
                }
            });
        }
    }

    @Override
    protected void initializePlayerProfileData() {
        super.initializePlayerProfileData();

        app.profileData.putDataWithID_int(PROFILE_DATA_VERSION, PROFILE_VERSION_ID);

        app.profileData.putPlayerGems(1000);//asd_tst_001; 10

        app.profileData.putDataWithID_int(ProfileDataIDs.BEST_SCORE, 0);

        app.profileData.putDataWithID_boolean(ProfileDataIDs.IS_TUTORIAL_FINISHED, false);

        app.profileData.putActivePlatformStyleId(PlatformStyleConfig.DefaultStyle.styleID);
        app.profileData.putIsPlatformStyleUnlocked(PlatformStyleConfig.DefaultStyle.styleID, true);

        app.profileData.putActiveBallStyleId(BallStyleConfig.DefaultStyle.styleID);
        app.profileData.putIsBallStyleUnlocked(BallStyleConfig.DefaultStyle.styleID, true);
    }

}
