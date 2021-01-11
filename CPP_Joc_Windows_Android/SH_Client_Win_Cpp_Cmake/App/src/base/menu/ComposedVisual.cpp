#include "ComposedVisual.h"
#include <base/exceptions/LogicException.h>
#include <graphics/visual2d/drawable/IDrawableNode2D.h>
#include <graphics/visual/drawable/PseudoVisualData.h>
#include <graphics/visual2d/drawable/IContainer2D.h>
#include <base/app/IAppMetrics.h>
#include <base/component/IComponent_UIView.h>

ComposedVisual::ComposedVisual(IAppMetrics* appMetrics)
    : super(IComponent_UIView::COMPONENT_CATEGORY),
    appMetrics(appMetrics)
{
    //void
}

void ComposedVisual::setCreateConfigMap(Map1D<std::string, void*>* createConfigMap) {
    this->createConfigMap = createConfigMap;
}

void ComposedVisual::create() {
    super::create();

    createFramework();
    createContent();
}

void ComposedVisual::createFramework() {
    //void
}

void ComposedVisual::createContent() {
    //void
}

IMap1D<std::string, std::shared_ptr<base::IWrappedValue>>* ComposedVisual::getDataMap() {
    return &dataMap;
}

void ComposedVisual::putDataMapValue(std::string& key, std::shared_ptr<base::IWrappedValue> val, bool doAnimatingVisualsSync) {
    dataMap.putReference(key, val);
}

void ComposedVisual::putDataMapValueB(std::string key, std::shared_ptr<base::IWrappedValue> val, bool doAnimatingVisualsSync) {
	putDataMapValue(key, val, doAnimatingVisualsSync);
}

void ComposedVisual::addDataMap(Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* dataMap) {
    this->dataMap.addMap(dataMap);
}

void ComposedVisual::recycleItem(IGenericDrawable* item, ArrayList<IComposedVisual*>* smartItemsList) {
    IDrawableNode2D* cEntry_dn2d;
    IComposedVisual* cEntry_cv;
    PseudoVisualData* cEntry_pvd;
    if ((cEntry_dn2d = dynamic_cast<IDrawableNode2D*>(item))) {
        if (cEntry_dn2d->getParent() != nullptr) {
            cEntry_dn2d->getParent()->removeChild(cEntry_dn2d);
        }
    } else if((cEntry_cv = dynamic_cast<IComposedVisual*>(item))){
        //void
    }else if((cEntry_pvd = dynamic_cast<PseudoVisualData*>(item))) {
        //void
    }else {
        throw LogicException(LOC);
    }
}

IGenericDrawable* ComposedVisual::getMenuItemOptional(
        IComposedVisual* itemsMapRoot, IComposedVisual* itemsMapLayer,
        UiId& composedId, int currentItemIDIndex)
{
    if(itemsMapRoot == nullptr || itemsMapLayer == nullptr) {
        throw LogicException(LOC);
    }

    int _currentItemIDIndex = currentItemIDIndex;
    std::string& currentIDLayer = composedId.getSplitId()->getReference(_currentItemIDIndex);
    _currentItemIDIndex++;

    IComposedVisual* _itemsMapLayer = nullptr;
    if(currentIDLayer == "_local") {
        _itemsMapLayer = itemsMapLayer;
    }else if(currentIDLayer == "_root") {
        _itemsMapLayer = itemsMapRoot;
    }else if(_currentItemIDIndex == 1) {
        // The first item fxId listEntry must be a modifier.

        throw LogicException(LOC);
    }else {
        IGenericDrawable* entry = itemsMapLayer->getLocalItem(currentIDLayer);
        if (entry == nullptr) {
            return nullptr;
        }

        if(_currentItemIDIndex < composedId.getSplitId()->size()) {
            // This is not the last listEntry, a Map object is expected.
            _itemsMapLayer = dynamic_cast<IComposedVisual*>(entry);
            if (!_itemsMapLayer) {
                throw LogicException(LOC);
            }
        }else {
            return entry;
        }
    }

    if(composedId.getSplitId()->size() <= _currentItemIDIndex) {
        return _itemsMapLayer;
    }

    return getMenuItemOptional(
            itemsMapRoot, _itemsMapLayer,
            composedId, _currentItemIDIndex);
}

