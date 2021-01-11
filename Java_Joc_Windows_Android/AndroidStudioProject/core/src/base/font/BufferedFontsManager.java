package base.font;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Preferences;
import com.badlogic.gdx.files.FileHandle;
import com.badlogic.gdx.graphics.g2d.BitmapFont;
import com.badlogic.gdx.graphics.g2d.PixmapPacker;
import com.badlogic.gdx.utils.GdxRuntimeException;

import base.font.config.FontConfig;
import base.font.freetype.CustomFreeTypeFontGenerator;
import base.font.util.FontSourceGenerated;
import base.font.util.writer.BitmapFontWriter;
import local.app.AppLocal;

public class BufferedFontsManager extends FontsManager {

	private Preferences fontPrefs;

    // Incrementing the fonts version will force the fonts recreation.
    private static final int newFontsVersion = 8;

	public static final String bufferedFontsLocationPrefix = "generated-fonts/";

	public BufferedFontsManager(AppLocal app) {
		super(app);

		//void
	}

	public void create() {
		fontPrefs = Gdx.app.getPreferences("fontsBuffer");

        int fontVersion = fontPrefs.getInteger("fonts-version", 0);
		int displayWidth = fontPrefs.getInteger("display-width", 0);
		int displayHeight = fontPrefs.getInteger("display-height", 0);

		if(fontVersion != newFontsVersion || displayWidth != Gdx.graphics.getWidth() || displayHeight != Gdx.graphics.getHeight()) {
			fontPrefs.clear();

			fontPrefs.putInteger("fonts-version", newFontsVersion);
			fontPrefs.putInteger("display-width", Gdx.graphics.getWidth());
			fontPrefs.putInteger("display-height", Gdx.graphics.getHeight());
			fontPrefs.flush();
		}
	}

	public boolean isFontBuffered(String fontID) {
		boolean ret = fontPrefs.getBoolean(fontID, false);

		return ret;
	}

    public void clearFontBufferedIndicator(String fontID) {
        fontPrefs.putBoolean(fontID, false);
        fontPrefs.flush();
    }

	public BitmapFont getBitmapFont(FontConfig fontConfig) {
        float sizeScaleSourceValue = app.getValueFromID(((FontSourceGenerated)fontConfig.fontSource).scaleSource);
        int bakedFontSize = (int)Math.ceil(((FontSourceGenerated)fontConfig.fontSource).baseSize * (sizeScaleSourceValue / ((FontSourceGenerated)fontConfig.fontSource).scaleBaseValue));
        String bakedBufferFontID = FontConfig.ComputeBakedFontId(fontConfig, bakedFontSize);

		if (!isBitmapFontInMap(bakedBufferFontID) && isFontBuffered(bakedBufferFontID)) {
			// The font is not in the fonts map. Try to load it from the local buffer.

			try {
				BitmapFont font = app.localAssetsManager.getBitmapFntFont(bufferedFontsLocationPrefix + bakedBufferFontID + ".fnt");

				pushBitmapFontToMap(bakedBufferFontID, font);
			}catch (GdxRuntimeException error) {
				// The buffered font files are not found. Continue to generate and save it.

                clearFontBufferedIndicator(bakedBufferFontID);
			}
		}

		CustomFreeTypeFontGenerator.FreeTypeFontParameter parameter = null;
		if(fontConfig.fontSource instanceof FontSourceGenerated && !isFontBuffered(bakedBufferFontID)) {
			parameter = new CustomFreeTypeFontGenerator.FreeTypeFontParameter();
		}

		BitmapFont font = super.getBitmapFont(fontConfig, parameter, bakedBufferFontID, bakedFontSize);

		if(fontConfig.fontSource instanceof FontSourceGenerated && !isFontBuffered(bakedBufferFontID)) {
			// Buffer the font locally as a bitmap fnt font.

			fontPrefs.putBoolean(bakedBufferFontID, true);
			fontPrefs.flush();

			saveFontToFile(font, bakedFontSize, bakedBufferFontID, parameter.usedPacker);
		}

		return font;
	}

	private void saveFontToFile(BitmapFont font, int fontSize, String fontID, PixmapPacker packer) {
		FileHandle fontFile = Gdx.files.local(bufferedFontsLocationPrefix + fontID + ".fnt"); // .fnt impact
		FileHandle pixmapDir = Gdx.files.local(bufferedFontsLocationPrefix + fontID); // png dir impact
		BitmapFontWriter.setOutputFormat(BitmapFontWriter.OutputFormat.Text);

		String[] pageRefs = BitmapFontWriter.writePixmaps(packer.getPages(), pixmapDir, fontID);
		// here we must add the png dir to the page refs
		for (int i = 0; i < pageRefs.length; i++) {
			pageRefs[i] = fontID + "/" + pageRefs[i];
		}
		BitmapFontWriter.writeFont(font.getData(), pageRefs, fontFile, new BitmapFontWriter.FontInfo(fontID, fontSize), 1, 1);
	}

}
