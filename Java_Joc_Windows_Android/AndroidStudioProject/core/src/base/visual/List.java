package base.visual;

import com.badlogic.gdx.Gdx;

import java.util.Map;
import java.util.Vector;

import base.audio.util.DynamicSoundItem;
import local.audio.util.SoundConfig;
import base.interaction.TouchListener;
import base.menu.AnimatedComposedStiledVisual;
import base.menu.util.EaseConfig;
import base.menu.util.IMenuAnimationListener;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_Visual;
import base.menu.util.MenuVisualAnimationProperty;
import base.menu.util.StateChangeDurations;
import local.app.AppLocal;
import base.util.AppStyles;
import base.visual.util.ListEventListener;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;

/**
 * Created by Adrin on 6/4/2016.
 */
public class List extends AnimatedComposedStiledVisual implements IDynamicPropertiesObject {

    public static final int UNDEFINED_LIST = 0;
    public static final int HORIZONTAL_LIST = 1;
    public static final int VERTICAL_LIST = 2;

    protected DynamicSoundItem entryActivateDSound;

    protected ListEventListener eventListener = null;

    protected Vector<ListEntry> entriesList;
    protected int entriesCount = 0;

    // Value is computed.
    protected float listWidth = 0f;
    protected float listHeight = 0f;

    protected int listOrientation = UNDEFINED_LIST;
    protected float entryWidth = 0f;
    protected float entryHeight = 0f;
    protected float entriesSpacingX = 0f;
    protected float entriesSpacingY = 0f;
    protected float listPaddingTop = 0f;
    protected float listPaddingBottom = 0f;
    protected float listPaddingLeft = 0f;
    protected float listPaddingRight = 0f;
    protected float listWidthOverride = 0f;
    protected float listHeightOverride = 0f;
    protected int rowsCount = 1;
    protected int columnsCount = 1;
    protected boolean scaleSizeToContent = true;

    protected String[][] scalableSizeElements = new String[][] {
            new String[] {"_local", "bg"},
            new String[] {"_local", "touchAreaBack"}
    };

    protected IContainer2D layerEntries = null;
    protected IInteractiveDrawable touchAreaBack = null;

    private TouchListener touchListener = new TouchListener() {
        @Override
        public boolean touchDown(IInteractiveDrawable target, float worldX, float worldY) {
            // Touch events are ignored while the menu is animating.
            if(target == touchAreaBack) {
                //void
            }else if(target.getUserDataMap().get("listEntry") != null) {
                if(eventListener != null) {
                    ListEntry entry = (ListEntry)target.getUserDataMap().get("listEntry");
                    eventListener.entryActivated(entry.list, entry);
                }

                entryActivateDSound.play();
            }else {
                Gdx.app.debug("Error", "Touch event received for a unknown target.");
            }

            return false;
        }
    };

    private static final MenuItemConfig[] itemConfigs = new MenuItemConfig[] {
            new MenuItemConfig_Visual("layerBase", new String[] {"_root", "visualsHolder"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    .put("x", 0f)
                    .put("y", 0f)
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_in_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_out_"}, 0f, null, null, EaseConfig.DEFAULT)
                    )
            ),

            new MenuItemConfig_Visual("bg", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), new VisualPropertiesConfig()
                    // The bg rendering3DSize is scaled to content in this example.
                    //.put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
                    //.put("height", 154f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 1.0f)
                    .put("x", 0f)
                    .put("y", 0f)
                    //.put("tintColor", Color.valueOf("#FF0000FF"))
            ),
            new MenuItemConfig_Visual("touchAreaBack", new String[] {"_local", "layerBase"}, VisualCreateConfig.newTouchAreaAudible(null, null), new VisualPropertiesConfig()
                    // The bg rendering3DSize is scaled to content in this example.
                    //.put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
                    //.put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 1.0f)
                    .put("x", 0f)
                    .put("y", 0f)
            ),
            new MenuItemConfig_Visual("layerEntries", new String[] {"_local", "layerBase"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    //void
            )
    };

    private static final StateChangeDurations[] animationDurations = new StateChangeDurations[] {
            new StateChangeDurations("_in_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_out_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_in_", "_out_", AppStyles.ANIMATION_DURATION_S_DEFAULT)
    };

