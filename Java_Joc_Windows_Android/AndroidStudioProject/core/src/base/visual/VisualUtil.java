package base.visual;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.files.FileHandle;
import com.badlogic.gdx.graphics.Pixmap;
import com.badlogic.gdx.graphics.PixmapIO;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.Animation;
import com.badlogic.gdx.graphics.g2d.BitmapFont;
import com.badlogic.gdx.graphics.g2d.TextureAtlas;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.math.Rectangle;
import com.badlogic.gdx.utils.Array;
import com.badlogic.gdx.utils.ScreenUtils;

import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Map;
import java.util.zip.Deflater;

import base.file.FileUtil;
import base.font.config.FontConfig;
import base.util.Size2DInt;
import base.visual.util.PixelVisualData;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualCreateConfig_Button;
import base.visual.util.VisualCreateConfig_CustomRectangleSprite;
import base.visual.util.VisualCreateConfig_Group;
import base.visual.util.VisualCreateConfig_Label;
import base.visual.util.VisualCreateConfig_MovieClip;
import base.visual.util.VisualCreateConfig_MovieClipButton;
import base.visual.util.VisualCreateConfig_PixelSprite;
import base.visual.util.VisualCreateConfig_Sprite;
import base.visual.util.VisualCreateConfig_TouchAreaAudible;
import base.visual.util.VisualCreateConfig_TouchAreaLightweight;
import local.app.AppLocal;
import local.audio.util.SoundConfig;

// General visual utility functions.
public class VisualUtil {

    public VisualUtil() {
        super();

        //void
    }

