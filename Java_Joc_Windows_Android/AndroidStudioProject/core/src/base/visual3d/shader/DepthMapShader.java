package base.visual3d.shader;

import com.badlogic.gdx.graphics.Camera;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.g3d.Attributes;
import com.badlogic.gdx.graphics.g3d.Renderable;
import com.badlogic.gdx.graphics.g3d.attributes.BlendingAttribute;
import com.badlogic.gdx.graphics.g3d.utils.RenderContext;
import com.badlogic.gdx.graphics.glutils.ShaderProgram;

/**
 * Created by adrin on 5/4/17.
 */

public class DepthMapShader extends BasicShader {
    
    public DepthMapShader(final Renderable renderable, final ShaderProgram shaderProgramModelBorder) {
        super(renderable, shaderProgramModelBorder);
        
        register(Inputs.worldTrans, Setters.worldTrans);
        register(Inputs.projViewTrans, Setters.projViewTrans);
    }
    
    @Override
    public void init() {
        super.init();
        
        //void
    }
    
    @Override
    public void begin(final Camera camera, final RenderContext context) {
        super.begin(camera, context);
        
        context.setDepthTest(GL20.GL_LEQUAL);
        context.setCullFace(GL20.GL_BACK);
    }
    
    @Override
    public void end() {
        super.end();
    }
    
    @Override
    public void render(final Renderable renderable) {
        if (!renderable.material.has(BlendingAttribute.Type)) {
            context.setBlending(false, GL20.GL_SRC_ALPHA, GL20.GL_ONE_MINUS_SRC_ALPHA);
        } else {
            context.setBlending(true, GL20.GL_SRC_ALPHA, GL20.GL_ONE_MINUS_SRC_ALPHA);
        }
        
        super.render(renderable);
    }
    
    @Override
    public boolean canRender(final Renderable instance) {
        return true;
    }
    
    @Override
    public void render(final Renderable renderable, final Attributes combinedAttributes) {
        super.render(renderable, combinedAttributes);
    }
    
}
