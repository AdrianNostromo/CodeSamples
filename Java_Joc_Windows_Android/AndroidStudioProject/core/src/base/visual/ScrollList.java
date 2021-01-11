package base.visual;

import com.badlogic.gdx.Gdx;

import java.util.Map;

import base.interaction.TouchListener;
import base.menu.util.MenuItemConfig;
import base.menu.util.StateChangeDurations;
import local.app.AppLocal;
import base.util.MathUtil;
import base.util.Point2D;
import base.visual.util.ScrollListEventListener;

/**
 * Created by Adrin on 6/4/2016.
 */
public class ScrollList extends List {

    private static final int DRAG_PHASE_OFF = 0;
    private static final int DRAG_PHASE_PRE = 1;
    private static final int DRAG_PHASE_MANUAL = 2;
    private static final int DRAG_PHASE_FLING = 3;

    private float preScrollDeltaThreshold = 0f;

    private ListEntry touchDownListEntry = null;

    private int _dragPhase = DRAG_PHASE_OFF;
    private Point2D currentDragLastTickPos = new Point2D(0f, 0f);
    private Point2D currentDragTotalDelta = new Point2D(0f, 0f);
    private Point2D currentDragTickDelta = new Point2D(0f, 0f);

    private boolean isScrollEnabled = true;

    // baseValue >= 0;
    private Point2D scrollLimit = new Point2D(0f, 0f);

    private TouchListener touchListener = new TouchListener() {
        @Override
        public boolean touchDown(IInteractiveDrawable target, float worldX, float worldY) {
            // Touch events are ignored while the menu is animating.
            if(!getIsScrollEnabled()) {
                return false;
            }

            if(target == touchAreaBack) {
                startScroll(worldX, worldY);

                if(eventListener != null) {
                    ((ScrollListEventListener) eventListener).bgTouchDownOccurred();
                }

                // Return true so we receive move events and/or touchUpOutside events.
                return true;
            }else if(target.getUserDataMap().get("listEntry") != null) {
                touchDownListEntry = (ListEntry)target.getUserDataMap().get("listEntry");
            }else {
                Gdx.app.debug("Error", "Touch event received for a unknown target.");
            }

            return false;
        }

        @Override
        public void touchUp(IInteractiveDrawable target, float worldX, float worldY) {
            super.touchUp(target, worldX, worldY);

            if(!getIsScrollEnabled()) {
                return;
            }

            if(target == touchAreaBack) {
                endScroll(true);
            }else if(target.getUserDataMap().get("listEntry") != null) {
                if(touchDownListEntry != null) {
                    if(touchDownListEntry == target.getUserDataMap().get("listEntry")) {
                        if (eventListener != null) {
                            ListEntry entry = (ListEntry) target.getUserDataMap().get("listEntry");
                            eventListener.entryActivated(entry.list, entry);
                        }

                        entryActivateDSound.play();
                    }

                    touchDownListEntry = null;
                }
            }
        }

        @Override
        public void touchUpOutside(IInteractiveDrawable target, float worldX, float worldY) {
            super.touchUpOutside(target, worldX, worldY);

            if(!getIsScrollEnabled()) {
                return;
            }

            if(target == touchAreaBack) {
                endScroll(true);
            }else if(target.getUserDataMap().get("listEntry") != null) {
                touchDownListEntry = null;
            }
        }

        @Override
        public void touchMove(IInteractiveDrawable target, float worldX, float worldY) {
            super.touchMove(target, worldX, worldY);

            if(!getIsScrollEnabled()) {
                return;
            }

            if(target == touchAreaBack) {
                scrollMove(worldX, worldY);
            }
        }
    };

    public ScrollList(AppLocal app) {
        super(app);

        //void
    }

    @Override
    protected void initializeListTouchAreaBackTouchListener() {
        // super.initializeListTouchAreaBackTouchListener();

        touchAreaBack.setTouchListener(touchListener);
    }

    @Override
    protected void initializeNewEntryTouchHandling(IInteractiveDrawable touchArea) {
        // super.initializeNewEntryTouchHandling(touchArea);

        // Enable item touch fall through.
        touchArea.setIsTouchFallthroughEnabled(true);
        touchArea.setTouchListener(touchListener);
    }

    private int getDragPhase() {
        return _dragPhase;
    }

