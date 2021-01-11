package base.assets;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.assets.AssetDescriptor;
import com.badlogic.gdx.assets.AssetErrorListener;
import com.badlogic.gdx.assets.AssetLoaderParameters;
import com.badlogic.gdx.assets.AssetManager;
import com.badlogic.gdx.assets.loaders.BitmapFontLoader;
import com.badlogic.gdx.assets.loaders.SoundLoader;
import com.badlogic.gdx.assets.loaders.TextureAtlasLoader;
import com.badlogic.gdx.assets.loaders.TextureLoader;
import com.badlogic.gdx.audio.Sound;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.BitmapFont;
import com.badlogic.gdx.graphics.g2d.TextureAtlas;
import com.badlogic.gdx.graphics.g3d.Model;
import com.badlogic.gdx.graphics.g3d.loader.ObjLoader;

import java.util.Vector;

import base.assets.util.LoadingFontStatus;
import base.file.Text;
import base.font.BufferedFontsManager;
import base.font.config.FontConfig;
import base.font.util.FontSourceFntFile;
import base.font.util.FontSourceGenerated;
import local.app.AppLocal;
import local.audio.util.SoundConfig;

public class AssetsPreloader {

    private AppLocal app;

    private int phaseIndex = 0;
    private boolean useHighPriorityLoading = false;
    private IAssetsPreloaderListener eventListener = null;

    private boolean isAssetsLoadInProgress = false;

    private Vector<LoadingFontStatus> loadingGeneratedFontsList;
    private int loadingGeneratedFontsListIndex = -1;
    private boolean loadingCurrentGeneratedFontsEntryFinished = false;

    private boolean lastAssetsLoadUpdateFinished = true;

    private long assetLoadStartTimeMs;
    private long lastAssetLoadFinishTimeMs;

    private boolean internalAssetsLoadError = false;
    private AssetErrorListener preloadingInternalAssetsErrorListener = new AssetErrorListener() {
        @Override
        public void error(AssetDescriptor asset, Throwable throwable) {
            Gdx.app.debug("Error", "Assets load error: " + asset.fileName);

            internalAssetsLoadError = true;
        }
    };

    private AssetErrorListener preloadingLocalAssetsErrorListener = new AssetErrorListener() {
        @Override
        public void error(AssetDescriptor asset, Throwable throwable) {
            if(loadingGeneratedFontsListIndex >= 0 && loadingGeneratedFontsListIndex < loadingGeneratedFontsList.size() && !loadingCurrentGeneratedFontsEntryFinished) {
                // Font hhtpRequestFailed to load from the buffer, generate it and save a new buffer version.
                LoadingFontStatus loadingFontStatus = loadingGeneratedFontsList.get(loadingGeneratedFontsListIndex);

                // Reset the font buffer indicator.
                app.fontsManager.clearFontBufferedIndicator(loadingFontStatus.bakedBufferFontID);

                app.fontsManager.getBitmapFont(loadingFontStatus.fontConfig);

                loadingCurrentGeneratedFontsEntryFinished = true;
                lastAssetsLoadUpdateFinished = true;
            }else {
                Gdx.app.debug("Error", "Assets load error: " + asset.fileName);
            }
        }
    };

    private AssetLoaderParameters.LoadedCallback assetsLoadingListener = new AssetLoaderParameters.LoadedCallback() {
        @Override
        public void finishedLoading(AssetManager assetManager, String fileName, Class type) {
            long tm = System.currentTimeMillis();

            app.log.postLog("Profiling. Asset item load time: " + (tm - lastAssetLoadFinishTimeMs) + ", fileName: " + fileName);

            lastAssetLoadFinishTimeMs = tm;
        }
    };

    public AssetsPreloader(AppLocal app) {
		super();

        this.app = app;
	}

    public void create() {
        //void
    }

    private <T> AssetLoaderParameters<T> hookLoaderParamsCallback(Class<T> type, AssetLoaderParameters params) {
        params.loadedCallback = assetsLoadingListener;

        return params;
    }

