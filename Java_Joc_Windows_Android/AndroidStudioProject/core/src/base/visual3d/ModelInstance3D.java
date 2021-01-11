package base.visual3d;

import com.badlogic.gdx.graphics.g3d.Model;
import com.badlogic.gdx.graphics.g3d.ModelInstance;

/**
 * Created by adrin on 3/29/17.
 */

public class ModelInstance3D extends ModelInstance {
    
    private IStage3D stage3D = null;
    
    private boolean visible = true;
    
    public ModelInstance3D(final Model model) {
        super(model);
        
        //void
    }
    
    public IStage3D getStage3D() {
        return stage3D;
    }
    
    public void putStage3D(IStage3D stage3D) {
        this.stage3D = stage3D;
    }
    
    public boolean getVisible() {
        return visible;
    }
    
    public void putVisible(boolean visible) {
        this.visible = visible;
    }
    
}
