package base.visual3d;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Input;
import com.badlogic.gdx.graphics.Camera;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.PerspectiveCamera;
import com.badlogic.gdx.graphics.g3d.ModelBatch;
import com.badlogic.gdx.graphics.g3d.Renderable;
import com.badlogic.gdx.graphics.g3d.Shader;
import com.badlogic.gdx.graphics.g3d.attributes.TextureAttribute;
import com.badlogic.gdx.graphics.glutils.ShaderProgram;
import com.badlogic.gdx.math.Vector3;

import base.app.util.IRenderable3D;
import base.visual3d.light.DirectionalLight;
import base.visual3d.light.Light;
import base.visual3d.shader.BasicColorShader;
import base.visual3d.shader.SceneShaderProvider;
import base.visual3d.shader.TextureShaderWithDirectionalLight;
import base.visual3d.util.IntFirstPersonCameraController;
import base.visual3d.util.ScreenshotFactory;
import local.app.AppLocal;

/**
 * Created by adrin on 5/1/17.
 */

public class World3D implements IRenderable3D {
    
    public AppLocal app;
    
    private boolean isRenderingEnabled = false;
    
    public Stage3D stage3D;
    
    public ModelBatch batch3D;
    
    public Camera camera3D = null;
    public boolean isDirty_camera = true;
    private IntFirstPersonCameraController firstPersonCameraController;
    
    public static final int DEPTHMAPSIZE = 1024;
    public Light currentLight;
    
    private ShaderProgram shaderProgram_color = null;
    private ShaderProgram shaderProgram_texture = null;
    
    // Debug code;
    private long lastScreenShot = 0;
    public boolean takeScreenshots = false;
    
    public World3D(AppLocal app) {
        super();
        
        this.app = app;
    }
    
    public void create() {
        stage3D = new Stage3D();
        stage3D.create();
    
        createMainOrthographicCamera();
        create_shader();
    
        currentLight = new DirectionalLight(this, new Vector3(1, -0.5f, 2), new Vector3(0, 1, -1));
    
        firstPersonCameraController = new IntFirstPersonCameraController(camera3D);
        firstPersonCameraController.setVelocity(5);
        // This overrides the
        Gdx.input.setInputProcessor(firstPersonCameraController);
    }
    
    private void create_shader() {
        shaderProgram_color = setupShader("color", "shaders/color.vertex.glsl", "shaders/color.fragment.glsl");
        
        shaderProgram_texture = setupShader("texture", "shaders/texture_WithDirectionalLight.vertex.glsl", "shaders/texture_WithDirectionalLight.fragment.glsl");
        //shaderProgram_texture = setupShader("texture", "shaders/texture_with_some_self_shadow.vertex.glsl", "shaders/texture_with_some_self_shadow.fragment.glsl");
        //shaderProgram_texture = setupShader("texture", "shaders/texture.vertex.glsl", "shaders/texture.fragment.glsl");
        
        batch3D = new ModelBatch(new SceneShaderProvider() {
            @Override
            protected Shader createShader(final Renderable renderable) {
                if(renderable.material.has(TextureAttribute.Diffuse)) {
                    //return new TextureShader(renderable, shaderProgram_texture);
                    //return new TextureShaderWithSomeSelfShadows(renderable, shaderProgram_texture);
                    return new TextureShaderWithDirectionalLight(renderable, shaderProgram_texture);
                }else {
                    return new BasicColorShader(renderable, shaderProgram_color);
                }
            }
        });
    }
    
    public void createMainPerspectiveCamera() {
        camera3D = new PerspectiveCamera(75, Gdx.graphics.getWidth(), Gdx.graphics.getHeight());
    }
    
    private void createMainOrthographicCamera() {
        if(camera3D != null) {
            app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");
    
            throw new Error("Error. 1.");
        }
        
        camera3D = new OrthographicCamera(Gdx.graphics.getWidth(), Gdx.graphics.getHeight());
        //((OrthographicCamera)camera3D).zoom = 1f / 2f;
    
        camera3D.near = 0.01f;
        camera3D.far = 300.0f;
    
        camera3D.up.set(Vector3.Z);
    }
    
    /**
     * Initialize a shader, vertex shader must be named prefix_v.glsl, fragment shader must be named
     * prefix_f.glsl
     *
     * @param prefix
     * @return
     */
    public ShaderProgram setupShader(final String prefix, final String vertexFileLocation, final String fragmentFileLocation) {
        ShaderProgram.pedantic = false;
        
        final ShaderProgram shaderProgram = new ShaderProgram(
                Gdx.files.internal(vertexFileLocation),
                Gdx.files.internal(fragmentFileLocation)
        );
        if (!shaderProgram.isCompiled()) {
            app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1. Shader " + prefix + " compilled " + shaderProgram.getLog());
    
            throw new Error("Error. 1.");
        }else {
            Gdx.app.log("init", "Shader " + prefix + " compilled " + shaderProgram.getLog());
        }
        return shaderProgram;
    }
    
    @Override
    public void render_pre() {
        currentLight.render(stage3D);
    }
    
    @Override
    public void handle3DRendering(float deltaS) {
        if(isDirty_camera) {
            isDirty_camera = false;
        
            camera3D.update();
        }
    
        currentLight.applyToShader(shaderProgram_color);
        currentLight.applyToShader(shaderProgram_texture);
        
        stage3D.batchDraw(batch3D, camera3D);
        
        if (takeScreenshots) {
            ScreenshotFactory.saveScreenshot(Gdx.graphics.getWidth(), Gdx.graphics.getHeight(), "scene");
        }
    }
    
    public void dispose() {
        if(stage3D != null) {
            // This is done in the top class so the game world can be used as a menu.
            //app.removeRenderable3D(this);
        
            stage3D = null;
        }
    }
    
    public void setEnableRendering(boolean isRenderingEnabled) {
        if(this.isRenderingEnabled == isRenderingEnabled) {
            return ;
        }
        
        this.isRenderingEnabled = isRenderingEnabled;
        
        if(!this.isRenderingEnabled) {
            app.removeRenderable3D(this);
        }else {
            app.addRenderable3D(this);
        }
    }
    
    public void appLoop(float deltaS, int deltaMS) {
        firstPersonCameraController.update(deltaS);
    
        if (System.currentTimeMillis() - lastScreenShot > 1000 * 1 && Gdx.input.isKeyJustPressed(Input.Keys.F2)) {
            takeScreenshots = true;
            lastScreenShot = System.currentTimeMillis();
        }
        else {
            takeScreenshots = false;
        }
    }
    
}
