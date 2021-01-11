package base.visual.util;

/**
 * Created by Adrin on 7/30/2016.
 */
public class VisualCreateConfig_Sprite extends VisualCreateConfig {

    public ManagedTextureAtlasPath managedAtlasPath;
    public String regionID;

    public VisualCreateConfig_Sprite(ManagedTextureAtlasPath managedAtlasPath, String regionID) {
        super(TYPE_SPRITE);

        this.managedAtlasPath = managedAtlasPath;
        this.regionID = regionID;
    }

}
