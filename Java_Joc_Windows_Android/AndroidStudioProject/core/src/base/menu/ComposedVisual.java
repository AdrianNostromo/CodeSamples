package base.menu;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import base.menu.util.ComputeSmartValuePoolData;
import local.app.AppLocal;
import base.visual.IDrawableNode;
import base.visual.IDynamicPropertiesObject;
import base.visual.PseudoVisualData;

/**
 * Created by Adrin on 6/4/2016.
 */
// IDrawableNode or Map<String, Object>;
public class ComposedVisual extends HashMap<String, Object> implements IComposedVisual {

    protected AppLocal app;

    // Only items created from templates are added to this list, manual items have to be manually added to this list.
    protected Vector<IComposedVisual> smartItemsList = new Vector<IComposedVisual>();

    private Map<String, Boolean> staticAnimatingPropertiesMap = new HashMap<String, Boolean>();

    private Map<String, Object> dataMap = new HashMap<String, Object>();

    protected Map<String, Object> createConfigMap = null;

    public ComposedVisual(AppLocal app) {
        super();

        this.app = app;
    }

    public void create(Map<String, Object> createConfigMap) {
        this.createConfigMap = createConfigMap;
    }

    public final void create() {
        create(null);
    }

    public Map<String, Object> getDataMap() {
        return dataMap;
    }

    public void setDataMapOverride(Map<String, Object> dataMap) {
        this.dataMap = dataMap;
    }

