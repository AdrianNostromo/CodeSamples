package base.visual3d.util;

import java.util.Vector;

import base.assets.util.ManagedFilePath;

/**
 * Created by adrin on 4/23/17.
 */

public class ManagedModel3DPath extends ManagedFilePath {
    // These assets are automatically preloaded.
    
    public static final Vector<ManagedFilePath> entriesList = new Vector<ManagedFilePath>();
    
    public ManagedModel3DPath(String path) {
        super(path);
        
        entriesList.add(this);
    }
    
}
