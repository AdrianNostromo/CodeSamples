package base.font;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.g2d.BitmapFont;

import java.util.HashMap;
import java.util.Map;

import base.font.config.FontConfig;
import base.font.freetype.CustomFreeTypeFontGenerator;
import base.font.util.FontSourceFntFile;
import base.font.util.FontSourceGenerated;
import local.app.AppLocal;

public class FontsManager {

	protected AppLocal app;

	public Map<String, CustomFreeTypeFontGenerator> bitmapFontGeneratorsMap = new HashMap<String, CustomFreeTypeFontGenerator>();
	public Map<String, BitmapFont> bitmapFontsMap = new HashMap<String, BitmapFont>();

	public FontsManager(AppLocal app) {
		super();

		this.app = app;
	}

	public CustomFreeTypeFontGenerator getFontGenerator(String fontID) {
		CustomFreeTypeFontGenerator fontGenerator = bitmapFontGeneratorsMap.get(fontID);
		if(fontGenerator == null) {
			fontGenerator = new CustomFreeTypeFontGenerator(Gdx.files.internal(fontID));

            fontGenerator.setMaxTextureSize(2048);

			bitmapFontGeneratorsMap.put(fontID, fontGenerator);
		}

		return fontGenerator;
	}

	protected BitmapFont getBitmapFont(FontConfig fontConfig, CustomFreeTypeFontGenerator.FreeTypeFontParameter parameter, String bakedBufferFontID, int bakedFontSize) {
		BitmapFont font = bitmapFontsMap.get(bakedBufferFontID);
		if(font == null) {
			if(fontConfig.fontSource instanceof FontSourceFntFile) {
				// Load font as a bitmap font.

				font = app.embeddedAssetsManager.getBitmapFntFont(((FontSourceFntFile)fontConfig.fontSource).fntFileName);

				if(((FontSourceFntFile)fontConfig.fontSource).color != null) {
					if (!font.getColor().equals(((FontSourceFntFile) fontConfig.fontSource).color)) {
						font.setColor(((FontSourceFntFile) fontConfig.fontSource).color);
					}
				}
			}else {
				// Generate the font using the fonts generator. This method is slower.
                long tm = System.currentTimeMillis();

				CustomFreeTypeFontGenerator fontGenerator = getFontGenerator(((FontSourceGenerated)fontConfig.fontSource).ttfFileName);

                if(parameter == null) {
                    Gdx.app.debug("Error", "Error occurred.");

                    return null;
                }

				parameter.size = bakedFontSize;
                // NOTE : Characters should be limited in the fnt file. Limiting characters here causes problems with libgdx not beying able to use them.
				parameter.characters = FontConfig.CHARS_LIST[fontConfig.charactersListId];
                parameter.color = ((FontSourceGenerated)fontConfig.fontSource).color;

                //asd_002;// Try to use multiple threads for fonts generation (4 threads).

                font = fontGenerator.generateFont(parameter);

                if(parameter.usedPacker == null) {
                    Gdx.app.debug("Error", "Font generation error occurred.");

                    return null;
                }

				pushBitmapFontToMap(bakedBufferFontID, font);

                app.log.postLog("Profiling. Font generation time: " + (System.currentTimeMillis() - tm) + ", Baked font id: " + bakedBufferFontID);
            }
		}

		return font;
	}

	public boolean isBitmapFontInMap(String fontID) {
		BitmapFont font = bitmapFontsMap.get(fontID);
		if(font != null) {
			return true;
		}

		return false;
	}

	public void pushBitmapFontToMap(String fontID, BitmapFont font) {
		bitmapFontsMap.put(fontID, font);
	}

}
