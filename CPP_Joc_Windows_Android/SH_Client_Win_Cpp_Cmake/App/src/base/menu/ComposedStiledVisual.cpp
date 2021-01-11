#include <base/visual2D/VisualUtil2D.h>
#include "ComposedStiledVisual.h"
#include "AnimatedComposedStiledVisual.h"
#include <base/exceptions/LogicException.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_SmartItem.h>
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <graphics/visual/drawable/PseudoVisualData.h>
#include <base/math/Math.h>
#include <base/ease/Ease.h>
#include <base/util/StringUtil.h>
#include <base/MM.h>

ComposedStiledVisual::ComposedStiledVisual(IAppMetrics* appMetrics, IAppAssets* appAssets, ArrayList<MenuItemConfig*>* itemConfigs)
    : super(appMetrics),
      appAssets(appAssets)
{
	if (itemConfigs == nullptr) {
		throw LogicException(LOC);
	}

	this->menuItemConfigs = itemConfigs;
}

/*asdZ;
protected static MenuItemConfig[] concatItemConfigLists(MenuItemConfig[] ...configLists) {
    int itemsCount = 0;
    for (MenuItemConfig[] arrEntry: configLists) {
        itemsCount += arrEntry.length;
    }

    MenuItemConfig[] ret = new MenuItemConfig[itemsCount];
    int index = 0;
    for (MenuItemConfig[] arrEntry: configLists) {
        for(int j=0;j<arrEntry.length;j++) {
            MenuItemConfig entry = arrEntry[j];

            ret[index] = entry;
            index++;
        }
    }

    return ret;
}*/

IContainer2D* ComposedStiledVisual::getVisualsHolder() {
    return visualsHolder;
}

void ComposedStiledVisual::createContent() {
    super::createContent();

    getSubItemsMap()->putDirectB("visualsHolder", visualsHolder = VisualUtil2D::newGroup(nullptr));
    
    // In case the visual is a toggle_switch or another smart component, the visuals are created using the parent logic.
    if(!(dynamic_cast<IDynamicPropertiesObject2D*>(this)) || getSubItemsMap()->getPointerB("parent") == nullptr/*manually handled smart item*/) {
        // This is a view or a toggle_switch or another smart component without a parent (no parent means it is manually created).
        createVisuals(createConfigMap);

        syncVisuals(true, false, "", "", 0);
    }
}

void ComposedStiledVisual::doFullVisualsSync() {
    syncVisuals(true, true, "", "", 0);
}

void ComposedStiledVisual::createVisuals(Map1D<std::string, void*>* createConfigMap) {
    createVisualItems(this, this, menuItemConfigs, createConfigMap, &smartItemsList);
}

