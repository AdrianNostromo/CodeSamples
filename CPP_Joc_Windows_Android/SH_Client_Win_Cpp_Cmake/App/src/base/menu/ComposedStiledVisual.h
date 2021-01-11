#pragma once

#include <base/menu/util/MenuItemConfig.h>
#include <graphics/visual2d/drawable/IContainer2D.h>
#include <base/menu/util/VisualPropertiesConfig.h>
#include "ComposedVisual.h"
#include <base/object/WrappedValue.h>
#include <memory>

class IAppAssets;

class ComposedStiledVisual : public ComposedVisual {
private: typedef ComposedVisual super;
protected:
    IAppAssets* appAssets;

    //asd_002;// remove this var and use {"_root", "layer"} instead. That will break anchor for smart items (slider, toggle_switch).
    IContainer2D* visualsHolder = nullptr;

private:
    //asdA5;// rename to itemConfigs;
    ArrayList<MenuItemConfig*>* menuItemConfigs;

public:
    ComposedStiledVisual(ComposedStiledVisual const&) = delete;
    ComposedStiledVisual(ComposedStiledVisual &&) = default;
    ComposedStiledVisual& operator=(ComposedStiledVisual const&) = delete;
    ComposedStiledVisual& operator=(ComposedStiledVisual &&) = default;

    explicit ComposedStiledVisual(IAppMetrics* appMetrics, IAppAssets* appAssets, ArrayList<MenuItemConfig*>* itemConfigs);

    IContainer2D* getVisualsHolder();

    virtual void doFullVisualsSync();

    static std::shared_ptr<base::IWrappedValue> interpolateSmartValues(
        std::shared_ptr<base::IWrappedValue>& startValue, std::shared_ptr<base::IWrappedValue>& endValue,
        float animationPercentValue);

    std::shared_ptr<base::IWrappedValue> computeTopLevelSmartValue(VisualValueConfig* smartValue, IDynamicPropertiesObject2D* selfObject, IMap1D<std::string, std::shared_ptr<base::IWrappedValue>>* dataMap);

    virtual void createVisuals(Map1D<std::string, void*>* createConfigMap);

    ~ComposedStiledVisual() override;
protected:
    void createContent() override;
    virtual void visualItemCreated(IGenericDrawable* item, MenuItemConfig* itemConfig, IComposedVisual* itemsMap, IComposedVisual* itemsMapLayer);
    virtual void syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent);

    static void syncVisualItem(
        IDynamicPropertiesObject2D* visual2D, VisualPropertiesConfig* itemProperties,
        bool syncAbsoluteValues, bool syncAnimatingValues,
        std::string startStateID, std::string endStateID,
        float stateCompletionPercent,
        IComposedVisual* itemsMap, IComposedVisual* itemsMapLayer,
        IMap1D<std::string, std::shared_ptr<base::IWrappedValue>>* dataMap,
        IAppMetrics* appMetrics);

    static bool checkIsStatePassed(std::string& stateID, UiId* composedStateIDFragment);

    void disposeMain() override;
private:
    void createVisualItems(IComposedVisual* itemsMap, IComposedVisual* itemsMapLayer, ArrayList<MenuItemConfig*>* menuItemConfigs, IMap1D<std::string, void*>* createConfigMap, ArrayList<IComposedVisual*>* smartItemsList);
    static void syncMenuItems(
        bool syncAbsoluteValues, bool syncAnimatingValues,
        std::string startStateID, std::string endStateID, float stateCompletionPercent,
        ComposedVisual* itemsMap, ComposedVisual* itemsMapLayer,
        ArrayList<MenuItemConfig*>* menuItemConfigs,
        IMap1D<std::string, std::shared_ptr<base::IWrappedValue>>* dataMap,
        IAppMetrics* appMetrics);

    static MenuVisualAnimationProperty::StateComposedValue* getValueWherePropertyHasFragment(std::string& menuStateID, ArrayList<MenuVisualAnimationProperty::StateComposedValue*>* stateValuesList);
    static std::shared_ptr<base::IWrappedValue> computeSmartValue(IAppMetrics* appMetrics, IComposedVisual* itemsMap, IComposedVisual* itemsMapLayer, VisualValueConfig* smartValue, IDynamicPropertiesObject2D* selfObject, ComputeSmartValuePoolData* poolData, IMap1D<std::string, std::shared_ptr<base::IWrappedValue>>* dataMap);

};
