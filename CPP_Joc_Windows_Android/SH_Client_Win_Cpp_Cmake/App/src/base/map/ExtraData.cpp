#include "ExtraData.h"
#include <base/exceptions/LogicException.h>

ExtraData::ExtraData() {
	//void
}

std::shared_ptr<base::IWrappedValue> ExtraData::getDataMustExist(std::string key) {
	std::shared_ptr<base::IWrappedValue> ret = map.getDirect(key, nullptr);
	if (ret == nullptr) {
		throw LogicException(LOC);
	}

	return ret;
}

std::shared_ptr<base::IWrappedValue> ExtraData::getDataOptional(std::string key) {
	std::shared_ptr<base::IWrappedValue> ret = map.getDirect(key, nullptr);
	
	return ret;
}

void ExtraData::setData(std::string key, std::shared_ptr<base::IWrappedValue> value) {
	if (map.containsKey(key)) {
		throw LogicException(LOC);
	}

	map.putReference(key, value);
}

ExtraData::~ExtraData() {
	//void
}
