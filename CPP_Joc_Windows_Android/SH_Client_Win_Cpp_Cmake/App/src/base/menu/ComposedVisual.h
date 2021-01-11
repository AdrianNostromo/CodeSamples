#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include <base/list/ListDL.h>
#include <base/map/Map1D.h>
#include "IComposedVisual.h"
#include <string>
#include <base/list/Array1D.h>
#include <base/map/Map1DComposed.h>
#include <base/menu/util/ComputeSmartValuePoolData.h>
#include <graphics/visual2d/drawable/IDynamicPropertiesObject2D.h>
#include <base/exceptions/LogicException.h>
#include <graphics/visual/drawable/IGenericDrawable.h>
#include <memory>
#include <base/menu/util/UiId.h>
#include <base/object/WrappedValue.h>
#include <graphics/visual2d/drawable/IDynamicPropertiesObject2DBase.h>
#include <base/MM.h>
#include <base/component/ComponentsHandler.h>
#include <base/component/IComponent_UIView.h>

class IAppMetrics;

class ComposedVisual : public ComponentsHandler, virtual public IComposedVisual, public virtual IComponent_UIView, virtual public IDynamicPropertiesObject2DBase {priv typedef ComponentsHandler super;
    prot IAppMetrics* appMetrics;

    // Only items created from templates are added to this list, manual items have to be manually added to this list.
    // Use a ListDL so that manually added items can be easily removed.
    prot ArrayList<IComposedVisual*> smartItemsList{};
    // These contain manual items that can be added or removed on the fly by custom code.
    prot ListDL<IComposedVisual*> manualSmartItemsList{LOC};

    prot Map1D<std::string, void*>* createConfigMap = nullptr;

private:
    Map1D<std::string, IGenericDrawable*> subItemsMap{};

    // Pseudo bool. -1, 0, 1
    Map1D<std::string, int> staticAnimatingPropertiesMap{};

    Map1DComposed<std::string, std::shared_ptr<base::IWrappedValue>> dataMap{new Map1D<std::string, std::shared_ptr<base::IWrappedValue>>()};

public:
    ComposedVisual(ComposedVisual const&) = delete;
    ComposedVisual(ComposedVisual &&) = default;
    ComposedVisual& operator=(ComposedVisual const&) = delete;
    ComposedVisual& operator=(ComposedVisual &&) = default;

    explicit ComposedVisual(IAppMetrics* appMetrics);

    Map1D<std::string, IGenericDrawable*>* getSubItemsMap() override {
        return &subItemsMap;
    }

    void setCreateConfigMap(Map1D<std::string, void*>* createConfigMap);
    IMap1D<std::string, std::shared_ptr<base::IWrappedValue>>* getDataMap();
    virtual void putDataMapValue(std::string& key, std::shared_ptr<base::IWrappedValue> val, bool doAnimatingVisualsSync);
    void putDataMapValueB(std::string key, std::shared_ptr<base::IWrappedValue> val, bool doAnimatingVisualsSync);
    void addDataMap(Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* dataMap);

    void setItem(UiId& composedId, IGenericDrawable* item);
    void setItem(std::string& composedId, IGenericDrawable* item);
    void setItemB(std::string composedId, IGenericDrawable* item);

    IGenericDrawable* getLocalItem(std::string itemIDFragment) override;
    void setLocalItem(std::string itemIDFragment, IGenericDrawable* item) override;

    template <typename T>
    T getItemAs(UiId& composedId, bool mustExist);
    template <typename T>
    T getItemAs(std::string composedItemId, bool mustExist);

    template <typename T>
    T getItemMustExistAs(UiId& composedId);
    template <typename T>
    T getItemMustExistAs(std::string composedItemId);
    template <typename T>
    T getItemOptionalAs(UiId& composedId);
    template <typename T>
    T getItemOptionalAs(std::string composedItemId);

    std::shared_ptr<base::IWrappedValue> getItemPropertyValue(UiId& composedId, std::string& property, ComputeSmartValuePoolData* poolData);
    std::shared_ptr<base::IWrappedValue> getItemPropertyValue(std::string smartItemIdAndProperty, ComputeSmartValuePoolData* poolData);

    void visualLoop(float deltaS, int deltaMS) override;
    void visualLoopB(float deltaS, int deltaMS) override;
	
	bool isInit_StaticAnimatingPropertyStatus(std::string& property) override;
	bool isTrue_StaticAnimatingPropertyStatus(std::string& property) override;
	bool isTrue_StaticAnimatingPropertyStatusDirect(std::string property);
	void putStaticAnimatingPropertyStatus(std::string& property, int status) override;
	void putStaticAnimatingPropertyStatusDirect(std::string property, int status);

    ~ComposedVisual() override;
protected:
    prot void create() override;
    virtual void createFramework();
    virtual void createContent();
    static IGenericDrawable* getMenuItemOptional(
        IComposedVisual* itemsMapRoot, IComposedVisual* itemsMapLayer,
        UiId& composedId, int currentItemIDIndex);
    static std::shared_ptr<base::IWrappedValue> getVisualPropertyValue(IComposedVisual* itemsMap, IComposedVisual* itemsMapLayer, UiId& itemComposedID, std::string& property, IDynamicPropertiesObject2D* selfObject, ComputeSmartValuePoolData* poolData, IMap1D<std::string, std::shared_ptr<base::IWrappedValue>>* dataMap, IAppMetrics* appMetrics);

    prot void disposeMain() override;
private:
    static void setMenuItem(
        IComposedVisual* itemsMapRoot, IComposedVisual* itemsMapLayer,
        UiId& composedId, int currentItemIDIndex, IGenericDrawable* item);
    static void recycleItem(IGenericDrawable* item, ArrayList<IComposedVisual*>* smartItemsList);
    static void disposeItemsMap(IComposedVisual* entriesMap, ArrayList<IComposedVisual*>* smartItemsList);

};

template <typename T>
T ComposedVisual::getItemMustExistAs(UiId& composedId) {
    T ret = getItemOptionalAs<T>(composedId);

    if(ret == nullptr) {
        throw LogicException(LOC);
    }

    return ret;
}

template <typename T>
T ComposedVisual::getItemMustExistAs(std::string composedItemId) {
    T ret = getItemOptionalAs<T>(composedItemId);

    if(ret == nullptr) {
        throw LogicException(LOC);
    }

    return ret;
}

template <typename T>
T ComposedVisual::getItemAs(UiId& composedId, bool mustExist) {
    T ret = (T)getMenuItemOptional(
        this, this,
        composedId, 0
    );
    if(ret == nullptr && mustExist) {
        throw LogicException(LOC);
    }

    return ret;
}

template <typename T>
T ComposedVisual::getItemAs(std::string composedItemId, bool mustExist) {
    UiId cId(composedItemId, "_root");

    T ret = dynamic_cast<T>(getMenuItemOptional(
        this, this,
        cId, 0
    ));
    if(ret == nullptr && mustExist) {
        throw LogicException(LOC);
    }

    return ret;
}

template <typename T>
T ComposedVisual::getItemOptionalAs(UiId& composedId) {
    return (T)getMenuItemOptional(
        this, this,
        composedId, 0
    );
}

template <typename T>
T ComposedVisual::getItemOptionalAs(std::string composedItemId) {
    UiId cId(composedItemId, "_root");

    T ret = dynamic_cast<T>(getMenuItemOptional(
        this, this,
        cId, 0
    ));

    return ret;
}
