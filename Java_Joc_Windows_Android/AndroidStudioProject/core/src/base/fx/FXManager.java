package base.fx;

import com.badlogic.gdx.Gdx;

import java.util.Vector;

import local.fx.config.FXConfig;
import base.fx.util.VisualFXParticle;
import base.fx.util.VisualFXParticleConfig;
import local.app.AppLocal;
import base.visual.VisualUtil;
import base.visual.IContainer2D;
import base.visual.IDrawableNode;
import base.visual.util.VisualPropertyValueConfig;
import base.visual.util.VisualValueConfig;

public class FXManager {

    protected AppLocal app;

    private Vector<VisualFXParticle> particlesList = new Vector<VisualFXParticle>();

    public static final int GROUP_GLOBAL = 1;
    public static final int GROUP_IN_GAME = 2;

    public FXManager(AppLocal app) {
        super();

        this.app = app;
    }

    public void create() {
        //void
    }

    // App loop can be used for user interface fx.
    public void appLoop(float deltaS, int deltaMS) {
        for(int i=particlesList.size()-1;i>=0;i--) {
            VisualFXParticle particle = particlesList.get(i);
            if(particle != null && particle.groupID == FXManager.GROUP_GLOBAL) {
                if(!gameLoop_particle(particle, deltaS, deltaMS)) {
                    // Particle animation finished, remove it.

                    recycleParticle(particle);

                    particlesList.remove(i);
                }
            }
        }
    }

    // Game loop is used so fx animations are paused when the game world is paused.
    public void gameLoop(float deltaS, int deltaMS) {
        for(int i=particlesList.size()-1;i>=0;i--) {
            VisualFXParticle particle = particlesList.get(i);
            if(particle != null && particle.groupID == FXManager.GROUP_IN_GAME) {
                if(!gameLoop_particle(particle, deltaS, deltaMS)) {
                    // Particle animation finished, remove it.

                    recycleParticle(particle);

                    particlesList.remove(i);
                }
            }
        }
    }

    private void recycleParticle(VisualFXParticle particle) {
        if(particle.visual != null) {
            if(particle.visual.getParent() != null) {
                particle.visual.getParent().removeChild(particle.visual);
            }

            particle.visual = null;
        }
    }

    private boolean gameLoop_particle(VisualFXParticle particle, float deltaS, int deltaMS) {
        // Enumerate each property and check if animation is required.

        particle.lifeS += deltaS;

        boolean isAnimationActive = false;
        for(int i=0;i<particle.activePropertyAnimationsList.length;i++) {
            base.fx.util.ActivePropertyAnimation activePropertyAnimation = particle.activePropertyAnimationsList[i];

            if(activePropertyAnimation.lastCompletionPercent < 1) {
                boolean isFirstUpdate = false;
                if(activePropertyAnimation.lastCompletionPercent <= 0) {
                    isFirstUpdate = true;
                }

                if(particle.lifeS >= activePropertyAnimation.config.animationStartDelayS) {
                    activePropertyAnimation.lastCompletionPercent = Math.min(1, activePropertyAnimation.lastCompletionPercent + deltaS / activePropertyAnimation.config.animationDurationS);

                    float endValue = getSmartValue(particle.visual, activePropertyAnimation.config.endComposedValue);

                    float newValue;

                    if(activePropertyAnimation.config.startComposedValue != null) {
                        // Use direct animation mode.

                        float startValue = getSmartValue(particle.visual, activePropertyAnimation.config.startComposedValue);

                        newValue = base.ease.EaseFunctions.computeEaseValue(activePropertyAnimation.config.easeFunctionID, activePropertyAnimation.config.easeFunctionConfig, activePropertyAnimation.lastCompletionPercent, startValue, endValue);
                    }else {
                        // Use offset mode.

                        float offsetValue = base.ease.EaseFunctions.computeEaseValue(activePropertyAnimation.config.easeFunctionID, activePropertyAnimation.config.easeFunctionConfig, activePropertyAnimation.lastCompletionPercent, 0, endValue);
                        float deltaValue = offsetValue - activePropertyAnimation.lastSyncValue;
                        activePropertyAnimation.lastSyncValue = offsetValue;

                        newValue = (Float)particle.visual.getPropertyValue(activePropertyAnimation.config.propertyID) + deltaValue;
                    }

                    particle.visual.setPropertyValue(activePropertyAnimation.config.propertyID, newValue);
                }

                if(activePropertyAnimation.lastCompletionPercent < 1) {
                    isAnimationActive = true;
                }
            }
        }

        if(!isAnimationActive) {
            // Particle animations finished.
            return false;
        }

        return true;
    }

