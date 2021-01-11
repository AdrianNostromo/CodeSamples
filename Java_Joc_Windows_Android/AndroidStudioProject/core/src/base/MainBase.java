package base;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Color;
import com.badlogic.gdx.graphics.GL20;

import java.util.Vector;

import base.assets.IAssetsPreloaderListener;
import base.assets.util.ManagedFilePath;
import base.font.config.FontConfig;
import base.launcher.IAppLauncher;
import base.launcher.ILocalDataHandler;
import base.launcher.IWebManager;
import base.text.ManagedTextFilePath;
import base.visual.util.ManagedTextureAtlasPath;
import base.visual.util.ManagedTexturePath;
import base.visual3d.util.ManagedModel3DPath;
import local.app.AppLocal;
import local.audio.util.SoundConfig;
import local.profile.config.ProfileDataIDs;

/**
 * Created by Adrin on 7/21/2016.
 */
public abstract class MainBase extends ApplicationAdapter implements IAssetsPreloaderListener {

    protected IAppLauncher appLauncher;
    protected ILocalDataHandler localDataHandler;
    protected IWebManager webManager;

    // The app object contains all major objects that are used by most of the game components.
    protected AppLocal app;
    
    public MainBase(IAppLauncher appLauncher, ILocalDataHandler localDataHandler, IWebManager webManager) {
        super();

        this.appLauncher = appLauncher;
        this.localDataHandler = localDataHandler;
        this.webManager = webManager;
    }

    // Create the game components and load the assets.
    @Override
    public final void create () {
        super.create();

        createMain();
        createPost();
    }

    protected void createMain() {
        createApp();


        if (!app.profileData.getDataWithID_boolean(ProfileDataIDs.IS_INITIALIZED)
                || isResetProfileDataRequired()) {
            initializePlayerProfileData();
        }
    }

    protected abstract void createApp();

    // Extendable
    protected abstract boolean isResetProfileDataRequired();

    protected void createPost() {
        startAssetsLoading();
    }
    
    private void startAssetsLoading() {
        Vector<String> texturesList = ManagedFilePath.buildPathsList(ManagedTexturePath.entriesList);
        Vector<String> textureAtlasesList = ManagedFilePath.buildPathsList(ManagedTextureAtlasPath.entriesList);
        Vector<String> textFilesList = ManagedFilePath.buildPathsList(ManagedTextFilePath.entriesList);
        Vector<String> model3DList = ManagedFilePath.buildPathsList(ManagedModel3DPath.entriesList);
        Vector<FontConfig> fontsList = FontConfig.entriesList;
        Vector<SoundConfig> soundsList = SoundConfig.entriesList;
    
        app.assetsPreloader.startPreloadPhase(1, texturesList, textureAtlasesList, fontsList, soundsList, true, this, model3DList, textFilesList);
    }
    
    protected void initializePlayerProfileData() {
        app.profileData.clearAllData();

        app.profileData.putDataWithID_boolean(ProfileDataIDs.IS_INITIALIZED, true);

        //asd_002; app.profileData.putDataWithID_boolean(ProfileDataIDs.IS_PLAYER_PREFFERES_LOGGED_IN, true);
    }

    @Override
    public void preloadPhaseEnded(int phaseIndex) {
        if(phaseIndex == 1) {
            //void
        }else {
            Gdx.app.debug("Error", "Unhandled assets load phase: " + phaseIndex);
        }
    }

    // This method is called 60 times a second to render the game world. It also calls the app and game loops.
    @Override
    public void render () {
        app.assetsPreloader.update();
    
        // START : App loop;
        float deltaS = Gdx.graphics.getDeltaTime();
        deltaS = Math.min(deltaS, 0.1f);

        int deltaMS = Math.round(deltaS * 1000f);

        // Call the app loops for the main app components.
        if(deltaS > 0) {
            mainLoop(deltaS, deltaMS);
        }
        // END : App loop;

        render_pre();
        
        Gdx.gl.glViewport(0, 0, Gdx.graphics.getWidth(), Gdx.graphics.getHeight());
    
        // Prepare for OpenGL rendering.
        Gdx.gl.glClearColor(Color.BLACK.r, Color.BLACK.g, Color.BLACK.b, Color.BLACK.a);
        Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT | GL20.GL_DEPTH_BUFFER_BIT);
        
        app.camera2D.update();
        
        app.spriteBatch.setProjectionMatrix(app.camera2D.combined);
        
        render_2DUnder(deltaS);
        render_3D();
        render_2DOver(deltaS);
        
        app.postRendering();
    }
    
    private void render_pre() {
        app.render_pre();
    }
    
    private void render_2DUnder(float deltaS) {
        if(app.enableComponent_backgroundStage) {
            app.spriteBatch.begin();
            app.backgroundStage.batchDraw(app.spriteBatch, app.camera2D, deltaS);
            app.spriteBatch.end();
        }
    }
    
    private void render_3D() {
        app.handle3DRendering();
    }
    
    private void render_2DOver(float deltaS) {
        app.spriteBatch.begin();
        app.stage.batchDraw(app.spriteBatch, app.camera2D, deltaS);
        app.spriteBatch.end();
    }

    protected void mainLoop(float deltaS, int deltaMS) {
        app.appLoop(deltaS, deltaMS);
    }

    // Clear the app components.
    @Override
    public void dispose() {
        if(app != null) {
            app.dispose();

            app = null;
        }

        super.dispose();
    }
    
    @Override
    public void resize(int width, int height) {
        super.resize(width, height);
        
        //asd_002;// Handle screen size change.
        
        //asd_002;// call:
        /*camera.viewportHeight = height;
        camera.viewportWidth = width;
        camera.update();*/
    }
    
}
