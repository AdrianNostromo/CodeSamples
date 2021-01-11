#pragma once

class IManagedDynamicVisual;

class IManagedDynamicVisualManager {
public:
    virtual void requireManagedVisualUpdate(IManagedDynamicVisual* targetManagedVisual) = 0;

	virtual ~IManagedDynamicVisualManager() = default;

};
