package base.fx.util;

public class VisualFXParticleConfig {

    public String particleID;

    public base.visual.util.VisualCreateConfig visualCreateConfig;

    public base.visual.util.VisualPropertiesConfig visualPropertiesConfig;

    public PropertyAnimationConfig[] animatedProperties;

    public VisualFXParticleConfig(String particleID, base.visual.util.VisualCreateConfig visualCreateConfig, base.visual.util.VisualPropertiesConfig visualPropertiesConfig, PropertyAnimationConfig[] animatedProperties) {
        super();

        this.particleID = particleID;

        this.visualCreateConfig = visualCreateConfig;

        this.visualPropertiesConfig = visualPropertiesConfig;

        this.animatedProperties = animatedProperties;
    }

}
