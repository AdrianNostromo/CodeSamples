package local.fx.config;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import base.ease.EaseFunctions;
import base.fx.util.PropertyAnimationConfig;
import base.fx.util.VisualFXParticleConfig;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;
import base.visual.util.VisualValueConfig;

public class FXConfig {

    public static final Map<String, FXConfig> entriesMap = new HashMap<String, FXConfig>();
    public static final Vector<FXConfig> entriesList = new Vector<FXConfig>();

    public static final FXConfig itemCollected = new FXConfig("itemCollected", new VisualFXParticleConfig[]{
            new VisualFXParticleConfig("imgItemCollectedVFXParticle", VisualCreateConfig.newSprite("visual/taGame.atlas", "gameWorld/fx/imgItemCollectedVFXParticle"), new VisualPropertiesConfig()
                    .put("width", 24f)
                    .put("height", 24f)
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f),
                    new PropertyAnimationConfig[]{
                            new PropertyAnimationConfig(
                                    "y",
                                    null,
                                    new VisualValueConfig(25f),
                                    0.0f,
                                    0.3f, EaseFunctions.easeOutCubic, null
                            ),
                            new PropertyAnimationConfig(
                                    "alpha",
                                    new VisualValueConfig(1f),
                                    new VisualValueConfig(0f),
                                    0.0f,
                                    0.3f, EaseFunctions.linear, null
                            ),
                            new PropertyAnimationConfig(
                                    "scaleX",
                                    new VisualValueConfig(1f),
                                    new VisualValueConfig(1.4f),
                                    0.0f,
                                    0.3f, EaseFunctions.easeOutCubic, null
                            ),
                            new PropertyAnimationConfig(
                                    "scaleY",
                                    new VisualValueConfig(1f),
                                    new VisualValueConfig(1.4f),
                                    0.0f,
                                    0.3f, EaseFunctions.easeOutCubic, null
                            )
                    }
            )
    });

    public String fxId;

    public VisualFXParticleConfig[] particlesList;

    public FXConfig(String fxId, VisualFXParticleConfig[] particlesList) {
        super();

        this.fxId = fxId;

        this.particlesList = particlesList;

        entriesMap.put(fxId, this);
        entriesList.add(this);
    }

}