    public static IDrawableNode newSmartVisual(AppLocal app, VisualCreateConfig visualCreateConfig, IContainer2D parent, Map<String, Object> createConfigMap) {
        if(visualCreateConfig == null) {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        IDrawableNode visual = null;
        if(visualCreateConfig.type == VisualCreateConfig.TYPE_GROUP) {
            VisualCreateConfig_Group localizedConfig = (VisualCreateConfig_Group)visualCreateConfig;

            visual = VisualUtil.newGroup(parent);
        }else if(visualCreateConfig.type == VisualCreateConfig.TYPE_SPRITE) {
            VisualCreateConfig_Sprite localizedConfig = (VisualCreateConfig_Sprite)visualCreateConfig;

            visual = VisualUtil.newSprite(app, localizedConfig.managedAtlasPath.path, localizedConfig.regionID, parent);
        }else if(visualCreateConfig.type == VisualCreateConfig.TYPE_MOVIE_CLIP) {
            VisualCreateConfig_MovieClip localizedConfig = (VisualCreateConfig_MovieClip)visualCreateConfig;

            visual = VisualUtil.newMovieClip(app, localizedConfig.managedAtlasPath.path, localizedConfig.regionIDPrefix, localizedConfig.regionIndexStart, localizedConfig.regionIndexEnd, localizedConfig.indexZeroPadding, localizedConfig.frameDuration, localizedConfig.playMode, parent);
            if(!localizedConfig.isPlaying) {
                ((MovieClip) visual).stop();
            }else {
                ((MovieClip)visual).play(false);
            }
        }else if(visualCreateConfig.type == VisualCreateConfig.TYPE_MOVIE_CLIP_BUTTON) {
            VisualCreateConfig_MovieClipButton localizedConfig = (VisualCreateConfig_MovieClipButton)visualCreateConfig;

            visual = VisualUtil.newMovieClipButton(app, localizedConfig.managedAtlasPath.path, localizedConfig.regionIDPrefix, localizedConfig.regionIndexStart, localizedConfig.regionIndexEnd, localizedConfig.indexZeroPadding, localizedConfig.frameDuration, localizedConfig.playMode, localizedConfig.touchDownSoundConfig, localizedConfig.touchUpSoundConfig, parent);
            if(!localizedConfig.isPlaying) {
                ((MovieClipButton) visual).stop();
            }else {
                ((MovieClipButton)visual).play(false);
            }
        }else if(visualCreateConfig.type == VisualCreateConfig.TYPE_BUTTON) {
            VisualCreateConfig_Button localizedConfig = (VisualCreateConfig_Button)visualCreateConfig;

            visual = VisualUtil.newButton(app, localizedConfig.managedAtlasPath.path, localizedConfig.buttonAssetsPrefix, parent, null, localizedConfig.touchDownSoundConfig, localizedConfig.touchUpSoundConfig, localizedConfig.disabledTouchDownSoundConfig);
        }else if(visualCreateConfig.type == VisualCreateConfig.TYPE_LABEL) {
            VisualCreateConfig_Label localizedConfig = (VisualCreateConfig_Label)visualCreateConfig;
    
            FontConfig fc;
            if(localizedConfig.fontData instanceof FontConfig) {
                fc = (FontConfig)localizedConfig.fontData;
            }else {
                fc = FontConfig.entriesMap.get(localizedConfig.fontData);
            }
            visual = VisualUtil.newLabel(app, fc, localizedConfig.defaultText, parent);
        }else if(visualCreateConfig.type == VisualCreateConfig.TYPE_TOUCH_AREA_AUDIBLE) {
            VisualCreateConfig_TouchAreaAudible localizedConfig = (VisualCreateConfig_TouchAreaAudible)visualCreateConfig;

            visual = VisualUtil.newTouchAreaButton(app, localizedConfig.touchDownSoundConfig, localizedConfig.touchUpSoundConfig, parent);
        }else if(visualCreateConfig.type == VisualCreateConfig.TYPE_TOUCH_AREA_LIGHTWEIGHT) {
            VisualCreateConfig_TouchAreaLightweight localizedConfig = (VisualCreateConfig_TouchAreaLightweight)visualCreateConfig;

            visual = VisualUtil.newTouchAreaLightweight(app, parent);
        }else if(visualCreateConfig.type == VisualCreateConfig.TYPE_CUSTOM_RECTANGLE_SPRITE) {
            VisualCreateConfig_CustomRectangleSprite localizedConfig = (VisualCreateConfig_CustomRectangleSprite)visualCreateConfig;

            Integer color;
            if(localizedConfig.color instanceof String) {
                color = (Integer) createConfigMap.get((String) localizedConfig.color);
            }else if(localizedConfig.color instanceof Integer) {
                color = (Integer)localizedConfig.color;
            }else {
                app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }

            visual = VisualUtil.newCustomRectangleSprite(app, color, localizedConfig.textureSize, parent);
        }else if(visualCreateConfig.type == VisualCreateConfig.TYPE_PIXEL_SPRITE) {
            VisualCreateConfig_PixelSprite localizedConfig = (VisualCreateConfig_PixelSprite)visualCreateConfig;

            visual = VisualUtil.newPixelSprite(app, localizedConfig.pixelData, parent);
        }else {
            Gdx.app.debug("Error", "Error creating visual of type: " + visualCreateConfig.type);

            return null;
        }

        return visual;
    }

    public static Container2D newGroup(IContainer2D parent, Rectangle maskRectangle) {
        Container2D spritesVisualsGroup = newGroup(parent);

        if(maskRectangle != null) {
            spritesVisualsGroup.setMaskRectangle(maskRectangle);
        }

        return spritesVisualsGroup;
    }

    public static Container2D newGroup(IContainer2D parent) {
        Container2D spritesVisualsGroup = new Container2D();

        spritesVisualsGroup.setIsInputInteractive(true);

        if(parent != null) {
            parent.addChild(spritesVisualsGroup);
        }

        return spritesVisualsGroup;
    }

    public static Sprite2D newSprite(AppLocal app, String atlasID, String regionID, IContainer2D parent) {
        Sprite2D sprite;

        if(atlasID == null && regionID == null) {
            sprite = new Sprite2D();
        }else {
            TextureAtlas.AtlasRegion region = app.embeddedAssetsManager.getTextureAtlasRegion(atlasID, regionID);
            if(region == null) {
                Gdx.app.debug("Error", "Texture atlas region not found. Atlas fxId: " + atlasID + ", region fxId: " + regionID);

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }

            sprite = new Sprite2D(region);
        }

        if(parent != null) {
            parent.addChild(sprite);
        }

        return sprite;
    }

    public static Sprite2D newPixelSprite(AppLocal app, PixelVisualData pixelData, IContainer2D parent) {
        Pixmap pixmap = new Pixmap(pixelData.width, pixelData.height, Pixmap.Format.RGBA8888);

        for(int i=0;i<pixelData.pixelsList.length;i++) {
            int pixelEntry = pixelData.pixelsList[i];
            pixmap.setColor(pixelEntry);
            pixmap.fillRectangle(i % pixelData.width, (int)Math.floor(i / pixelData.width), 1, 1);
        }

        Texture pixmapTex = new Texture(pixmap);
        pixmap.dispose();

        Sprite2D sprite = VisualUtil.newTextureSprite(app, pixmapTex, null);

        if(parent != null) {
            parent.addChild(sprite);
        }

        return sprite;
    }

    public static Sprite2D newCustomRectangleSprite(AppLocal app, Integer finalColor, Size2DInt textureSize, IContainer2D parent) {
        String texBufferedID = "cTex_" + finalColor.toString() + "_" + Integer.toString(textureSize.width) + "_" + Integer.toString(textureSize.height);
        Texture tex = app.embeddedAssetsManager.peekCustomTexture(texBufferedID);

        if(tex == null) {
            Pixmap pixmap = new Pixmap(textureSize.width, textureSize.height, Pixmap.Format.RGBA8888);
            pixmap.setColor(finalColor);
            pixmap.fillRectangle(0, 0, textureSize.width, textureSize.height);
            tex = new Texture(pixmap);
            pixmap.dispose();

            app.embeddedAssetsManager.pushCustomTexture(texBufferedID, tex);
        }

        Sprite2D sprite = VisualUtil.newTextureSprite(app, tex, null);

        if(parent != null) {
            parent.addChild(sprite);
        }

        return sprite;
    }

    public static Sprite2D newTextureSprite(AppLocal app, Texture texture, IContainer2D parent) {
        Sprite2D sprite = new Sprite2D(texture);

        if(parent != null) {
            parent.addChild(sprite);
        }

        return sprite;
    }

    public static Sprite2D newSprite(
            AppLocal app,
            String atlasID, String regionID,
            float posX, float posY,
            float width, float height,
            float anchorX, float anchorY,
            IContainer2D parent
    ) {
        Sprite2D sprite = newSprite(app, atlasID, regionID, parent);
        if(sprite == null) {
            return null;
        }

        sprite.setSize(width, height);
        sprite.setPosition(posX, posY);
        sprite.setAnchor(anchorX, anchorY);

        return sprite;
    }

    public static TouchAreaAudible newTouchAreaButton(AppLocal app, float posX, float posY, float width, float height, float anchorX, float anchorY, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig, IContainer2D parent, base.interaction.TouchListener touchListener) {
        TouchAreaAudible area2D = newTouchAreaButton(app, touchDownSoundConfig, touchUpSoundConfig, parent);

        area2D.setSize(width, height);
        area2D.setAnchor(anchorX, anchorY);
        area2D.setPosition(posX, posY);

        if(touchListener != null) {
            area2D.setTouchListener(touchListener);
        }

        return area2D;
    }

    public static TouchAreaAudible newTouchAreaButton(AppLocal app, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig, IContainer2D parent) {
        base.audio.util.SoundItem touchDownSound = null;
        base.audio.util.SoundItem touchUpSound = null;
        if(touchDownSoundConfig != null) {
            touchDownSound = app.soundManager.hookNewSoundItem(touchDownSoundConfig);
        }
        if(touchUpSoundConfig != null) {
            touchUpSound = app.soundManager.hookNewSoundItem(touchUpSoundConfig);
        }

        TouchAreaAudible area2D = new TouchAreaAudible(touchDownSound, touchUpSound);

        area2D.setIsInputInteractive(true);

        if(parent != null) {
            parent.addChild(area2D);
        }

        return area2D;
    }

    public static TouchAreaLightweight newTouchAreaLightweight(AppLocal app, float posX, float posY, float width, float height, float anchorX, float anchorY, IContainer2D parent, base.interaction.TouchListener touchListener) {
        TouchAreaLightweight area2D = newTouchAreaLightweight(app, parent);

        area2D.setSize(width, height);
        area2D.setAnchor(anchorX, anchorY);
        area2D.setPosition(posX, posY);

        if(touchListener != null) {
            area2D.setTouchListener(touchListener);
        }

        return area2D;
    }

    public static TouchAreaLightweight newTouchAreaLightweight(AppLocal app, IContainer2D parent) {
        TouchAreaLightweight area2D = new TouchAreaLightweight();

        area2D.setIsInputInteractive(true);

        if(parent != null) {
            parent.addChild(area2D);
        }

        return area2D;
    }

    public static BitmapLabel newLabel(AppLocal app, FontConfig fontConfig, String text, IContainer2D parent) {
        BitmapFont font = app.fontsManager.getBitmapFont(fontConfig);

        BitmapLabel label = new BitmapLabel(font);

        label.setText(text);

        if(parent != null) {
            parent.addChild(label);
        }

        return label;
    }

    public static BitmapLabel newLabel(AppLocal app, FontConfig fontConfig, float posX, float posY, float anchorX, float anchorY, String text, IContainer2D parent) {
        BitmapLabel label = newLabel(app, fontConfig, text, parent);

        label.setText(text);

        label.setAnchor(anchorX, anchorY);
        label.setPosition(posX, posY);

        return label;
    }

    public static MovieClip newMovieClip(AppLocal app, String atlasID, String regionIDPrefix, int regionIndexStart, int regionIndexEnd, int indexZeroPadding, float frameDuration, Animation.PlayMode playMode, float x, float y, float width, float height, float anchorX, float anchorY, IContainer2D parent, base.interaction.TouchListener touchListener) {
        MovieClip mc = newMovieClip(app, atlasID, regionIDPrefix, regionIndexStart, regionIndexEnd, indexZeroPadding, frameDuration, playMode, parent);

        mc.setAnchor(anchorX, anchorY);
        mc.setSize(width, height);
        mc.setPosition(x, y);

        if(touchListener != null) {
            mc.setIsInputInteractive(true);
            mc.setTouchListener(touchListener);
        }

        return mc;
    }

    public static MovieClip newMovieClip(AppLocal app, String atlasID, String regionIDPrefix, int regionIndexStart, int regionIndexEnd, int indexZeroPadding, float frameDuration, Animation.PlayMode playMode, IContainer2D parent) {
        Array<TextureAtlas.AtlasRegion> keyFrames = app.embeddedAssetsManager.getTextureAtlasRegionsArray(atlasID, regionIDPrefix, regionIndexStart, regionIndexEnd, indexZeroPadding);

        Animation<TextureRegion> animation = new Animation<TextureRegion>(frameDuration, keyFrames, playMode);
        MovieClip mc = new MovieClip(animation);

        if(parent != null) {
            parent.addChild(mc);
        }

        return mc;
    }

    public static MovieClipButton newMovieClipButton(AppLocal app, String atlasID, String regionIDPrefix, int regionIndexStart, int regionIndexEnd, int indexZeroPadding, float frameDuration, Animation.PlayMode playMode, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig, IContainer2D parent) {
        return newMovieClipButton(app, atlasID, regionIDPrefix, regionIndexStart, regionIndexEnd, indexZeroPadding, frameDuration, playMode, touchDownSoundConfig, touchUpSoundConfig, null, parent);
    }

    public static MovieClipButton newMovieClipButton(AppLocal app, String atlasID, String regionIDPrefix, int regionIndexStart, int regionIndexEnd, int indexZeroPadding, float frameDuration, Animation.PlayMode playMode, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig, SoundConfig disabledTouchDownSoundConfig, IContainer2D parent) {
        base.audio.util.SoundItem touchDownSound = null;
        base.audio.util.SoundItem touchUpSound = null;
        if(touchDownSoundConfig != null) {
            touchDownSound = app.soundManager.hookNewSoundItem(touchDownSoundConfig);
        }
        if(touchUpSoundConfig != null) {
            touchUpSound = app.soundManager.hookNewSoundItem(touchUpSoundConfig);
        }

        Array<TextureAtlas.AtlasRegion> keyFrames = app.embeddedAssetsManager.getTextureAtlasRegionsArray(atlasID, regionIDPrefix, regionIndexStart, regionIndexEnd, indexZeroPadding);

        Animation<TextureRegion> animation = new Animation<TextureRegion>(frameDuration, keyFrames, playMode);
        MovieClipButton mc = new MovieClipButton(animation, touchDownSound, touchUpSound);

        if(parent != null) {
            parent.addChild(mc);
        }

        return mc;
    }

    public static SpriteButton newButton(AppLocal app, String atlasID, String buttonAssetsPrefix, float posX, float posY, float width, float height, float anchorX, float anchorY, IContainer2D parent, base.interaction.TouchListener touchListener, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig, SoundConfig disabledTouchDownSoundConfig) {
        SpriteButton spriteButton = newButton(app, atlasID, buttonAssetsPrefix, parent, touchListener, touchDownSoundConfig, touchUpSoundConfig, disabledTouchDownSoundConfig);
        if(spriteButton == null) {
            return null;
        }

        spriteButton.setBounds(posX, posY, width, height);
        spriteButton.setAnchor(anchorX, anchorY);

        return spriteButton;
    }

    public static SpriteButton newButton(AppLocal app, String atlasID, String buttonAssetsPrefix, IContainer2D parent, base.interaction.TouchListener touchListener, SoundConfig touchDownSoundConfig, SoundConfig touchUpSoundConfig, SoundConfig disabledTouchDownSoundConfig) {
        Map<String, TextureAtlas.AtlasRegion> stateVisuals = new HashMap<String, TextureAtlas.AtlasRegion>();

        TextureAtlas atlas = app.embeddedAssetsManager.getTextureAtlas(atlasID);

        String[] spriteIDs = new String[]{"up", "down", "disabled", "selected_up", "selected_down", "selected_disabled"};
        for(String stateID : spriteIDs) {
            TextureAtlas.AtlasRegion atlasRegion = atlas.findRegion(buttonAssetsPrefix + stateID);
            if(atlasRegion != null) {
                stateVisuals.put(stateID, atlasRegion);
            }else if(stateID.equals("up")) {
                Gdx.app.debug("CDU", "Button up state not found. AtlasID: " + atlasID + ", Button assets prefix: " + buttonAssetsPrefix);

                return null;
            }
        }

        base.audio.util.SoundItem touchDownSound = null;
        base.audio.util.SoundItem touchUpSound = null;
        base.audio.util.SoundItem disabledTouchDownSound = null;
        if(touchDownSoundConfig != null) {
            touchDownSound = app.soundManager.hookNewSoundItem(touchDownSoundConfig);
        }
        if(touchUpSoundConfig != null) {
            touchUpSound = app.soundManager.hookNewSoundItem(touchUpSoundConfig);
        }
        if(disabledTouchDownSoundConfig != null) {
            disabledTouchDownSound = app.soundManager.hookNewSoundItem(disabledTouchDownSoundConfig);
        }

        SpriteButton spriteButton = new SpriteButton(touchDownSound, touchUpSound, disabledTouchDownSound, stateVisuals);

        spriteButton.setIsInputInteractive(true);

        spriteButton.setTouchListener(touchListener);

        if(parent != null) {
            parent.addChild(spriteButton);
        }

        return spriteButton;
    }

    //asdq;// This should be called after rendering finishes, not instantly. Use a flag on AppBase with a callback{success, fail}.
    public static final Pixmap getScreenshotPixmap(AppLocal app, int x, int y, int w, int h, boolean yDown) {
        final Pixmap pixmap = ScreenUtils.getFrameBufferPixmap(x, y, w, h);

        if (yDown)
        {
            // Flip the pixmap upside down;
            ByteBuffer pixels = pixmap.getPixels();
            int numBytes = w * h * 4;
            byte[] lines = new byte[numBytes];
            int numBytesPerLine = w * 4;
            for (int i = 0; i < h; i++) {
                pixels.position((h - i - 1) * numBytesPerLine);
                pixels.get(lines, i * numBytesPerLine, numBytesPerLine);
            }
            pixels.clear();
            pixels.put(lines);
            pixels.clear();
        }

        return pixmap;
    }

    public static boolean savePixmapPng(AppLocal app, String filePath, Pixmap pixmap, boolean flipY){
        if(!FileUtil.deleteFile(FileUtil.FILE_SYSTEM_EXTERNAL, filePath)) {
            return false;
        }

        try {
            FileHandle fh = Gdx.files.external(filePath);

            PixmapIO.PNG pngWriter = new PixmapIO.PNG();
            pngWriter.setFlipY(flipY);
            pngWriter.setCompression(Deflater.BEST_SPEED);
            pngWriter.write(fh, pixmap);
            //PixmapIO.writePNG(fh, pixmap);
        }catch (Exception e) {
            // Screenshot save hhtpRequestFailed. Reason may be insufficient space.

            Gdx.app.debug("Warning", "Warning occurred while saving the screenshot.");

            return false;
        }

        return true;
    }

    public static boolean saveScreenshot(AppLocal app, String filePath){
        if(!FileUtil.deleteFile(FileUtil.FILE_SYSTEM_EXTERNAL, filePath)) {
            return false;
        }

        int ww = Gdx.graphics.getWidth();
        int hh = Gdx.graphics.getHeight();
        Pixmap pixmap = getScreenshotPixmap(app, 0, 0, ww, hh, false);
        if(pixmap == null) {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        boolean b = savePixmapPng(app, filePath, pixmap, true);
        pixmap.dispose();

        if(!b) {
            return false;
        }

        return true;
    }

    public static void copyBitmapDataAndConvert_ARGB8888_To_RGBA8888(int[] destinationPixelData, int[] sourcePixelData) {
        for(int i=0;i<sourcePixelData.length;i++) {
            int pixel = sourcePixelData[i];

            int a = (int)(((pixel & 0xff000000)>>24) & 0x000000ff);
            int r = (int)(((pixel & 0x00ff0000)>>16) & 0x000000ff);
            int g = (int)(((pixel & 0x0000ff00)>>8 ) & 0x000000ff);
            int b = (int)(((pixel & 0x000000ff)>>0 ) & 0x000000ff);

            pixel = (int)( r << 24 | g << 16 | b << 8 | a );

            destinationPixelData[i] = pixel;
        }
    }

    public static void bitmapData_applyCircularMaskRGBA8888(int[] pixelData, int width, int height) {
        float distThresholdPowered = (width / 2f) * (width / 2f);
        for(int i=0;i<pixelData.length;i++) {
            float x = i % width - width / 2f;
            float y = (int)Math.floor(i / width) - height / 2f;
            float distPowered = x * x + y * y;

            if(distPowered > distThresholdPowered) {
                pixelData[i] = 0x00000000;
            }
        }
    }

    public static void disposeVisualItemsMap(AppLocal app, Map<String, IDrawableNode> visualsMap) {
        for (Map.Entry<String, IDrawableNode> entry : visualsMap.entrySet()) {
            if(entry != null && entry.getValue() != null && !entry.getKey().equals("parent")) {
                IDrawableNode item = entry.getValue();

                item.dispose();

                recycleVisualItem(app, item);
            }
        }
    }

    private static void recycleVisualItem(AppLocal app, IDrawableNode item) {
        IDrawableNode cEntry = (IDrawableNode) item;

        if (cEntry.getParent() != null) {
            cEntry.getParent().removeChild(cEntry);
        }
    }

}