    public void startPreloadPhase(int phaseIndex, Vector<String> texturesList, Vector<String> textureAtlasesList, Vector<FontConfig> fontsList, Vector<SoundConfig> soundsList, boolean useHighPriorityLoading, IAssetsPreloaderListener eventListener, Vector<String> model3DList, Vector<String> textFiles) {
        assetLoadStartTimeMs = System.currentTimeMillis();
        lastAssetLoadFinishTimeMs = System.currentTimeMillis();

        this.phaseIndex = phaseIndex;
        this.useHighPriorityLoading = useHighPriorityLoading;
        this.eventListener = eventListener;

        app.embeddedAssetsManager.setErrorListener(preloadingInternalAssetsErrorListener);
        app.localAssetsManager.setErrorListener(preloadingLocalAssetsErrorListener);

        isAssetsLoadInProgress = true;
        lastAssetsLoadUpdateFinished = false;

        for(int i=0;i<texturesList.size();i++) {
            String entry = texturesList.get(i);
    
            if(!app.embeddedAssetsManager.isLoaded(entry, Texture.class)) {
                app.embeddedAssetsManager.load(entry, Texture.class, hookLoaderParamsCallback(Texture.class, new TextureLoader.TextureParameter()));
            }
        }

        for(int i=0;i<textureAtlasesList.size();i++) {
            String entry = textureAtlasesList.get(i);
    
            if(!app.embeddedAssetsManager.isLoaded(entry, Texture.class)) {
                app.embeddedAssetsManager.load(entry, TextureAtlas.class, hookLoaderParamsCallback(TextureAtlas.class, new TextureAtlasLoader.TextureAtlasParameter()));
            }
        }

        loadingGeneratedFontsList = new Vector<LoadingFontStatus>();
        loadingGeneratedFontsListIndex = -1;
        loadingCurrentGeneratedFontsEntryFinished = true;
        for(int i=0;i<fontsList.size();i++) {
            FontConfig fontConfig = fontsList.get(i);

            float sizeScaleSourceValue = app.getValueFromID(((FontSourceGenerated)fontConfig.fontSource).scaleSource);
            int bakedFontSize = (int)Math.ceil(((FontSourceGenerated)fontConfig.fontSource).baseSize * (sizeScaleSourceValue / ((FontSourceGenerated)fontConfig.fontSource).scaleBaseValue));
            String bakedBufferFontID = FontConfig.ComputeBakedFontId(fontConfig, bakedFontSize);

            if(!app.fontsManager.isBitmapFontInMap(bakedBufferFontID)) {
                if (fontConfig.fontSource instanceof FontSourceFntFile) {
                    app.embeddedAssetsManager.load(((FontSourceFntFile) fontConfig.fontSource).fntFileName, BitmapFont.class, hookLoaderParamsCallback(BitmapFont.class, new BitmapFontLoader.BitmapFontParameter()));
                } else if (fontConfig.fontSource instanceof FontSourceGenerated) {
                    loadingGeneratedFontsList.add(new LoadingFontStatus(fontConfig, bakedFontSize, bakedBufferFontID));
                } else {
                    Gdx.app.debug("Error", "Unknown font source.");

                    return;
                }
            }
        }

        for(int i=0;i<soundsList.size();i++) {
            SoundConfig entry = soundsList.get(i);

            app.embeddedAssetsManager.load(entry.fileName, Sound.class, hookLoaderParamsCallback(Sound.class, new SoundLoader.SoundParameter()));
        }

        for(int i=0;i<model3DList.size();i++) {
            String modelID = model3DList.get(i);

            app.embeddedAssetsManager.load(modelID, Model.class, hookLoaderParamsCallback(Model.class, new ObjLoader.ObjLoaderParameters()));
        }

        for(int i=0;i<textFiles.size();i++) {
            String textFileID = textFiles.get(i);

            app.embeddedAssetsManager.load( new AssetDescriptor<Text>(textFileID, Text.class, hookLoaderParamsCallback(Text.class, new TextureLoader.TextureParameter())));
        }
    }

