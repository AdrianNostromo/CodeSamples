package base.visual3d;

import com.badlogic.gdx.graphics.g3d.Model;

/**
 * Created by adrin on 3/29/17.
 */

public class BufferedModelInstance extends ModelInstance3D {
    
    public String modelId;
    
    public BufferedModelInstance(final Model model, String modelId) {
        super(model);
        
        this.modelId = modelId;
    }
    
}