void ComposedStiledVisual::createVisualItems(IComposedVisual* itemsMap, IComposedVisual* itemsMapLayer, ArrayList<MenuItemConfig*>* menuItemConfigs, IMap1D<std::string, void*>* createConfigMap, ArrayList<IComposedVisual*>* smartItemsList) {
    if(menuItemConfigs == nullptr) {
        throw LogicException(LOC);
    }

    for(int i = 0; i< menuItemConfigs->size(); i++) {
        MenuItemConfig* itemConfig = menuItemConfigs->getDirect(i);

        if(itemConfig == nullptr) {
            throw LogicException(LOC);
        }

        if(itemsMapLayer->getLocalItem(itemConfig->itemID) != nullptr) {
            throw LogicException(LOC);
        }

        bool isItemAddedToMap = false;
        IGenericDrawable* item = nullptr;

        if(dynamic_cast<MenuItemConfig_SmartItem*>(itemConfig)) {
            MenuItemConfig_SmartItem* cItemConfig = dynamic_cast<MenuItemConfig_SmartItem*>(itemConfig);

            ComposedStiledVisual* newSmartItem;
            newSmartItem = cItemConfig->classContructor(appMetrics, appAssets);
            if(newSmartItem == nullptr) {
                throw LogicException(LOC);
            }

            newSmartItem->setLocalItem("parent", itemsMapLayer);

            item = newSmartItem;

            newSmartItem->reservedCreate();

            if(cItemConfig->parentVisualID != nullptr) {
				IGenericDrawable* menuItem = getMenuItemOptional(itemsMap, itemsMapLayer, *cItemConfig->parentVisualID, 0);
				if (menuItem == nullptr) {
					throw LogicException(LOC);
				}

				IContainer2D* parentContainer = dynamic_cast<IContainer2D*>(menuItem);
                if(parentContainer == nullptr) {
                    throw LogicException(LOC);
                }

                parentContainer->addChild(newSmartItem->visualsHolder);
            }
        }else if(dynamic_cast<MenuItemConfig_Data*>(itemConfig)) {
            MenuItemConfig_Data* cItemConfig = dynamic_cast<MenuItemConfig_Data*>(itemConfig);
            (void)cItemConfig;

            item = new PseudoVisualData();
        }else if(dynamic_cast<MenuItemConfig_Visual*>(itemConfig)) {
            MenuItemConfig_Visual* cItemConfig = dynamic_cast<MenuItemConfig_Visual*>(itemConfig);

            IContainer2D* parentContainer = nullptr;
			if (cItemConfig->parentComposedID != nullptr) {
                parentContainer = dynamic_cast<IContainer2D*>(getMenuItemOptional(itemsMap, itemsMapLayer, *cItemConfig->parentComposedID, 0));
                if(parentContainer == nullptr) {
                    throw LogicException(LOC);
                }
            }

            if(cItemConfig->visualCreateConfig != nullptr) {
                item = VisualUtil2D::newSmartVisual(appAssets, cItemConfig->visualCreateConfig, parentContainer, createConfigMap);
                if (item == nullptr) {
                    throw LogicException(LOC);
                }
            }else {
                // The item is optional will be nullptr, don't add it to the items map using the default method.

                isItemAddedToMap = true;
            }
        }else if(dynamic_cast<MenuItemConfig_ComposedVisual*>(itemConfig)) {
            IMenuItemConfig_ComposedVisual* cItemConfig = dynamic_cast<MenuItemConfig_ComposedVisual*>(itemConfig);

            IComposedVisual* subItemsMapLayer = new ComposedVisual(appMetrics);
            subItemsMapLayer->setLocalItem("parent", itemsMapLayer);
            item = subItemsMapLayer;

            isItemAddedToMap = true;
            itemsMapLayer->setLocalItem(itemConfig->itemID, item);
            visualItemCreated(item, itemConfig, itemsMap, itemsMapLayer);

            subItemsMapLayer->reservedCreate();

            createVisualItems(itemsMap, subItemsMapLayer, cItemConfig->getMenuItemConfigs(), createConfigMap, smartItemsList);
        }else {
            throw LogicException(LOC);
        }

        // Add item to smartItemsList if it is instance of ISmartItem.
        if(!isItemAddedToMap) {
            if(item == nullptr) {
                throw LogicException(LOC);
            }

            itemsMapLayer->setLocalItem(itemConfig->itemID, item);
            visualItemCreated(item, itemConfig, itemsMap, itemsMapLayer);
        }
    }
}

void ComposedStiledVisual::visualItemCreated(IGenericDrawable* item, MenuItemConfig* itemConfig, IComposedVisual* itemsMap, IComposedVisual* itemsMapLayer) {
    if(dynamic_cast<IDynamicPropertiesObject2D*>(item) && dynamic_cast<IMenuItemConfig_DynamicProperties*>(itemConfig)) {
        syncVisualItem(
            dynamic_cast<IDynamicPropertiesObject2D*>(item), (dynamic_cast<IMenuItemConfig_DynamicProperties*>(itemConfig))->getItemProperties(),
            true, false,
            nullptr, nullptr,
            0.0f,
            itemsMap, itemsMapLayer,
            getDataMap(),
            appMetrics
        );
    }

    if(dynamic_cast<ComposedStiledVisual*>(item) && dynamic_cast<IDynamicPropertiesObject2D*>(item)) {
        (dynamic_cast<ComposedStiledVisual*>(item))->createVisuals(createConfigMap);
    }

    if(dynamic_cast<ComposedStiledVisual*>(item)) {
        (dynamic_cast<ComposedStiledVisual*>(item))->doFullVisualsSync();
    }

    if(dynamic_cast<ComposedStiledVisual*>(item) && dynamic_cast<MenuItemConfig_SmartItem*>(itemConfig)) {
        MenuItemConfig_SmartItem* cItemConfig = dynamic_cast<MenuItemConfig_SmartItem*>(itemConfig);

        if(cItemConfig->initialBaseStateId != "") {
            std::string newState = (dynamic_cast<AnimatedComposedStiledVisual*>(item))->computeStateFlags(cItemConfig->initialBaseStateId);
            (dynamic_cast<AnimatedComposedStiledVisual*>(item))->gotoShowState(newState, false, nullptr, false);
        }
    }
}

