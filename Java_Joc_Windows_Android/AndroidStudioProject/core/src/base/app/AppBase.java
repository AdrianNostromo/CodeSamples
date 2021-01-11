package base.app;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.assets.loaders.resolvers.ExternalFileHandleResolver;
import com.badlogic.gdx.assets.loaders.resolvers.InternalFileHandleResolver;
import com.badlogic.gdx.assets.loaders.resolvers.LocalFileHandleResolver;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;

import java.util.Vector;

import base.IBaseAppComponent;
import base.assets.AssetsPreloader;
import base.assets.BufferedAssetsManager;
import base.assets.TypedAssetsManager;
import base.audio.BufferedSoundManager;
import base.error.IErrorHandler;
import base.file.Text;
import base.file.loaders.TextLoader;
import base.font.BufferedFontsManager;
import base.fx.FXManager;
import base.launcher.IAppLauncher;
import base.launcher.ILocalDataHandler;
import base.launcher.IWebManager;
import base.log.ILogHandler;
import base.log.LogHandler;
import base.view.popupmenu.IPopupMenu;
import base.view.popupmenu.PopupMenu;
import base.timer.TimerManager;
import base.util.AppStyles;
import base.util.Point2D;
import base.util.Size2D;
import base.view.dialog.DialogView;
import base.view.listmenucardpopup.ListMenuCardPopupView;
import base.visual.Container2D;
import base.visual.IContainer2D;
import base.visual.SpriteStage;
import base.visual.VisualUtil;
import local.profile.PlayerProfileDataLocal;

/**
 * Created by Adrin on 7/27/2016.
 */
public class AppBase {

    public IAppLauncher appLauncher;

    public ILocalDataHandler localDataHandler;

    public IWebManager webManager;

    public ILogHandler log;
    public IErrorHandler errorHandler;

    // This is initialize on app create.
    public Size2D screenSize = null;
    public Size2D templateSize = null;

    public Point2D screenPixelsPerInch = null;
    public Point2D screenDensityMultiplier = null;

    public float screenAspectRatio;

    public PlayerProfileDataLocal profileData;

    protected Vector<IBaseAppComponent> appComponentsList = new Vector<IBaseAppComponent>();

    public static final boolean enableComponent_backgroundStage = true;
    public SpriteStage backgroundStage;

    public SpriteStage stage;

    public OrthographicCamera camera2D;
    public SpriteBatch spriteBatch;

    // This manager is used to load internal assets. Assets that are packed inside the jar file.
    public BufferedAssetsManager embeddedAssetsManager;
    // This manager is used to load local assets. Assets that are saved by the game, like the buffered generated fonts.
    public TypedAssetsManager localAssetsManager;
    // This manager is used to load external assets. Assets that are on the phone memory or sd card. Like the screen shoots or the editor data images.
    public TypedAssetsManager externalAssetsManager;

    public BufferedFontsManager fontsManager;

    public BufferedSoundManager soundManager;

    public FXManager fxManager;

    public AssetsPreloader assetsPreloader;

    public TimerManager timerManager;

    // Back layers are under the 3d game world;
    public Container2D backLayerBackground;

    private Container2D layerDialogs;
    private Container2D layerPopupMenu;
    private Container2D layerGlobalVisualFX;

    private PopupMenu popupMenu = null;
    private DialogView dialogView = null;
    
    // ListMenuCardPopupView
    private Vector<IBaseAppComponent> listMenuCardPopupViewsList = new Vector<IBaseAppComponent>();

    //asd_002;// Update U.I. on screen rendering3DSize change.

    public AppBase(IAppLauncher appLauncher, ILocalDataHandler localDataHandler, IWebManager webManager) {
        super();

        this.appLauncher = appLauncher;

        this.localDataHandler = localDataHandler;

        this.webManager = webManager;
    }