    public List(AppLocal app) {
        super(app, itemConfigs, animationDurations);

        //void
    }

    public ListEventListener getEventListener() {
        return eventListener;
    }

    public void setEventListener(ListEventListener eventListener) {
        this.eventListener = eventListener;
    }

    @Override
    public void create(Map<String, Object> createConfigMap) {
        super.create(createConfigMap);

        entryActivateDSound = new DynamicSoundItem(app);

        // Because these value are used by templates they have to be initialized.
        if(getStaticAnimatingPropertyStatus("listWidth") == null) {
            putStaticAnimatingPropertyStatus("listWidth", true);
        }
        if(getStaticAnimatingPropertyStatus("listHeight") == null) {
            putStaticAnimatingPropertyStatus("listHeight", true);
        }

        entriesList = new Vector<ListEntry>();
    }

    protected void initializeListTouchAreaBackTouchListener() {
        touchAreaBack.setTouchListener(touchListener);
    }

    protected float computeContentWidth() {
        float contentWidth;

        if(listOrientation == HORIZONTAL_LIST) {
            float currentUsedColumnsCount = (float)Math.ceil((float)entriesCount / (float)rowsCount);
            float currentUsedSpacingColumns = Math.max(0, currentUsedColumnsCount - 1);
            contentWidth = listPaddingLeft + listPaddingRight + currentUsedColumnsCount * entryWidth + currentUsedSpacingColumns * entriesSpacingX;
        }else if(listOrientation == VERTICAL_LIST) {
            float currentUsedColumnsCount = (float)Math.min(entriesCount, columnsCount);
            float currentUsedSpacingColumns = Math.max(0, currentUsedColumnsCount - 1);
            contentWidth = listPaddingLeft + listPaddingRight + currentUsedColumnsCount * entryWidth + currentUsedSpacingColumns * entriesSpacingX;
        }else {
            app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

            throw new Error("PathFollowerNodeConfig_Pause.Error.1.");
        }

        return contentWidth;
    }

    protected float computeContentHeight() {
        float contentHeight;

        if(listOrientation == HORIZONTAL_LIST) {
            float currentUsedRowsCount = (float)Math.min(entriesCount, rowsCount);
            float currentUsedSpacingBandsY = Math.max(0, currentUsedRowsCount - 1);
            contentHeight = listPaddingTop + listPaddingBottom + currentUsedRowsCount * entryHeight + currentUsedSpacingBandsY * entriesSpacingY;
        }else if(listOrientation == VERTICAL_LIST) {
            float currentUsedRowsCount = (float)Math.ceil((float)entriesCount / (float)columnsCount);
            float currentUsedSpacingRows = Math.max(0, currentUsedRowsCount - 1);
            contentHeight = listPaddingTop + listPaddingBottom + currentUsedRowsCount * entryHeight + currentUsedSpacingRows * entriesSpacingY;
        }else {
            app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

            throw new Error("PathFollowerNodeConfig_Pause.Error.1.");
        }

        return contentHeight;
    }

    @Override
    protected void syncVisuals(boolean syncAbsoluteValues, boolean syncAnimatingValues, String startStateID, String endStateID, float stateCompletionPercent) {
        super.syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

        // Scale rendering3DSize to content or or use height override.
        if(scaleSizeToContent) {
            listWidth = computeContentWidth();
            listHeight = computeContentHeight();
        }else {
            listHeight = listHeightOverride;
            listWidth = listWidthOverride;
        }

        for(int i=0;i<scalableSizeElements.length;i++) {
            String[] itemID = scalableSizeElements[i];

            IDrawable2D item = (IDrawable2D)getItem(itemID);
            if(item == null) {
                app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

                throw new Error("PathFollowerNodeConfig_Pause.Error.1.");
            }

            item.setSize(listWidth, listHeight);
        }
    }