void ComposedStiledVisual::syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) {
    if(visualsHolder == nullptr) {
        // Visuals are not created yet.

        throw LogicException(LOC);
    }

    syncMenuItems(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent, this, this, menuItemConfigs, getDataMap(), appMetrics);
}

void ComposedStiledVisual::syncMenuItems(
    bool syncAbsoluteValues, bool syncAnimatingValues,
    std::string startStateID, std::string endStateID, float stateCompletionPercent,
    ComposedVisual* itemsMap, ComposedVisual* itemsMapLayer,
    ArrayList<MenuItemConfig*>* menuItemConfigs,
    IMap1D<std::string, std::shared_ptr<base::IWrappedValue>>* dataMap,
    IAppMetrics* appMetrics
) {
    for (int i = 0; i < menuItemConfigs->size(); i++) {
        MenuItemConfig* itemConfig = menuItemConfigs->getDirect(i);

        if(dynamic_cast<MenuItemConfig_Visual*>(itemConfig)) {
            MenuItemConfig_Visual* cItemConfig = dynamic_cast<MenuItemConfig_Visual*>(itemConfig);

            IDrawableNode2D* visual2D = dynamic_cast<IDrawableNode2D*>(itemsMapLayer->getSubItemsMap()->getDirect(cItemConfig->itemID, nullptr));
            if(visual2D == nullptr && cItemConfig->visualCreateConfig == nullptr) {
                // Visual is optional and it is currently not created.

                continue;
            }
        }

        if(dynamic_cast<IMenuItemConfig_ComposedVisual*>(itemConfig)) {
            IMenuItemConfig_ComposedVisual* itemConfig_local = dynamic_cast<IMenuItemConfig_ComposedVisual*>(itemConfig);
            ComposedVisual* subItemsMapLayer = dynamic_cast<ComposedVisual*>(itemsMapLayer->getSubItemsMap()->getDirect(itemConfig->itemID));
            if(subItemsMapLayer == nullptr) {
                throw LogicException(LOC);
            }

            syncMenuItems(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent, itemsMap, subItemsMapLayer, itemConfig_local->getMenuItemConfigs(), dataMap, appMetrics);
        }

        IGenericDrawable* item = itemsMapLayer->getSubItemsMap()->getDirect(itemConfig->itemID);

        if(dynamic_cast<IMenuItemConfig_DynamicProperties*>(itemConfig)) {
            IMenuItemConfig_DynamicProperties* itemConfigLocal = dynamic_cast<IMenuItemConfig_DynamicProperties*>(itemConfig);

            IDynamicPropertiesObject2D* localItem = dynamic_cast<IDynamicPropertiesObject2D*>(item);
            if(localItem == nullptr) {
                throw LogicException(LOC);
            }
            syncVisualItem(localItem, itemConfigLocal->getItemProperties(), syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent, itemsMap, itemsMapLayer, dataMap, appMetrics);
        }
    }
}

