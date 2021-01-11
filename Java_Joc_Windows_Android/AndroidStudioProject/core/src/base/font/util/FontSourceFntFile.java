package base.font.util;

import com.badlogic.gdx.graphics.Color;

public class FontSourceFntFile extends FontSource {

	public String fntFileName;
	public Color color;

	public FontSourceFntFile(String fntFileName, Color color) {
        super(fntFileName.replaceAll("/", "_").replaceAll("\\.", "_") + "_" + color.toString());

		this.fntFileName = fntFileName;
		this.color = color;
	}
}
