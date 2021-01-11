package base.visual3d.shader;

import com.badlogic.gdx.graphics.Camera;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.g3d.Renderable;
import com.badlogic.gdx.graphics.g3d.attributes.BlendingAttribute;
import com.badlogic.gdx.graphics.g3d.attributes.TextureAttribute;
import com.badlogic.gdx.graphics.g3d.utils.RenderContext;
import com.badlogic.gdx.graphics.glutils.ShaderProgram;

/**
 * Simple shader for an object with a texture
 */
public class BasicColorShader extends BasicShader {
    
	public BasicColorShader(final Renderable renderable, final ShaderProgram program) {
        super(renderable, program);
        
        register(Inputs.projViewTrans, Setters.projViewTrans);
        register(Inputs.worldTrans, Setters.worldTrans);
        
        register(Inputs.diffuseColor, Setters.diffuseColor);
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
    public void render(Renderable renderable) {
        if (!renderable.material.has(BlendingAttribute.Type)) {
			context.setBlending(false, GL20.GL_SRC_ALPHA, GL20.GL_ONE_MINUS_SRC_ALPHA);
		} else {
			context.setBlending(true, GL20.GL_SRC_ALPHA, GL20.GL_ONE_MINUS_SRC_ALPHA);
		}
		
		super.render(renderable);
	}
    
    @Override
    public boolean canRender(Renderable instance) {
        //return super.canRender(instance);
        
        if(!instance.material.has(TextureAttribute.Diffuse)) {
            return true;
        }
        
        return false;
    }
    
}