void ComposedStiledVisual::syncVisualItem(
    IDynamicPropertiesObject2D* visual2D, VisualPropertiesConfig* itemProperties,
    bool syncAbsoluteValues, bool syncAnimatingValues,
    std::string startStateID, std::string endStateID,
    float stateCompletionPercent,
    IComposedVisual* itemsMap, IComposedVisual* itemsMapLayer,
    IMap1D<std::string, std::shared_ptr<base::IWrappedValue>>* dataMap,
    IAppMetrics* appMetrics
) {
    for(int i=0;i<itemProperties->propertyConfigs.size();i++) {
        VisualPropertyValueConfig* propertyValue = itemProperties->propertyConfigs.getDirect(i);

        std::shared_ptr<base::IWrappedValue> value = nullptr;
        bool doUpdate = false;
        if(propertyValue->smartValue == nullptr) {
            throw LogicException(LOC);
        }
        if(propertyValue->smartValue->baseValue != nullptr && propertyValue->smartValue->baseValue->checkType(base::IWrappedValue::Type::T_MenuVisualAnimationProperty_ptr)) {
            if (syncAnimatingValues) {
                doUpdate = true;

                MenuVisualAnimationProperty* animationProperty = propertyValue->smartValue->baseValue->getDirectAs_MenuVisualAnimationProperty_ptr();

                MenuVisualAnimationProperty::StateComposedValue* composedStartValue = getValueWherePropertyHasFragment(startStateID, &animationProperty->stateValuesList);

                MenuVisualAnimationProperty::StateComposedValue* composedEndValue = getValueWherePropertyHasFragment(endStateID, &animationProperty->stateValuesList);

                if (composedStartValue == nullptr && composedEndValue == nullptr) {
                    continue;
                }

                if (composedStartValue == nullptr && composedEndValue != nullptr) {
                    composedStartValue = composedEndValue;
                } else if (composedEndValue == nullptr && composedStartValue != nullptr) {
                    composedEndValue = composedStartValue;
                }

				float animationPercentValue;
				if (composedEndValue->easeConfig != nullptr) {
					animationPercentValue = Ease::computeEaseValue(
						stateCompletionPercent,
						*composedEndValue->easeConfig
					);
				} else {
					animationPercentValue = stateCompletionPercent;
				}

                std::shared_ptr<base::IWrappedValue> processedStartValue = computeSmartValue(appMetrics, itemsMap, itemsMapLayer, composedStartValue->smartValue, visual2D, nullptr, dataMap);
                std::shared_ptr<base::IWrappedValue> processedEndValue = computeSmartValue(appMetrics, itemsMap, itemsMapLayer, composedEndValue->smartValue, visual2D, nullptr, dataMap);

                value = interpolateSmartValues(
                    processedStartValue, processedEndValue,
                    animationPercentValue
                );

                if(!visual2D->isInit_StaticAnimatingPropertyStatus(propertyValue->property)) {
                    visual2D->putStaticAnimatingPropertyStatus(propertyValue->property, true);
                }
            }
        }else {
            if(!syncAbsoluteValues && !visual2D->isInit_StaticAnimatingPropertyStatus(propertyValue->property)) {
                throw LogicException(LOC);
            }

            if(syncAbsoluteValues || (syncAnimatingValues && visual2D->isTrue_StaticAnimatingPropertyStatus(propertyValue->property))) {
                doUpdate = true;

                ComputeSmartValuePoolData* poolData = new ComputeSmartValuePoolData();
                value = computeSmartValue(appMetrics, itemsMap, itemsMapLayer, propertyValue->smartValue, visual2D, poolData, dataMap);

                if(!visual2D->isInit_StaticAnimatingPropertyStatus(propertyValue->property)) {
                    if(!poolData->isInit_isPropDependingOnAnimating()) {
                        // This error may occur because the property name is not correct, check for typos.

                        throw LogicException(LOC);
                    }

                    visual2D->putStaticAnimatingPropertyStatus(propertyValue->property, poolData->isTrue_isPropDependingOnAnimating());
                }
            }
        }

        if(doUpdate) {
            visual2D->setPropertyValue(propertyValue->property, value);
        }
    }
}

std::shared_ptr<base::IWrappedValue> ComposedStiledVisual::interpolateSmartValues(
    std::shared_ptr<base::IWrappedValue>& startValue, std::shared_ptr<base::IWrappedValue>& endValue,
    float animationPercentValue)
{
    std::shared_ptr<base::IWrappedValue> value;

    if (startValue->checkType(base::IWrappedValue::Type::T_float) && endValue->checkType(base::IWrappedValue::Type::T_float)) {
        float valS = startValue->getDirectAs_float();
        float valE = endValue->getDirectAs_float();

        value = base::IWrappedValue::new_float(valS + (valE - valS) * animationPercentValue);
    } else if (startValue->checkType(base::IWrappedValue::Type::T_bool) && endValue->checkType(base::IWrappedValue::Type::T_bool)) {
        value = endValue;
    } else if (startValue->checkType(base::IWrappedValue::Type::T_Color) && endValue->checkType(base::IWrappedValue::Type::T_Color)) {
        Color& colStart = startValue->getReferenceAs_Color();
        Color& colEnd = endValue->getReferenceAs_Color();

        value = base::IWrappedValue::new_ColorDirect(
            Color(
                colStart.r + (colEnd.r - colStart.r) * animationPercentValue,
                colStart.g + (colEnd.g - colStart.g) * animationPercentValue,
                colStart.b + (colEnd.b - colStart.b) * animationPercentValue,
                colStart.a + (colEnd.a - colStart.a) * animationPercentValue
            )
        );
    }/*asdB;// implement when needed.
    else if (startValue instanceof IndexedFontData && endValue instanceof IndexedFontData) {
        IndexedFontData fDataStart = (IndexedFontData)startValue;
        IndexedFontData fDataEnd = (IndexedFontData)endValue;

        value = new IndexedFontsInterpolationData(
            fDataStart.fontIndex,
            fDataEnd.fontIndex,
            animationPercentValue
        );
    }*/ else {
        throw LogicException(LOC);
    }

    return value;
}

