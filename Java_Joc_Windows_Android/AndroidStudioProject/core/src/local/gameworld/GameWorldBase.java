package local.gameworld;

import com.badlogic.gdx.graphics.Color;
import com.badlogic.gdx.graphics.g3d.attributes.PointLightsAttribute;
import com.badlogic.gdx.graphics.g3d.environment.PointLight;

import java.util.HashMap;
import java.util.Map;

import local.audio.util.SoundConfig;
import base.audio.util.SoundItem;
import base.visual.IContainer2D;
import base.visual.IDrawableNode;
import base.visual.VisualUtil;
import local.app.AppLocal;

// The base class for the game world.
public class GameWorldBase {

    protected AppLocal app;

    public PointLight sunLight3D;
    public PointLightsAttribute sunLight3DAttribute;

    // false : game is paused;
    // true : game play is enabled;
    private boolean isPlayEnabled = false;

    private boolean isLevelStarted = false;

    private boolean isLevelEnded = false;
    protected int levelEndType = 0;

    private SoundItem sound_level_start = null;
    private SoundItem sound_level_end = null;

    protected Map<String, IDrawableNode> visualsMap;
    protected IContainer2D gameWorldContainer2D = null;

    public GameWorldBase(AppLocal app) {
        super();

        this.app = app;
    }

    public void create() {
        visualsMap = new HashMap<String, IDrawableNode>();

        sunLight3D = new PointLight();
        sunLight3D.set(Color.WHITE, 0f, 0f, 0f, 3f);
        sunLight3DAttribute = new PointLightsAttribute();
        sunLight3DAttribute.lights.add(sunLight3D);

        createLayers();

        if(SoundConfig.level_start != null) {
            sound_level_start = app.soundManager.hookNewSoundItem(SoundConfig.level_start);
        }

        if(SoundConfig.level_end != null) {
            sound_level_end = app.soundManager.hookNewSoundItem(SoundConfig.level_end);
        }
    }

    protected void createLayers() {
        visualsMap.put("gameWorldContainer2D", gameWorldContainer2D = VisualUtil.newGroup(null));
    }

    public IContainer2D getGameWorldContainer2D() {
        return gameWorldContainer2D;
    }

    public void startLevelPlay() {
        isLevelStarted = true;

        if(sound_level_start != null) {
            sound_level_start.play();
        }
    }

    public boolean getIsLevelEnded() {
        return isLevelEnded;
    }

    public int getLevelEndType() {
        return levelEndType;
    }

    protected void levelEnded(int levelEndType) {
        //setIsPlayEnabled(false);

        isLevelEnded = true;

        this.levelEndType = levelEndType;

        if(sound_level_end != null) {
            sound_level_end.play();
        }
    }

	public void appLoop(float deltaS, int deltaMS) {
        // The app loop is converted to a game loop. Game pause mechanisms is implemented here.
		if(isPlayEnabled) {
            gameLoop(deltaS, deltaMS);
        }
	}

    public boolean getIsPlayEnabled() {
        return isPlayEnabled;
    }

    public void setIsPlayEnabled(boolean isPlayEnabled) {
        if(this.isPlayEnabled == isPlayEnabled) {
            return ;
        }

        this.isPlayEnabled = isPlayEnabled;
        isPlayEnabled_changedEvent();
    }

    protected void isPlayEnabled_changedEvent() {
        //void
    }

    public boolean getIsLevelStarted() {
        return isLevelStarted;
    }

    protected void gameLoop(float deltaS, int deltaMS) {
		//void
	}

    public void dispose() {
        for (Map.Entry<String, IDrawableNode> entry : visualsMap.entrySet()) {
            if(entry != null && entry.getValue() != null) {
                if(entry.getValue().getParent() != null) {
                    entry.getValue().getParent().removeChild(entry.getValue());
                }
            }
        }

        if(gameWorldContainer2D != null) {
            if(gameWorldContainer2D.getParent() != null) {
                gameWorldContainer2D.getParent().removeChild(gameWorldContainer2D);
            }

            gameWorldContainer2D = null;
        }
    }

}
