#pragma once

#include "IDynamicPropertiesObject2DBase.h"
#include <base/object/WrappedValue.h>
#include <memory>
#include <string>

class Color;

class IDynamicPropertiesObject2D : virtual public IDynamicPropertiesObject2DBase {
public:
    virtual std::shared_ptr<base::IWrappedValue> getPropertyValue(std::string property) = 0;
    virtual void setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) = 0;
protected:

	// "dest" and "src" always exists.
	static bool CheckedSet(float& dest, float src);
	static bool CheckedSet(int& dest, int src);
	static bool CheckedSet(bool& dest, bool src);
	static bool CheckedSet(Color* dest, Color* src);
	static bool CheckedSet(std::string** dest, std::string* src);

    ~IDynamicPropertiesObject2D() override = default;
};
