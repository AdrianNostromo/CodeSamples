package base.assets;

import com.badlogic.gdx.assets.loaders.FileHandleResolver;

import local.app.AppLocal;

public class BufferedAssetsManager extends BufferedSingleInstanceCustomTextures {

    //asd_002;// implement multi threaded assets loading using multiple EmbededAssetsManagerFragment. The fragments are handled by this class. The content from this class is moved to fragments. Use a list to save the assets that are contained by each fragment.

    public BufferedAssetsManager(AppLocal app, FileHandleResolver resolver) {
        super(app, resolver);

        //void
    }

}
