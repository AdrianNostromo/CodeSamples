#pragma once

#include <string>
#include <graphics/visual/drawable/IGenericDrawable.h>
#include <base/object/WrappedValue.h>
#include <memory>

class IDynamicPropertiesObject3D : virtual public IGenericDrawable {
public:
    virtual std::shared_ptr<base::IWrappedValue> getPropertyValue(std::string property) = 0;
    virtual void setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) = 0;

    ~IDynamicPropertiesObject3D() override = default;
};
