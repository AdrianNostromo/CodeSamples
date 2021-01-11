#include "ProjectileConfig.h"

using namespace rpg3D;

Map2D<std::string, std::string, ProjectileConfig*> ProjectileConfig::entriesMap{};

ProjectileConfig::ProjectileConfig(std::string& nSpace, std::string& id)
	: super(),
	  nSpace(nSpace), id(id)
{
	//void
}

ProjectileConfig* ProjectileConfig::newEntry(std::string nSpace, std::string& id) {
	// Check if the scenario already exists.
	if (entriesMap.containsKey(nSpace, id)) {
		throw LogicException(LOC);
	}

	ProjectileConfig* newTemplate = *entriesMap.putDirect(
		nSpace, id,
		new ProjectileConfig(nSpace, id)
	);

	return newTemplate;
}

ProjectileConfig* ProjectileConfig::fromId(std::string nSpace, std::string id) {
	return entriesMap.getDirect(nSpace, id);
}

ProjectileConfig* ProjectileConfig::init(std::string& projectileTemplateNSpace, std::string& projectileTemplateId) {
	this->projectileTemplateNSpace = projectileTemplateNSpace;
	this->projectileTemplateId = projectileTemplateId;

	return this;
}

ProjectileConfig::~ProjectileConfig() {
	//void
}
