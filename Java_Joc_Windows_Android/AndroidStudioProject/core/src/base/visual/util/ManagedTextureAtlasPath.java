package base.visual.util;

import java.util.Vector;

import base.assets.util.ManagedFilePath;

/**
 * Created by adrin on 4/23/17.
 */

public class ManagedTextureAtlasPath extends ManagedFilePath {
    // These assets are automatically preloaded.
    
    public static final Vector<ManagedFilePath> entriesList = new Vector<ManagedFilePath>();
    
    public ManagedTextureAtlasPath(String path) {
        super(path);
        
        entriesList.add(this);
    }
    
}