package base.util;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Adrin on 5/14/2016.
 */
public class PhysicsParticleFXEntityConfig {

    public int particlesCount;

    public String particleEntityID;
    public Map<String, Object> particleExtraDataMap = new HashMap<String, Object>();

    public PhysicsParticleFXEntityConfig(int particlesCount, String particleEntityID, int particleColorRGBA) {
        super();

        this.particlesCount = particlesCount;

        this.particleEntityID = particleEntityID;

        particleExtraDataMap.put("particleColorRGBA", particleColorRGBA);
    }

}
