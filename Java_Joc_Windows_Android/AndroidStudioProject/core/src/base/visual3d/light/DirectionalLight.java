package base.visual3d.light;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.PerspectiveCamera;
import com.badlogic.gdx.graphics.Pixmap.Format;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.glutils.FrameBuffer;
import com.badlogic.gdx.graphics.glutils.ShaderProgram;
import com.badlogic.gdx.math.Vector3;

import base.visual3d.Stage3D;
import base.visual3d.World3D;
import base.visual3d.util.ScreenshotFactory;

public class DirectionalLight extends Light {

	public Vector3		direction;
	public FrameBuffer	frameBuffer;
	public Texture		depthMap;

	public DirectionalLight(final World3D mainScreen, final Vector3 position, final Vector3 direction) {
		super(mainScreen);
        
		this.position = position;
		this.direction = direction;
        
		init();
	}

	@Override
	public void applyToShader(final ShaderProgram sceneShaderProgram) {
		sceneShaderProgram.begin();
		final int textureNum = 2;
		depthMap.bind(textureNum);
		sceneShaderProgram.setUniformi("u_depthMap", textureNum);
		sceneShaderProgram.setUniformMatrix("u_lightTrans", camera.combined);
		sceneShaderProgram.setUniformf("u_cameraFar", camera.far);
		sceneShaderProgram.setUniformf("u_lightPosition", camera.position);
		sceneShaderProgram.end();
	}

	@Override
	public void init() {
		super.init();

		camera = new PerspectiveCamera(120f, World3D.DEPTHMAPSIZE, World3D.DEPTHMAPSIZE);
		camera.near = 0.1f;
		camera.far = 16f;
		camera.position.set(position);
		camera.direction.set(direction);
		camera.update();
	}

	@Override
	public void render(final Stage3D stage3D) {
		if (frameBuffer == null) {
			frameBuffer = new FrameBuffer(Format.RGBA8888, World3D.DEPTHMAPSIZE, World3D.DEPTHMAPSIZE, true);
		}
		frameBuffer.begin();
		Gdx.gl.glClearColor(0, 0, 0, 1);
		Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT | GL20.GL_DEPTH_BUFFER_BIT);

		shaderProgram.begin();
		shaderProgram.setUniformf("u_cameraFar", camera.far);
		shaderProgram.setUniformf("u_lightPosition", camera.position);
		shaderProgram.end();

        stage3D.batchDraw(modelBatch, camera);

		if (mainScreen.takeScreenshots) {
			ScreenshotFactory.saveScreenshot(frameBuffer.getWidth(), frameBuffer.getHeight(), "depthmap");
		}
		frameBuffer.end();
		depthMap = frameBuffer.getColorBufferTexture();
	}

}