    public boolean isScrollActive() {
        if(getDragPhase() == DRAG_PHASE_OFF) {
            return false;
        }

        return true;
    }

    private void setDragPhase(int dragPhase) {
        this._dragPhase = dragPhase;
        if(getDragPhase() != DRAG_PHASE_OFF) {
            if(eventListener != null) {
                ((ScrollListEventListener) eventListener).scrollStarted();
            }
        }else {
            if(eventListener != null) {
                ((ScrollListEventListener) eventListener).scrollEnded();
            }
        }
    }

    @Override
    protected void syncVisuals(boolean syncAbsoluteValues, boolean syncAnimatingValues, String startStateID, String endStateID, float stateCompletionPercent) {
        super.syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

        updateScrollingLimits();

        scrollToPosition(layerEntries.getX(), layerEntries.getY(), false);
    }

    @Override
    public ListEntry createNewEntry(MenuItemConfig[] useItemConfigs, StateChangeDurations[] easeDurationsSList, int listPositionIndex, Map<String, Object> dataMapOverride) {
        ListEntry ret = super.createNewEntry(useItemConfigs, easeDurationsSList, listPositionIndex, dataMapOverride);

        updateScrollingLimits();

        return ret;
    }

    @Override
    public ListEntry createNewEntry(MenuItemConfig[] useItemConfigs, StateChangeDurations[] easeDurationsSList, Map<String, Object> dataMapOverride) {
        return createNewEntry(useItemConfigs, easeDurationsSList, -1, dataMapOverride);
    }

    @Override
    public void removeEntry(ListEntry entry) {
        super.removeEntry(entry);

        updateScrollingLimits();
    }

    public boolean getIsScrollEnabled() {
        return isScrollEnabled;
    }

    public void setIsScrollEnabled(boolean isScrollEnabled) {
        this.isScrollEnabled = isScrollEnabled;

        endScroll(false);

        if(!this.isScrollEnabled) {
            touchDownListEntry = null;
        }
    }

    @Override
    public void removeAllEntries() {
        super.removeAllEntries();

        touchDownListEntry = null;
    }

    public float getScrollOffset() {
        return MathUtil.dist(0f, 0f, layerEntries.getX(), layerEntries.getY());
    }

    public float getScrollOffsetX() {
        return layerEntries.getX();
    }

    public float getScrollOffsetY() {
        return layerEntries.getY();
    }

    public float getScrollOffsetDIP() {
        return MathUtil.dist(0f, 0f, layerEntries.getX(), layerEntries.getY()) / app.screenDensityMultiplier.x;
    }

    public void scrollToPosition(float listPosX, float listPosY, boolean dispatchEvent) {
        float newPosX = Math.min(0f, Math.max(scrollLimit.x, listPosX));
        float newPosY = Math.max(0f, Math.min(scrollLimit.y, listPosY));

        float scrollDeltaX = newPosX - layerEntries.getX();
        float scrollDeltaY = newPosY - layerEntries.getY();

        float scrollDeltaTotal = MathUtil.dist(0f, 0f, scrollDeltaX, scrollDeltaY);

        layerEntries.setPosition(newPosX, newPosY);

        if(dispatchEvent && eventListener != null) {
            if(scrollDeltaY != 0) {
                ((ScrollListEventListener) eventListener).scrollOccurred(scrollDeltaX, scrollDeltaY, newPosX, newPosY, scrollDeltaTotal);
            }
        }
    }

    public boolean isScrollAtBeginning() {
        if(layerEntries.getX() == 0 && layerEntries.getY() == 0) {
            return true;
        }

        return false;
    }

