#pragma once

#include <worldGame3D/gw/handler/IHandler_GameWorld.h>
#include <string>
#include <memory>
#include <functional>
#include <base/map/Map1D.h>
#include <assert.h>
#include <worldGame3D/scenariohandler/util/ScenarioConfig.h>
#include <base/list/ArrayList.h>

class IGameWorldBase;
namespace base {
	class IGameWorld;
};
namespace worldGame3D {
	class ContentZone;
};

class ScenarioTemplate {
public:
	class IWorldFactory {
	public:
		virtual base::IGameWorld* newInstance(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig) = 0;

		virtual ~IWorldFactory() = default;
	};

public:
	static Map1D<std::string, ScenarioTemplate*> entriesMap;
	
	std::string id;
	
	// These must be populated by the zones in their static init.
	Map1D<std::string, std::function<std::shared_ptr<worldGame3D::ContentZone>()>> zonesFactoriesMap{};
	std::string defaultZoneName = "";

	IWorldFactory* worldFactory;

public:
	static ScenarioTemplate* newEntry(std::string id, IWorldFactory* worldFactory);
	static ScenarioTemplate* getFromId(std::string id);

	ScenarioTemplate(ScenarioTemplate const&) = delete;
	ScenarioTemplate(ScenarioTemplate &&) = default;
	ScenarioTemplate& operator=(ScenarioTemplate const&) = delete;
	ScenarioTemplate& operator=(ScenarioTemplate &&) = default;
	explicit ScenarioTemplate(std::string& id, IWorldFactory* worldFactory);

	void addZoneFactory(std::string& zoneName, std::function<std::shared_ptr<worldGame3D::ContentZone>()> factoryFunction, bool isDefaultScenario);

	~ScenarioTemplate();
};
