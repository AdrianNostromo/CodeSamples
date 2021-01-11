package base.assets;

import com.badlogic.gdx.assets.loaders.FileHandleResolver;

import java.util.HashMap;
import java.util.Map;

import local.app.AppLocal;

/**
 * Created by Adrin on 7/31/2016.
 */
public class BufferedObjects extends Buffered3DModelInstances {

    private Map<String, Object> bufferCustomObjects = new HashMap<String, Object>();

    public BufferedObjects(AppLocal app, FileHandleResolver resolver) {
        super(app, resolver);

        //void
    }

    public void pushCustomObject(String id, Object obj) {
        if(peekCustomObject(id) != null) {
            app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

            throw new Error("Error. 1.");
        }

        bufferCustomObjects.put(id, obj);
    }

    public Object peekCustomObject(String id) {
        Object obj = bufferCustomObjects.get(id);

        return obj;
    }

}