    public void create() {
        log = new LogHandler();
        log.create();

        errorHandler = new base.error.ErrorHandler((local.app.AppLocal)this);
        errorHandler.create();

        screenSize = new Size2D(Gdx.graphics.getWidth(), Gdx.graphics.getHeight());
        templateSize = new Size2D(360, 570);

        //asd_003;// this may be 56f for mobile or 64f for tablets or larger screens; use some logic.
        // Source: https://material.google.com/patterns/navigation-drawer.html#navigation-drawer-specs
        AppStyles.standardIncrement = 56f;

        screenPixelsPerInch = new Point2D(Gdx.graphics.getPpiX(), Gdx.graphics.getPpiY());
        screenDensityMultiplier = computeScreenDensityMultiplier();
        
        //asd_tst_001;// For desktop debug, the multiplier is currently 1;
        screenDensityMultiplier = new Point2D(1f, 1f);

        screenAspectRatio = (float) Gdx.graphics.getWidth() / (float) Gdx.graphics.getHeight();
        // Gdx.app.debug("Screen", "Screen aspect ratio is: " + screenAspectRatio);

        // Create the player profile object. It uses the localDataHandler to read and save key, baseValue pairs.
        profileData = new PlayerProfileDataLocal((local.app.AppLocal)this, localDataHandler);

        // This is the background stage. It contains the background menu and doesn't receive touch events.
        if(enableComponent_backgroundStage) {
            backgroundStage = new SpriteStage((local.app.AppLocal)this);
            backgroundStage.create();
        }else {
            backgroundStage = null;
        }

        // The stage renders all the game visuals and implements layering and grouping functionality.
        stage = new SpriteStage((local.app.AppLocal)this);
        stage.create();
        Gdx.input.setInputProcessor(stage);

        if(enableComponent_backgroundStage) {
            backLayerBackground = VisualUtil.newGroup(backgroundStage);
        }

        // Create the main app visuals layer.
        createLayers();

        // Create a orthographic camera2D used for 2D rendering. The camera2D only scales the game world so it better fits the device screen.
        camera2D = new OrthographicCamera();
        camera2D.setToOrtho(false, screenSize.width, screenSize.height);

        // Create the sprite batch. It receives rendering commands and pushes them to the GPU for rendering.
        spriteBatch = new SpriteBatch();
        // spriteBatch.setBlendFunction(GL20.GL_ONE, GL20.GL_ONE_MINUS_SRC_ALPHA);

        Gdx.gl.glClearColor(1f, 1f, 1f, 1f);

        // The assets manager handles loading of all game assets.
        embeddedAssetsManager = new BufferedAssetsManager((local.app.AppLocal)this, new InternalFileHandleResolver());
        embeddedAssetsManager.setLoader(
                Text.class,
                new TextLoader(
                        new InternalFileHandleResolver()
                )
        );

        localAssetsManager = new TypedAssetsManager((local.app.AppLocal)this, new LocalFileHandleResolver());
        localAssetsManager.setLoader(
                Text.class,
                new TextLoader(
                        new LocalFileHandleResolver()
                )
        );

        externalAssetsManager = new TypedAssetsManager((local.app.AppLocal)this, new ExternalFileHandleResolver());
        externalAssetsManager.setLoader(
                Text.class,
                new TextLoader(
                        new ExternalFileHandleResolver()
                )
        );

        fontsManager = new base.font.BufferedFontsManager((local.app.AppLocal)this);
        fontsManager.create();
        soundManager = new BufferedSoundManager((local.app.AppLocal)this);

        fxManager = new FXManager((local.app.AppLocal)this);
        fxManager.create();

        assetsPreloader = new AssetsPreloader((local.app.AppLocal)this);
        assetsPreloader.create();

        timerManager = new TimerManager();
        timerManager.create();
    }

    public void assetsPreloadFinished() {
        //void
    }

    protected void createLayers() {
        layerDialogs = VisualUtil.newGroup(stage);
        layerPopupMenu = VisualUtil.newGroup(stage);
        layerGlobalVisualFX = VisualUtil.newGroup(stage);
    }

    public void pushAppComponent(IBaseAppComponent appComponent) {
        appComponentsList.add(appComponent);
    }

