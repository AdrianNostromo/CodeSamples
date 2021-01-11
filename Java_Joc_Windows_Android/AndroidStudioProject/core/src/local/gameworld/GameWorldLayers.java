package local.gameworld;

import base.visual.IContainer2D;
import base.visual.VisualUtil;
import local.app.AppLocal;

public class GameWorldLayers extends GameWorldStage3D {

    protected IContainer2D layerPannableWorld = null;
    /**/protected IContainer2D layerVFX = null;
    protected IContainer2D layerTutorial = null;
    protected IContainer2D layerUserInterface = null;

    public GameWorldLayers(AppLocal app) {
        super(app);

        //void
    }

    protected void scaleGameWorldToWidth(float gameWorldWidth) {
        float n = app.screenSize.width / gameWorldWidth;
        layerPannableWorld.setScale(n, n);
    }

    @Override
    protected void createLayers() {
        super.createLayers();

        visualsMap.put("layerTutorial", layerPannableWorld = VisualUtil.newGroup(gameWorldContainer2D));
        // NOTE - The game world is scaled to a fixed rendering3DSize, not the screen rendering3DSize.
        /**/visualsMap.put("layerVFX", layerVFX = VisualUtil.newGroup(layerPannableWorld));

        visualsMap.put("layerUserInterface", layerUserInterface = VisualUtil.newGroup(gameWorldContainer2D));
        visualsMap.put("layerTutorial", layerTutorial = VisualUtil.newGroup(gameWorldContainer2D));
    }
}
