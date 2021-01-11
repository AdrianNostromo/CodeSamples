package base.util;

/**
 * Created by Adrin on 5/14/2016.
 */
public class PhysicsParticlesFXBurstConfig {

    public float spawnRadius;
    public float particlesImpulse;
    public PhysicsParticleFXEntityConfig[] particlesList;

    public PhysicsParticlesFXBurstConfig(float spawnRadius, float particlesImpulse, PhysicsParticleFXEntityConfig[] particlesList) {
        super();

        this.spawnRadius = spawnRadius;
        this.particlesImpulse = particlesImpulse;
        this.particlesList = particlesList;
    }

}