    public void update() {
        // Load some assets if needed.
        if(isAssetsLoadInProgress && app.isSplashScreenView() && !app.getSplashScreenView().isAnimating()) {
            int updateDurationMS = 0;
            if(useHighPriorityLoading) {
                // The base assets are loaded with a higher priority. This will limit the splash screen fps.
                updateDurationMS = 200;
            }

            // Push the next font to the loading list.
            if(lastAssetsLoadUpdateFinished && loadingCurrentGeneratedFontsEntryFinished && loadingGeneratedFontsListIndex < loadingGeneratedFontsList.size()) {
                loadingGeneratedFontsListIndex++;
                loadingCurrentGeneratedFontsEntryFinished = false;

                if(loadingGeneratedFontsListIndex < loadingGeneratedFontsList.size()) {
                    LoadingFontStatus loadingFontStatus = loadingGeneratedFontsList.get(loadingGeneratedFontsListIndex);

                    // Check if the font can be loaded from the saved buffer version.
                    if(app.fontsManager.isFontBuffered(loadingFontStatus.bakedBufferFontID)) {
                        // Add the fnt to the assets manager load queue and wait for it to finish loading.

                        app.localAssetsManager.load(BufferedFontsManager.bufferedFontsLocationPrefix + loadingFontStatus.bakedBufferFontID + ".fnt", BitmapFont.class, hookLoaderParamsCallback(BitmapFont.class, new BitmapFontLoader.BitmapFontParameter()));
                        lastAssetsLoadUpdateFinished = false;
                    }else {
                        // There is no local buffer version for this font.

                        app.fontsManager.getBitmapFont(loadingFontStatus.fontConfig);
                        loadingCurrentGeneratedFontsEntryFinished = true;
                    }
                }
            }

            // Try catch is not needed because a error listener is used (this.preloadingAssetsErrorListener).
            if(loadingGeneratedFontsListIndex < 0) {
                lastAssetsLoadUpdateFinished = app.embeddedAssetsManager.update(updateDurationMS);
                if(internalAssetsLoadError) {
                    // Error occurred while loading game assets.

                    Gdx.app.debug("Error", "Missing game assets.");

                    return ;
                }
            }else {
                lastAssetsLoadUpdateFinished = app.localAssetsManager.update(updateDurationMS);
            }
            boolean assetsLoadFinished = lastAssetsLoadUpdateFinished;

            if(assetsLoadFinished) {
                if(loadingGeneratedFontsListIndex >= 0 && loadingGeneratedFontsListIndex < loadingGeneratedFontsList.size() && !loadingCurrentGeneratedFontsEntryFinished) {
                    // A buffered font finished loading. Add it to the fonts list map.

                    loadingCurrentGeneratedFontsEntryFinished = true;
                    LoadingFontStatus loadingFontStatus = loadingGeneratedFontsList.get(loadingGeneratedFontsListIndex);

                    BitmapFont font = app.localAssetsManager.get(base.font.BufferedFontsManager.bufferedFontsLocationPrefix + loadingFontStatus.bakedBufferFontID + ".fnt", BitmapFont.class);
                    app.fontsManager.pushBitmapFontToMap(loadingFontStatus.bakedBufferFontID, font);
                }
                // Check if there are fonts that need to be loaded.
                if(loadingGeneratedFontsListIndex < loadingGeneratedFontsList.size() - 1 || (loadingGeneratedFontsListIndex == loadingGeneratedFontsList.size() - 1 && !loadingCurrentGeneratedFontsEntryFinished)) {
                    assetsLoadFinished = false;
                }
            }

            if(assetsLoadFinished) {
                isAssetsLoadInProgress = false;

                app.embeddedAssetsManager.setErrorListener(null);
                app.localAssetsManager.setErrorListener(null);

                if(eventListener != null) {
                    eventListener.preloadPhaseEnded(phaseIndex);
                }

                long tm = System.currentTimeMillis();

                app.log.postLog("Profiling. Total assets load time: " + (tm - assetLoadStartTimeMs));
            }
        }
    }

}