std::shared_ptr<base::IWrappedValue> ComposedVisual::getVisualPropertyValue(IComposedVisual* itemsMap, IComposedVisual* itemsMapLayer, UiId& itemComposedID, std::string& property, IDynamicPropertiesObject2D* selfObject, ComputeSmartValuePoolData* poolData, IMap1D<std::string, std::shared_ptr<base::IWrappedValue>>* dataMap, IAppMetrics* appMetrics) {
    IDynamicPropertiesObject2D* visualItem = nullptr;

    std::string firstIDLayer = itemComposedID.getSplitId()->getDirect(0);
    if(firstIDLayer == "_appSize") {
        if(poolData != nullptr && (!poolData->isInit_isPropDependingOnAnimating() || poolData->isTrue_isPropDependingOnAnimating() != true)) {
            poolData->set_isPropDependsOnAnimating(false);
        }

        if(property == "width") {
            return base::IWrappedValue::new_float(appMetrics->getScreen()->getWindowResolutionX());
        }else if(property == "height") {
            return base::IWrappedValue::new_float(appMetrics->getScreen()->getWindowResolutionY());
        }else {
            throw LogicException(LOC);
        }
    }else if(firstIDLayer == "_dpm") {
        if(poolData != nullptr && (!poolData->isInit_isPropDependingOnAnimating() || poolData->isTrue_isPropDependingOnAnimating() != true)) {
            poolData->set_isPropDependsOnAnimating(false);
        }

        if(property == "width") {
            return base::IWrappedValue::new_float(appMetrics->getScreen()->getScreenDensityMultiplier()->x);
        }else if(property == "height") {
            return base::IWrappedValue::new_float(appMetrics->getScreen()->getScreenDensityMultiplier()->y);
        }else {
            throw LogicException(LOC);
        }
    }else if(firstIDLayer == "_dpm_font") {
        if (poolData != nullptr && (!poolData->isInit_isPropDependingOnAnimating() || poolData->isTrue_isPropDependingOnAnimating() != true)) {
            poolData->set_isPropDependsOnAnimating(false);
        }
        
        return base::IWrappedValue::new_float(appMetrics->getScreen()->getScreenDensityMultiplier_fonts());
    }else if(firstIDLayer == "_tSize") {
        if(poolData != nullptr && (!poolData->isInit_isPropDependingOnAnimating() || poolData->isTrue_isPropDependingOnAnimating() != true)) {
            poolData->set_isPropDependsOnAnimating(false);
        }

        if(property == "width") {
            return base::IWrappedValue::new_float(appMetrics->getScreen()->getWindowResolutionX() / appMetrics->getScreen()->getTemplateSize()->x);
        }else if(property == "height") {
            return base::IWrappedValue::new_float(appMetrics->getScreen()->getWindowResolutionY() / appMetrics->getScreen()->getTemplateSize()->y);
        }else {
            throw LogicException(LOC);
        }
    }else if(firstIDLayer == "_self") {
        if(selfObject == nullptr) {
            throw LogicException(LOC);
        }

        visualItem = selfObject;
    }else if(firstIDLayer == "_dataMap") {
        if(poolData != nullptr && (!poolData->isInit_isPropDependingOnAnimating() || poolData->isTrue_isPropDependingOnAnimating() != true)) {
            poolData->set_isPropDependsOnAnimating(true);
        }

        if(dataMap == nullptr) {
            throw LogicException(LOC);
        }

        return dataMap->getDirect(property);
    }else if(firstIDLayer == "_standardIncrement") {
        if(poolData != nullptr && (!poolData->isInit_isPropDependingOnAnimating() || poolData->isTrue_isPropDependingOnAnimating() != true)) {
            poolData->set_isPropDependsOnAnimating(false);
        }

        return base::IWrappedValue::new_float(appMetrics->getScreen()->getStandardIncrement());
    }else {
        visualItem = dynamic_cast<IDynamicPropertiesObject2D*>(getMenuItemOptional(itemsMap, itemsMapLayer, itemComposedID, 0));
        if(visualItem == nullptr) {
            // Check these values when a error occurs. This ussually occurs when attempting to access a object that doesn't exist.
            std::string s0 = itemComposedID.getSplitId()->getReference(0);
            std::string s1 = itemComposedID.getSplitId()->getReference(1);

            throw LogicException(LOC);
        }
    }

    if(visualItem == nullptr) {
        throw LogicException(LOC);
    }

    std::shared_ptr<base::IWrappedValue> ret;

    if(poolData != nullptr && (!poolData->isInit_isPropDependingOnAnimating() || poolData->isTrue_isPropDependingOnAnimating() != true)) {
        if (!visualItem->isInit_StaticAnimatingPropertyStatus(property)) {
            throw LogicException(LOC);
        }

        bool b = visualItem->isTrue_StaticAnimatingPropertyStatus(property);
        poolData->set_isPropDependsOnAnimating(b);
    }
    ret = visualItem->getPropertyValue(property);

    return ret;
}

