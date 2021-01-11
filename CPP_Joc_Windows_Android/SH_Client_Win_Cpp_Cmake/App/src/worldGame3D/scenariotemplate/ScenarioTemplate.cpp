#include "ScenarioTemplate.h"
#include <base/exceptions/LogicException.h>

Map1D<std::string, ScenarioTemplate*> ScenarioTemplate::entriesMap{};

ScenarioTemplate* ScenarioTemplate::newEntry(std::string id, IWorldFactory* worldFactory) {
	// Check if the scenario already exists.
	if (entriesMap.containsKey(id)) {
		assert(false);
	}

	ScenarioTemplate* newTemplate = *entriesMap.putDirect(
		id, 
		new ScenarioTemplate(id, worldFactory)
	);

	return newTemplate;
}

ScenarioTemplate* ScenarioTemplate::getFromId(std::string id) {
	ScenarioTemplate* ret = entriesMap.getDirect(id, nullptr);

	return ret;
}

ScenarioTemplate::ScenarioTemplate(std::string& id, IWorldFactory* worldFactory) {
	this->id = id;

	this->worldFactory = worldFactory;
}

void ScenarioTemplate::addZoneFactory(std::string& zoneName, std::function<std::shared_ptr<worldGame3D::ContentZone>()> factoryFunction, bool isDefaultScenario) {
	if (zonesFactoriesMap.containsKey(zoneName)) {
		throw LogicException(LOC);
	}

	zonesFactoriesMap.putDirect(zoneName, factoryFunction);

	if (isDefaultScenario) {
		if (!defaultZoneName.empty()) {
			throw LogicException(LOC);
		}

		defaultZoneName = zoneName;
	}
}

ScenarioTemplate::~ScenarioTemplate() {
	//void
}
