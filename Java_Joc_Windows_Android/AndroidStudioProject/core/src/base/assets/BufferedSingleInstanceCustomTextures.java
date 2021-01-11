package base.assets;

import com.badlogic.gdx.assets.loaders.FileHandleResolver;
import com.badlogic.gdx.graphics.Texture;

import java.util.HashMap;
import java.util.Map;

import local.app.AppLocal;

/**
 * Created by Adrin on 7/31/2016.
 */
public class BufferedSingleInstanceCustomTextures extends BufferedObjects {

    private Map<String, Texture> bufferCustomTextures = new HashMap<String, Texture>();

    public BufferedSingleInstanceCustomTextures(AppLocal app, FileHandleResolver resolver) {
        super(app, resolver);

        //void
    }

    public void pushCustomTexture(String id, Texture tex) {
        if(peekCustomTexture(id) != null) {
            app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

            throw new Error("Error. 1.");
        }

        bufferCustomTextures.put(id, tex);
    }

    public Texture peekCustomTexture(String id) {
        Texture tex = bufferCustomTextures.get(id);

        return tex;
    }

}
