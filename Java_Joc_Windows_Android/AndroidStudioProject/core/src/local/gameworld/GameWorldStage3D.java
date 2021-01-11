package local.gameworld;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Camera;
import com.badlogic.gdx.graphics.PerspectiveCamera;
import com.badlogic.gdx.graphics.g3d.Environment;
import com.badlogic.gdx.graphics.g3d.ModelBatch;
import com.badlogic.gdx.math.Vector3;

import base.app.util.IRenderable3D;
import base.visual3d.Stage3D;
import local.app.AppLocal;

public class GameWorldStage3D extends GameWorldBase implements IRenderable3D {

    private ModelBatch batch3D;
    protected Stage3D stage3D;
    private Camera camera3D;
    // The camera looks at this position.
    private Vector3 camera3DLookAtTarget;
    public Environment environment3D;

    private boolean isDirty_camera = true;

    public GameWorldStage3D(AppLocal app) {
        super(app);

        //void
    }

    @Override
    public void create() {
        super.create();

        batch3D = new ModelBatch();

        stage3D = new Stage3D();
        stage3D.create();

        // Perspective camera.
        camera3D = new PerspectiveCamera(75, Gdx.graphics.getWidth(), Gdx.graphics.getHeight());

        // Ortographic camera.
        // float camScale = Gdx.graphics.getWidth() / 9f;
        // camera3D = new OrthographicCamera(Gdx.graphics.getWidth() / camScale, Gdx.graphics.getHeight() / camScale);
        // Not used. ((OrthographicCamera)camera3D).zoom = 1f / 5f;

        camera3D.near = 0.01f;
        camera3D.far = 300.0f;

        environment3D = new Environment();

        app.addRenderable3D(this);
    }

    public Vector3 getCamera3DPosition() {
        return camera3D.position;
    }

    public void setCamera3DPosition(float x, float y, float z) {
        camera3D.position.set(x, y, z);
        isDirty_camera = true;
    }

    public Vector3 getCamera3DLookAtTarget() {
        return camera3DLookAtTarget;
    }

    protected final void setCamera3DLookAtTarget(float x, float y, float z) {
        if(camera3DLookAtTarget == null) {
            camera3DLookAtTarget = new Vector3(x, y, z);
        }else {
            camera3DLookAtTarget.set(x, y, z);
        }

        camera3D.lookAt(camera3DLookAtTarget.x, camera3DLookAtTarget.y, camera3DLookAtTarget.z);
        isDirty_camera = true;
    }

    @Override
    public void render_pre() {
        //void
    }

    @Override
    public void handle3DRendering(float deltaS) {
        if(isDirty_camera) {
            isDirty_camera = false;

            camera3D.update();
        }

        stage3D.batchDraw(batch3D, camera3D, environment3D);
    }

    @Override
    public void dispose() {
        if(stage3D != null) {
            app.removeRenderable3D(this);

            stage3D = null;
        }

        super.dispose();
    }

}
