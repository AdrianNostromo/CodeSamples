package base.visual3d.shader;

import com.badlogic.gdx.graphics.g3d.Renderable;
import com.badlogic.gdx.graphics.glutils.ShaderProgram;

/**
 * Simple shader for an object with a texture
 */
public class TextureShaderWithDirectionalLight extends TextureShaderWithSomeSelfShadows {
    
	public TextureShaderWithDirectionalLight(final Renderable renderable, final ShaderProgram program) {
        super(renderable, program);
        
        //void
	}
    
}