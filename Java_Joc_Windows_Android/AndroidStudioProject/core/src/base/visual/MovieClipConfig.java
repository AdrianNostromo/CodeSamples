package base.visual;

// A configuration class for reel symbols.
public class MovieClipConfig {

	public String atlasID;
	public String framePrefix;
	public int regionIndexStart;
	public int regionIndexEnd;
	public int indexZeroPadding;
	public float frameDuration;

	public MovieClipConfig(String atlasID, String framePrefix, int regionIndexStart, int regionIndexEnd, int indexZeroPadding, float frameDuration) {
		super();

		this.atlasID = atlasID;
		this.framePrefix = framePrefix;
		this.regionIndexStart = regionIndexStart;
		this.regionIndexEnd = regionIndexEnd;
		this.indexZeroPadding = indexZeroPadding;
		this.frameDuration = frameDuration;
	}

}
