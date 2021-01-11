#pragma once

#include <string>
#include <graphics/visual/drawable/IGenericDrawable.h>
#include <base/map/Map1D.h>
#include <base/component/IComponentsHandler.h>

class IComposedVisual : virtual public IComponentsHandler, virtual public IGenericDrawable {
public:
    virtual void visualLoop(float deltaS, int deltaMS) = 0;
    virtual void visualLoopB(float deltaS, int deltaMS) = 0;

    virtual IGenericDrawable* getLocalItem(std::string itemIDFragment) = 0;
    virtual void setLocalItem(std::string itemIDFragment, IGenericDrawable* item) = 0;

    virtual Map1D<std::string, IGenericDrawable*>* getSubItemsMap() = 0;

    ~IComposedVisual() override = default;

};