bool ComposedStiledVisual::checkIsStatePassed(std::string& stateID, UiId* composedStateIDFragment) {
    bool isStateCheckPassed = true;
    for(int j=0;j<composedStateIDFragment->getSplitId()->size();j++) {
        std::string& entryStateID = composedStateIDFragment->getSplitId()->getReference(j);

        if(!StringUtil::contains(stateID, entryStateID)) {
            isStateCheckPassed = false;

            break;
        }
    }

    return isStateCheckPassed;
}

MenuVisualAnimationProperty::StateComposedValue* ComposedStiledVisual::getValueWherePropertyHasFragment(std::string& menuStateID, ArrayList<MenuVisualAnimationProperty::StateComposedValue*>* stateValuesList) {
    MenuVisualAnimationProperty::StateComposedValue* composedStartValue = nullptr;

    if(stateValuesList != nullptr) {
        for(int i=0;i<stateValuesList->size();i++) {
            MenuVisualAnimationProperty::StateComposedValue* valueEntry = stateValuesList->getDirect(i);

            bool isStateCheckPassed = checkIsStatePassed(menuStateID, valueEntry->stateIDFragments);
            if(isStateCheckPassed) {
                composedStartValue = valueEntry;

                break;
            }
        }
    }

    return composedStartValue;
}

// Helper function for external animations managers (eg: Particle effects animation).
std::shared_ptr<base::IWrappedValue> ComposedStiledVisual::computeTopLevelSmartValue(VisualValueConfig* smartValue, IDynamicPropertiesObject2D* selfObject, IMap1D<std::string, std::shared_ptr<base::IWrappedValue>>* dataMap) {
    std::shared_ptr<base::IWrappedValue> ret = computeSmartValue(
        appMetrics, this, this,
        smartValue, selfObject, nullptr, dataMap
    );

    return ret;
}

