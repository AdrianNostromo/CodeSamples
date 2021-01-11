package base.fx.util;

import base.visual.IDrawableNode;

public class VisualFXParticle {

    public VisualFXParticleConfig config;

    public VisualFXParticleConfig animationConfig;

    public IDrawableNode visual;

    public ActivePropertyAnimation[] activePropertyAnimationsList;

    public int groupID;

    public float lifeS = 0;

    public VisualFXParticle(VisualFXParticleConfig config, VisualFXParticleConfig animationConfig, IDrawableNode visual, ActivePropertyAnimation[] activePropertyAnimationsList, int groupID) {
        super();

        this.config = config;

        this.animationConfig = animationConfig;

        this.visual = visual;

        this.activePropertyAnimationsList = activePropertyAnimationsList;

        this.groupID = groupID;
    }

}
