package base.menu;

import com.badlogic.gdx.graphics.Color;

import java.lang.reflect.InvocationTargetException;
import java.util.Map;
import java.util.Vector;

import base.ease.EaseFunctions;
import base.menu.util.ComputeSmartValuePoolData;
import base.menu.util.EaseConfig;
import base.menu.util.IMenuItemConfig_ComposedVisual;
import base.menu.util.IMenuItemConfig_DynamicProperties;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_ComposedVisual;
import base.menu.util.MenuItemConfig_Data;
import base.menu.util.MenuItemConfig_SmartItem;
import base.menu.util.MenuItemConfig_Visual;
import base.menu.util.MenuVisualAnimationProperty;
import local.app.AppLocal;
import base.visual.IContainer2D;
import base.visual.IDrawableNode;
import base.visual.IDynamicPropertiesObject;
import base.visual.PseudoVisualData;
import base.visual.VisualUtil;
import base.visual.util.VisualPropertiesConfig;
import base.visual.util.VisualPropertyValueConfig;
import base.visual.util.VisualValueConfig;

/**
 * Created by Adrin on 6/4/2016.
 */
// IDrawableNode or Map<String, Object>;
public class ComposedStiledVisual extends ComposedVisual {

    protected MenuItemConfig[] menuItemConfigs;

    //asd_002;// remove this var and usr {"_root", "layerBase"} instead.
    protected IContainer2D visualsHolder = null;

    public ComposedStiledVisual(AppLocal app, MenuItemConfig[] menuItemConfigs) {
        super(app);

        this.menuItemConfigs = menuItemConfigs;
    }

    public IContainer2D getVisualsHolder() {
        return visualsHolder;
    }

    @Override
    public void create(Map<String, Object> createConfigMap) {
        super.create(createConfigMap);

        put("visualsHolder", visualsHolder = VisualUtil.newGroup(null));

        if(!(this instanceof IDynamicPropertiesObject)) {
            createVisuals(createConfigMap);

            syncVisuals(true, false, null, null, 0);
        }
    }

    public void doFullVisualsSync() {
        syncVisuals(true, true, null, null, 0);
    }

    protected void createVisuals(Map<String, Object> createConfigMap) {
        createVisualItems(app, this, this, menuItemConfigs, createConfigMap, smartItemsList);
    }

