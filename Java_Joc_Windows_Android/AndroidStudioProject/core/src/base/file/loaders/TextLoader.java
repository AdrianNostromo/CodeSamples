package base.file.loaders;

import com.badlogic.gdx.assets.AssetDescriptor;
import com.badlogic.gdx.assets.AssetLoaderParameters;
import com.badlogic.gdx.assets.AssetManager;
import com.badlogic.gdx.assets.loaders.AsynchronousAssetLoader;
import com.badlogic.gdx.assets.loaders.FileHandleResolver;
import com.badlogic.gdx.files.FileHandle;
import com.badlogic.gdx.utils.Array;

/**
 * Created by Adrin on 5/29/2016.
 */
public class TextLoader extends AsynchronousAssetLoader<base.file.Text, TextLoader.TextParameter> {

    public TextLoader(FileHandleResolver resolver) {

        super(resolver);

    }

    base.file.Text text;

    @Override
    public void loadAsync(AssetManager manager, String fileName, FileHandle file, TextParameter parameter) {

        this.text = null;
        this.text = new base.file.Text(file);

    }

    @Override
    public base.file.Text loadSync(AssetManager manager, String fileName, FileHandle file, TextParameter parameter) {

        base.file.Text text = this.text;
        this.text = null;

        return text;

    }

    @SuppressWarnings("rawtypes")
    @Override
    public Array<AssetDescriptor> getDependencies(String fileName, FileHandle file, TextParameter parameter) {

        return null;

    }

    public static class TextParameter extends AssetLoaderParameters<base.file.Text> {

    }
}