    private void updateEntryPos(ListEntry entry, int entryIndex) {
        float posX;
        float posY;

        if(listOrientation == HORIZONTAL_LIST) {
            posX = listPaddingLeft + (float)Math.floor((float)entryIndex / (float)rowsCount) * (entryWidth + entriesSpacingX);
            posY = 0f - listPaddingTop - entryHeight - (entryIndex % rowsCount) * (entriesSpacingY + entryHeight);
        }else if(listOrientation == VERTICAL_LIST) {
            posX = listPaddingLeft + (entryIndex % columnsCount) * (entryWidth + entriesSpacingX);
            posY = 0f - listPaddingTop - entryHeight - (float)Math.floor((float)entryIndex / (float)columnsCount) * (entriesSpacingY + entryHeight);
        }else {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        IDrawableNode entryMainLayer = (IDrawableNode)entry.getItem(new String[] {"_root", "layerBase"});

        entryMainLayer.setPosition(posX, posY);
    }

    @Override
    protected void createVisuals(Map<String, Object> createConfigMap) {
        super.createVisuals(createConfigMap);

        layerEntries = (IContainer2D)getItem(new String[] {"_root", "layerEntries"});

        touchAreaBack = (IInteractiveDrawable)getItem(new String[] {"_root", "touchAreaBack"});

        initializeListTouchAreaBackTouchListener();
    }

    public ListEntry createNewEntry(MenuItemConfig[] useItemConfigs, StateChangeDurations[] easeDurationsSList, int listPositionIndex, Map<String, Object> dataMapOverride) {
        if(useItemConfigs == null) {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        ListEntry entry = new ListEntry(app, useItemConfigs, easeDurationsSList, this);

        if(dataMapOverride != null) {
            entry.setDataMapOverride(dataMapOverride);
        }

        entry.create();

        int entryIndex;
        if(listPositionIndex >= 0) {
            entryIndex = Math.min(listPositionIndex, entriesList.size());
        }else {
            entryIndex = Math.max(0, entriesList.size() + 1 + listPositionIndex);
        }

        IDrawableNode entryMainLayer = (IDrawableNode)entry.getItem(new String[] {"_root", "layerBase"});
        layerEntries.addChild(entryMainLayer);

        entriesList.add(entryIndex, entry);
        entriesCount++;

        IInteractiveDrawable touchArea = (IInteractiveDrawable)entry.getItem(new String[] {"_root", "touchArea"});
        if(touchArea != null) {
            touchArea.getUserDataMap().put("listEntry", entry);

            initializeNewEntryTouchHandling(touchArea);
        }

        for(int i=entryIndex;i<entriesList.size();i++) {
            ListEntry entryItem = entriesList.get(i);

            updateEntryPos(entryItem, i);
        }

        doFullVisualsSync();

        return entry;
    }

    public void removeEntry(ListEntry entry) {
        int entryIndex = entriesList.indexOf(entry);

        sanitizeEntry(entry);

        entriesList.remove(entryIndex);
        entriesCount--;

        for(int i=entryIndex;i<entriesList.size();i++) {
            ListEntry entryItem = entriesList.get(i);

            updateEntryPos(entryItem, i);
        }

        doFullVisualsSync();
    }

    public ListEntry createNewEntry(MenuItemConfig[] itemInstanceConfigOverride, StateChangeDurations[] easeDurationsSList, Map<String, Object> dataMapOverride) {
        return createNewEntry(itemInstanceConfigOverride, easeDurationsSList, -1, dataMapOverride);
    }

    protected void initializeNewEntryTouchHandling(IInteractiveDrawable touchArea) {
        touchArea.setTouchListener(touchListener);
    }

    public void removeAllEntries() {
        if(entriesList == null) {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        for (int i = entriesList.size() - 1; i >= 0; i--) {
            ListEntry entry = entriesList.get(i);

            if(entry != null) {
                sanitizeEntry(entry);

                sanitizeEntry(entry);

                entriesList.remove(i);
                entriesCount--;
            }
        }

        doFullVisualsSync();
    }

    private void sanitizeEntry(ListEntry entry) {
        IDrawableNode entryMainLayer = (IDrawableNode) entry.getItem(new String[]{"_root", "layerBase"});
        if (entryMainLayer.getParent() != null) {
            entryMainLayer.getParent().removeChild(entryMainLayer);
        }

        entry.dispose();
    }

    @Override
    public Object getPropertyValue(String property) {
        if(property.equals("x")) {
            return visualsHolder.getX();
        }else if(property.equals("y")) {
            return visualsHolder.getY();
        }

        else if(property.equals("listOrientation")) {
            return listOrientation;
        }else if(property.equals("entryHeight")) {
            return entryHeight;
        }else if(property.equals("entryWidth")) {
            return entryWidth;
        }else if(property.equals("entriesSpacingX")) {
            return entriesSpacingX;
        }else if(property.equals("entriesSpacingY")) {
            return entriesSpacingY;
        }else if(property.equals("listPaddingTop")) {
            return listPaddingTop;
        }else if(property.equals("listPaddingBottom")) {
            return listPaddingBottom;
        }else if(property.equals("listPaddingLeft")) {
            return listPaddingLeft;
        }else if(property.equals("listPaddingRight")) {
            return listPaddingRight;
        }else if(property.equals("listWidthOverride")) {
            return listWidthOverride;
        }else if(property.equals("listHeightOverride")) {
            return listHeightOverride;
        }else if(property.equals("rowsCount")) {
            return rowsCount;
        }else if(property.equals("columnsCount")) {
            return columnsCount;
        }else if(property.equals("scaleSizeToContent")) {
            return scaleSizeToContent;
        }

        else if(property.equals("entryActivateSoundConfig")) {
            return entryActivateDSound.getSoundConfig();
        }

        else if(property.equals("listWidth")) {
            return listWidth;
        }else if(property.equals("listHeight")) {
            return listHeight;
        }

        app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

        throw new Error("AnimatedComposedStiledVisual.Error.7.");
    }

    @Override
    public void setPropertyValue(String property, Object value) {
        if(property.equals("x")) {
            visualsHolder.setX((Float)value);
        }else if(property.equals("y")) {
            visualsHolder.setY((Float)value);
        }

        else if(property.equals("listOrientation")) {
            listOrientation = (Integer)value;
        }else if(property.equals("entryHeight")) {
            entryHeight = (Float)value;
        }else if(property.equals("entryWidth")) {
            entryWidth = (Float)value;
        }else if(property.equals("entriesSpacingX")) {
            entriesSpacingX = (Float)value;
        }else if(property.equals("entriesSpacingY")) {
            entriesSpacingY = (Float)value;
        }else if(property.equals("listPaddingTop")) {
            listPaddingTop = (Float)value;
        }else if(property.equals("listPaddingBottom")) {
            listPaddingBottom = (Float)value;
        }else if(property.equals("listPaddingLeft")) {
            listPaddingLeft = (Float)value;
        }else if(property.equals("listPaddingRight")) {
            listPaddingRight = (Float)value;
        }else if(property.equals("listWidthOverride")) {
            listWidthOverride = (Float)value;
        }else if(property.equals("listHeightOverride")) {
            listHeightOverride = (Float)value;
        }else if(property.equals("rowsCount")) {
            rowsCount = (Integer)value;
        }else if(property.equals("columnsCount")) {
            columnsCount = (Integer)value;
        }else if(property.equals("scaleSizeToContent")) {
            scaleSizeToContent = (Boolean)value;
        }

        else if(property.equals("entryActivateSoundConfig")) {
            entryActivateDSound.putSoundConfig((SoundConfig) value);
        }

        else if(property.equals("listWidth")) {
            // Read only.

            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }else if(property.equals("listHeight")) {
            // Read only.

            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        else {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        for(int i=0;i<entriesList.size();i++) {
            ListEntry entry = entriesList.get(i);
            if(entry != null) {
                entry.appLoop(deltaS, deltaMS);
            }
        }
    }

    @Override
    public void gotoShowState(String endStateID, boolean doAnimation, IMenuAnimationListener menuAnimationListener, boolean restartAnimation) {
        super.gotoShowState(endStateID, doAnimation, menuAnimationListener, restartAnimation);
    }

    @Override
    public void dispose() {
        if(entriesList != null) {
            removeAllEntries();

            entriesList = null;
        }

        super.dispose();
    }

}