    public void disposeAppComponent(IBaseAppComponent appComponent) {
        if(!appComponentsList.remove(appComponent)) {
            Gdx.app.debug("Error", "AnimatedComposedStiledVisual.Error.7.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        appComponent.dispose();
    }

    protected Point2D computeScreenDensityMultiplier() {
        //float multX = Gdx.graphics.getDensity();
        //float multY = Gdx.graphics.getDensity();

        float multX = Gdx.graphics.getPpiX() / 160f;
        float multY = Gdx.graphics.getPpiY() / 160f;

        return new Point2D(multX, multY);
    }

    public void appLoop(float deltaS, int deltaMS) {
        if(appLauncher != null) {
            appLauncher.appLoop(deltaS, deltaMS);
        }

        if(timerManager != null) {
            timerManager.appLoop(deltaS, deltaMS);
        }

        if(soundManager != null) {
            soundManager.appLoop(deltaS, deltaMS);
        }

        if(fxManager != null) {
            fxManager.appLoop(deltaS, deltaMS);
        }

        for(int i=0;i<appComponentsList.size();i++) {
            IBaseAppComponent entry = appComponentsList.get(i);
            if(entry != null) {
                entry.appLoop(deltaS, deltaMS);
            }
        }
    }

    public IPopupMenu getPopupMenu() {
        if(popupMenu == null) {
            appComponentsList.add(popupMenu = new PopupMenu((local.app.AppLocal)this));
            popupMenu.create();
            layerPopupMenu.addChild(popupMenu.getVisualsHolder());
        }

        return popupMenu;
    }

    public boolean isPopupMenu() {
        return popupMenu != null;
    }

    public DialogView getNewDialogView() {
        if(dialogView != null) {
            dialogView.dispose();

            dialogView = null;
        }

        if(dialogView == null) {
            appComponentsList.add(dialogView = new DialogView((local.app.AppLocal)this));
            dialogView.create();
            layerDialogs.addChild(dialogView.getVisualsHolder());
        }

        return dialogView;
    }

    public boolean isDialogView() {
        return dialogView != null;
    }

    public ListMenuCardPopupView getNewListMenuCardPopupView(IContainer2D parentHolderOverride) {
        ListMenuCardPopupView newView = new ListMenuCardPopupView((local.app.AppLocal)this);
        appComponentsList.add(newView);
        listMenuCardPopupViewsList.add(newView);
        newView.create();

        if(parentHolderOverride != null) {
            parentHolderOverride.addChild(newView.getVisualsHolder());
        }else {
            layerDialogs.addChild(newView.getVisualsHolder());
        }

        return newView;
    }

    public void disposeListMenuCardPopupView(IBaseAppComponent listMenuCardPopupView) {
        if(!appComponentsList.remove(listMenuCardPopupView)) {
            Gdx.app.debug("Error", "AnimatedComposedStiledVisual.Error.7.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }
        if(!listMenuCardPopupViewsList.remove(listMenuCardPopupView)) {
            Gdx.app.debug("Error", "AnimatedComposedStiledVisual.Error.7.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        listMenuCardPopupView.dispose();
    }

    public void dispose() {
        if(appComponentsList != null) {
            for (int i = appComponentsList.size() - 1; i >= 0; i--) {
                IBaseAppComponent entry = appComponentsList.get(i);
                if (entry != null) {
                    entry.dispose();
                }
            }

            appComponentsList = null;
        }
    }

    public float getValueFromID(String valueID) {
        if(valueID.equals("_appSize_width")) {
            return screenSize.width;
        }else if(valueID.equals("_appSize_height")) {
            return screenSize.height;
        }else if(valueID.equals("_dpm_width")) {
            return screenDensityMultiplier.x;
        }else if(valueID.equals("_dpm_height")) {
            return screenDensityMultiplier.y;
        }

        Gdx.app.debug("Error", "AnimatedComposedStiledVisual.Error.7.");

        throw new Error("AnimatedComposedStiledVisual.Error.7.");
    }

    public float templateXToScreenX(float templateX) {
        float ret = templateX * screenDensityMultiplier.x;

        return ret;
    }

    public float templateYToScreenY(float templateY) {
        float ret = templateY * screenDensityMultiplier.y;

        return ret;
    }

    public float screenXToTemplateX(float screenX) {
        float ret = screenX / screenDensityMultiplier.x;

        return ret;
    }

    public float screenYToTemplateY(float screenY) {
        float ret = screenY / screenDensityMultiplier.y;

        return ret;
    }

    public void postRendering() {
        //void
    }

}