    private void createVisualItems(AppLocal app, IComposedVisual itemsMap, IComposedVisual itemsMapLayer, MenuItemConfig[] menuItemConfigs, Map<String, Object> createConfigMap, Vector<IComposedVisual> smartItemsList) {
        if(menuItemConfigs == null) {
            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        for(int i = 0; i< menuItemConfigs.length; i++) {
            MenuItemConfig itemConfig = menuItemConfigs[i];

            if(itemsMapLayer.getLocalItem(itemConfig.itemID) != null) {
                app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }

            boolean isItemAddedToMap = false;
            Object item = null;

            if(itemConfig instanceof MenuItemConfig_SmartItem) {
                MenuItemConfig_SmartItem cItemConfig = (MenuItemConfig_SmartItem)itemConfig;

                AnimatedComposedStiledVisual newSmartItem;
                try {
                    newSmartItem = (AnimatedComposedStiledVisual)cItemConfig.classContructor.newInstance(app);
                }catch(InstantiationException x) {
                    throw new Error("Error occurred.");
                }catch(IllegalAccessException x) {
                    throw new Error("Error occurred.");
                }catch(InvocationTargetException x) {
                    throw new Error("Error occurred.");
                }catch(IllegalArgumentException x) {
                    throw new Error("Error occurred.");
                }
                if(newSmartItem == null) {
                    app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                    throw new Error("AnimatedComposedStiledVisual.Error.7.");
                }

                newSmartItem.put("parent", itemsMapLayer);

                item = newSmartItem;

                newSmartItem.create();

                if(cItemConfig.parentVisualID != null) {
                    IContainer2D parentContainer = (IContainer2D)getMenuItem(app, itemsMap, itemsMapLayer, cItemConfig.parentVisualID, 0);
                    if(parentContainer == null) {
                        app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                        throw new Error("AnimatedComposedStiledVisual.Error.7.");
                    }

                    parentContainer.addChild(newSmartItem.visualsHolder);
                }
            }else if(itemConfig instanceof MenuItemConfig_Data) {
                MenuItemConfig_Data cItemConfig = (MenuItemConfig_Data)itemConfig;

                item = new PseudoVisualData();
            }else if(itemConfig instanceof MenuItemConfig_Visual) {
                MenuItemConfig_Visual cItemConfig = (MenuItemConfig_Visual)itemConfig;

                IContainer2D parentContainer = null;
                if(cItemConfig.parentVisualID != null) {
                    parentContainer = (IContainer2D)getMenuItem(app, itemsMap, itemsMapLayer, cItemConfig.parentVisualID, 0);
                    if(parentContainer == null) {
                        app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                        throw new Error("AnimatedComposedStiledVisual.Error.7.");
                    }
                }

                if(cItemConfig.visualCreateConfig != null) {
                    item = VisualUtil.newSmartVisual(app, cItemConfig.visualCreateConfig, parentContainer, createConfigMap);
                    if (item == null) {
                        app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                        throw new Error("AnimatedComposedStiledVisual.Error.7.");
                    }
                }else {
                    // The item is optional will be null, don't add it to the items map using the default method.

                    isItemAddedToMap = true;
                }
            }else if(itemConfig instanceof MenuItemConfig_ComposedVisual) {
                IMenuItemConfig_ComposedVisual cItemConfig = (IMenuItemConfig_ComposedVisual)itemConfig;

                IComposedVisual subItemsMapLayer = new ComposedVisual(app);
                subItemsMapLayer.setLocalItem("parent", itemsMapLayer);
                item = subItemsMapLayer;

                isItemAddedToMap = true;
                itemsMapLayer.setLocalItem(itemConfig.itemID, item);
                visualItemCreated(item, itemConfig, itemsMap, itemsMapLayer);

                subItemsMapLayer.create();

                createVisualItems(app, itemsMap, subItemsMapLayer, cItemConfig.getMenuItemConfigs(), createConfigMap, smartItemsList);
            }else {
                app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }

            // Add item to smartItemsList if it is instance of ISmartItem.
            if(!isItemAddedToMap) {
                if(item == null) {
                    app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                    throw new Error("AnimatedComposedStiledVisual.Error.7.");
                }

                itemsMapLayer.setLocalItem(itemConfig.itemID, item);
                visualItemCreated(item, itemConfig, itemsMap, itemsMapLayer);
            }
        }
    }

    protected void visualItemCreated(Object item, MenuItemConfig itemConfig, IComposedVisual itemsMap, IComposedVisual itemsMapLayer) {
        if(item instanceof IDynamicPropertiesObject && itemConfig instanceof IMenuItemConfig_DynamicProperties) {
            syncVisualItem(
                    (IDynamicPropertiesObject)item, ((IMenuItemConfig_DynamicProperties) itemConfig).getItemProperties(),
                    true, false,
                    null, null,
                    0f,
                    app, itemsMap, itemsMapLayer,
                    getDataMap()
            );
        }

        if(item instanceof ComposedStiledVisual && item instanceof IDynamicPropertiesObject) {
            ((ComposedStiledVisual) item).createVisuals(createConfigMap);
        }

        if(item instanceof ComposedStiledVisual) {
            ((ComposedStiledVisual)item).doFullVisualsSync();
        }

        if(item instanceof AnimatedComposedStiledVisual && itemConfig instanceof MenuItemConfig_SmartItem) {
            MenuItemConfig_SmartItem cItemConfig = (MenuItemConfig_SmartItem)itemConfig;

            if(cItemConfig.initialBaseStateId != null) {
                String newState = ((AnimatedComposedStiledVisual)item).computeStateFlags(cItemConfig.initialBaseStateId);
                ((AnimatedComposedStiledVisual)item).gotoShowState(newState, false, null, false);
            }
        }
    }

    protected void syncVisuals(boolean syncAbsoluteValues, boolean syncAnimatingValues, String startStateID, String endStateID, float stateCompletionPercent) {
        if(visualsHolder == null) {
            // Visuals are not create yet.

            app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        syncMenuItems(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent, app, this, this, menuItemConfigs, getDataMap());
    }

    private static void syncMenuItems(
            boolean syncAbsoluteValues, boolean syncAnimatingValues,
            String startStateID, String endStateID, float stateCompletionPercent, AppLocal app,
            ComposedVisual itemsMap, ComposedVisual itemsMapLayer,
            MenuItemConfig[] menuItemConfigs,
            Map<String, Object> dataMap
    ) {
        for (int i = 0; i < menuItemConfigs.length; i++) {
            MenuItemConfig itemConfig = menuItemConfigs[i];

            if(itemConfig instanceof MenuItemConfig_Visual) {
                MenuItemConfig_Visual cItemConfig = (MenuItemConfig_Visual)itemConfig;

                IDrawableNode visual = (IDrawableNode)itemsMapLayer.get(cItemConfig.itemID);
                if(visual == null && cItemConfig.visualCreateConfig == null) {
                    // Visual is optional and it is currently not created.

                    continue;
                }
            }

            if(itemConfig instanceof IMenuItemConfig_ComposedVisual) {
                IMenuItemConfig_ComposedVisual itemConfig_local = (IMenuItemConfig_ComposedVisual)itemConfig;
                ComposedVisual subItemsMapLayer = (ComposedVisual)itemsMapLayer.get(itemConfig.itemID);
                if(subItemsMapLayer == null) {
                    app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                    throw new Error("AnimatedComposedStiledVisual.Error.7.");
                }

                syncMenuItems(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent, app, itemsMap, subItemsMapLayer, itemConfig_local.getMenuItemConfigs(), dataMap);
            }

            Object item = itemsMapLayer.get(itemConfig.itemID);

            if(itemConfig instanceof IMenuItemConfig_DynamicProperties) {
                IMenuItemConfig_DynamicProperties itemConfigLocal = (IMenuItemConfig_DynamicProperties)itemConfig;

                IDynamicPropertiesObject localItem = (IDynamicPropertiesObject)item;
                if(localItem == null) {
                    app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                    throw new Error("AnimatedComposedStiledVisual.Error.7.");
                }
                syncVisualItem(localItem, itemConfigLocal.getItemProperties(), syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent, app, itemsMap, itemsMapLayer, dataMap);
            }
        }
    }

    protected static void syncVisualItem(IDynamicPropertiesObject visual, VisualPropertiesConfig itemProperties,
                                         boolean syncAbsoluteValues, boolean syncAnimatingValues,
                                         String startStateID, String endStateID,
                                         float stateCompletionPercent,
                                         AppLocal app, IComposedVisual itemsMap, IComposedVisual itemsMapLayer,
                                         Map<String, Object> dataMap
    ) {
        for(int i=0;i<itemProperties.propertyConfigs.size();i++) {
            VisualPropertyValueConfig propertyValue = itemProperties.propertyConfigs.get(i);

            Object value = 0;
            boolean doUpdate = false;
            if(propertyValue.smartValue == null) {
                app.errorHandler.handleAppCriticalError("WorldRoomLayers.1", "Error.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }
            if(propertyValue.smartValue.baseValue instanceof MenuVisualAnimationProperty) {
                if (syncAnimatingValues) {
                    doUpdate = true;

                    MenuVisualAnimationProperty animationProperty = (MenuVisualAnimationProperty) propertyValue.smartValue.baseValue;

                    MenuVisualAnimationProperty.StateComposedValue composedStartValue = getValueWherePropertyHasFragment(startStateID, animationProperty.stateValuesList);

                    MenuVisualAnimationProperty.StateComposedValue composedEndValue = getValueWherePropertyHasFragment(endStateID, animationProperty.stateValuesList);

                    if (composedStartValue == null && composedEndValue == null) {
                        continue;
                    }

                    if (composedStartValue == null && composedEndValue != null) {
                        composedStartValue = composedEndValue;
                    } else if (composedEndValue == null && composedStartValue != null) {
                        composedEndValue = composedStartValue;
                    }

                    EaseConfig easeConfig = composedEndValue.easeConfig;

                    float animationPercentValue;

                    if(stateCompletionPercent < easeConfig.startPercentOverride) {
                        animationPercentValue = 0f;
                    }else if(stateCompletionPercent > easeConfig.endPercentOverride) {
                        animationPercentValue = 1f;
                    }else {
                        float animationClipAmount = easeConfig.startPercentOverride + (1f - easeConfig.endPercentOverride);

                        if(animationClipAmount > 0) {
                            float animationRegionSize = 1f - animationClipAmount;

                            float n = (stateCompletionPercent - easeConfig.startPercentOverride) / animationRegionSize;

                            animationPercentValue = EaseFunctions.computeEaseValue(
                                    easeConfig.easeFunctionID, easeConfig.easeFunctionConfig,
                                    n
                            );
                        }else {
                            animationPercentValue = EaseFunctions.computeEaseValue(
                                    easeConfig.easeFunctionID, easeConfig.easeFunctionConfig,
                                    stateCompletionPercent
                            );
                        }
                    }

                    Object processedStartValue = computeSmartValue(app, itemsMap, itemsMapLayer, composedStartValue.smartValue, visual, null, dataMap);
                    Object processedEndValue = computeSmartValue(app, itemsMap, itemsMapLayer, composedEndValue.smartValue, visual, null, dataMap);

                    if (processedStartValue instanceof Float && processedEndValue instanceof Float) {
                        float valS = (Float) processedStartValue;
                        float valE = (Float) processedEndValue;
                        value = valS + (valE - valS) * animationPercentValue;
                    } else if (processedStartValue instanceof Boolean && processedEndValue instanceof Boolean) {
                        value = processedEndValue;
                    } else if (processedStartValue instanceof Color && processedEndValue instanceof Color) {
                        Color colStart = (Color)processedStartValue;
                        Color colEnd = (Color)processedEndValue;

                        value = new Color(
                                colStart.r + (colEnd.r - colStart.r) * animationPercentValue,
                                colStart.g + (colEnd.g - colStart.g) * animationPercentValue,
                                colStart.b + (colEnd.b - colStart.b) * animationPercentValue,
                                colStart.a + (colEnd.a - colStart.a) * animationPercentValue
                        );
                    } else {
                        app.log.postError("Error", "AnimatedComposedStiledVisual.Error.7.");

                        throw new Error("AnimatedComposedStiledVisual.Error.7.");
                    }

                    if(visual.getStaticAnimatingPropertyStatus(propertyValue.property) == null) {
                        visual.putStaticAnimatingPropertyStatus(propertyValue.property, true);
                    }
                }
            }else {
                Boolean isPropertyDependsOnAnimating = visual.getStaticAnimatingPropertyStatus(propertyValue.property);
                if(!syncAbsoluteValues && isPropertyDependsOnAnimating == null) {
                    app.log.postError("Error", "AnimatedComposedStiledVisual.Error.7.");

                    throw new Error("AnimatedComposedStiledVisual.Error.7.");
                }

                if(syncAbsoluteValues || (syncAnimatingValues && isPropertyDependsOnAnimating == true)) {
                    doUpdate = true;

                    ComputeSmartValuePoolData poolData = new ComputeSmartValuePoolData();
                    value = computeSmartValue(app, itemsMap, itemsMapLayer, propertyValue.smartValue, visual, poolData, dataMap);

                    if(isPropertyDependsOnAnimating == null) {
                        if(poolData.isPropDependsOnAnimating == null) {
                            // This error may occur because the property name is not correct, check for typos.

                            app.log.postError("Error", "AnimatedComposedStiledVisual.Error.7.");

                            throw new Error("AnimatedComposedStiledVisual.Error.7.");
                        }

                        visual.putStaticAnimatingPropertyStatus(propertyValue.property, poolData.isPropDependsOnAnimating);
                    }
                }
            }

            if(doUpdate) {
                visual.setPropertyValue(propertyValue.property, value);
            }
        }
    }

    protected static boolean checkIsStatePassed(String stateID, Object composedStateIDFragment) {
        boolean isStateCheckPassed = true;
        if(composedStateIDFragment instanceof String) {
            String entryStateID = (String)composedStateIDFragment;
            if(!stateID.contains(entryStateID)) {
                isStateCheckPassed = false;
            }
        }else {
            for(int j=0;j<((String[])composedStateIDFragment).length;j++) {
                String entryStateID = ((String[])composedStateIDFragment)[j];
                if(!stateID.contains(entryStateID)) {
                    isStateCheckPassed = false;

                    break;
                }
            }
        }

        return isStateCheckPassed;
    }

    private static MenuVisualAnimationProperty.StateComposedValue getValueWherePropertyHasFragment(String menuStateID, Vector<MenuVisualAnimationProperty.StateComposedValue> stateValuesList) {
        MenuVisualAnimationProperty.StateComposedValue composedStartValue = null;

        if(stateValuesList != null) {
            for(int i=0;i<stateValuesList.size();i++) {
                MenuVisualAnimationProperty.StateComposedValue valueEntry = stateValuesList.get(i);

                boolean isStateCheckPassed = checkIsStatePassed(menuStateID, valueEntry.stateIDFragments);
                if(isStateCheckPassed) {
                    composedStartValue = valueEntry;

                    break;
                }
            }
        }

        return composedStartValue;
    }

    private static Object computeSmartValue(AppLocal app, IComposedVisual itemsMap, IComposedVisual itemsMapLayer, VisualValueConfig smartValue, IDynamicPropertiesObject selfObject, ComputeSmartValuePoolData poolData, Map<String, Object> dataMap) {
        Object value;
        if(smartValue.baseMultiplierSourceID != null && smartValue.baseMultiplierProperty != null) {
            float multiplier;
            try {
                multiplier = (Float)getVisualPropertyValue(app, itemsMap, itemsMapLayer, smartValue.baseMultiplierSourceID, smartValue.baseMultiplierProperty, selfObject, poolData, dataMap);
            }catch(NullPointerException x) {
                app.log.postError("Error", "AnimatedComposedStiledVisual.Error.7.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }catch(ClassCastException x) {
                app.log.postError("Error", "AnimatedComposedStiledVisual.Error.7.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }

            value = (Float)smartValue.baseValue * multiplier;
        }else {
            value = smartValue.baseValue;

            if(poolData != null && (poolData.isPropDependsOnAnimating == null || poolData.isPropDependsOnAnimating != true)) {
                poolData.isPropDependsOnAnimating = false;
            }
        }

        if(smartValue.secondarySmartValue != null) {
            Object secondaryValue = computeSmartValue(app, itemsMap, itemsMapLayer, smartValue.secondarySmartValue, selfObject, poolData, dataMap);

            if(smartValue.operationType == VisualValueConfig.OPERATION_ADD) {
                value = (Float)value + (Float)secondaryValue;
            }else if(smartValue.operationType == VisualValueConfig.OPERATION_SUBTRACT) {
                value = (Float)value - (Float)secondaryValue;
            }else if(smartValue.operationType == VisualValueConfig.OPERATION_MULTIPLY) {
                value = (Float)value * (Float)secondaryValue;
            }else if(smartValue.operationType == VisualValueConfig.OPERATION_DIVIDE) {
                value = (Float)value / (Float)secondaryValue;
            }else if(smartValue.operationType == VisualValueConfig.OPERATION_MIN) {
                value = Math.min((Float)value, (Float)secondaryValue);
            }else if(smartValue.operationType == VisualValueConfig.OPERATION_MAX) {
                value = Math.max((Float)value, (Float)secondaryValue);
            }else {
                app.log.postError("Error", "AnimatedComposedStiledVisual.Error.7.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }
        }else {
            if(smartValue.operationType == VisualValueConfig.OPERATION_NONE) {
                //void
            }else if(smartValue.operationType == VisualValueConfig.OPERATION_ROUND) {
                value = Math.round((Float)value);
            }else if(smartValue.operationType == VisualValueConfig.OPERATION_FLOOR) {
                value = (float)Math.floor((Float)value);
            }else if(smartValue.operationType == VisualValueConfig.OPERATION_CEIL) {
                value = (float)Math.ceil((Float)value);
            }else {
                app.log.postError("Error", "AnimatedComposedStiledVisual.Error.7.");

                throw new Error("AnimatedComposedStiledVisual.Error.7.");
            }
        }

        return value;
    }

    @Override
    public void dispose() {
        if(getVisualsHolder().getParent() != null) {
            getVisualsHolder().getParent().removeChild(getVisualsHolder());
        }

        super.dispose();
    }
}
