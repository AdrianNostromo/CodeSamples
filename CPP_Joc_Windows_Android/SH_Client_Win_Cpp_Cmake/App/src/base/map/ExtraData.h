#pragma once

#include <base/gh.h>
#include "Map1D.h"
#include <memory>
#include <string>
#include <base/map/Map1D.h>
#include <base/object/IWrappedValue.h>

class ExtraData {pub dCtor(ExtraData);
	priv Map1D<std::string, std::shared_ptr<base::IWrappedValue>> map{};
	
	pub explicit ExtraData();

	std::shared_ptr<base::IWrappedValue> getDataMustExist(std::string key);
	std::shared_ptr<base::IWrappedValue> getDataOptional(std::string key);
	void setData(std::string key, std::shared_ptr<base::IWrappedValue> value);

	pub virtual ~ExtraData();
};
