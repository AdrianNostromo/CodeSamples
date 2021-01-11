package base.assets;

import com.badlogic.gdx.assets.loaders.FileHandleResolver;
import com.badlogic.gdx.graphics.g3d.Model;

import java.util.HashMap;
import java.util.Map;
import java.util.Stack;

import base.visual.Container2D;
import base.visual3d.BufferedModelInstance;
import base.visual3d.ModelInstance3D;
import local.app.AppLocal;

/**
 * Created by Adrin on 7/31/2016.
 */
public class Buffered3DModelInstances extends TypedAssetsManager {

    private Map<String, Stack<ModelInstance3D>> bufferModelInstance = new HashMap<String, Stack<ModelInstance3D>>();

    public Buffered3DModelInstances(AppLocal app, FileHandleResolver resolver) {
        super(app, resolver);

        //void
    }

    public ModelInstance3D getModelInstance(String modelId) {
        ModelInstance3D modelInstance = null;

        // Check if instance exists already.
        Stack<ModelInstance3D> modelInstancesList = bufferModelInstance.get(modelId);
        if(modelInstancesList != null && modelInstancesList.size() > 0) {
            modelInstance = modelInstancesList.pop();
        }

        if(modelInstance == null) {
            Model model = getModel3D(modelId);

            modelInstance = new BufferedModelInstance(model, modelId);
        }

        return modelInstance;
    }

    public void releaseModelInstance(ModelInstance3D modelInstance) {
        String modelId = ((BufferedModelInstance)modelInstance).modelId;
        
        Stack<ModelInstance3D> modelInstancesList = bufferModelInstance.get(modelId);
        if(modelInstancesList == null) {
            modelInstancesList = new Stack<ModelInstance3D>();

            bufferModelInstance.put(modelId, modelInstancesList);
        }

        modelInstancesList.push(modelInstance);
    }

    public void releaseSprite(base.visual.Sprite2D sprite) {
        // Sprites are not recycled currently.
    }

    public void releaseGroup(Container2D group) {
        // Groups are not recycled currently.
    }

}