void ComposedVisual::setItem(UiId& composedId, IGenericDrawable* item) {
    setMenuItem(
        this, this,
        composedId, 0, item);
}

void ComposedVisual::setItem(std::string& composedId, IGenericDrawable* item) {
    UiId* id = UiId::newIdFromComposedId(composedId, "_root");

    setItem(*id, item);

    delete id;
}

void ComposedVisual::setItemB(std::string composedId, IGenericDrawable* item) {
	setItem(composedId, item);
}

void ComposedVisual::setMenuItem(
        IComposedVisual* itemsMapRoot, IComposedVisual* itemsMapLayer,
        UiId& composedId, int currentItemIDIndex, IGenericDrawable* item)
{
    UiId subComposedId(composedId, 0, composedId.getSplitId()->size() - 1);

    std::string& itemID = composedId.getSplitId()->getReference(composedId.getSplitId()->size() - 1);

    IComposedVisual* itemLayer = dynamic_cast<IComposedVisual*>(getMenuItemOptional(
            itemsMapRoot, itemsMapLayer,
            subComposedId, currentItemIDIndex));

    if(itemLayer == nullptr) {
        throw LogicException(LOC);
    }

    itemLayer->setLocalItem(itemID, item);
}

IGenericDrawable* ComposedVisual::getLocalItem(std::string itemIDFragment) {
    return subItemsMap.getDirect(itemIDFragment, nullptr);
}

void ComposedVisual::setLocalItem(std::string itemIDFragment, IGenericDrawable* item) {
    IGenericDrawable* existentItem = subItemsMap.getDirect(itemIDFragment, nullptr);
    if(existentItem == nullptr && item == nullptr) {
        return ;
    }

    if(existentItem != nullptr && item != nullptr) {
        throw LogicException(LOC);
    }

    if(existentItem != nullptr) {
        subItemsMap.putDirect(itemIDFragment, nullptr);

        IComposedVisual* existentItem_icv;
        if((existentItem_icv = dynamic_cast<IComposedVisual*>(existentItem))) {
            if(smartItemsList.indexOf(existentItem_icv) < 0) {
                throw LogicException(LOC);
            }

            smartItemsList.remove(existentItem_icv);
        }
    }

    if(item != nullptr) {
        subItemsMap.putDirect(itemIDFragment, item);

        IComposedVisual* item_icv;
        if(itemIDFragment != "parent" && (item_icv = dynamic_cast<IComposedVisual*>(item))) {
            if(smartItemsList.indexOf(item_icv) >= 0) {
                throw LogicException(LOC);
            }

            smartItemsList.appendDirect(dynamic_cast<IComposedVisual *>(item));
        }
    }
}