    public boolean isScrollAtEnd() {
        if(layerEntries.getX() == scrollLimit.x && layerEntries.getY() == scrollLimit.y) {
            return true;
        }

        return false;
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        if(!getIsScrollEnabled()) {
            return ;
        }

        if(getDragPhase() != DRAG_PHASE_OFF) {
            if(getDragPhase() == DRAG_PHASE_PRE) {
                if(Math.abs(currentDragTotalDelta.computeDist()) >= preScrollDeltaThreshold) {
                    touchDownListEntry = null;

                    setDragPhase(DRAG_PHASE_MANUAL);
                }
            }else if(getDragPhase() == DRAG_PHASE_MANUAL) {
                if(currentDragTickDelta.x != 0 || currentDragTickDelta.y != 0) {
                    float newListPosX;
                    float newListPosY;

                    if(listOrientation == HORIZONTAL_LIST) {
                        newListPosX = layerEntries.getX() + currentDragTickDelta.x;
                        newListPosY = 0f;
                    }else if(listOrientation == VERTICAL_LIST) {
                        newListPosX = 0f;
                        newListPosY = layerEntries.getY() + currentDragTickDelta.y;
                    }else {
                        app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                        throw new Error("AnimatedComposedStiledVisual.Error.7.");
                    }

                    scrollToPosition(newListPosX, newListPosY, true);
                }
            }else if(getDragPhase() == DRAG_PHASE_FLING) {
                //asd_002;
            }else {
                app.errorHandler.handleAppCriticalError("ScrollList.1", "ScrollList.1.");

                return ;
            }

            currentDragTickDelta.set(0f, 0f);
        }
    }

    //asd_002;// implement scrolling inertia fling;
    //asd_002;// when the list is scrolled to the end, add a loading more spinner preloader.
    //asd_002;// list scroll bar as indicator only.
    //asd_002;// the list has some elasticity when over-dragging is attempted.
    //asd_002;// implement a refresh when the list is dragged down on the first entry.

    private void updateScrollingLimits() {
        int entriesCount = entriesList.size();

        //scrollLimit = Math.max(0, totalEntriesListHeight - componentHeight);
        if(listOrientation == HORIZONTAL_LIST) {
            float totalEntriesListWidth = computeContentWidth();

            scrollLimit.set(Math.min(0, listWidth - totalEntriesListWidth), 0f);
        }else if(listOrientation == VERTICAL_LIST) {
            float totalEntriesListHeight = computeContentHeight();

            scrollLimit.set(0f, Math.max(0, totalEntriesListHeight - listHeight));
        }else {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }
    }

    private void startScroll(float worldX, float worldY) {
        if(getDragPhase() != DRAG_PHASE_OFF) {
            endScroll(false);
        }

        float processedWorldX = 0f;
        float processedWorldY = 0f;
        if(listOrientation == HORIZONTAL_LIST) {
            processedWorldX = worldX;
        }else if(listOrientation == VERTICAL_LIST) {
            processedWorldY = worldY;
        }else {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        setDragPhase(DRAG_PHASE_PRE);
        currentDragTotalDelta.set(0f, 0f);
        currentDragTickDelta.set(0f, 0f);
        currentDragLastTickPos.set(processedWorldX, processedWorldY);
    }

    private void endScroll(boolean doFling) {
        if(getDragPhase() == DRAG_PHASE_OFF) {
            return;
        }

        if(!doFling) {
            setDragPhase(DRAG_PHASE_OFF);
        }else {
            setDragPhase(DRAG_PHASE_OFF);//asd_tst;
            /*asd_tst;
            if(asd_enoughVelocityForFling) {
                setDragPhase(DRAG_PHASE_FLING);
            }*/
        }
    }

    private void scrollMove(float worldX, float worldY) {
        if(getDragPhase() == DRAG_PHASE_OFF) {
            return;
        }

        float processedWorldX = 0f;
        float processedWorldY = 0f;
        if(listOrientation == HORIZONTAL_LIST) {
            processedWorldX = worldX;
        }else if(listOrientation == VERTICAL_LIST) {
            processedWorldY = worldY;
        }else {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        float deltaX = processedWorldX - currentDragLastTickPos.x;
        float deltaY = processedWorldY - currentDragLastTickPos.y;

        currentDragLastTickPos.set(processedWorldX, processedWorldY);

        currentDragTotalDelta.set(currentDragTotalDelta.x + deltaX, currentDragTotalDelta.y + deltaY);
        currentDragTickDelta.set(currentDragTickDelta.x + deltaX, currentDragTickDelta.y + deltaY);
    }

    @Override
    public Object getPropertyValue(String property) {
        if(property.equals("preScrollDeltaThreshold")) {
            return preScrollDeltaThreshold;
        }

        return super.getPropertyValue(property);
    }

    @Override
    public void setPropertyValue(String property, Object value) {
        if(property.equals("preScrollDeltaThreshold")) {
            preScrollDeltaThreshold = (Float)value;
        }else {
            super.setPropertyValue(property, value);
        }
    }

}
