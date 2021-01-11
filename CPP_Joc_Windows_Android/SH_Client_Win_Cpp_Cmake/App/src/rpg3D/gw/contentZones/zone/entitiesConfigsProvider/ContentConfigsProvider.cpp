#include "ContentConfigsProvider.h"
#include <base/exceptions/LogicException.h>

using namespace rpg3D;

ContentConfigsProvider::ContentConfigsProvider() {
	//void
}

EntityConfigBase* ContentConfigsProvider::getConfig(std::shared_ptr<std::string> instanceId, int configType) {
	if (instanceId == nullptr) {
		throw LogicException(LOC);
	}

	sp<EntityConfigBase> configEntry = configsMap.getDirect(instanceId, nullptr);
	if (configEntry == nullptr) {
		return nullptr;
	}

	if (configEntry->getType() != configType) {
		throw LogicException(LOC);
	}

	return configEntry.get();
}

ContentConfigsProvider::~ContentConfigsProvider() {
	//void
}