    public Object getItemNonNull(String[] itemID) {
        Object ret = getItem(itemID);

        if(ret == null) {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        return ret;
    }
    public Object getItem(String[] itemID) {
        return getMenuItem(app,
                this, this,
                itemID, 0);
    }

    private static void recycleItem(AppLocal app, Object item, Vector<IComposedVisual> smartItemsList) {
        if (item instanceof IDrawableNode) {
            IDrawableNode cEntry = (IDrawableNode) item;

            if (cEntry.getParent() != null) {
                cEntry.getParent().removeChild(cEntry);
            }
        } else if(item instanceof IComposedVisual){
            IComposedVisual cEntry = (IComposedVisual) item;

            disposeItemsMap(app, cEntry, smartItemsList);
        }else if(item instanceof PseudoVisualData) {
            //void
        }else {
            app.log.postError("Error", "AnimatedComposedStiledVisual.Error.7.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }
    }

    protected static synchronized Object getMenuItem(
            AppLocal app,
            IComposedVisual itemsMapRoot, IComposedVisual itemsMapLayer,
            String[] itemID, int currentItemIDIndex)
    {
        if(itemsMapRoot == null || itemsMapLayer == null) {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        int _currentItemIDIndex = currentItemIDIndex;
        String currentIDLayer = itemID[_currentItemIDIndex];
        _currentItemIDIndex++;

        IComposedVisual _itemsMapLayer = null;
        if(currentIDLayer.equals("_local")) {
            _itemsMapLayer = itemsMapLayer;
        }else if(currentIDLayer.equals("_root")) {
            _itemsMapLayer = itemsMapRoot;
        }else if(_currentItemIDIndex == 1) {
            // The first item fxId entry must be a modifier.

            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }else {
            Object entry = itemsMapLayer.getLocalItem(currentIDLayer);
            if (entry == null) {
                return null;
            }

            if(_currentItemIDIndex < itemID.length) {
                // This is not the last entry, a Map object is expected.
                if (!(entry instanceof IComposedVisual)) {
                    app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                    throw new Error("AnimatedComposedStiledVisual.Error.7.");
                }

                _itemsMapLayer = (IComposedVisual)entry;
            }else {
                return entry;
            }
        }

        if(itemID.length <= _currentItemIDIndex) {
            return _itemsMapLayer;
        }

        return getMenuItem(
                app,
                itemsMapRoot, _itemsMapLayer,
                itemID, _currentItemIDIndex);
    }

    public void setItem(String[] itemID, Object item) {
        setMenuItem(app,
                this, this,
                itemID, 0, item);
    }

    private static synchronized void setMenuItem(
            AppLocal app,
            IComposedVisual itemsMapRoot, IComposedVisual itemsMapLayer,
            String[] itemID, int currentItemIDIndex, Object item)
    {
        String[] itemLayerID = new String[itemID.length - 1];
        System.arraycopy(itemID, 0, itemLayerID, 0, itemLayerID.length);
        String itemIDFragment = itemID[itemID.length - 1];

        IComposedVisual itemLayer = (IComposedVisual)getMenuItem(app,
                itemsMapRoot, itemsMapLayer,
                itemLayerID, currentItemIDIndex);

        if(itemLayer == null) {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        itemLayer.setLocalItem(itemIDFragment, item);
    }

    @Override
    public Object getLocalItem(String itemIDFragment) {
        return get(itemIDFragment);
    }

    @Override
    public void setLocalItem(String itemIDFragment, Object item) {
        Object existentItem = get(itemIDFragment);
        if(existentItem == null && item == null) {
            return ;
        }

        if(existentItem != null && item != null) {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        if(existentItem != null) {
            put(itemIDFragment, null);

            if(existentItem instanceof IComposedVisual) {
                if(smartItemsList.indexOf(existentItem) < 0) {
                    app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                    throw new Error("AnimatedComposedStiledVisual.Error.7.");
                }

                smartItemsList.remove((IComposedVisual)existentItem);
            }
        }

        if(item != null) {
            put(itemIDFragment, item);

            if(!itemIDFragment.equals("parent") && item instanceof IComposedVisual) {
                if(smartItemsList.indexOf(item) >= 0) {
                    app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                    throw new Error("AnimatedComposedStiledVisual.Error.7.");
                }

                smartItemsList.add((IComposedVisual)item);
            }
        }
    }

    public Object getItemPropertyValue(String[] itemID, String property, ComputeSmartValuePoolData poolData) {
        return getVisualPropertyValue(app, this, this, itemID, property, null, poolData, dataMap);
    }

    protected static Object getVisualPropertyValue(AppLocal app, IComposedVisual itemsMap, IComposedVisual itemsMapLayer, String[] itemID, String property, IDynamicPropertiesObject selfObject, ComputeSmartValuePoolData poolData, Map<String, Object> dataMap) {
        IDynamicPropertiesObject visualItem = null;

        String firstIDLayer = itemID[0];
        if(firstIDLayer.equals("_appSize")) {
            if(poolData != null && (poolData.isPropDependsOnAnimating == null || poolData.isPropDependsOnAnimating != true)) {
                poolData.isPropDependsOnAnimating = false;
            }

            if(property.equals("width")) {
                return app.screenSize.width;
            }else if(property.equals("height")) {
                return app.screenSize.height;
            }else {
                app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }
        }else if(firstIDLayer.equals("_dpm")) {
            if(poolData != null && (poolData.isPropDependsOnAnimating == null || poolData.isPropDependsOnAnimating != true)) {
                poolData.isPropDependsOnAnimating = false;
            }

            if(property.equals("width")) {
                return app.screenDensityMultiplier.x;
            }else if(property.equals("height")) {
                return app.screenDensityMultiplier.y;
            }else {
                app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }
        }else if(firstIDLayer.equals("_self")) {
            if(selfObject == null) {
                app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }

            visualItem = selfObject;
        }else if(firstIDLayer.equals("_dataMap")) {
            if(poolData != null && (poolData.isPropDependsOnAnimating == null || poolData.isPropDependsOnAnimating != true)) {
                poolData.isPropDependsOnAnimating = true;
            }

            if(dataMap == null) {
                app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }

            return dataMap.get(property);
        }else {
            try {
                visualItem = (IDynamicPropertiesObject)getMenuItem(app, itemsMap, itemsMapLayer, itemID, 0);
            }catch(ClassCastException x) {
                app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }
        }

        if(visualItem == null) {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        float ret;

        if(poolData != null && (poolData.isPropDependsOnAnimating == null || poolData.isPropDependsOnAnimating != true)) {
            Boolean b = visualItem.getStaticAnimatingPropertyStatus(property);
            if (b == null) {
                app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }

            poolData.isPropDependsOnAnimating = b;
        }
        try {
            ret = (Float) visualItem.getPropertyValue(property);
        }catch(ClassCastException x) {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }catch(NullPointerException x) {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        return ret;
    }

    private static void disposeItemsMap(AppLocal app, IComposedVisual entriesMap, Vector<IComposedVisual> smartItemsList) {
        for (Map.Entry<String, Object> entry : entriesMap.entrySet()) {
            if(entry != null && entry.getValue() != null && !entry.getKey().equals("parent")) {
                Object item = entry.getValue();

                if(item instanceof IComposedVisual) {
                    ((IComposedVisual)item).dispose();

                    smartItemsList.remove((IComposedVisual)item);
                }

                recycleItem(app, item, smartItemsList);
            }
        }
    }

    public void appLoop(float deltaS, int deltaMS) {
        // Call app loop on items that use it and implement IAppLoopItem.
        for(int i=0;i<smartItemsList.size();i++) {
            IComposedVisual entry = smartItemsList.get(i);
            if(entry != null) {
                entry.appLoop(deltaS, deltaMS);
            }
        }
    }

    public Boolean getStaticAnimatingPropertyStatus(String property) {
        return staticAnimatingPropertiesMap.get(property);
    }

    public void putStaticAnimatingPropertyStatus(String property, Boolean status) {
        staticAnimatingPropertiesMap.put(property, status);
    }

    public void dispose() {
        disposeItemsMap(app, this, smartItemsList);
    }

}
