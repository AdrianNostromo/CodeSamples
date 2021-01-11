#pragma once

class IManagedDynamicVisualManager;

class IManagedDynamicVisual : virtual public base::IObject {
public:
    virtual void reservedCreate() = 0;

    virtual void updateVisual() = 0;

};
