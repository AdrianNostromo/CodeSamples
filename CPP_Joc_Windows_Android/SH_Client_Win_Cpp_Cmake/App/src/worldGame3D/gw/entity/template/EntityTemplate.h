#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include <base/map/Map2D.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>
#include <functional>
#include <sstream>
#include <base/sp.h>

namespace base {
    class IWrappedValue;
};

//typedef factoryFunction EntityTemplate* (*arr[])();

class EntityTemplate {pub dCtor(EntityTemplate);
    pub static std::string ExtraDataID_tilingInformation;

    pub static Map2D<std::string, std::string, EntityTemplate*> entriesMap;
    pub static Map2D<std::string, std::string, std::function<EntityTemplate*(sp<Array1D<std::shared_ptr<base::IWrappedValue>>>& configValues)>> customTemplatesFactoriesMap;

    pub std::string nSpace;
    pub std::string id;

    // Use for custom templates.
    pub std::string baseId;
    pub sp<Array1D<std::shared_ptr<base::IWrappedValue>>> configParams;

    // Eg: ROCK, TREE, GROUND, PROJECTILE, UNIT, PLATFORM, DOODAD, ITEM, ...;
    // NOTE : Tags are compared as objects, not as strings.
    pub ArrayList<std::string> tagsList{};

    pub Map1D<int, BasicModuleTemplate*> moduleTemplatesIdMap{};

    pub ArrayList<BasicModuleTemplate*> moduleTemplatesList{};

    pub static EntityTemplate* newEntry(std::string& scenarioNamespace, std::string& id);
    pub static EntityTemplate* newEntry(std::string& scenarioNamespace, std::string& id, std::string& baseId, sp<Array1D<std::shared_ptr<base::IWrappedValue>>> configParams);
    // Optional
    pub static EntityTemplate* fromId(std::string& scenarioNamespace, std::string id);

    priv explicit EntityTemplate(std::string& scenarioNamespace, std::string& id);
    priv explicit EntityTemplate(std::string& scenarioNamespace, std::string& id, std::string& baseId, sp<Array1D<std::shared_ptr<base::IWrappedValue>>> configParams);

    pub EntityTemplate* setModuleTemplate(BasicModuleTemplate* moduleTemplate);
    pub bool hasModuleTemplate(int moduleId);
    pub bool hasTagsList(ArrayList<std::string> tagsList);
    pub bool hasTag(std::string& tag);

    pub template <typename T>
    T getModuleTemplateAs(int moduleId);

    pub void toPartsString(std::stringstream& ss);
    pub static EntityTemplate* FromPartsString(std::shared_ptr<ArrayList<std::string>> partsList, int startPartIndex);

    pub virtual ~EntityTemplate();
};

template <typename T>
T EntityTemplate::getModuleTemplateAs(int moduleId) {
	if(!hasModuleTemplate(moduleId)) {
		return nullptr;
	}

	BasicModuleTemplate* t = moduleTemplatesIdMap.getDirect(moduleId);
	if(t == nullptr) {
		return nullptr;
	}

	T cT = dynamic_cast<T>(t);
	if(cT == nullptr) {
		throw LogicException(LOC);
	}

	return cT;
}
