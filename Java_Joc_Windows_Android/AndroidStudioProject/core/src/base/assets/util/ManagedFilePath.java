package base.assets.util;

import java.util.Vector;

/**
 * Created by adrin on 4/23/17.
 */

public class ManagedFilePath {
    
    public String path;
    
    public ManagedFilePath(String path) {
        super();
        
        this.path = path;
    }
    
    public static Vector<String> buildPathsList(Vector<ManagedFilePath> inList) {
        Vector<String> retList = new Vector<String>();
        for(int i = 0; i< inList.size(); i++) {
            ManagedFilePath entry = inList.get(i);
            if(entry != null && !retList.contains(entry.path)) {
                retList.add(entry.path);
            }
        }
        
        return retList;
    }
    
}