std::shared_ptr<base::IWrappedValue> ComposedStiledVisual::computeSmartValue(IAppMetrics* appMetrics, IComposedVisual* itemsMap, IComposedVisual* itemsMapLayer, VisualValueConfig* smartValue, IDynamicPropertiesObject2D* selfObject, ComputeSmartValuePoolData* poolData, IMap1D<std::string, std::shared_ptr<base::IWrappedValue>>* dataMap) {
    std::shared_ptr<base::IWrappedValue> value;
	
    if(smartValue->baseMultiplierSourceID != nullptr) {
        // (smartValue.baseMultiplierProperty) May be nullptr.

        std::shared_ptr<base::IWrappedValue> multiplier = getVisualPropertyValue(itemsMap, itemsMapLayer, *smartValue->baseMultiplierSourceID, *smartValue->baseMultiplierProperty, selfObject, poolData, dataMap, appMetrics);
        if(multiplier == nullptr) {
            throw LogicException(LOC);
        }

        if(multiplier->checkType(base::IWrappedValue::Type::T_float)) {
            if(smartValue->baseValue->checkType(base::IWrappedValue::Type::T_VisualValueConfig_ptr)) {
				std::shared_ptr<base::IWrappedValue> vTmp = computeSmartValue(appMetrics, itemsMap, itemsMapLayer, smartValue->baseValue->getDirectAs_VisualValueConfig_ptr(), selfObject, poolData, dataMap);
				value = base::IWrappedValue::new_float(vTmp->getDirectAs_float() * multiplier->getDirectAs_float());
            }else if(smartValue->baseValue->checkType(base::IWrappedValue::Type::T_float)) {
				value = base::IWrappedValue::new_float(smartValue->baseValue->getDirectAs_float() * multiplier->getDirectAs_float());
			}else if(smartValue->baseValue->checkType(base::IWrappedValue::Type::T_int)) {
				value = base::IWrappedValue::new_int(Math::floor(smartValue->baseValue->getDirectAs_int() * multiplier->getDirectAs_float()));
			} else if(smartValue->baseValue->checkType(base::IWrappedValue::Type::T_float_sharedPtr)) {
				if (smartValue->baseValue->getDirectAs_float_sharedPtr() != nullptr) {
					float vTmp = *smartValue->baseValue->getDirectAs_float_sharedPtr();
					value = base::IWrappedValue::new_float_sharedPtr(std::make_shared<float>(vTmp * multiplier->getDirectAs_float()));
				} else {
					value = nullptr;
				}
			} else {
				throw LogicException(LOC);
			}
        }else {
			// Currently the value must be nullptr if the multiplier is not a float (eg: Color);
            if(smartValue->baseValue != nullptr) {
                throw LogicException(LOC);
            }

            value = multiplier;
        }
    }else {
        if(smartValue->baseValue->checkType(base::IWrappedValue::Type::T_VisualValueConfig_ptr)) {
            value = computeSmartValue(appMetrics, itemsMap, itemsMapLayer, smartValue->baseValue->getDirectAs_VisualValueConfig_ptr(), selfObject, poolData, dataMap);
        }else {
            value = smartValue->baseValue;
        }

        if(poolData != nullptr && (!poolData->isInit_isPropDependingOnAnimating() || !poolData->isTrue_isPropDependingOnAnimating())) {
            poolData->set_isPropDependsOnAnimating(false);
        }
    }

    if(smartValue->secondarySmartValue != nullptr) {
        std::shared_ptr<base::IWrappedValue> secondaryValue = computeSmartValue(appMetrics, itemsMap, itemsMapLayer, smartValue->secondarySmartValue, selfObject, poolData, dataMap);

        float valueF;
        if(smartValue->operationType == VisualValueConfig::OPERATION_ADD) {
            valueF = value->getDirectAs_float() + secondaryValue->getDirectAs_float();
        }else if(smartValue->operationType == VisualValueConfig::OPERATION_SUBTRACT) {
            valueF = value->getDirectAs_float() - secondaryValue->getDirectAs_float();
        }else if(smartValue->operationType == VisualValueConfig::OPERATION_MULTIPLY) {
            valueF = value->getDirectAs_float() * secondaryValue->getDirectAs_float();
        }else if(smartValue->operationType == VisualValueConfig::OPERATION_DIVIDE) {
            valueF = value->getDirectAs_float() / secondaryValue->getDirectAs_float();
        }else if(smartValue->operationType == VisualValueConfig::OPERATION_MIN) {
            valueF = Math::min(value->getDirectAs_float(), secondaryValue->getDirectAs_float());
        }else if(smartValue->operationType == VisualValueConfig::OPERATION_MAX) {
            valueF = Math::max(value->getDirectAs_float(), secondaryValue->getDirectAs_float());
        }else {
            throw LogicException(LOC);
        }
			
        value = base::IWrappedValue::new_float(valueF);
    }else {
        if (smartValue->operationType == VisualValueConfig::OPERATION_NONE) {
            //void
        } else {
            float valueF;
            if (smartValue->operationType == VisualValueConfig::OPERATION_ROUND) {
                valueF = Math::round(value->getDirectAs_float());
            } else if (smartValue->operationType == VisualValueConfig::OPERATION_FLOOR) {
                valueF = Math::floor(value->getDirectAs_float());
            } else if (smartValue->operationType == VisualValueConfig::OPERATION_CEIL) {
                valueF = Math::ceil(value->getDirectAs_float());
            } else {
                throw LogicException(LOC);
            }

            value = base::IWrappedValue::new_float(valueF);
        }
    }

    return value;
}

void ComposedStiledVisual::disposeMain() {
    if(getVisualsHolder()->getParent() != nullptr) {
        getVisualsHolder()->getParent()->removeChild(getVisualsHolder());
    }

    super::disposeMain();
}

ComposedStiledVisual::~ComposedStiledVisual() {
    //void
}