    public void playFX(String fxID, IContainer2D parentLayer, float posX, float posY, int groupID) {
        if(parentLayer == null) {
            Gdx.app.debug("Error", "Fx template not found. Fx fxId: " + fxID);

            return ;
        }

        FXConfig fxConfig = FXConfig.entriesMap.get(fxID);
        if(fxConfig == null) {
            Gdx.app.debug("Error", "Fx template not found. Fx fxId: " + fxID);

            return ;
        }

        for(int i=0;i<fxConfig.particlesList.length;i++) {
            VisualFXParticleConfig particleConfig = fxConfig.particlesList[i];

            spawnVisualParticle(particleConfig, parentLayer, posX, posY, groupID);
        }
    }

    public void spawnVisualParticle(VisualFXParticleConfig particleConfig, IContainer2D parentLayer, float posX, float posY, int groupID) {
        IDrawableNode visual = VisualUtil.newSmartVisual(app, particleConfig.visualCreateConfig, parentLayer, null);
        if(visual == null) {
            Gdx.app.debug("Error", "Fx particle visual create error with particle fxId: " + particleConfig.particleID);

            return ;
        }
        visual.setPosition(posX, posY);

        VisualFXParticle particle = new VisualFXParticle(particleConfig, particleConfig, visual, generateActivePropertyAnimationsList(particleConfig.animatedProperties), groupID);

        for(int i=0;i<particleConfig.visualPropertiesConfig.propertyConfigs.size();i++) {
            VisualPropertyValueConfig propertyValuePair = particleConfig.visualPropertiesConfig.propertyConfigs.get(i);

            float finalValue = getSmartValue(visual, propertyValuePair.smartValue);

            particle.visual.setPropertyValue(propertyValuePair.property, finalValue);
        }

        particlesList.add(particle);
    }

    private float getSmartValue(IDrawableNode visual, VisualValueConfig valueConfig) {
        float multiplier = 1f;
        if(valueConfig.baseMultiplierSourceID == null) {
            //void
        }else if("_appSize".equals(valueConfig.baseMultiplierSourceID)) {
            if ("width".equals(valueConfig.baseMultiplierProperty)) {
                multiplier = app.screenSize.width;
            } else if ("height".equals(valueConfig.baseMultiplierProperty)) {
                multiplier = app.screenSize.height;
            } else {
                Gdx.app.debug("Error", "Unknown app rendering3DSize property with property fxId: " + valueConfig.baseMultiplierProperty);

                return 0;
            }
        }else if("_self".equals(valueConfig.baseMultiplierSourceID)) {
            multiplier = (Float)visual.getPropertyValue(valueConfig.baseMultiplierProperty);
        }else {
            Gdx.app.debug("Error", "Unknown smart source: " + valueConfig.baseMultiplierSourceID);

            return 0;
        }

        float finalValue = (Float)valueConfig.baseValue * multiplier;

        return finalValue;
    }

    private static base.fx.util.ActivePropertyAnimation[] generateActivePropertyAnimationsList(base.fx.util.PropertyAnimationConfig[] animatedProperties) {
        base.fx.util.ActivePropertyAnimation[] activePropertyAnimations = new base.fx.util.ActivePropertyAnimation[animatedProperties.length];

        for(int i=0;i<animatedProperties.length;i++) {
            activePropertyAnimations[i] = new base.fx.util.ActivePropertyAnimation(animatedProperties[i]);
        }

        return activePropertyAnimations;
    }

    public void removeVFXWithGroupID(int groupID) {
        for(int i=particlesList.size()-1;i>=0;i--) {
            VisualFXParticle particle = particlesList.get(i);
            if(particle != null && particle.groupID == groupID) {
                recycleParticle(particle);

                particlesList.remove(i);
            }
        }
    }

}
