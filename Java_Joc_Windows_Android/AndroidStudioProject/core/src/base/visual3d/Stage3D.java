package base.visual3d;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Camera;
import com.badlogic.gdx.graphics.g3d.Environment;
import com.badlogic.gdx.graphics.g3d.ModelBatch;

import java.util.Vector;

public class Stage3D implements IStage3D {

    private Vector<ModelInstance3D> modelsList = new Vector<ModelInstance3D>();

    public Stage3D() {
        super();
        
        //void
    }

    public void create() {
        //void
    }

    public void addChild(ModelInstance3D model) {
        if(model.getStage3D() != null) {
            Gdx.app.debug("Error", "Visual is already a child of the current group.");
    
            return ;
        }
        
        if(modelsList.indexOf(model) >= 0) {
            Gdx.app.debug("Error", "Visual is already a child of the current group.");

            return ;
        }

        modelsList.add(model);
        model.putStage3D(this);
    }

    public void removeChild(ModelInstance3D model) {
        if(model.getStage3D() != this) {
            Gdx.app.debug("Error", "Visual is already a child of the current group.");
    
            return ;
        }
        
        int i = modelsList.indexOf(model);
        if(i >= 0) {
            model.putStage3D(null);
            
            modelsList.remove(i);
        }
    }

    public void batchDraw(ModelBatch modelBatch, Camera camera) {
        batchDraw(modelBatch, camera, null);
    }

    public void batchDraw(ModelBatch modelBatch, Camera camera, Environment environment3D) {
        modelBatch.begin(camera);

        for(int i=0;i<modelsList.size();i++) {
            ModelInstance3D model = modelsList.get(i);

            if(model.getVisible()) {
                if(environment3D != null) {
                    modelBatch.render(model, environment3D);
                }else {
                    modelBatch.render(model);
                }
            }
        }
    
        modelBatch.end();
    }

}
