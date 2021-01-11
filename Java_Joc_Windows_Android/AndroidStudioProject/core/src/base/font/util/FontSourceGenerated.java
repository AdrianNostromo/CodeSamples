package base.font.util;

import com.badlogic.gdx.graphics.Color;

public class FontSourceGenerated extends FontSource {

	public String ttfFileName;
	public float baseSize;
	public Color color;

    public String scaleSource;
    public float scaleBaseValue;

	public FontSourceGenerated(String ttfFileName, float baseSize, Color color, String scaleSource, float scaleBaseValue) {
        super(ttfFileName.replaceAll("/", "_").replaceAll("\\.", "_") + "_" + color.toString());
        
		this.ttfFileName = ttfFileName;
		this.baseSize = baseSize;
		this.color = color;

        this.scaleSource = scaleSource;
        this.scaleBaseValue = scaleBaseValue;
	}
}
