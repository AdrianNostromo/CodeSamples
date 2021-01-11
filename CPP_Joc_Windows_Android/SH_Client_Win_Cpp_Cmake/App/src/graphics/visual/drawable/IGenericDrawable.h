#pragma once

// Used as the base class for all UI elements. This is used for class casting.
class IGenericDrawable {
public:
    virtual ~IGenericDrawable() = default;
};
