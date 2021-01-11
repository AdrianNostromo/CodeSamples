package base.visual3d;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Camera;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.g3d.Attributes;
import com.badlogic.gdx.graphics.g3d.Material;
import com.badlogic.gdx.graphics.g3d.Renderable;
import com.badlogic.gdx.graphics.g3d.Shader;
import com.badlogic.gdx.graphics.g3d.attributes.TextureAttribute;
import com.badlogic.gdx.graphics.g3d.utils.RenderContext;
import com.badlogic.gdx.graphics.glutils.ShaderProgram;
import com.badlogic.gdx.math.Matrix3;
import com.badlogic.gdx.utils.GdxRuntimeException;

/**
 * Simple shader for an object with a texture
 */
public class MRTShader implements Shader {
    ShaderProgram shaderProgram;
    long attributes;
    
    RenderContext context;
    
    Matrix3 matrix3 = new Matrix3();
    static Attributes tmpAttributes = new Attributes();
    
    public MRTShader (Renderable renderable) {
        String prefix = "";
        if (renderable.material.has(TextureAttribute.Normal)) {
            prefix += "#define texturedFlag\n";
        }
        
        String vert = Gdx.files.internal("shaders/mrt.vert").readString();
        String frag = Gdx.files.internal("shaders/mrt.frag").readString();
        shaderProgram = new ShaderProgram(prefix + vert, prefix + frag);
        if (!shaderProgram.isCompiled()) {
            throw new GdxRuntimeException(shaderProgram.getLog());
        }
        renderable.material.set(tmpAttributes);
        attributes = tmpAttributes.getMask();
    }
    
    @Override
    public void init () {
    }
    
    @Override
    public int compareTo (Shader other) {
        //quick and dirty shader sort
        if (((MRTShader) other).attributes == attributes) return 0;
        if ((((MRTShader) other).attributes & TextureAttribute.Normal) == 1) return -1;
        return 1;
        
    }
    
    @Override
    public boolean canRender (Renderable instance) {
        return attributes == instance.material.getMask();
    }
    
    @Override
    public void begin (Camera camera, RenderContext context) {
        this.context = context;
        shaderProgram.begin();
        shaderProgram.setUniformMatrix("u_projViewTrans", camera.combined);
        context.setDepthTest(GL20.GL_LEQUAL);
        context.setCullFace(GL20.GL_BACK);
    }
    
    @Override
    public void render (Renderable renderable) {
        Material material = renderable.material;
        
        TextureAttribute diffuseTexture = (TextureAttribute)material.get(TextureAttribute.Diffuse);
        TextureAttribute normalTexture = (TextureAttribute)material.get(TextureAttribute.Normal);
        TextureAttribute specTexture = (TextureAttribute)material.get(TextureAttribute.Specular);
        
        if (diffuseTexture != null) {
            shaderProgram.setUniformi("u_diffuseTexture", context.textureBinder.bind(diffuseTexture.textureDescription.texture));
        }
        if (normalTexture != null) {
            shaderProgram.setUniformi("u_normalTexture", context.textureBinder.bind(normalTexture.textureDescription.texture));
        }
        if (specTexture != null) {
            shaderProgram.setUniformi("u_specularTexture", context.textureBinder.bind(specTexture.textureDescription.texture));
        }
        
        shaderProgram.setUniformMatrix("u_worldTrans", renderable.worldTransform);
        shaderProgram.setUniformMatrix("u_normalMatrix", matrix3.set(renderable.worldTransform).inv().transpose());
        
        renderable.meshPart.render(shaderProgram);
    }
    
    @Override
    public void end () {
        
    }
    
    @Override
    public void dispose () {
        shaderProgram.dispose();
    }
}
