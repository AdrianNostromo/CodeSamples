package base.assets;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.assets.AssetManager;
import com.badlogic.gdx.assets.loaders.FileHandleResolver;
import com.badlogic.gdx.audio.Sound;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.BitmapFont;
import com.badlogic.gdx.graphics.g2d.TextureAtlas;
import com.badlogic.gdx.graphics.g3d.Model;
import com.badlogic.gdx.utils.Array;

import local.app.AppLocal;
import base.util.MathUtil;

public class TypedAssetsManager extends AssetManager {

	public AppLocal app;

	public TypedAssetsManager(AppLocal app, FileHandleResolver resolver) {
		super(resolver);

		this.app = app;
	}
	
	public Sound getSound(String soundName) {
		if(!isLoaded(soundName, Sound.class)) {
			load(soundName, Sound.class);
            finishLoadingAsset(soundName);
		}
		
		return get(soundName, Sound.class);
	}

	public TextureAtlas getTextureAtlas(String atlasID) {
		if(!isLoaded(atlasID, TextureAtlas.class)) {
			load(atlasID, TextureAtlas.class);
            finishLoadingAsset(atlasID);
		}
		
		return get(atlasID, TextureAtlas.class);
	}

	public BitmapFont getBitmapFntFont(String fntFontID) {
		if(!isLoaded(fntFontID, BitmapFont.class)) {
			load(fntFontID, BitmapFont.class);
			finishLoadingAsset(fntFontID);
		}

		return get(fntFontID, BitmapFont.class);
	}

	public Texture getTexture(String textureID) {
		if(!isLoaded(textureID, Texture.class)) {
			load(textureID, Texture.class);
            finishLoadingAsset(textureID);
		}
		
		return get(textureID, Texture.class);
	}

	public void unloadAsset(String fileName) {
        if(isLoaded(fileName)) {
            unload(fileName);
        }
	}
	
	public Array<TextureAtlas.AtlasRegion> getTextureAtlasRegionsArray(String atlasID, String regionIDPrefix, int regionIndiceStart, int regionIndiceEnd, int indiceZeroPadding) {
		Array<TextureAtlas.AtlasRegion> kfList = new Array<TextureAtlas.AtlasRegion>();
		
		for(int i=regionIndiceStart;i<=regionIndiceEnd;i++) {
			String frameIndex = MathUtil.intToPaddedString(i, indiceZeroPadding);
			
			TextureAtlas.AtlasRegion kf = getTextureAtlasRegion(atlasID, regionIDPrefix + frameIndex);
			kfList.add(kf);
		}
		
		return kfList;
	}

	public TextureAtlas.AtlasRegion getTextureAtlasRegion(String atlasID, String regionID) {
		TextureAtlas textureAtlas = getTextureAtlas(atlasID);
		if(textureAtlas == null) {
			Gdx.app.debug("Error", "Texture atlas not found. AtlasID: " + atlasID);
			
			return null;
		}
		
		TextureAtlas.AtlasRegion atlasRegion = textureAtlas.findRegion(regionID);
		if(atlasRegion == null) {
			Gdx.app.debug("Error", "Texture region not found. AtlasID: " + atlasID + ", RegionID: " + regionID);
			
			return null;
		}
		
		return atlasRegion;
	}

    public Model getModel3D(String modelID) {
        if(!isLoaded(modelID, Model.class)) {
            load(modelID, Model.class);
            finishLoadingAsset(modelID);
        }

        return get(modelID, Model.class);
    }

	public String getTextData(String textFileID) {
		if(!isLoaded(textFileID, base.file.Text.class)) {
			load(textFileID, base.file.Text.class);
			finishLoadingAsset(textFileID);
		}

		return get(textFileID, base.file.Text.class).getString();
	}

}