void ComposedVisual::disposeItemsMap(IComposedVisual* entriesMap, ArrayList<IComposedVisual*>* smartItemsList) {
    for (std::unordered_map<std::string, IGenericDrawable*>::iterator it = entriesMap->getSubItemsMap()->map.begin(); it != entriesMap->getSubItemsMap()->map.end(); ++it) {
        if(it->second != nullptr && it->first != "parent") {
            IGenericDrawable* item = it->second;

            IComposedVisual* item_icv;
            if((item_icv = dynamic_cast<IComposedVisual*>(item))) {
                item_icv->reservedDisposeMain();

                smartItemsList->remove(item_icv);
            }

            recycleItem(item, smartItemsList);
        }
    }
}

void ComposedVisual::visualLoop(float deltaS, int deltaMS) {
    // Call app loop on items that use it and implement IAppLoopItem.
	// Call children first even tough this can set their properties because this may use values that are already updated in the children.
	// visualLoopB will ensure the children won't have any update delay.
    for(int i=0;i<smartItemsList.size();i++) {
        IComposedVisual* entry = smartItemsList.getDirect(i);
        if(entry != nullptr) {
            entry->visualLoop(deltaS, deltaMS);
        }
    }

    for (ListDL<IComposedVisual*>::Entry* entry = manualSmartItemsList.getFirst(); entry != nullptr; entry = entry->next) {
        IComposedVisual* entryData = entry->data;

        entryData->visualLoop(deltaS, deltaMS);
    }
}

void ComposedVisual::visualLoopB(float deltaS, int deltaMS) {
    // Call app loop on items that use it and implement IAppLoopItem.
	// Call children first even tough this can set their properties because this may use values that are already updated in the children.
	// The children may be updated 2 times in certain cases (eg. window size change) and have a 1 frame update delay but that is ok.
    for(int i=0;i<smartItemsList.size();i++) {
        IComposedVisual* entry = smartItemsList.getDirect(i);
        if(entry != nullptr) {
            entry->visualLoopB(deltaS, deltaMS);
        }
    }

    for (ListDL<IComposedVisual*>::Entry* entry = manualSmartItemsList.getFirst(); entry != nullptr; entry = entry->next) {
        IComposedVisual* entryData = entry->data;

        entryData->visualLoopB(deltaS, deltaMS);
    }
}

bool ComposedVisual::isInit_StaticAnimatingPropertyStatus(std::string& property) {
	if (staticAnimatingPropertiesMap.getDirect(property, -1) != -1) {
		return true;
	}

	return false;
}

bool ComposedVisual::isTrue_StaticAnimatingPropertyStatus(std::string& property) {
	int v = staticAnimatingPropertiesMap.getDirect(property, -1);

	if (v == 1) {
		return true;
	} else if (v == 0) {
		return false;
	}

	throw LogicException(LOC);
}

bool ComposedVisual::isTrue_StaticAnimatingPropertyStatusDirect(std::string property) {
	return isTrue_StaticAnimatingPropertyStatus(property);
}

void ComposedVisual::putStaticAnimatingPropertyStatus(std::string& property, int status) {
	staticAnimatingPropertiesMap.putDirect(property, status);
}

void ComposedVisual::putStaticAnimatingPropertyStatusDirect(std::string property, int status) {
	putStaticAnimatingPropertyStatus(property, status);
}

std::shared_ptr<base::IWrappedValue> ComposedVisual::getItemPropertyValue(UiId& composedId, std::string& property, ComputeSmartValuePoolData* poolData) {
    return getVisualPropertyValue(this, this, composedId, property, nullptr, poolData, &dataMap, appMetrics);
}

std::shared_ptr<base::IWrappedValue> ComposedVisual::getItemPropertyValue(std::string smartItemIdAndProperty, ComputeSmartValuePoolData* poolData) {
    UiId cId{smartItemIdAndProperty, "_root", true};
    std::string prop = UiId::extractPropStringFromComposedIdProp(smartItemIdAndProperty);

    return getItemPropertyValue(
        cId,
        prop,
        poolData
    );
}

void ComposedVisual::disposeMain() {
    disposeItemsMap(this, &smartItemsList);

    super::disposeMain();
}

ComposedVisual::~ComposedVisual() {
    //void
}
