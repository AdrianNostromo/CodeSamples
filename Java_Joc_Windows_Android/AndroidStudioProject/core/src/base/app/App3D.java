package base.app;

import java.util.Vector;

import base.app.util.IRenderable3D;
import base.launcher.IAppLauncher;
import base.launcher.ILocalDataHandler;
import base.launcher.IWebManager;

/**
 * Created by adrin on 11/29/16.
 */

public abstract class App3D extends AppBase {

    private Vector<IRenderable3D> renderable3DList = new Vector<IRenderable3D>();

    public App3D(IAppLauncher appLauncher, ILocalDataHandler localDataHandler, IWebManager webManager) {
        super(appLauncher, localDataHandler, webManager);

        //void
    }

    public void render_pre() {
        for(int i=0;i<renderable3DList.size();i++) {
            IRenderable3D entry = renderable3DList.get(i);
            if(entry != null) {
                entry.render_pre();
            }
        }
    }

    public void handle3DRendering() {
        for(int i=0;i<renderable3DList.size();i++) {
            IRenderable3D entry = renderable3DList.get(i);
            if(entry != null) {
                entry.handle3DRendering();
            }
        }
    }

    public void addRenderable3D(IRenderable3D renderable3D) {
        if(renderable3DList.indexOf(renderable3D) >= 0) {
            throw new Error("Error. 1.");
        }

        renderable3DList.add(renderable3D);
    }

    public void removeRenderable3D(IRenderable3D renderable3D) {
        int i = renderable3DList.indexOf(renderable3D);
        if(i < 0) {
            throw new Error("Error. 1.");
        }

        renderable3DList.remove(i);
    }

}
