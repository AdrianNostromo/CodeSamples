package local.gameworld;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.g3d.ModelInstance;
import com.badlogic.gdx.graphics.g3d.attributes.BlendingAttribute;
import com.badlogic.gdx.graphics.g3d.attributes.ColorAttribute;
import com.badlogic.gdx.graphics.g3d.attributes.TextureAttribute;
import com.badlogic.gdx.math.Vector3;

import java.util.Stack;

import base.profile.util.PlayerProfileDataEventListener;
import local.app.AppLocal;
import local.gameworld.util.BallStyleConfig;
import local.gameworld.util.BallConfig;
import local.gameworld.util.WorldUnit;
import local.profile.PlayerProfileDataLocal;

public class GameWorldUnits extends GameWorldItems {

    protected Stack<WorldUnit> worldUnits = new Stack<WorldUnit>();

    private PlayerProfileDataEventListener profileDataDataEventListener = new PlayerProfileDataEventListener() {
        @Override
        public void profileDataChanged(String dataID) {
            super.profileDataChanged(dataID);

            if(dataID == PlayerProfileDataLocal.ACTIVE_BALL_STYLE_ID){
                updateAllUnitStyles();
            }
        }
    };

    public GameWorldUnits(AppLocal app) {
        super(app);

        //void
    }

    @Override
    public void create() {
        super.create();

        app.profileData.addEventsListener(profileDataDataEventListener);
    }

    protected WorldUnit createNewUnit(BallConfig ballConfig, int tubeRowIndex, Vector3 pos) {
        String unitStyleID = app.profileData.getActiveBallStyleId();
        BallStyleConfig ballStyle = BallStyleConfig.entriesMap.get(unitStyleID);

        ModelInstance modelInstance = app.embeddedAssetsManager.getModelInstance(ballConfig.modelID);

        ModelInstance outlineModelInstance = app.embeddedAssetsManager.getModelInstance(ballConfig.outlineModelID);
        outlineModelInstance.materials.get(0).set(ColorAttribute.createDiffuse(ballStyle.outlineDiffuseColor));
        outlineModelInstance.materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));

        ModelInstance shadowModelInstance = getNewShadowModelInstance(ballConfig.shadowModelID, ballConfig.shadowTextureAtlasID, ballConfig.shadowTextureRegionID);
        if(shadowModelInstance == null) {
            Gdx.app.debug("Error", "Error creating the unit shadow model instance.");

            return null;
        }

        WorldUnit unit = new WorldUnit(ballConfig, modelInstance, outlineModelInstance, shadowModelInstance, tubeRowIndex, pos);

        updateUnitStyle(unit);

        unit.syncVisuals();

        worldUnits.add(unit);

        stage3D.addChild(unit.modelInstance);
        stage3D.addChild(unit.outlineModelInstance);
        stage3D.addChild(unit.shadowModelInstance);

        return unit;
    }

    private void updateAllUnitStyles() {
        for(int i=0;i< worldUnits.size();i++) {
            WorldUnit worldUnit = worldUnits.get(i);
            if (worldUnit != null) {
                updateUnitStyle(worldUnit);
            }
        }
    }

    private void updateUnitStyle(WorldUnit worldUnit) {
        BallStyleConfig activeBallStyle = BallStyleConfig.entriesMap.get(app.profileData.getActiveBallStyleId());

        worldUnit.modelInstance.materials.get(0).set(TextureAttribute.createDiffuse(app.embeddedAssetsManager.getTextureAtlasRegion(activeBallStyle.textureAtlasID, activeBallStyle.textureRegionID)));
        //worldUnit.modelInstance.materials.get(0).set(ColorAttribute.createDiffuse(Color.RED));
        worldUnit.modelInstance.materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 1.0f, 1.0f, 1.0f, 1.0f));
        worldUnit.modelInstance.materials.get(0).set(sunLight3DAttribute);

        worldUnit.outlineModelInstance.materials.get(0).set(ColorAttribute.createDiffuse(activeBallStyle.outlineDiffuseColor));
        worldUnit.outlineModelInstance.materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));
    }

    private ModelInstance getNewShadowModelInstance(String modelID, String textureAtlasID, String textureRegionID) {
        ModelInstance modelInstance = app.embeddedAssetsManager.getModelInstance(modelID);

        modelInstance.materials.get(0).set(TextureAttribute.createDiffuse(app.embeddedAssetsManager.getTextureAtlasRegion("visual/taTextures.atlas", "modelTextures/balls/ballShadow")));
        modelInstance.materials.get(0).set(new BlendingAttribute(GL20.GL_SRC_ALPHA, GL20.GL_ONE_MINUS_SRC_ALPHA));

        return modelInstance;
    }

    private void recycleUnit(WorldUnit worldUnit) {
        if(worldUnit.modelInstance != null) {
            stage3D.removeChild(worldUnit.modelInstance);

            app.embeddedAssetsManager.releaseModelInstance(worldUnit.modelInstance);

            worldUnit.modelInstance = null;
        }
        if(worldUnit.outlineModelInstance != null) {
            stage3D.removeChild(worldUnit.outlineModelInstance);

            app.embeddedAssetsManager.releaseModelInstance(worldUnit.outlineModelInstance);

            worldUnit.outlineModelInstance = null;
        }
        if(worldUnit.shadowModelInstance != null) {
            stage3D.removeChild(worldUnit.shadowModelInstance);

            app.embeddedAssetsManager.releaseModelInstance(worldUnit.shadowModelInstance);

            worldUnit.shadowModelInstance = null;
        }
    }

    @Override
    public void dispose() {
        app.profileData.removeEventsListener(profileDataDataEventListener);

        while(worldUnits.size() > 0) {
            WorldUnit worldUnit = worldUnits.pop();
            if(worldUnit != null) {
                recycleUnit(worldUnit);
            }
        }

        super.dispose();
    }

}
