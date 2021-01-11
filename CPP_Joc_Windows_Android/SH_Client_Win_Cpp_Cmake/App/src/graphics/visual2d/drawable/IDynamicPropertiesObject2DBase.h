#pragma once

#include <string>
#include <graphics/visual/drawable/IGenericDrawable.h>

class IDynamicPropertiesObject2DBase : virtual public IGenericDrawable {
public:
	// Pseudo Boolean.
	// -1 : Not set;
	// 0 : false;
	// 1 : true;
	virtual bool isInit_StaticAnimatingPropertyStatus(std::string& property) = 0;
	virtual bool isTrue_StaticAnimatingPropertyStatus(std::string& property) = 0;
	virtual void putStaticAnimatingPropertyStatus(std::string& property, int status) = 0;

	~IDynamicPropertiesObject2DBase() override = default;
};
